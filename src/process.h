#pragma once

typedef struct {
  bool enabled : 1;
  bool ctl : 1;
  bool alt : 1;
  bool gui : 1;
  bool any : 1;
} TabbingState;

typedef struct {
  uint8_t state;
  uint16_t time;
  uint16_t timer;
  uint16_t last_timer;
  bool enabled : 1;
  bool running : 1;
  bool exec : 1;
  bool caps : 1;
} StatsState;

typedef struct {
  bool active : 1;
  int taps;
  uint16_t timer;
} HypeState;

typedef union {
  struct {
    uint8_t layer;
    bool pressed : 1;
    TabbingState tabbing;
    StatsState stats;
    uint16_t snap_tap[2];
    HypeState hype;
  };
} State;

extern State state;