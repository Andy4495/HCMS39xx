# HCMS39xx Library

This library interfaces with Broadcom/Avago [HCMS-39xx][2] and [HCMS-29xx][4] series LED displays.

There are multiple displays in this series with various colors and display characters.

The library was specifically tested with a single HCMS-3964 display (4 character, 0.2", orange), but should work with other displays in the either the 29xx or 39xx series. It should also work with multiple displays cascaded together.

## Usage

_Be sure to review the example sketch included with the library._

First, **include** the library header file:

```cpp
#include <HCMS39xx.h>
```

Next, **instantiate** an HCMS39xx object:

```cpp
HCMS39xx myDisplay(...TBD...);
```

Then **initialize** the object (typically within `setup()`):

```cpp
myDisplay.begin();
```

### Library Methods

```cpp
... Add supported methods here ...
```

Note that display is blanked when initially created (if blank_pin is defined), so need to use the Unblank() method to display characters.

## To-Do

1. Explain the ordering of pixels in this README. Bottom pixels are most significant (i.e., pixels start from bottom left to upper right by column)
2. Document default settings when device is created and after begin(). Pixels, control word 0, control word 1.
3. Compare to Stoffregen/Igoe library: pixel current control, blank pin, uses more RAM for pixel data (20 bytes per 4 chars) and string (5 bytes). Check memory usage compiling with each library.
4. Use pixel map from Broadcom App Note. DONE
5. Add a clear() method, test, and add to EX1. DONE
6. Add print methods for ints and floats (I think I only need one each, as automatic casting will cover other cases).
7. Mention in readme that Font5x7 array is available to code, and could potentially be used in a sketch (e.g., to find the last array element, specific bit maps to use if using printDirect to add strikethrough to letters, etc)
8. Test library (both examples) on both Arduino and Energia before publishing.

## References

+ HCMS-39xx [datasheet][1]
+ Broadcom [Application Note][3] with ASCII font encoding data.
+ Broadcom 29xx series [datasheet][4]

## License

The software and other files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE`][101] in this repository.

[1]:https://docs.broadcom.com/docs/AV02-0868EN
[2]:https://www.broadcom.com/products/leds-and-displays/smart-alphanumeric-displays/serial-interface/hcms-3964
[3]:https://docs.broadcom.com/doc/5988-7539EN
[4]:https://docs.broadcom.com/doc/HCMS-29xx-Series-High-Performance-CMOS-5-x-7-Alphanumeric-Displays
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE
[200]: https://github.com/Andy4495/HCMS39xx
