#include <Arduino.h>

#include "config.h"
#include "keymap.h"
#include "keycodes.h"
#include "bluetooth.h"

#define LAYERS 4
#define BASE 0
#define LOWER 1
#define RAISE 2
#define HOME 3
#define IS_LAYER_ON(layer) (active_layers & (1 << layer))
uint8_t active_layers = 1; // layer 0 is always active

inline
void set_layer(uint8_t layer) {
  active_layers |= (1 << layer);
}

inline
void unset_layer(uint8_t layer) {
  active_layers &= ~(1 << layer);
}

uint32_t last_pressed = 0;
uint8_t pressed_alone = 0;
#define PRESSED_LSFT 0
#define PRESSED_RSFT 1
#define PRESSED_LOW  2
#define PRESSED_RAIS 3
#define PRESSED_HOME 4

#define DUAL_TIMEOUT 300

bool caps_lock = false;


/*
uint16_t layers_cache[6][15] = {
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______}
};
*/

uint16_t layers_cache[8][16] = {
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______}
  
};





const uint16_t PROGMEM layers[LAYERS][8][16] = {
  [BASE] = {
  {K_1,    K_1,    K_2,    K_3,    K_4,    K_5,    K_6,    K_7,    K_8,    K_9,    K_0,    ______, ______, ______, ______, ______},  
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {K_ESC,  K_F1,   K_F2,   K_F3,   K_F4,   K_F5,   K_F6,   K_F7,   K_F8,   K_F9,   K_F10,  K_F11,  K_F12,  K_PRTSC,K_PGUP, ______},
  {K_TILDE,K_1,    K_2,    K_3,    K_4,    K_5,    K_6,    K_7,    K_8,    K_9,    K_0,    K_RPRN,K_EQUALS,K_BSPC, K_PGDWN,______},
  {K_TAB,  K_Q,    K_W,    K_E,    K_R,    K_T,    K_Y,    K_U,    K_I,    K_O,    K_P,    K_FBRKT,K_BBRKT,K_BSLH, K_DEL,  ______},
  {K_BSPC, K_A,    K_S,    K_D,    K_F,    K_G,    K_H,    K_J,    K_K,    K_L,    K_SEMI, K_UGRV, K_ENT,  K_ENT,  K_DEL,  ______},
  {K_SHFT, K_Z,    K_X,    K_C,    K_V,    K_B,    K_N,    K_M,    K_SCLN, K_COLN, K_EQL,  K_SHFT, K_HOMEK,K_UP,   K_END,  ______},
  {K_RCTRL,K_WIN,  K_RALT, K_COPY, K_SPC,  K_SPC,  K_SPC,  K_SPC,  K_PAST, K_CUT,  K_LCTRL,K_LCTRL,K_LEFT, K_DOWN, K_RGHT, ______}
  },

  [LOWER] = {
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______}
  },

  [RAISE] = {
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______}
  },

  [HOME] = {
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______},
  {______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______}
  }
};


uint16_t get_keycode_at(uint8_t row, uint8_t col) {  /* Returns keycode at given row col combinaltion from layers_cache if not ______  */
  uint16_t cached = layers_cache[row][col];
  if (cached != ______) {
    return cached;
  }

  for (uint8_t layer = LAYERS - 1; layer >= 0; layer--) {     /* finds current layer returns keycode at row/col from layer using pgm_read_word */
    if (IS_LAYER_ON(layer)) {
      uint16_t keycode = pgm_read_word(&(layers[layer][row][col])); 
      if (keycode != ______) {
        return keycode;
      }
    }
  }
  return K_NO;
}  /* first checks the cached layer if no code at row/col returns nothing then checks current layer at same row/col point returns keycode or K_no*/

void handle_keychange(uint8_t row, uint8_t col, state_t state) {
  uint16_t keycode = get_keycode_at(row, col);
  if (state == DOWN) {
    pressed_alone = 0;
  }

  switch (keycode) {
    case K_LSFT: {
      if (state == DOWN) {
        pressed_alone |= (1 << PRESSED_LSFT);
        last_pressed = millis();
        register_keydown(K_SHFT);
      } else {
        register_keyup(K_SHFT);
        if (
            (millis() - last_pressed < DUAL_TIMEOUT) &&
            (pressed_alone & (1 << PRESSED_LSFT))
        ) {
          if (IS_LAYER_ON(RAISE)) {
            register_keydown(K_LCBR);
            register_keyup(K_LCBR);
          } else if (IS_LAYER_ON(LOWER)) {
            register_keydown(K_LBRC);
            register_keyup(K_LBRC);
          } else {
            register_keydown(K_LPRN);
            register_keyup(K_LPRN);
          }
        }
      }
      break;
    }

    case K_RSFT: {
      if (state == DOWN) {
        pressed_alone |= (1 << PRESSED_RSFT);
        last_pressed = millis();
        register_keydown(K_SHFT);
      } else {
        register_keyup(K_SHFT);
        if (
            (millis() - last_pressed < DUAL_TIMEOUT) &&
            (pressed_alone & (1 << PRESSED_RSFT))
        ) {
          if (IS_LAYER_ON(RAISE)) {
            register_keydown(K_RCBR);
            register_keyup(K_RCBR);
          } else if (IS_LAYER_ON(LOWER)) {
            register_keydown(K_RBRC);
            register_keyup(K_RBRC);
          } else {
            register_keydown(K_RPRN);
            register_keyup(K_RPRN);
          }
        }
      }
      break;
    }

    case K_LOW: {
      if (state == DOWN) {
        set_layer(LOWER);
        last_pressed = millis();
        pressed_alone |= (1 << PRESSED_LOW);
      } else {
        unset_layer(LOWER);
        if (
            (millis() - last_pressed < DUAL_TIMEOUT) &&
            (pressed_alone & (1 << PRESSED_LOW))
        ) {
          register_keydown(K_QUOT);
          register_keyup(K_QUOT);
        }
      }
      break;
    }

    case K_RAIS: {
      if (state == DOWN) {
        set_layer(RAISE);
        last_pressed = millis();
        pressed_alone |= (1 << PRESSED_RAIS);
      } else {
        unset_layer(RAISE);
        if (
            (millis() - last_pressed < DUAL_TIMEOUT) &&
            (pressed_alone & (1 << PRESSED_RAIS))
        ) {
         // register_keydown(K_APOS);
        //register_keyup(K_APOS);
        }
      }
      break;
    }

    case K_HOME: {
      if (state == DOWN) {
        set_layer(HOME);
        last_pressed = millis();
        pressed_alone |= (1 << PRESSED_HOME);
      } else {
        unset_layer(HOME);
        if (
            (millis() - last_pressed < 1000) &&
            (pressed_alone & (1 << PRESSED_HOME))
        ) {
          register_keydown(K_CAPS);
          register_keyup(K_CAPS);

          caps_lock = !caps_lock;
          
        }
      }
      break;
    }
    case K_WIN: {
      if (state == DOWN) {
        register_keydown(K_RCTRL);
        register_keydown(K_ESC);
        register_keyup(K_ESC);
      }else{
        register_keyup(K_RCTRL);
        
      }
      break;
    }

     case K_COPY: {
      if (state == DOWN) {
        register_keydown(K_RCTRL);
        register_keydown(K_C);
        register_keyup(K_C);
      }else{
        register_keyup(K_RCTRL);
        
      }
      break;
    }
     case K_PAST: {
      if (state == DOWN) {
        register_keydown(K_RCTRL);
        register_keydown(K_V);
        register_keyup(K_V);
      }else{
        register_keyup(K_RCTRL);
        
      }
      break;
    }
     case K_CUT: {
      if (state == DOWN) {
        register_keydown(K_RCTRL);
        register_keydown(K_X);
        register_keyup(K_X);
      }else{
        register_keyup(K_RCTRL);
        
      }
      break;
    }

    default: {
      if (state == DOWN) {
        register_keydown(keycode);
      } else {
        register_keyup(keycode);
      }
      break;
    }

  }
  if (state == DOWN) {
    layers_cache[row][col] = keycode;
  } else {
    layers_cache[row][col] = ______;
  }
}
