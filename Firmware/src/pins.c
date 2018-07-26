#if 0
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

typedef struct {
  uint8_t count;
  bool level;
  uint8_t mask;
  uint8_t *port;
} sPinRef;

sPinRef pins[NUM_PINS] =
{
  {
      .count = 0;
      .level = false;
      .mask = (1<<0);
      .port = &PORTB;
  }

}
#endif