#include "debounce.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

#define DEBOUNCE_TIMER_COMPARE_COUNT   63 // 0.25 ms @ 250 kHz
#define DEBOUNCE_TRIGGER_COUNT_BUTTON  3
#define DEBOUNCE_TRIGGER_COUNT_ENCODER 1

typedef struct {
  uint8_t count;
  const uint8_t trigger_count;
  bool level;
  const uint8_t mask;
  const volatile uint8_t *port;
} sPinRef;

static sPinRef pins[NUM_PINS] =
{
  {
    /* ENC_LEFT_A */
    .count = 0,
    .trigger_count = DEBOUNCE_TRIGGER_COUNT_ENCODER,
    .level = 0,
    .mask = (1 << 4),
    .port = &PINB
  },
  {
    /* ENC_LEFT_B */
    .count = 0,
    .trigger_count = DEBOUNCE_TRIGGER_COUNT_ENCODER,
    .level = 0,
    .mask = (1 << 5),
    .port = &PINB
  },
  {
    /* ENC_RIGHT_A */
    .count = 0,
    .trigger_count = DEBOUNCE_TRIGGER_COUNT_ENCODER,
    .level = 0,
    .mask = (1 << 0),
    .port = &PINB
  },
  {
    /* ENC_RIGHT_B */
    .count = 0,
    .trigger_count = DEBOUNCE_TRIGGER_COUNT_ENCODER,
    .level = 0,
    .mask = (1 << 7),
    .port = &PINB
  },
  {
    /* BT_A */
    .count = 0,
    .trigger_count = DEBOUNCE_TRIGGER_COUNT_ENCODER,
    .level = 0,
    .mask = (1 << 7),
    .port = &PIND
  },
  {
    /* BT_B */
    .count = 0,
    .trigger_count = DEBOUNCE_TRIGGER_COUNT_BUTTON,
    .level = 0,
    .mask = (1 << 4),
    .port = &PIND
  },
  {
    /* BT_C */
    .count = 0,
    .trigger_count = DEBOUNCE_TRIGGER_COUNT_BUTTON,
    .level = 0,
    .mask = (1 << 2),
    .port = &PIND
  },
  {
    /* BT_D */
    .count = 0,
    .trigger_count = DEBOUNCE_TRIGGER_COUNT_BUTTON,
    .level = 0,
    .mask = (1 << 0),
    .port = &PIND
  },
  {
    /* FX-L */
    .count = 0,
    .trigger_count = DEBOUNCE_TRIGGER_COUNT_BUTTON,
    .level = 0,
    .mask = (1 << 6),
    .port = &PIND
  },
  {
    /* FX-R */
    .count = 0,
    .trigger_count = DEBOUNCE_TRIGGER_COUNT_BUTTON,
    .level = 0,
    .mask = (1 << 1),
    .port = &PIND
  },
  {
    /* Start */
    .count = 0,
    .trigger_count = DEBOUNCE_TRIGGER_COUNT_BUTTON,
    .level = 0,
    .mask = (1 << 2),
    .port = &PINE
  },
};

void DebounceInit(void)
{
  /* Setup Debounce Timer */
  TCCR0A |= (1 << COM0A1) | (1 << COM0A0); // Set OC0A to compare
  TCNT0 = 0;                               // Reset timer count
  OCR0A = DEBOUNCE_TIMER_COMPARE_COUNT;    // Compare value
  TIFR0 = (1 << OCF0A);                    // Clear compare flag
  TCCR0B |= (1 << CS01) | (1 << CS00);     // Select clock : 64 prescale div -> 250 kHz
}

static uint8_t stat_previous_cnt = 0;

sDebounceStats debounce_stats = {0, 0xff, 0};

void DebounceUpdate(void)
{
  // Has our sample timer triggered?
  if (TIFR0 & (1 << OCF0A)) {
    for (ePinId p = 0; p < NUM_PINS; p++) {
      sPinRef *pr = &pins[p];
      if (pr->level != (bool) (pr->mask & *(pr->port))) {
        pr->count++;
        if (pr->count > pr->trigger_count) {
          pr->level = !pr->level;
          pr->count = 0;
        }
      } else {
        pr->count = 0;
      }
    }
    // Reset sample timer
    TIFR0 = (1 << OCF0A);
    TCNT0 = 0;
    stat_previous_cnt = 0;
  } else {
    // Collect statistics on scheduling rate
    uint8_t current_cnt = TCNT0;
    if (current_cnt > stat_previous_cnt) {
      uint8_t delta = current_cnt - stat_previous_cnt;
      // Exponential moving average for simplicity
      debounce_stats.avg = 0.3 * delta + (1 - 0.3) * debounce_stats.avg;
      if (delta < debounce_stats.min) debounce_stats.min = delta;
      if (delta > debounce_stats.max) debounce_stats.max = delta;
    }
  }
}

bool DebounceGetLevel(ePinId id)
{
  return pins[id].level;
}

