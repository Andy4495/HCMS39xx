# HCMS39xx Library

This library interfaces with Broadcom/Avago [HCMS-39xx][2] series LED displays.

There are multiple displays in this series with various colors and display characters.

The library was specifically tested with a single HCMS-3964 display, but should work with other displays in the series. It should also work with multiple displays cascaded together per the datasheet recommended configuration.

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

## To-Do

1. Research different num_chars and daisy-chaining of devices in family. May need to update code in begin() and elsewhere to account for different display sizes and cascading.
2. No error checking for size of string sent to display (does this matter?)

## References

+ HCMS-39xx [datasheet][1]

## License

The software and other files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE`][101] in this repository.

[1]:https://docs.broadcom.com/docs/AV02-0868EN
[2]:https://www.broadcom.com/products/leds-and-displays/smart-alphanumeric-displays/serial-interface/hcms-3964
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE
[200]: https://github.com/Andy4495/HCMS39xx
