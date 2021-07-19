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

    The constructor sets the oscillator pin (if defined) to internal oscillator, the CE pin to HIGH (inactive), and the blank pin (if defined) to blank the display.

3. **initialize** the object (typically within `setup()`):

    ```cpp
    myDisplay.begin();
    ```

    `begin()` initializes the LED hardware with default values:

    - Dot register is set to all 0s (all pixels off)
    - Control word 0 set to sleep mode, default current (4.0 mA), default brightness (47%)
    - Control word 1 set to simultaneous mode and external prescaler turned off

4. Use the available **library methods** listed below to control the display.

### Library Methods

```cpp
  void print(const char* s);  // Print the characters pointed to by s to the display.
  void printDirect(const uint8_t* d, uint8_t len); // Send the pixel data pointed to by d to the display. Direct pixel control of the display. 
  void clear();  // Clear the display (by writing all 0s to the dot register)
  void displaySleep();  // Put the display into low power sleep mode. This turns off the pixel drivers and the internal oscillator. 
  void displayWakeup();  // Wake up the display
  void displayBlank();  // Blank the display using the blank pin.
  void displayUnblank();  // Unblank the display using the blank pin
  void setBrightness(uint8_t value); // 0 <= value <= 15
  void setCurrent(DISPLAY_CURRENT value); // See header file for enumerated definitions for value
  void setExtOsc();  // Enable external oscillator with the oscillator select pin
  void setIntOsc();  // Enable internal oscillator with the oscillator select pin
  void setExternalPrescaleDiv8();  // Set the external oscillator prescale divider to 8
  void setExternalPrescaleNormal();  // Set the external oscillator prescale divider to 1
  void setSimultaneousMode();  // Enable simultaneous mode (only affects writes to control word registers)
  void setSerialMode();  // Enable serial mode (only affects writes to control word registers)
```

Note that the display is blanked when it is initially created (if blank_pin is defined), so you need to use the `displayUnblank()` method before any characters will actually be visible on the display.

## Implementation Details

- The pixels are controlled bottom to top. The most significant bit (bit 7) of the dot data byte is unused. So to turn on the bottom pixel in a column, use a value of `0x40` with the `printDirect()` method. To turn on the top pixel, use `0x01`.
- The pixel map for the 5x7 font included with this library is adapted from Broadcom Application Brief [D-003][3] Table 1. I changed a handful of pixel definitions to make them look a little better than the ones defined in the table.
- The `font5x7[]` pixel map can be accessed from sketches. It is therefore possible to create some effects like overlaying a strikethrough line across charactesr by reading the pixel data from `font5x7[]` and ORing it with `0x10` and using printDirect() to send the resulting pixels to the display.
- The pixel map in `font5x7.h` can be modified by adding character pixel maps to the bottom of the array. Note that you should also update the "last ASCII character" value at array position \[1\] if you add characters.
- It is also possible to save flash memory by decreasing the size of the `font5x7[]` array. In particular, 31 bytes can be saved by removing the symbols less than ASCII 0x20 (since those technically aren't ASCII characters anyway). Just be sure to update the "ASCII offset" value at array position \[0\].

Review the [datasheet][1] for other chip operation details.

## To-Do

1. Add print methods for ints and floats (I think I only need one each, as automatic casting will cover other cases).
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
[7]:../examples
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE
[200]: https://github.com/Andy4495/HCMS39xx
