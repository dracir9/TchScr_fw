# Touch Screen Firmware
Firmware for a custom resistive touch controller based on a EFM8BB microcontroller.

This code is intended to run in a EFM8BB10F8G microcontroller from Silicon Labs. It uses four pins to drive a common resistive touchscreen and two more pins to communicate with the master through I2C.

## Features
- Map touch screen coordinates to any range between 0 and 65535. Ideal to map to LCD coordinates
- Configurable minimum and maximum touch pressure thresholds for reduced noise and false positives
- Built-in filtering of touch coordinates
- Up to 28 fully configurable buttons
  + Detect events inside a per-button configurable area
  + Press, release, and hold events per button
  + Per-button configurable hold timeout between 0 and 6.2 s (0.2 s resolution)
- Report touch (X, Y) coordinates

## Communication
The EFM8BB communicates with the host using the I2C protocol, which only uses 2 pins from the master controller. The implementation used in this firmware uses a multi master aproach, that means that the EFM8BB can act as both master and slave in the I2C communication. This introduces some complexity in the exchange of information but also has some advantages.

**Slave mode**
- This is the default mode
- The touch controller acts as an I2C slave and therefore the host controller can send configuration parameters and ask for the latest touch coordinates and button events.

**Master modde**
- Used to send events
- If enabled in the configuration, when an event occurs the touch controller will to automatically report to the host controller
This avoids having to constantly poll the EFM8BB to check for new events. Once the host controller has sent all the configuration parameters it can be switched to slave mode and wait to receive events.

### Commands
The touch controller uses a 7-bit address + one read/writte bite. It will respont to these addresses:
- Read
  + 0x81
  + 0x91
- Writte
  + 0x40
  + 0x48
  + 0x50
  + 0x58
