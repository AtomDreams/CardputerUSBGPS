# CardputerUSBGPS

Raw NMEA passthrough firmware for an M5 Cardputer connected to an AT6668 GPS Unit.

After flashing, the Cardputer appears on the host as a USB serial device and streams live NMEA sentences directly from the GPS Unit.

## Hardware

- M5 Cardputer, ESP32-S3
- AT6668 GPS Unit, connected through the Cardputer HY2.0-4P / PORT.CUSTOM port

| GPS Unit signal | Cardputer port | ESP32 role |
| --- | --- | --- |
| GND | GND | Ground |
| 5V | 5V | Power |
| GPS UART TX | G1 / GPIO1 | ESP32 UART1 RX |
| GPS UART RX | G2 / GPIO2 | ESP32 UART1 TX |

If no NMEA data appears, swap G1 and G2.

## Firmware behavior

The firmware is a direct passthrough:

- UART1 is opened at 115200 baud.
- UART1 RX is GPIO1 and UART1 TX is GPIO2.
- USB CDC is enabled so the ESP32-S3 exposes a host-visible serial device.
- Every byte received from the GPS Unit is written unchanged to USB serial.
- No GPS parsing, display, filtering, storage, or TinyGPSPlus processing is performed.

## Flash with PlatformIO

From the repository directory:

```bash
pio run
pio run -t upload --upload-port /dev/ttyACM0
```

PlatformIO Core is required. If the upload fails, re-plug the Cardputer and confirm the selected serial port.

## Read NMEA on Linux

The device usually appears as:

```text
/dev/ttyACM0
```

Set serial parameters and read raw NMEA:

```bash
stty -F /dev/ttyACM0 115200 raw -echo -echoe -echok
timeout 8 dd if=/dev/ttyACM0 bs=1 count=700 2>/dev/null
```

Expected output contains GPS sentences such as:

```text
$GNRMC,...
$GNGGA,...
$GNGSA,...
$GNGSV,...
```

A valid fix commonly shows `A` in the RMC sentence and satellite/HDOP data in the GGA sentence.

## Optional uses

This is a raw serial GPS source. It can be used by:

- a serial terminal, for example `screen /dev/ttyACM0 115200`
- GPSBabel
- QGIS or other GIS tools that can read a NMEA serial source
- `gpsd`, if a system NMEA bridge is desired

## Permissions

If the current user cannot open `/dev/ttyACM0`, verify data availability once with `sudo`, then configure a udev rule or user permission for repeated non-sudo access.
