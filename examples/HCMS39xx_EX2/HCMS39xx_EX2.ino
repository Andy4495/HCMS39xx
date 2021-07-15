/* -----------------------------------------------------------------

   Example 2 - Display each every character in font file

   HCMS39xx Library
   https://github.com/Andy4495/HCMS39xx

   07/11/21 - A.T. - Original

*/
#include "HCMS39xx.h"

// HCMS39xx(uint8_t num_chars, uint8_t data_pin, uint8_t rs_pin, uint8_t clk_pin,
//          uint8_t ce_pin, uint8_t blank_pin = NO_PIN, uint8_t osc_select_pin = NO_PIN)
HCMS39xx myDisplay(4, 6, 7, 8, 9, 10); // osc_select_pin tied high, not connected to microcontroller

// Since we are only displaying one character at a time, then displaydata
// can be pretty small. If you want to test printing longer strings at
// a time, then size MAXLEN accordingly, makeing sure to allocate space
// for the NULL terminator. 
#define MAXLEN 2
char displaydata[MAXLEN];

void setup() {
  Serial.begin(9600);
  myDisplay.begin();
  myDisplay.displayUnblank();

  Serial.println("Character Font Test");
}

void loop() {
  int i;

  displaydata[1] = 0; // Null terminate

  for (i = 32; i < 128; i++) {
    displaydata[0] = i;
    myDisplay.print(displaydata);
    Serial.print(displaydata);
    Serial.print(" ");
    delay(200);
  }
  Serial.println("");
}