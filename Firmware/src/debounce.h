#ifndef DEBOUNCE_H_
#define DEBOUNCE_H_

#include "debounce.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
  uint8_t avg;
  uint8_t min;
  uint8_t max;
} sDebounceStats;

extern sDebounceStats debounce_stats;

typedef enum {
  ENC_LEFT_A = 0,
  ENC_LEFT_B,
  ENC_RIGHT_A,
  ENC_RIGHT_B,
  BT_A,
  BT_B,
  BT_C,
  BT_D,
  FX_L,
  FX_R,
  START,
  NUM_PINS
} ePinId;

void DebounceInit(void);
void DebounceUpdate(void);
bool DebounceGetLevel(ePinId id);

#endif /* DEBOUNCE_H_ */