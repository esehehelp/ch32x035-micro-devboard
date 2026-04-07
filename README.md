# CH32X035F7P6 Micro Devboard

CH32X035F7P6 (RISC-V, 48 MHz, 62 KB flash, 20 KB RAM) の小型開発ボード。

## 概要

USB CDC-ACM ライブラリ (`ch32x-cdc`) を中心に、PlatformIO と Arduino IDE の両方をサポート。
ESP32/RP2040 の Arduino Serial と同じ体験 — 初期化を呼ぶだけで USB シリアルが使え、1200bps タッチでファームウェア更新ができる。

## PlatformIO (firmware/)

### 構成

```
firmware/
  platformio.ini          # PlatformIO 設定
  ch32_1k2touch.py        # カスタムアップロードスクリプト
  src/
    main.c                # サンプルアプリ (PC3 blink + CDC echo)
    startup.S             # ベクタテーブル + 起動コード
    link.ld               # リンカスクリプト
  lib/
    ch32x-cdc/
      include/
        ch32x_cdc.h       # 公開 API
        ch32x_regs.h      # MIT レジスタ定義 (WCH HAL 非依存)
      src/
        ch32x_cdc.c       # CDC 実装 (ディスクリプタ, ISR, バッファ)
```

### ビルド & 書き込み

```bash
cd firmware

# ビルド
pio run

# 書き込み (初回: wch-link 経由)
pio run -t upload --upload-port wlink

# 書き込み (2回目以降: USB CDC 1200bps タッチ経由, wch-link 不要)
pio run -t upload
```

初回書き込みは wch-link (SWD) が必要。一度ファームウェアが動けば、以降は USB ケーブルだけで `pio run -t upload` でフラッシュできる。

## Arduino IDE (arduino/)

### インストール

1. `arduino/ch32x035f7p6/` を Arduino のハードウェアディレクトリにコピーまたはシンボリックリンク:
   ```bash
   # Linux
   ln -s $(pwd)/arduino/ch32x035f7p6 ~/.arduino15/packages/ch32x035f7p6

   # または Arduino IDE の「ファイル > 環境設定 > スケッチブックの保存場所」内の
   # hardware/ch32x035f7p6/ に配置
   ```
2. ツールチェイン: WCH の RISC-V GCC (`xpack-riscv-none-embed-gcc`) をパスに追加
3. ボード選択: **CH32X035F7P6** を選択
4. アップロード: 1200bps タッチ + wchisp による自動書き込み

### Arduino API サポート

- `Serial` — USB CDC シリアル (`USBSerial` クラス、`Print` 継承)
- `millis()` / `micros()` / `delay()` / `delayMicroseconds()` — SysTick ベース
- `pinMode()` / `digitalWrite()` / `digitalRead()`

### API

```c
#include "ch32x_cdc.h"

// 初期化 (NULL でデフォルト設定)
ch32x_cdc_config_t cfg = { .magic_baud_enable = 1 };
ch32x_cdc_init(&cfg);

// 読み込み
int ch32x_cdc_available(void);       // 受信バイト数
int ch32x_cdc_read(void);            // 1バイト読み込み (-1 = 空)
size_t ch32x_cdc_read_buf(buf, max); // バッファに一括読み込み

// 書き込み
size_t ch32x_cdc_write(buf, len);    // バイト列送信
size_t ch32x_cdc_print(str);         // 文字列送信
void ch32x_cdc_flush(void);          // TX 完了待ち

// BootROM へリブート (手動呼び出し用)
void ch32x_cdc_reboot_to_bootrom(void);
```

#### 設定

```c
typedef struct {
    int      magic_baud_enable;  // 1200bps リブートトリガ (デフォルト: 有効)
    uint32_t magic_baud;         // トリガボーレート (デフォルト: 1200)
    void (*pre_reboot)(void);    // リブート前コールバック
} ch32x_cdc_config_t;
```

#### RX コールバック

```c
// ISR コンテキストで呼ばれる (weak シンボル、アプリ側でオーバーライド可)
void ch32x_cdc_on_rx(const uint8_t *buf, size_t len) {
    // 受信データを処理
}
```

### サンプルアプリ (PlatformIO)

`firmware/src/main.c` — PC3 LED 点滅 (500ms) + USB CDC エコー。

```c
#include "ch32x_regs.h"
#include "ch32x_cdc.h"

int main(void) {
    ch32x_cdc_init(NULL);

    /* PC3 push-pull output (cdc_init already enabled GPIOC clock) */
    GPIOC->CFGLR = (GPIOC->CFGLR & ~(0xFu << 12)) | (0x3u << 12);

    for (;;) {
        uint8_t buf[64];
        size_t n = ch32x_cdc_read_buf(buf, sizeof(buf));
        if (n > 0)
            ch32x_cdc_write(buf, n);
        ch32x_cdc_poll();
        IWDG->CTLR = 0xAAAA;
    }
}
```

### 技術メモ

- **クロック設定**: `ch32x_cdc_init()` 内で HSI 有効化、Flash 2 wait-state 設定、HPRE=DIV1 (HCLK=48MHz) を行う。BootROM が HPRE を div6 (8MHz) のまま残すため、明示的なクリアが必要。
- **SysTick**: QingKe V4C の 64-bit カウンタ。HCLK/8 (6MHz) で動作。カウンタリセットは CTLR bit 4 で行う（CNTL/CNTH 直接書き込みは不可）。
- **レジスタ定義**: `ch32x_regs.h` は TRM から自作した MIT ライセンスの定義。WCH HAL/EVT に依存しない。
- **割り込み**: `USBFS_IRQHandler` はライブラリが strong シンボルとして提供。`startup.S` の weak シンボルをリンカが解決。
- **ISR 属性**: GCC 8.2 (Arduino/WCH ツールチェイン) では `__attribute__((interrupt))` を使用。`WCH-Interrupt-fast` はカリーセーブドレジスタしか保存しないバグがある。GCC 12 (PlatformIO) では `WCH-Interrupt-fast` で問題なし。
- **BootROM 突入**: `FLASH->BOOT_MODEKEYR` アンロック → `FLASH->STATR |= BOOT_MODE` → PFIC システムリセット。チップ内蔵 ISP (0x1FFF0000) が起動し、wchisp で書き込み可能。
- **USB-C**: PC14/PC15 に CC プルダウンを設定 (USBPD ペリフェラル経由)。ホストが VBUS を供給するために必要。
- **HardFault**: サンプルアプリでは HardFault 時に BootROM へリブート。クラッシュしてもファームウェア書き直し可能。
- **IWDG**: 旧ファームウェアが IWDG を起動していた場合、ソフトウェアリセットでは停止しない。メインループで `IWDG->CTLR = 0xAAAA` で給餌するか、電源を入れ直す。
- **C++ 初期化**: startup.S で `__libc_init_array()` を呼び出し、vtable やグローバルコンストラクタを初期化。`-nostartfiles` 使用時は `_init`/`_fini` スタブも必要。

## License

MIT
