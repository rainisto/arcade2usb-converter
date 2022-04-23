/*
 * c64mini-arcade2usb-converter.ino
 * 
 * C64 Mini Arcade Joystick to USB Converter.
 *  
 * Version 1.1 (See CHANGELOG)
 * Release date: 2019-04-19
 *  
 * Written & released by M.F. Wieland (TSB)
 *  
 *  Licensed under the MIT License. See LICENSE file in the project root for full license information.  
 */

#include "joystick.h" 
#include <EEPROM.h>

// define buttons
const int pin_joystick_up = 2;
const int pin_joystick_down = 3;
const int pin_joystick_left = 4;
const int pin_joystick_right = 5;
const int pin_joystick_button1 = 6;

const int pin_joystick_button_menu = 7;
const int pin_joystick_button_a = 8;
const int pin_joystick_button_b = 9;
const int pin_joystick_button_c = 10;


int debounceDelay = 1; // debounce delay
int value = 0; // A500 mode
int counter = 0;

void setup() {
  
  // setup input pins
  pinMode(pin_joystick_up, INPUT_PULLUP);       
  pinMode(pin_joystick_down, INPUT_PULLUP);       
  pinMode(pin_joystick_left, INPUT_PULLUP);       
  pinMode(pin_joystick_right, INPUT_PULLUP);       
  pinMode(pin_joystick_button1, INPUT_PULLUP);       
  pinMode(pin_joystick_button_menu, INPUT_PULLUP);       
  pinMode(pin_joystick_button_a, INPUT_PULLUP);       
  pinMode(pin_joystick_button_b, INPUT_PULLUP);       
  pinMode(pin_joystick_button_c, INPUT_PULLUP);       
  // read A500 value
  value = EEPROM.read(0);
}

void loop() {
  
  // Reset values for next loop..  
  Joystick.reset();
  counter = 0;

  // Button - Fire (1)
  if (digitalRead(pin_joystick_button1) == LOW) {
    
    if (value == 1) Joystick.button_press(0x4);
    else Joystick.button_press(0x40);
    // see documentation to use other device
    counter = counter+1;
  }
  
  // Button - Menu
  if (digitalRead(pin_joystick_button_menu) == LOW) {
    
    Joystick.button_press(0x0200);
    // see documentation to use other device
    counter = counter+2;
  }

  // Button - C
  if (digitalRead(pin_joystick_button_a) == LOW) {
    
    Joystick.button_press(0x0100);
    // see documentation to use other device
  }
  
  // Button - B (not in use at this moment)
  if (digitalRead(pin_joystick_button_b) == LOW) {
    
    Joystick.button_press(0x2);
    // see documentation to use other device
  }

  // Button - A
  if (digitalRead(pin_joystick_button_c) == LOW) {
    
    Joystick.button_press(0x4);
    // see documentation to use other device
    if (counter == 1) {
      value = 0;
      EEPROM.write(0, 0);
    } else if (counter == 3) {
      value = 1;
      EEPROM.write(0, 1);
    }
  }

  // Button - Up
  if (digitalRead(pin_joystick_up) == LOW) {
    
    Joystick.up();  
  }
  
  // Button - Right
  if (digitalRead(pin_joystick_right) == LOW) {
    Joystick.right();
  }
  
  // Button - Down
  if (digitalRead(pin_joystick_down) == LOW) {
    
    Joystick.down();
  }
  
  // Button - Left
  if (digitalRead(pin_joystick_left) == LOW) {

    Joystick.left();
  }
  
  // update joystick actions
  Joystick.usb_update();
  
  delay(debounceDelay);
}
