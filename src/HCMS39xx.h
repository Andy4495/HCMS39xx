/* -----------------------------------------------------------------
   HCMS39xx Library
   https://github.com/Andy4495/HCMS39xx

   07/11/21 - A.T. - Original

*/

#ifndef HCMS39xx_H
#define HCMS39xx_H

#include "Arduino.h"

class HCMS39xx { 
 
public:
  enum {NO_PIN = 255};
  enum {DEFAULT_BRIGHTNESS = 0x0C}; // 0x0C => HHLL -> 47% relative brightness
  ///enum {DEFAULT_CURRENT = 0x20};   // 4.0 mA peak pixel current (lowest setting) 
  enum DISPLAY_CURRENT {DEFAULT_CURRENT = 0x20, CURRENT_4_0_mA = 0x20, CURRENT_6_4_mA = 0x10, CURRENT_9_3_mA = 0x00, CURRENT_12_8_mA = 0x30}; 

  HCMS39xx(uint8_t num_chars, uint8_t data_pin, uint8_t rs_pin, uint8_t clk_pin, 
           uint8_t ce_pin, uint8_t blank_pin = NO_PIN, uint8_t osc_select_pin = NO_PIN);
  void begin(); 
  void print(const char*);
  void printDirect(const uint8_t*, uint8_t len);
  void clear();
  void displaySleep();
  void displayWakeup();
  void displayBlank();
  void displayUnblank();
  void setBrightness(uint8_t value);
  void setCurrent(DISPLAY_CURRENT value);
  void setExtOsc();
  void setIntOsc();
  void setExternalPrescaleDiv8();
  void setExternalPrescaleNormal();
  void setSimultaneousMode();
  void setSerialMode();

private:
  enum POWER_MODE {WAKEUP = 0x40, SLEEP = 0};
  enum {SLEEP_MASK = 0x40};
  enum {PIXEL_CURRENT_MASK = 0x30}; 
  enum {BRIGHTNESS_MASK = 0x0F}; 
  enum {CONTROL_WORD1  = 0x80}; 
  enum {EXT_PRESCALER_DIV8 = 0x02}; 
  enum {DATA_OUT_MODE_SIMUL = 0x01}; 
  enum {CHARS_PER_DEVICE = 4, COLUMNS_PER_CHAR = 5U};
  uint8_t _num_chars; 
  uint8_t _first_ascii_index; 
  uint8_t _data_pin, _clk_pin, _rs_pin, _ce_pin, _blank_pin, _osc_select_pin; 
  uint8_t _control_word0;
  uint8_t _control_word1; 

  void setupDotData();
  void setupControlData();
  void endTransmission();
  void sendFontData(const uint8_t *b, uint8_t length);
  void sendByte(uint8_t b);   
};

#endif