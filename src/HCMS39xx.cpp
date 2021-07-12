/* -----------------------------------------------------------------
   HCMS39xx Library
   https://github.com/Andy4495/HCMS39xx

   07/11/21 - A.T. - Original

*/

#include "HCMS39xx.h"
#include "font5x7.h"

HCMS39xx::HCMS39xx(uint8_t num_chars, uint8_t data_pin, uint8_t rs_pin, uint8_t clk_pin, 
                   uint8_t ce_pin, uint8_t blank_pin, uint8_t osc_select_pin) {

    _num_chars      = num_chars;
    _data_pin       = data_pin; 
    _clk_pin        = clk_pin; 
    _rs_pin         = rs_pin; 
    _ce_pin         = ce_pin; 
    _blank_pin      = blank_pin; 
    _osc_select_pin = osc_select_pin; 

    pinMode(_data_pin, OUTPUT); 
    digitalWrite(_clk_pin, LOW); 
    pinMode(_clk_pin, OUTPUT); 
    pinMode(_rs_pin, OUTPUT);
    digitalWrite(_ce_pin, HIGH); 
    pinMode(_ce_pin, OUTPUT); 
    if (_blank_pin != NO_PIN) {
        digitalWrite(_blank_pin, HIGH); // default is for display to be blanked when initialized
        pinMode(_blank_pin, OUTPUT); 
    }
    if (_osc_select_pin != NO_PIN) {
        pinMode(_osc_select_pin, OUTPUT); 
    }
}

void HCMS39xx::begin() {
    uint8_t i; 

    // Set all dot values to LOW
    digitalWrite(_clk_pin, HIGH); 
    digitalWrite(_rs_pin, LOW); 
    digitalWrite(_ce_pin, LOW); 
    for (i = 0; i < _num_chars * COLUMNS_PER_CHAR; i++) {
        send_byte(0); 
    }
    digitalWrite(_ce_pin, HIGH); 
    digitalWrite(_clk_pin, LOW); 

    // Load control word 0 with desired brightness and set sleep bit HIGH
    _control_word0 = WAKEUP | DEFAULT_BRIGHTNESS | DEFAULT_CURRENT; 
    // send_control(_control_word0);
    digitalWrite(_clk_pin, HIGH); 
    digitalWrite(_rs_pin, HIGH); 
    digitalWrite(_ce_pin, LOW); 
    for (i = 0; i < _num_chars / CHARS_PER_DEVICE; i++) {
        send_byte(_control_word0);
    }
    digitalWrite(_ce_pin, HIGH); 
    digitalWrite(_clk_pin, LOW);

    // Load control word 1 with serial mode and external prescale normal
    _control_word1 = CONTROL_WORD1 | 0; 
    // send_control(_control_word1); 
    digitalWrite(_clk_pin, HIGH); 
    digitalWrite(_rs_pin, HIGH); 
    digitalWrite(_ce_pin, LOW); 
    for (i = 0; i < _num_chars / CHARS_PER_DEVICE; i++) {
        send_byte(_control_word1);
    }
    digitalWrite(_ce_pin, HIGH); 
    digitalWrite(_clk_pin, LOW);
}

void HCMS39xx::print(const char* s) {
    uint8_t i; 

    for (i = 0; i < _num_chars; i++) { // Don't loop for more chars in the display
        if (s[i] != 0) {
            sendDotData(Font5x7 + (s[i] - 32) * 5, 5);
        }
        else  { // If we find a NULL terminator, then exit out
            break;
        }
    }
}

void HCMS39xx::printDirect(const uint8_t* s, uint8_t len) {
    sendDotData(s, len);
}

void HCMS39xx::displaySleep() {
    // Save current control word 1 value so we can temporarily turn on simultaneous mode
    uint8_t temp_control_word1 = _control_word1; 

    setSimultaneousMode(); // Turn on simultaneous 
    _control_word0 = _control_word0 & ~SLEEP_MASK; 
    send_control(_control_word0);
    _control_word1 = temp_control_word1; // Restore previous setting
    send_control(_control_word1); 
}

void HCMS39xx::displayWakeup() {
    // Save current control word 1 value so we can temporarily turn on simultaneous mode
    uint8_t temp_control_word1 = _control_word1; 

    setSimultaneousMode(); // Turn on simultaneous 
    _control_word0 = _control_word0 | SLEEP_MASK; 
    send_control(_control_word0);
    _control_word1 = temp_control_word1; // Restore previous setting
    send_control(_control_word1); }

void HCMS39xx::displayBlank() {
    if (_blank_pin != NO_PIN) {
        digitalWrite(_blank_pin, HIGH); 
    }
}

void HCMS39xx::displayUnblank() {
    if (_blank_pin != NO_PIN) {
        digitalWrite(_blank_pin, LOW); 
    }
}

void HCMS39xx::setBrightness(uint8_t value) {
    // Save current control word 1 value so we can temporarily turn on simultaneous mode
    uint8_t temp_control_word1 = _control_word1; 

    setSimultaneousMode(); // Turn on simultaneous 
    _control_word0 = (_control_word0 & ~BRIGHTNESS_MASK) | (value & BRIGHTNESS_MASK); 
    send_control(_control_word0); 
    _control_word1 = temp_control_word1; // Restore previous setting
    send_control(_control_word1); }

void HCMS39xx::setCurrent(uint8_t value) {
    // Save current control word 1 value so we can temporarily turn on simultaneous mode
    uint8_t temp_control_word1 = _control_word1; 

    setSimultaneousMode(); // Turn on simultaneous 
    _control_word0 = (_control_word0 & ~PIXEL_CURRENT_MASK) | ((value << 4) & PIXEL_CURRENT_MASK); 
    send_control(_control_word0); 
    _control_word1 = temp_control_word1; // Restore previous setting
    send_control(_control_word1); }

void HCMS39xx::setIntOsc() {
    if (_osc_select_pin != NO_PIN) {
        digitalWrite(_osc_select_pin, HIGH); 
    }
}

void HCMS39xx::setExtOsc() {
    if (_osc_select_pin != NO_PIN) {
        digitalWrite(_osc_select_pin, LOW); 
    }
}

void HCMS39xx::setExternalPrescaleDiv8() {
    // Save current control word 1 value so we can temporarily turn on simultaneous mode
    uint8_t temp_control_word1 = _control_word1; 

    setSimultaneousMode(); // Turn on simultaneous 
    _control_word1 = _control_word1 | EXT_PRESCALER_DIV8; 
    send_control(_control_word1); 
    _control_word1 = temp_control_word1; // Restore previous setting
    send_control(_control_word1); 
}

void HCMS39xx::setExternalPrescaleNormal() {
    // Save current control word 1 value so we can temporarily turn on simultaneous mode
    uint8_t temp_control_word1 = _control_word1; 

    setSimultaneousMode(); // Turn on simultaneous 
    _control_word1 = _control_word1 & ~EXT_PRESCALER_DIV8;
    send_control(_control_word1); 
    _control_word1 = temp_control_word1; // Restore previous setting
    send_control(_control_word1);    
}

void HCMS39xx::setSimultaneousMode() {
    uint8_t i; 
    
    for (i = 0; i < _num_chars / CHARS_PER_DEVICE; i++) {
        _control_word1 = _control_word1 | DATA_OUT_MODE_SIMUL; 
        send_control(_control_word1); 
    }
}

void HCMS39xx::setSerialMode() {
     uint8_t i; 

    for (i = 0; i < _num_chars / CHARS_PER_DEVICE; i++) {   
        _control_word1 = _control_word1 & ~DATA_OUT_MODE_SIMUL;
        send_control(_control_word1);
    }
}

void HCMS39xx::sendDotData(const uint8_t *b, uint8_t length) {
    uint8_t i; 

    digitalWrite(_clk_pin, HIGH); 
    digitalWrite(_rs_pin, LOW); 
    digitalWrite(_ce_pin, LOW); 
    for (i = 0; i < length; i++) {
        send_byte(b[i]); 
    }
    digitalWrite(_ce_pin, HIGH); 
    digitalWrite(_clk_pin, LOW); 
}

void HCMS39xx::sendControlData(const uint8_t* b, uint8_t length) {
    uint8_t i; 

    digitalWrite(_clk_pin, HIGH); 
    digitalWrite(_rs_pin, HIGH); 
    digitalWrite(_ce_pin, LOW); 
    for (i = 0; i < length; i++) {
        send_byte(b[i]); 
    }
    digitalWrite(_ce_pin, HIGH); 
    digitalWrite(_clk_pin, LOW);    
}

void HCMS39xx::send_control(uint8_t b) {
    digitalWrite(_clk_pin, HIGH); 
    digitalWrite(_rs_pin, HIGH); 
    digitalWrite(_ce_pin, LOW); 
    send_byte(b);
    digitalWrite(_ce_pin, HIGH); 
    digitalWrite(_clk_pin, LOW); 
}

void HCMS39xx::send_byte(uint8_t b) {
    uint8_t i; 

    for (i = 0; i < 8; i++) {
        digitalWrite(_clk_pin, LOW); 
        digitalWrite(_data_pin, b & 0x80); // msb first
        digitalWrite(_clk_pin, HIGH); 
        b = b << 1; 
    }
}