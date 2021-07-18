# HCMS39xx Library

This library interfaces with Broadcom/Avago [HCMS-39xx][2] and [HCMS-29xx][4] series LED displays.

There are multiple displays in this series with various colors and display characters.

The library was specifically tested with a single HCMS-3964 display (4 character, 0.2", orange), but should work with other displays in the either the 29xx or 39xx series. It should also work with multiple displays cascaded together.

## Comparison With LedDisplay Library

This library provides similar functionality as the [LedDisplay library][5]. The following summarizes the main differences:

- HCMS39xx does not inherit from the Print class.
  - HCMS39xx requires less flash and RAM
  - HCMS39xx does not support cursor functionality
  - HCMS39xx does not suport scrolling functionality, but the serial nature of loading the pixel data into the chip makes scrolling simple to implement outside of the library.
- HCMS39xx supports controlling the _blank_ and _osc_ pins.
- HCMS39xx supports the display sleep functionality.
- HCMS39xx supports the LED display current control functionality.
- HCMS39xx supports the external oscillator prescale settings
- HCMS39xx more elegantly supports simultaneous mode
  - LedDisplay has a method to use simultaneous mode when writing control registers, but it serially writes all the chips to enable the mode, thereby defeating the purpose of simultaneous mode.

## Usage

_Be sure to review the example sketch included with the library._

First, **include** the library header file:

```cpp
#include <HCMS39xx.h>
```

Next, **instantiate** an HCMS39xx object:

```cpp
HCMS39xx myDisplay(uint8_t num_chars, uint8_t data_pin, uint8_t rs_pin, uint8_t clk_pin, 
           uint8_t ce_pin, uint8_t blank_pin = NO_PIN, uint8_t osc_select_pin = NO_PIN);
```

Then **initialize** the object (typically within `setup()`):

```cpp
myDisplay.begin();
```

### Library Methods

```cpp
  void print(const char*);
  void printDirect(const uint8_t*, uint8_t len);
  void clear();
  void displaySleep();
  void displayWakeup();
  void displayBlank();
  void displayUnblank();
  void setBrightness(uint8_t value); // 0 <= value <= 15 
  void setCurrent(uint8_t value); // See header file for enumerated definitions for value
  void setExtOsc();
  void setIntOsc();
  void setExternalPrescaleDiv8();
  void setExternalPrescaleNormal();
  void setSimultaneousMode();
  void setSerialMode();
```

Note that the display is blanked when it is initially created (if blank_pin is defined), so you need to use the displayUnblank() method before any characters will actually be displayed.

## Implementation Details

- Ordering of pixels. Bottom pixels are most significant (i.e., pixels start from bottom left to upper right by column)
- Default settings after calling begin(): pixels, control word0, control word 1. Screen blanked.
- Pixel map from Broadcom [app note][3] Table 1. Notes on updating pixel map and adding characters. Also font map meta-data. How to easily add strikethrough to characters and other hints.

## To-Do

1. Add print methods for ints and floats (I think I only need one each, as automatic casting will cover other cases).
2. Test library (both examples) on both Arduino and Energia before publishing.

## References

- HCMS-39xx [datasheet][1]
- Broadcom [Application Note][3] with ASCII font encoding data.
- Broadcom 29xx series [datasheet][4]

## License

The software and other files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE`][101] in this repository.

[1]:https://docs.broadcom.com/docs/AV02-0868EN
[2]:https://www.broadcom.com/products/leds-and-displays/smart-alphanumeric-displays/serial-interface/hcms-3964
[3]:https://docs.broadcom.com/doc/5988-7539EN
[4]:https://docs.broadcom.com/doc/HCMS-29xx-Series-High-Performance-CMOS-5-x-7-Alphanumeric-Displays
[5]:https://github.com/PaulStoffregen/LedDisplay
[6]:https://playground.arduino.cc/Main/LedDisplay/
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE
[200]: https://github.com/Andy4495/HCMS39xx
