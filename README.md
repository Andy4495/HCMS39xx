# HCMS39xx Library

This library interfaces with Broadcom/Avago [HCMS-39xx][2] and [HCMS-29xx][4] series LED displays.

There are multiple displays in this series with various colors and display characters.

The library was specifically tested with a single HCMS-3964 display (4 character, 0.2", orange), but should work with other displays in either the 29xx or 39xx series. It should also work with multiple displays cascaded together.

## Comparison With LedDisplay Library

This library provides similar functionality as the [LedDisplay library][5]. The following summarizes the main differences:

- HCMS39xx does not inherit from the Print class.
  - HCMS39xx requires less flash and RAM (compiling with a simple `print()` sketch: about 175 fewer bytes flash, and 200 fewer bytes RAM).
  - HCMS39xx does not support cursor functionality.
  - HCMS39xx does not suport scrolling functionality, but the serial nature of loading the pixel data into the chip makes scrolling simple to implement outside of the library.
- HCMS39xx supports controlling the _blank_ and _sel (oscillator select)_ pins.
- HCMS39xx supports the display sleep functionality.
- HCMS39xx supports the LED display current control functionality.
- HCMS39xx supports the external oscillator prescale settings.
- HCMS39xx provides more character pixel definitions in the included font file (while still using less flash).
- HCMS39xx more elegantly supports simultaneous mode.
  - LedDisplay has a method to use simultaneous mode when writing control registers, but it serially writes all the chips to enable the mode when it uses it, thereby defeating the purpose of simultaneous mode.

## Usage

_Be sure to review the [example sketches][7] included with the library._

1. **include** the library header file:

    ```cpp
    #include <HCMS39xx.h>
    ```

2. **instantiate** an HCMS39xx object:

    ```cpp
    HCMS39xx myDisplay(uint8_t num_chars, uint8_t data_pin, uint8_t rs_pin, uint8_t clk_pin, 
                       uint8_t ce_pin, uint8_t blank_pin = NO_PIN, uint8_t osc_select_pin = NO_PIN);
    ```

    `blank_pin` and `osc_select_pin` have default values defined in the constructor, and therefore do not need to be included if they are not used (i.e., the are hardwired to specific logic levels instead of controlled by the microcontroller). It is also possible to use the enumeration `HCMS39xx::NO_PIN` if they are included in the constructor.

    The constructor sets the OSCILLATOR pin (if defined) to internal oscillator, the CE pin to HIGH (inactive), and the BLANK pin (if defined) to blank the display.

3. **initialize** the object (typically within `setup()`):

    ```cpp
    myDisplay.begin();
    ```

    `begin()` initializes the LED hardware with default values:

    - Dot register is set to all 0s (all pixels off)
    - Control word 0 set to wakeup mode, default current (4.0 mA), default brightness (47%)
    - Control word 1 set to simultaneous mode and external prescaler turned off

4. Use the available **library methods** listed below to control the display.

### Library Methods

```cpp
  void print(const char* s);  // Print the characters pointed to by s to the display.
  void print(int j); // Prints the integer j. See below for more details, including other integer types supported.
  void printDirect(const uint8_t* d, uint8_t len); // Send the pixel data pointed to by d to the display. Direct pixel control of the display. 
  void clear();  // Clear the display (by writing all 0s to the dot register). [default]
  void displaySleep();  // Put the display into low power sleep mode. This turns off the pixel drivers and the internal oscillator. 
  void displayWakeup();  // Wake up the display. [default]
  void displayBlank();  // Blank the display using the blank pin. [default]
  void displayUnblank();  // Unblank the display using the blank pin -- need to unBlank() after begin().
  void setBrightness(uint8_t value); // Set display brightness level, where 0 <= value <= 15. [default is 12 (47%)]
  void setCurrent(DISPLAY_CURRENT value); // Set display current level. See header file for enumerated definitions for value. [default is 4.0 mA]
  void setExtOsc();  // Enable external oscillator with the oscillator select pin.
  void setIntOsc();  // Enable internal oscillator with the oscillator select pin. [default]
  void setExternalPrescaleDiv8();  // Set the external oscillator prescale divider to 8.
  void setExternalPrescaleNormal();  // Set the external oscillator prescale divider to 1. [default]
  void setSimultaneousMode();  // Enable simultaneous mode (only affects writes to control word registers). [default]
  void setSerialMode();  // Enable serial mode (only affects writes to control word registers).
```

`print()` is overloaded to support both string and integer parameters. The string version works as expected and prints the null-terminated string `s` to the display.

The integer version of `print()` is actually four separate functions to suport parameter types of `int`, `unsigned int`, `long`, and `unsigned long` integers. When using the integer version, if the number to be displayed has fewer digits than the display supports, then the number is left-padded with spaces. If the number is larger than can be represented on the display, then only the left-most digits will be shown. For example, when using a 4-digit display, `12345` will be displayed as `1234` and `8` will be displayed as `<spc><spc><spc>8` (3 spaces and the numeral 8). For more complex display combinations, use [`snprintf()`][8] to format a string into a character buffer, and use `print(const char* s)` to display the formatted character buffer `s`.

## Implementation Details

- The display is blanked when the object instance is initially created (if blank_pin is defined), so you need to use the `displayUnblank()` method before any characters will actually be visible on the display.
- The pixels are controlled bottom to top. The most significant bit (bit 7) of the dot data byte is unused. So to turn on the bottom pixel in a column, use a value of `0x40` with the `printDirect()` method. To turn on the top pixel, use `0x01`.
- The pixel map for the 5x7 font included with this library is adapted from Broadcom Application Brief [D-003][3] Table 1. A handful of pixel definitions were changed to make them look a little better than the ones defined in the table.
- The `font5x7[]` pixel map can be accessed from sketches if you `#include "font5x7.h"`. It is therefore possible to create some effects like overlaying a strikethrough line across charactesr by reading the pixel data from `font5x7[]` and ORing it with `0x10` and using printDirect() to send the resulting pixels to the display.
- The pixel map in `font5x7.h` can be modified by adding character pixel maps to the bottom of the array. Note that you should also update the "last ASCII character" value at array position \[1\] if you add characters.
- It is also possible to save flash memory by decreasing the size of the `font5x7[]` array. In particular, 31 bytes can be saved by removing the symbols less than ASCII 0x20 (since those technically aren't ASCII characters anyway). Just be sure to update the "ASCII offset" value at array position \[0\].

Review the [datasheet][1] for other chip operation details.

## To-Do

1. Fix support for character values > 127 (signed/unsigned datatype issue). 
2. Test library (both examples) on both Arduino and Energia before publishing.

## References

- Broadcom HCMS-39xx series [datasheet][1] and HCMS-3964 [product page][2]
- Broadcom [Application Note][3] with ASCII font encoding data
- Broadcom HCMS-29xx series [datasheet][4]
- Arduino [Playground][6] description of the [LedDisplay library][5]

## License

The software and other files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE`][101] in this repository.

[1]:https://docs.broadcom.com/docs/AV02-0868EN
[2]:https://www.broadcom.com/products/leds-and-displays/smart-alphanumeric-displays/serial-interface/hcms-3964
[3]:https://docs.broadcom.com/doc/5988-7539EN
[4]:https://docs.broadcom.com/doc/HCMS-29xx-Series-High-Performance-CMOS-5-x-7-Alphanumeric-Displays
[5]:https://github.com/PaulStoffregen/LedDisplay
[6]:https://playground.arduino.cc/Main/LedDisplay/
[7]:./examples
[8]:https://www.cplusplus.com/reference/cstdio/snprintf/?kw=snprintf
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE
[200]: https://github.com/Andy4495/HCMS39xx
