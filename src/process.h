#pragma once

typedef union {
  struct {
    bool enabled;
    bool ctl;
    bool gui;
    bool alt;
    bool any;
    bool all;
  };
} TabState;

extern TabState tab_state;