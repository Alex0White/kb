#include <Arduino.h>
#include <bluefruit.h>

#include "config.h"
#include "keycodes.h"

BLEHidAdafruit blehid;

BLEDis bledis;
#define REPORT_KEYS 6

uint8_t report[REPORT_KEYS] = { 0 };
uint8_t active_mods = 0;
char payload[43] = "";

void add_mods(uint8_t mods) {
  active_mods |= mods;
}

void del_mods(uint8_t mods) {
  active_mods &= ~mods;
}

void send_report_keyboard() {
  bool err = blehid.keyboardReport(
    active_mods, report[0], report[1], report[2], report[3], report[4], report[5]
  );
}

void register_keydown(uint16_t keycode) {
  if (IN_KEYBOARD_RANGE(keycode)) {
    add_mods((uint8_t)(keycode >> 8));

    for (uint8_t i = 0; i < REPORT_KEYS; i++) {
      if (report[i] == (uint8_t)(keycode & 0xFF)) {
        break;
      }
      if (report[i] == 0) {
        report[i] = (uint8_t)(keycode & 0xFF);
        break;
      }
    }
    send_report_keyboard();
  }
}

void register_keyup(uint16_t keycode) {
  if (IN_KEYBOARD_RANGE(keycode)) {
    del_mods((uint8_t)(keycode >> 8));

    for (uint8_t i = 0; i < REPORT_KEYS; i++) {
      if (report[i] == (uint8_t)(keycode & 0xFF)) {
        report[i] = 0;
      }
    }
    send_report_keyboard();
  }
}

void init_bluetooth() {
  //Bluefruit.clearBonds();
  Bluefruit.setName("AlexKeyboard");
  Bluefruit.setTxPower(-4);
  Bluefruit.begin();


  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather 52");
  bledis.begin();

 

 

 

  blehid.begin();

  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_KEYBOARD);
  Bluefruit.Advertising.addService(blehid);

  Bluefruit.Advertising.addName();
 
  //uint8_t cod_data[2] = {0x25, 0x40};
  
  //Bluefruit.Advertising.addData(BLE_GAP_AD_TYPE_CLASS_OF_DEVICE, cod_data, 2);

  
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30); 
 

  Bluefruit.Advertising.start(0);
}


bool is_bluetooth_connected() {
  return Bluefruit.connected();
}
