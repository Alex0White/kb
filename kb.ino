#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "keycodes.h"
#include "keymap.h"
#include "bluetooth.h"
#include <SoftwareSerial.h>
#include "Adafruit_MCP23017.h"


#ifdef __AVR__
  #include <avr/power.h>
#endif

Adafruit_MCP23017 mcp0;
Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;


#define COLS 10

#define ROWS 9

#define DEBOUNCING_DELAY 10

uint16_t col_pins[COLS] = COL_PINS;
uint16_t row_pins[ROWS] = ROW_PINS;

uint16_t prev_states[ROWS] = { 0 };
uint16_t curr_states[ROWS] = { 0 };
uint16_t temp_states[ROWS] = { 0 };
uint8_t debouncing = DEBOUNCING_DELAY;
 
uint16_t keyboard_wire_to_virt[9][10][2] = {
  //1       2      3      4      5      6      7      8      9      10
  {{2,0}, {4,1}, {2,3}, {4,4}, {2,6}, {4,7}, {2,9}, {4,10}, {2,12}, {4,13}},
  
  {{3,0}, {3,1}, {3,3}, {3,4}, {3,6}, {3,7}, {3,9}, {3,10}, {3,12}, {3,13}},

  {{4,0}, {2,1}, {4,3}, {2,4}, {4,6}, {2,7}, {4,9}, {2,10}, {4,12}, {2,13}},

  {{5,0}, {2,2}, {5,3}, {2,5}, {5,6}, {2,8}, {5,9}, {2,11}, {5,12}, {2,14}},

  {{6,0}, {3,2}, {6,3}, {3,5}, {6,6}, {3,8}, {6,9}, {3,11}, {6,12}, {3,14}},

  {{7,0}, {4,2}, {7,3}, {4,5}, {7,6}, {4,8}, {7,9}, {4,11}, {7,12}, {4,14}},

  {{7,1}, {5,2}, {7,4}, {5,5}, {7,7}, {5,8}, {7,10}, {5,11}, {7,13}, {5,14}},

  {{6,1}, {6,2}, {6,4}, {6,5}, {6,7}, {6,8}, {6,10}, {6,11}, {6,13}, {6,14}},

  {{5,1}, {7,2}, {5,4}, {7,5}, {5,7}, {7,8}, {5,10}, {7,11}, {5,13}, {7,14}}
};

state_t IOExpander0KeyState[16] = { false };
state_t IOExpander1KeyState[16] = { false };
state_t IOExpander2KeyState[6] = { false };


/*
inline
state_t get_state(uint8_t row, uint8_t col) {
  return (curr_states[2 * row + (col & 1)] >> (col >> 1)) & 1;
}
*/
const int baudrate = 115200;

void setup(void) {
  //pinMode(LED, OUTPUT);
  //pinMode(CAPS_LED, OUTPUT);

  init_bluetooth();

  for (uint8_t row = 0; row < ROWS; row++) {
    pinMode(row_pins[row], OUTPUT);
    digitalWrite(row_pins[row], HIGH);
  }
  for (uint8_t col = 0; col < COLS; col++) {
    pinMode(col_pins[col], INPUT_PULLUP);
    digitalWrite(col_pins[col], HIGH);
  }

  Serial.begin(9600);
  /*
  mcp0.begin(0);      // use default address 0
  mcp1.begin(1);      // use address 1
  mcp2.begin(2); // use address 2

  mcp0.pinMode(0, INPUT);
  mcp0.pullUp(0, HIGH);  // turn on a 100K pullup internally
  mcp0.pinMode(1, INPUT);
  mcp0.pullUp(1, HIGH);  // turn on a 100K pullup internally
  mcp0.pinMode(2, INPUT);
  mcp0.pullUp(2, HIGH);  // turn on a 100K pullup internally
  mcp0.pinMode(3, INPUT);
  mcp0.pullUp(3, HIGH);  // turn on a 100K pullup internally
  mcp0.pinMode(4, INPUT);
  mcp0.pullUp(4, HIGH);  // turn on a 100K pullup internally
  mcp0.pinMode(5, INPUT);
  mcp0.pullUp(5, HIGH);  // turn on a 100K pullup internally
  mcp0.pinMode(6, INPUT);
  mcp0.pullUp(6, HIGH);  // turn on a 100K pullup internally
  mcp0.pinMode(7, INPUT);
  mcp0.pullUp(7, HIGH);  // turn on a 100K pullup internally
  mcp0.pinMode(8, INPUT);
  mcp0.pullUp(8, HIGH);  // turn on a 100K pullup internally
  mcp0.pinMode(9, INPUT);
  mcp0.pullUp(9, HIGH);  // turn on a 100K pullup internally
  mcp0.pinMode(10, INPUT);
  mcp0.pullUp(10, HIGH);  // turn on a 100K pullup internally
  mcp0.pinMode(11, INPUT);
  mcp0.pullUp(11, HIGH);  // turn on a 100K pullup internally
  mcp0.pinMode(12, INPUT);
  mcp0.pullUp(12, HIGH);  // turn on a 100K pullup internally
  mcp0.pinMode(13, INPUT);
  mcp0.pullUp(13, HIGH);  // turn on a 100K pullup internally
  mcp0.pinMode(14, INPUT);
  mcp0.pullUp(14, HIGH);  // turn on a 100K pullup internally
  mcp0.pinMode(15, INPUT);
  mcp0.pullUp(15, HIGH);  // turn on a 100K pullup internally
  
  mcp1.pinMode(0, INPUT);
  mcp1.pullUp(0, HIGH);  // turn on a 100K pullup internally
  mcp1.pinMode(1, INPUT);
  mcp1.pullUp(1, HIGH);  // turn on a 100K pullup internally
  mcp1.pinMode(2, INPUT);
  mcp1.pullUp(2, HIGH);  // turn on a 100K pullup internally
  mcp1.pinMode(3, INPUT);
  mcp1.pullUp(3, HIGH);  // turn on a 100K pullup internally
  mcp1.pinMode(4, INPUT);
  mcp1.pullUp(4, HIGH);  // turn on a 100K pullup internally
  mcp1.pinMode(5, INPUT);
  mcp1.pullUp(5, HIGH);  // turn on a 100K pullup internally
  mcp1.pinMode(6, INPUT);
  mcp1.pullUp(6, HIGH);  // turn on a 100K pullup internally
  mcp1.pinMode(7, INPUT);
  mcp1.pullUp(7, HIGH);  // turn on a 100K pullup internally
  mcp1.pinMode(8, INPUT);
  mcp1.pullUp(8, HIGH);  // turn on a 100K pullup internally
  mcp1.pinMode(9, INPUT);
  mcp1.pullUp(9, HIGH);  // turn on a 100K pullup internally
  mcp1.pinMode(10, INPUT);
  mcp1.pullUp(10, HIGH);  // turn on a 100K pullup internally
  mcp1.pinMode(11, INPUT);
  mcp1.pullUp(11, HIGH);  // turn on a 100K pullup internally
  mcp1.pinMode(12, INPUT);
  mcp1.pullUp(12, HIGH);  // turn on a 100K pullup internally
  mcp1.pinMode(13, INPUT);
  mcp1.pullUp(13, HIGH);  // turn on a 100K pullup internally
  mcp1.pinMode(14, INPUT);
  mcp1.pullUp(14, HIGH);  // turn on a 100K pullup internally
  mcp1.pinMode(15, INPUT);
  mcp1.pullUp(15, HIGH);  // turn on a 100K pullup internally

  mcp2.pinMode(0, INPUT);
  mcp2.pullUp(0, HIGH);  // turn on a 100K pullup internally
  mcp2.pinMode(1, INPUT);
  mcp2.pullUp(1, HIGH);  // turn on a 100K pullup internally
  mcp2.pinMode(2, INPUT);
  mcp2.pullUp(2, HIGH);  // turn on a 100K pullup internally
  mcp2.pinMode(3, INPUT);
  mcp2.pullUp(3, HIGH);  // turn on a 100K pullup internally
  mcp2.pinMode(4, INPUT);
  mcp2.pullUp(4, HIGH);  // turn on a 100K pullup internally
  mcp2.pinMode(5, INPUT);
  mcp2.pullUp(5, HIGH);  // turn on a 100K pullup internally
  
  
  mcp2.pinMode(6, INPUT);
  mcp2.pullUp(6, HIGH);  // turn on a 100K pullup internally
  mcp2.pinMode(7, INPUT);
  mcp2.pullUp(7, HIGH);  // turn on a 100K pullup internally
  mcp2.pinMode(8, INPUT);
  mcp2.pullUp(8, HIGH);  // turn on a 100K pullup internally
  mcp2.pinMode(9, INPUT);
  mcp2.pullUp(9, HIGH);  // turn on a 100K pullup internally
  mcp2.pinMode(10, INPUT);
  mcp2.pullUp(10, HIGH);  // turn on a 100K pullup internally
  mcp2.pinMode(11, INPUT);
  mcp2.pullUp(11, HIGH);  // turn on a 100K pullup internally
  mcp2.pinMode(12, INPUT);
  mcp2.pullUp(12, HIGH);  // turn on a 100K pullup internally
  mcp2.pinMode(13, INPUT);
  mcp2.pullUp(13, HIGH);  // turn on a 100K pullup internally
  mcp2.pinMode(14, INPUT);
  mcp2.pullUp(14, HIGH);  // turn on a 100K pullup internally
  mcp2.pinMode(15, INPUT);
  mcp2.pullUp(15, HIGH); // turn on a 100K pullup internally


*/

}

uint8_t stop = 100;

uint8_t last_reported = 999;
uint32_t last_report_time = 0;

void loop(void) {
  if (!is_bluetooth_connected()) {
    last_reported = 999;
    last_report_time = 0;
    //analogWrite(LED, 128);
    delay(500);
    return;
  }
  
//Serial.println("print works");

/*
for (uint8_t IOEx0 = 0; IOEx0 < 16; IOEx0++) {
   if(mcp0.digitalRead(0) == LOW) {
     //Serial.println("Success Preview 1!");
     IOExpander0KeyState[IOEx0] = true;
     handle_keychange(0,IOEx0, IOExpander0KeyState[IOEx0]);
     goto END_OF_LOOP;
    }else if(IOExpander0KeyState[IOEx0]){
   IOExpander0KeyState[IOEx0] = false;
   handle_keychange(0,IOEx0, IOExpander0KeyState[IOEx0]);
   goto END_OF_LOOP;
}
}



for (uint8_t IOEx1 = 0; IOEx1 < 16; IOEx1++) {
   if(mcp1.digitalRead(0) == LOW) {
     //Serial.println("Success Preview 1!");
     IOExpander1KeyState[IOEx1] = true;
     handle_keychange(1,IOEx1, IOExpander1KeyState[IOEx1]);
     goto END_OF_LOOP;
    }else if(IOExpander1KeyState[IOEx1]){
   IOExpander1KeyState[IOEx1] = false;
   handle_keychange(1,IOEx1, IOExpander1KeyState[IOEx1]);
   goto END_OF_LOOP;
}
}



   if(mcp2.digitalRead(0) == LOW) {
     //Serial.println("Success Preview 1!");
     IOExpander2KeyState[0] = true;
     handle_keychange(2,15, IOExpander2KeyState[0]);
     goto END_OF_LOOP;
     
    }else if(IOExpander2KeyState[0]){
   IOExpander2KeyState[0] = false;
   handle_keychange(3,14, IOExpander2KeyState[0]);
   
   goto END_OF_LOOP;
}

*/


  //analogWrite(LED, 1 + (exp(sin(millis()/2000.0*PI)) - 0.36787944) * 107.0);

  for (uint8_t row = 0; row < ROWS; row++) {
    uint16_t row_read = 0;

    digitalWrite(row_pins[row], LOW);
    delayMicroseconds(30);

    for (uint8_t col = 0; col < COLS; col++) {
      if (digitalRead(col_pins[col]) == LOW) {  //matrix run    //   2         col
        row_read |= 1 << col;                   //    00000000     00000010 << 00000001             0 2 
       }
    }

    if (temp_states[row] != row_read) {
      temp_states[row] = row_read;
      debouncing = DEBOUNCING_DELAY;
    }

    digitalWrite(row_pins[row], HIGH);
  }



  if (debouncing) {
    if (--debouncing) {
      delay(1);
    } else {
      for (uint8_t row = 0; row < ROWS; row++) {
        prev_states[row] = curr_states[row];    //if not debouncing change states
        curr_states[row] = temp_states[row];
      }
    }
  }



  for (uint8_t row = 0; row < ROWS; row++) {
    if (curr_states[row] == prev_states[row]) { // if no state change in row
      continue;
    }

    for (uint8_t col = 0; col < COLS; col++) {
      state_t curr = (curr_states[row] >> col) & 1;  // curr and prev are either 1 or 0 based on weather the states[row] >> col is an even or odd number
      state_t prev = (prev_states[row] >> col) & 1;
      if (curr != prev) {
        
        
        handle_keychange(keyboard_wire_to_virt[row][col][0], keyboard_wire_to_virt[row][col][1], curr); // old row >> 1, 2 * col + (row & 1), curr 
        prev_states[row] ^= (uint16_t)1 << col;   
        goto END_OF_LOOP;
      }
    }
  }
 
  END_OF_LOOP:;
}
