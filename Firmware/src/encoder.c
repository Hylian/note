#include "encoder.h"

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

#include "debounce.h"

static uint8_t old_AB_left = 0;
static uint8_t old_AB_right = 0;
static const int8_t enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
static int8_t delta_left = 0;
static int8_t delta_right = 0;

void EncoderInit(void)
{
  DDRB = 0x0;
}
  
void EncoderUpdate(void)
{
  bool new_A_left = DebounceGetLevel(ENC_LEFT_A);
  bool new_B_left = DebounceGetLevel(ENC_LEFT_B);
  bool new_A_right = DebounceGetLevel(ENC_RIGHT_A);
  bool new_B_right = DebounceGetLevel(ENC_RIGHT_B);

  old_AB_left <<= 2;
  old_AB_left |= (uint8_t) ( (new_A_left << 1) | new_B_left );
  
  old_AB_right <<= 2;
  old_AB_right |= (uint8_t) ( (new_A_right << 1) | new_B_right );
  
  if (delta_left > -126 && delta_left < 127) {
    delta_left += enc_states[( old_AB_left & 0x0f )];
  }    
  if (delta_right > -126 && delta_right < 127) {
    delta_right += enc_states[( old_AB_right & 0x0f )];
  }    
}

int8_t EncoderGetLeftDelta(void)
{
  return delta_left;
}

void EncoderResetLeftDelta(void)
{
  delta_left = 0;
}

int8_t EncoderGetRightDelta(void)
{
  return delta_right;
}

void EncoderResetRightDelta(void)
{
  delta_right = 0;
}