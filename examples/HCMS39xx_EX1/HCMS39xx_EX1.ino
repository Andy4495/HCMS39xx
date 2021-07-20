/* -----------------------------------------------------------------

   Example 1 - Example usage of most library features.

   HCMS39xx Library
   https://github.com/Andy4495/HCMS39xx

   07/11/21 - A.T. - Original

*/

#include "HCMS39xx.h"
#include "font5x7.h"   // This is only needed for example 15 below

// HCMS39xx(uint8_t num_chars, uint8_t data_pin, uint8_t rs_pin, uint8_t clk_pin, 
//          uint8_t ce_pin, uint8_t blank_pin = NO_PIN, uint8_t osc_select_pin = NO_PIN)
HCMS39xx myDisplay(4, 6, 7, 8, 9, 10); // osc_select_pin tied high, not connected to microcontroller

#define MAXLEN 20
uint8_t displaydata[MAXLEN];

void setup() {
  Serial.begin(9600);
  myDisplay.begin();          // Initialize the display
  myDisplay.displayUnblank(); // Default configuration blanks the display, so need to unblank before using

  Serial.println("HCMS39xx Library Example 1");
}

void loop() {
  int i;

  Serial.println("Example 1: All pixels on.");
  // Fill all the pixel bits with 1s
  for (i = 0; i < MAXLEN; i++) {
    displaydata[i] = 0xFF;
  }
  // The printDirect() method is used to individually control pixels 
  // See print() in examples 13 and 14 below for ASCII characters 
  myDisplay.printDirect(displaydata, MAXLEN);
  delay(3000);

  Serial.println("Example 2: Sleep mode. Display turns off, OSC pin output should stop oscillating.");
  // Sleep mode puts the chip in low power mode, turning off the pixel drivers and internal oscillator
  myDisplay.displaySleep();
  delay(3000);

  Serial.println("Example 3: Wake up. All pixels back on, OSC pin output oscillating again.");
  // Wake up the display -- pixel memory is retained during sleep.
  myDisplay.displayWakeup();
  delay(3000);

  Serial.println("Example 4: Blank the display with the BL pin. OSC pin keeps oscillating.");
  // displayBlank() has no effect if blank_pin was defined as "NO_PIN" in the constructor
  myDisplay.displayBlank();
  delay(2000);

  Serial.println("Example 5: Un-blank display with the BL pin.");
  myDisplay.displayUnblank();
  delay(2000);

  Serial.println("Example 6: Columns alternating on/off. Leftmost column on.");
  // Fill alternating colums with 1s and 0s
  for (i = 1; i < MAXLEN; i = i + 2) {
    displaydata[i] = 0;
  }
  myDisplay.printDirect(displaydata, MAXLEN);
  delay(3000);

  Serial.println("Example 7: Rows alternating on/off. Topmost row on.");
  // Note that msb of column byte is the bottom (unused) row (see datasheet)
  for (i = 0; i < MAXLEN; i++) {
    displaydata[i] = 0x55;
  }
  myDisplay.printDirect(displaydata, MAXLEN);
  delay(3000);

  Serial.println("Example 8: 1 column in left character, 2 columns in 2nd character, etc.");
  for (i = 0; i < MAXLEN; i++) {
    displaydata[i] = 0;
  }
  displaydata[4]  = 0xFF;
  displaydata[8]  = 0xFF;
  displaydata[9]  = 0xFF;
  displaydata[12] = 0xFF;
  displaydata[13] = 0xFF;
  displaydata[14] = 0xFF;
  displaydata[16] = 0xFF;
  displaydata[17] = 0xFF;
  displaydata[18] = 0xFF;
  displaydata[19] = 0xFF;
  myDisplay.printDirect(displaydata, MAXLEN);
  delay(3000);

  Serial.println("Example 9: Scroll a character at a time.");
  // Each call to printDirect is a single scroll.
  // So sending 5 columns at a time will scroll one character
  // Note that because the operation is so fast, you can't actually 
  // see the scroll unless you add a delay.
  displaydata[15] = 0xFF;
  myDisplay.printDirect(displaydata + 15, 5);
  delay(1000);
  displaydata[19] = 0;
  myDisplay.printDirect(displaydata + 15, 5);
  delay(1000);
  displaydata[18] = 0;
  myDisplay.printDirect(displaydata + 15, 5);
  delay(1000);
  displaydata[17] = 0;
  myDisplay.printDirect(displaydata + 15, 5);
  delay(1000);
  displaydata[16] = 0;
  myDisplay.printDirect(displaydata + 15, 5);
  delay(1000);

  Serial.print("Example 10: Scroll a column at a time, first slowly...");
  for (i = 0; i < 15; i++) {
    displaydata[0] = 0;
    myDisplay.printDirect(displaydata, 1);
    delay(500);
  }
  delay(1000);
  Serial.println("then fast.");
  for (i = 0; i < MAXLEN; i++) {
    displaydata[0] = 0xFF;
    myDisplay.printDirect(displaydata, 1);
    delay(50); // Note that zero delay is too fast -- just looks like you printed all the pixels at once.
  }
  for (i = 0; i < MAXLEN; i++) {
    displaydata[0] = 0;
    myDisplay.printDirect(displaydata, 1);
    delay(50); // Note that zero delay is too fast -- just looks like you printed all the pixels at once.
  }
  delay(500);

  Serial.println("Example 11: Brightness control. All pixels on, dimmest to brighExample (16 steps, 1 second per step).");
  // Setting brightness using internal PWM
  // Per the datasheet, brightness level 0 is 0% brightness 
  // -- so a setting of zero brightness is another way to blank the display
  for (i = 0; i < MAXLEN; i++) {
    displaydata[i] = 0xFF;
  }
  myDisplay.setBrightness(0);
  myDisplay.printDirect(displaydata, MAXLEN);
  Serial.print("    Brightness Level: ");
  for (i = 0; i < 16; i++) {
    Serial.print(i);
    Serial.print("...");
    myDisplay.setBrightness(i);
    delay(1000);
  }
  Serial.println("");
  Serial.println("    Setting brightness back to default level.");
  myDisplay.setBrightness(HCMS39xx::DEFAULT_BRIGHTNESS);
  delay(2000);

  Serial.println("Example 12: Pixel current control. Lowest current to highest (4 steps, 1 second per step).");
  // Another way to control brightness
  Serial.print("    Pixel current 4.0mA...");
  myDisplay.setCurrent(HCMS39xx::CURRENT_4_0_mA);
  delay(1000);
  Serial.print("6.4mA...");
  myDisplay.setCurrent(HCMS39xx::CURRENT_6_4_mA);
  delay(1000);
  Serial.print("9.3mA...");
  myDisplay.setCurrent(HCMS39xx::CURRENT_9_3_mA);
  delay(1000);
  Serial.print("12.8mA...");
  myDisplay.setCurrent(HCMS39xx::CURRENT_12_8_mA);
  delay(1000);
  Serial.println("");
  Serial.println("    Setting current control back to default level.");
  myDisplay.setCurrent(HCMS39xx::DEFAULT_CURRENT);
  delay(2000);

  Serial.println("Example 13: Display characters 'AbCd' all at once.");
  // Use print() method to display ASCII chars. 
  myDisplay.print("AbCd");
  delay(3000);

  Serial.println("Example 14: Display characters 'EfGh' by scrolling.");
  myDisplay.print("E");
  delay(200); // Need a little delay or else you can't see the scroll
  myDisplay.print("f"); 
  delay(200);
  myDisplay.print("G");
  delay(200);
  myDisplay.print("h"); 
  delay(3000);

  Serial.println("Example 15: Manually create strikethrough characters."); 
  uint8_t p[5];  // there are 5 pixel columns per character
  for (i = 0; i < 5; i++) p[i] = pgm_read_byte(&font5x7[0x53 * 5 + i]) | 0x10;  // Add a strikethrough bar to existing font pixel definition
  myDisplay.printDirect(p, 5);
  for (i = 0; i < 5; i++) p[i] = pgm_read_byte(&font5x7[0x74 * 5 + i]) | 0x10;  // Add a strikethrough bar to existing font pixel definition
  myDisplay.printDirect(p, 5);
  for (i = 0; i < 5; i++) p[i] = pgm_read_byte(&font5x7[0x72 * 5 + i]) | 0x10;  // Add a strikethrough bar to existing font pixel definition
  myDisplay.printDirect(p, 5);
  for (i = 0; i < 5; i++) p[i] = pgm_read_byte(&font5x7[0x6B * 5 + i]) | 0x10;  // Add a strikethrough bar to existing font pixel definition
  myDisplay.printDirect(p, 5);
  delay(3000);

  Serial.println("Example 16: Print small integers -10 through 10. Number is right justified with padding to the left."); 
  Serial.print("  "); 
  for (i = -10; i < 11; i++) {
    myDisplay.print(i); 
    Serial.print(i); 
    Serial.print("...");
    delay(1000);
  }
  Serial.println(""); 
  delay(2000);

  Serial.println("Example 17: Print a large integer 4,294,967,295. Number is truncated on the right if there are not enough characters to display."); 
  myDisplay.print(4294967295UL); 
  sleep(3000);

  Serial.println("Example 18: Clear the display with clear() method.");
  myDisplay.clear();
  delay(3000);
}
