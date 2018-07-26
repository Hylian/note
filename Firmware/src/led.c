#include "led.h"

#include <stdint.h>
#include <stdbool.h>

#include "debounce.h"
#include "neopixel.h"

#define NUM_BUTTONS  6

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} sLedColor;

typedef struct {
  ePinId pinId;
  bool state;
  uint8_t led1;
  uint8_t led2;
  sLedColor color_on;
  sLedColor color_off;
} sButtonRef;

#define COLOR_OFF   {100, 100, 100}
#define COLOR_ON_BT {52, 108, 239}
#define COLOR_ON_FX {252, 178, 20}

static sButtonRef buttons[NUM_BUTTONS] =
{
  {
    .pinId = BT_A,
    .state = false,
    .led1 = 3,
    .led2 = 4,
    .color_on = COLOR_ON_BT,
    .color_off = COLOR_OFF
  },
  {
    .pinId = BT_B,
    .state = false,
    .led1 = 2,
    .led2 = 5,
    .color_on = COLOR_ON_BT,
    .color_off = COLOR_OFF
  },
  {
    .pinId = BT_C,
    .state = false,
    .led1 = 1,
    .led2 = 6,
    .color_on = COLOR_ON_BT,
    .color_off = COLOR_OFF
  },
  {
    .pinId = BT_D,
    .state = false,
    .led1 = 0,
    .led2 = 7,
    .color_on = COLOR_ON_BT,
    .color_off = COLOR_OFF
  },
  {
    .pinId = FX_L,
    .state = false,
    .led1 = 9,
    .led2 = 10,
    .color_on = COLOR_ON_FX,
    .color_off = COLOR_OFF
  },
  {
    .pinId = FX_R,
    .state = false,
    .led1 = 8,
    .led2 = 11,
    .color_on = COLOR_ON_FX,
    .color_off = COLOR_OFF
  }
};

void LedInit(void)
{
  NeoPixelInit();
  for (int i = 0; i < NUM_BUTTONS; i++) {
    NeoPixelSetPixelColor(buttons[i].led1, buttons[i].color_off.r, buttons[i].color_off.g, buttons[i].color_off.b);
    NeoPixelSetPixelColor(buttons[i].led2, buttons[i].color_off.r, buttons[i].color_off.g, buttons[i].color_off.b);
  }
  NeoPixelUpdate();
}

void LedUpdate(void)
{
  bool neopixel_update_required = false;

  for (int i = 0; i < NUM_BUTTONS; i++) {
    bool button_level = DebounceGetLevel(buttons[i].pinId);
    
    if (button_level != buttons[i].state) {
      buttons[i].state = button_level;
      neopixel_update_required = true;
      if (button_level) {
        NeoPixelSetPixelColor(buttons[i].led1, buttons[i].color_off.r, buttons[i].color_off.g, buttons[i].color_off.b);
        NeoPixelSetPixelColor(buttons[i].led2, buttons[i].color_off.r, buttons[i].color_off.g, buttons[i].color_off.b);
      } else {
        NeoPixelSetPixelColor(buttons[i].led1, buttons[i].color_on.r, buttons[i].color_on.g, buttons[i].color_on.b);
        NeoPixelSetPixelColor(buttons[i].led2, buttons[i].color_on.r, buttons[i].color_on.g, buttons[i].color_on.b);
      }
    }
  }

  if (neopixel_update_required) {
    NeoPixelUpdate();
  }
}