#!/usr/bin/env python3
"""
ch32_upload.py — 1200bps touch + wchisp upload for Arduino IDE.

Usage (called from platform.txt):
    python3 ch32_upload.py <serial_port> <firmware.bin>

Sends 1200bps CDC touch to trigger BootROM, then flashes with wchisp.

SPDX-License-Identifier: MIT
"""

import os
import sys
import shutil
import subprocess
import time


def find_wchisp():
    """Locate wchisp binary."""
    # Check PATH first
    w = shutil.which("wchisp")
    if w:
        return w
    # Check common PlatformIO install
    home = os.path.expanduser("~")
    pio_wchisp = os.path.join(home, ".platformio", "packages", "tool-wchisp", "wchisp")
    if os.path.isfile(pio_wchisp):
        return pio_wchisp
    # Check cargo install location
    cargo_wchisp = os.path.join(home, ".cargo", "bin", "wchisp")
    if os.path.isfile(cargo_wchisp):
        return cargo_wchisp
    return None


def main():
    if len(sys.argv) < 3:
        print("Usage: %s <serial_port> <firmware.bin>" % sys.argv[0], file=sys.stderr)
        sys.exit(2)

    port = sys.argv[1]
    firmware = sys.argv[2]

    wchisp = find_wchisp()
    if not wchisp:
        print("ERROR: wchisp not found. Install with: cargo install wchisp", file=sys.stderr)
        sys.exit(1)

    if not os.path.isfile(firmware):
        print("ERROR: firmware file not found: %s" % firmware, file=sys.stderr)
        sys.exit(1)

    # 1200bps touch to trigger BootROM
    if port and port != "none":
        try:
            import serial
            print("Triggering BootROM via 1200bps touch on %s" % port)
            serial.Serial(port, 1200).close()
            time.sleep(6.0)
        except ImportError:
            print("WARNING: pyserial not installed, trying raw open")
            try:
                import termios
                fd = os.open(port, os.O_RDWR | os.O_NOCTTY | os.O_NONBLOCK)
                attrs = termios.tcgetattr(fd)
                termios.cfsetispeed(attrs, termios.B1200)
                termios.cfsetospeed(attrs, termios.B1200)
                termios.tcsetattr(fd, termios.TCSANOW, attrs)
                os.close(fd)
                time.sleep(6.0)
            except Exception as e:
                print("WARNING: 1200bps touch failed: %s" % e)
        except Exception as e:
            print("WARNING: 1200bps touch failed: %s" % e)
    else:
        print("No serial port specified, trying wchisp directly")

    # Flash with wchisp
    print("Flashing %s" % firmware)
    r = subprocess.run([wchisp, "flash", firmware])
    if r.returncode != 0:
        print("Retrying...")
        time.sleep(1.0)
        r = subprocess.run([wchisp, "flash", firmware])

    sys.exit(r.returncode)


if __name__ == "__main__":
    main()
