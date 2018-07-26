#include "neopixel.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define NEOPIXEL_PORT &PORTD
#define NEOPIXEL_PIN_MASK (1 << 5)
#define NEOPIXEL_NUM_LEDS 12
#define NEOPIXEL_COLORS_PER_LED 3
#define NEOPIXEL_BYTES_PER_COLOR 2
#define NEOPIXEL_BUFFER_SIZE (NEOPIXEL_NUM_LEDS * NEOPIXEL_COLORS_PER_LED * NEOPIXEL_BYTES_PER_COLOR)

static uint8_t pixelBuffer[NEOPIXEL_BUFFER_SIZE] = {0};
static uint8_t brightness = 100;
 
void NeoPixelInit(void)
{
  DDRD |= NEOPIXEL_PIN_MASK;
  PORTD &= ~NEOPIXEL_PIN_MASK;
}

void NeoPixelSetBrightness(uint8_t b)
{
  brightness = b;
}

void NeoPixelSetPixelColor(uint8_t n, uint8_t r, uint8_t g, uint8_t b)
{
  r = (r * brightness) >> 8;
  g = (g * brightness) >> 8;
  b = (b * brightness) >> 8;
  
  uint8_t *p = &pixelBuffer[n * 3];
  p[1] = r;
  p[0] = g;
  p[2] = b;
}

void NeoPixelUpdate(void)
{
  // WS2811 and WS2812 have different hi/lo duty cycles; this is
  // similar but NOT an exact copy of the prior 400-on-8 code.

  // 20 inst. clocks per bit: HHHHHxxxxxxxxLLLLLLL
  // ST instructions:         ^   ^        ^       (T=0,5,13)
  
  volatile uint16_t
  i   = NEOPIXEL_BUFFER_SIZE; // Loop counter
  volatile uint8_t
  *ptr = pixelBuffer,   // Pointer to next byte
  b   = *ptr++,   // Current byte value
  hi,             // PORT w/output bit set high
  lo;             // PORT w/output bit set low

  volatile uint8_t next, bit;
  volatile uint8_t *port = NEOPIXEL_PORT;
  
  hi   = *NEOPIXEL_PORT |  NEOPIXEL_PIN_MASK;
  lo   = *NEOPIXEL_PORT & ~NEOPIXEL_PIN_MASK;
  next = lo;
  bit  = 8;
  
  cli();

  asm volatile(
  "head20:"                   "\n\t" // Clk  Pseudocode    (T =  0)
  "st   %a[port],  %[hi]"    "\n\t" // 2    PORT = hi     (T =  2)
  "sbrc %[byte],  7"         "\n\t" // 1-2  if(b & 128)
  "mov  %[next], %[hi]"     "\n\t" // 0-1   next = hi    (T =  4)
  "dec  %[bit]"              "\n\t" // 1    bit--         (T =  5)
  "st   %a[port],  %[next]"  "\n\t" // 2    PORT = next   (T =  7)
  "mov  %[next] ,  %[lo]"    "\n\t" // 1    next = lo     (T =  8)
  "breq nextbyte20"          "\n\t" // 1-2  if(bit == 0) (from dec above)
  "rol  %[byte]"             "\n\t" // 1    b <<= 1       (T = 10)
  "rjmp .+0"                 "\n\t" // 2    nop nop       (T = 12)
  "nop"                      "\n\t" // 1    nop           (T = 13)
  "st   %a[port],  %[lo]"    "\n\t" // 2    PORT = lo     (T = 15)
  "nop"                      "\n\t" // 1    nop           (T = 16)
  "rjmp .+0"                 "\n\t" // 2    nop nop       (T = 18)
  "rjmp head20"              "\n\t" // 2    -> head20 (next bit out)
  "nextbyte20:"               "\n\t" //                    (T = 10)
  "ldi  %[bit]  ,  8"        "\n\t" // 1    bit = 8       (T = 11)
  "ld   %[byte] ,  %a[ptr]+" "\n\t" // 2    b = *ptr++    (T = 13)
  "st   %a[port], %[lo]"     "\n\t" // 2    PORT = lo     (T = 15)
  "nop"                      "\n\t" // 1    nop           (T = 16)
  "sbiw %[count], 1"         "\n\t" // 2    i--           (T = 18)
  "brne head20"             "\n"   // 2    if(i != 0) -> (next byte)
  : [port]  "+e" (port),
  [byte]  "+r" (b),
  [bit]   "+r" (bit),
  [next]  "+r" (next),
  [count] "+w" (i)
  : [ptr]    "e" (ptr),
  [hi]     "r" (hi),
  [lo]     "r" (lo));
  
  sei();
}  