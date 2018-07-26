#ifndef NEOPIXEL_H_
#define NEOPIXEL_H_

#include <stdint.h>

void NeoPixelInit(void);
void NeoPixelSetBrightness(uint8_t b);
void NeoPixelSetPixelColor(uint8_t n, uint8_t r, uint8_t g, uint8_t b);
void NeoPixelUpdate(void);

#endif /* NEOPIXEL_H_ */