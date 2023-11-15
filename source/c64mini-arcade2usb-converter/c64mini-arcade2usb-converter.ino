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
 *  Version 1.2 (multibutton variant)
 *  Release date: 2023-11-04
 *
 *  Written & released by Jonni Rainisto (rainisto@iki.fi)
 *  
 *  Licensed under the MIT License. See LICENSE file in the project root for full license information.  
 */

#include "joystick.h" 

// define buttons
const int pin_joystick_up      = 2;
const int pin_joystick_down    = 3;
const int pin_joystick_left    = 4;
const int pin_joystick_right   = 5;
const int pin_joystick_button1 = 6;

const int pin_joystick_button_menu = 7;  // MENU-button
const int pin_joystick_button_a    = 21; // A-Button (A3)
const int pin_joystick_button_b    = 20; // B-Button (A2)
const int pin_joystick_button_c    = 8;  // C-Button
const int pin_joystick_button_u6   = 9;  // Y-Button or left triangle
const int pin_joystick_button_u7   = 10; // X-Button or right triangle
const int pin_joystick_button_u8   = 15; // AutoFire multiplier-Button
const int pin_joystick_button_sw1  = 16; // TheC64 mode vs A500 mini mode-Switch
const int pin_joystick_button_sw2  = 14; // AutoFire enable-Switch
const int pin_joystick_button_sw4  = 18; // Unused (A0)
const int pin_joystick_button_sw5  = 19; // Unused (A1)

// parallel placeholder for switch in multiplier button pin
//const int pin_joystick_button_sw3  = 15;

int debounceDelay = 1; // debounce delay
int a500_mode = 0;
int autofire_multiplier = 1;
int autofire_enabled = 0;
int autofire_counter = 0;
int autofire_previous = 0;

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
  pinMode(pin_joystick_button_u6, INPUT_PULLUP);
  pinMode(pin_joystick_button_u7, INPUT_PULLUP);
  pinMode(pin_joystick_button_u8, INPUT_PULLUP);
  pinMode(pin_joystick_button_sw1, INPUT_PULLUP);
  pinMode(pin_joystick_button_sw2, INPUT_PULLUP);
  pinMode(pin_joystick_button_sw4, INPUT_PULLUP);
  pinMode(pin_joystick_button_sw5, INPUT_PULLUP);
}

void loop() {
  
  // Reset values for next loop..  
  Joystick.reset();

  // check if a500-mode or not
  if (a500_mode == 0 && digitalRead(pin_joystick_button_sw2) == LOW) {
    a500_mode = 1; // set A500 mini mode
  } else if (a500_mode == 1 && digitalRead(pin_joystick_button_sw2) == HIGH) {
    a500_mode = 0;
  }

  if (autofire_enabled == 0 && digitalRead(pin_joystick_button_sw5) == LOW) {
    autofire_enabled = 1;
  } else if (autofire_enabled == 1 && digitalRead(pin_joystick_button_sw5) == HIGH) {
    autofire_enabled = 0;
  }

  // Button - Fire (1)
  if (digitalRead(pin_joystick_button1) == LOW) {
    if (a500_mode == 1) { // if A500 mini mode
      Joystick.button_press(0x4); // send A (Mapped as fire in A500 mini mode)
    } else {
      Joystick.button_press(0x40); // FIRE-button event on TheC64 Maxi/Mini
    }
  } else if (autofire_enabled) {
    if (autofire_counter < 300) {
      if (a500_mode == 1) { // if A500 mini mode
        Joystick.button_press(0x4); // send A (Mapped as fire in A500 mini mode)
      } else {
        Joystick.button_press(0x40); // FIRE-button event on TheC64 Maxi/Mini
      }
    }
    if (autofire_counter > 600) {
      autofire_counter = 0;
    }
    autofire_counter = autofire_counter + autofire_multiplier;
  }
  
  // Button - Menu - (Home in A500 mode)
  if (digitalRead(pin_joystick_button_menu) == LOW) {
    Joystick.button_press(0x0200);
  }

  // Button - C - (Settings Menu in A500 mode)
  if (digitalRead(pin_joystick_button_c) == LOW) {
    Joystick.button_press(0x0100);
  }

  // Button - X (A500) or Left triangle (TheC64)
  if (digitalRead(pin_joystick_button_u6) == LOW) {
    Joystick.button_press(0x8);
  }

  // Button - Y (A500) or Right triangle (TheC64)
  if (digitalRead(pin_joystick_button_u7) == LOW) {
    Joystick.button_press(0x1);
  }

  // Button - Autofire multiplier setter
  if (digitalRead(pin_joystick_button_u8) == LOW) {
    if ( autofire_multiplier < 6) {
      if ( ! autofire_previous ) {
        autofire_multiplier = autofire_multiplier + 1;
        autofire_previous = 1;
      }
    } else {
      autofire_multiplier = 1;
    }
  } else {
    autofire_previous = 0;
  }

  // Button - B
  if (digitalRead(pin_joystick_button_b) == LOW) {
    Joystick.button_press(0x2);
  }

  // Button - A
  if (digitalRead(pin_joystick_button_a) == LOW) {
    Joystick.button_press(0x4);
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
