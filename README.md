# HCMS39xx Library

This library interfaces with Broadcom/Avago [HCMS-39xx][2] series LED displays.

There are multiple displays in this series with various colors and display characters.

The library was specifically tested with a single HCMS-3964 display (4 character, 0.2", orange), but should work with other displays in the series. It should also work with multiple displays cascaded together.

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

1. Research different num_chars and daisy-chaining of devices in family. May need to update code in begin() and elsewhere to account for different display sizes and cascading. Either 4 chars or 8 chars per device. Each device has one IC per 4 chars. So just divide by 4 to get number of controller ICs. DONE.
2. No error checking for size of string sent to display. DOESN'T MATTER. String is declared as const. 
3. Make BLANK pin optional with NO_PIN default constructor. DONE
4. Add font table and support for characters (implement print() method). DONE
5. Implement setPrescale(). DONE
6. Add and implement setSerialMode(). DONE 
7. Explain the ordering of pixels in this README. Bottom pixels are most significant (i.e., pixels start from bottom left to upper right by column)
8. Document default settings when device is created and after begin(). Pixels, control word 0, control word 1. 

## References

+ HCMS-39xx [datasheet][1]

## License

The software and other files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE`][101] in this repository.

[1]:https://docs.broadcom.com/docs/AV02-0868EN
[2]:https://www.broadcom.com/products/leds-and-displays/smart-alphanumeric-displays/serial-interface/hcms-3964
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE
[200]: https://github.com/Andy4495/HCMS39xx
