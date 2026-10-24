#pragma once

#include <math.h>
#include <stdint.h>

// Local UI only. LightState remains the source of truth for both HA and the button.
namespace kids_lamp {

enum Page { HOME, MENU, COLOR, BRIGHTNESS };
enum Gesture { NONE, SINGLE, DOUBLE, LONG, WAKE };

struct RGB { float r, g, b; };
static constexpr RGB PALETTE[] = {
    {1.0f, 0.35f, 0.05f},  // Warm white: starting point, tune on the real rings.
    {1.0f, 0.8f, 0.0f},   // Yellow.
    {0.0f, 1.0f, 0.0f},   // Green.
    {0.0f, 0.6f, 1.0f},   // Light blue.
    {0.6f, 0.0f, 1.0f},   // Purple.
};

inline int palette_index(float r, float g, float b) {
  for (int i = 0; i < 5; i++) {
    if (fabsf(r - PALETTE[i].r) < 0.015f &&
        fabsf(g - PALETTE[i].g) < 0.015f &&
        fabsf(b - PALETTE[i].b) < 0.015f) return i;
  }
  return -1;
}

inline float next_brightness(float current) {
  // Ignore floating point round-off at the four exact preset boundaries.
  static constexpr float levels[] = {0.30f, 0.50f, 0.75f, 1.0f};
  for (float level : levels) {
    if (current < level - 0.0001f) return level;
  }
  return 0.30f;
}

struct Controls {
  Page page{HOME};
  int selected{0};
  int menu_item{0};  // Color, brightness, power.
  bool sleeping{false};
  uint32_t long_ms{700};
  uint32_t double_ms{350};
  uint32_t idle_ms{30000};
  uint32_t last_activity{0};
  uint32_t pressed_at{0};
  uint32_t released_at{0};
  bool down{false};
  bool pending_single{false};
  bool second_press{false};
  bool long_sent{false};
  bool waking{false};

  // Called every 10 ms with the debounced button state. Unsigned elapsed times
  // also work across millis() wraparound. A wake gesture is fully consumed,
  // including its release and a possible second tap, so it cannot toggle lamps.
  Gesture poll(uint32_t now, bool pressed) {
    if (waking) {
      last_activity = now;
      if (pressed) released_at = now;
      else if (down) released_at = now;
      down = pressed;
      if (!pressed && uint32_t(now - released_at) > double_ms) waking = false;
      return NONE;
    }

    if (pressed && !down) {
      down = true;
      pressed_at = now;
      long_sent = false;
      last_activity = now;
      if (sleeping) {
        sleeping = false;
        page = HOME;
        pending_single = false;
        second_press = false;
        waking = true;
        released_at = now;
        return WAKE;
      }
      second_press = pending_single && uint32_t(now - released_at) <= double_ms;
      if (second_press) pending_single = false;
      // If the loop was delayed past the single-click deadline, dispatch the
      // previous single now while still tracking the new press normally.
      if (pending_single) {
        pending_single = false;
        return SINGLE;
      }
    }

    // Check duration before the release edge: an interval delayed until release
    // must still recognize a long press and must not emit an extra short click.
    if (down && !long_sent && uint32_t(now - pressed_at) >= long_ms) {
      long_sent = true;
      pending_single = false;
      second_press = false;
      last_activity = now;
      if (!pressed) down = false;
      return LONG;
    }

    if (!pressed && down) {
      down = false;
      last_activity = now;
      released_at = now;
      if (!long_sent) {
        if (second_press) {
          second_press = false;
          return DOUBLE;
        }
        pending_single = true;
      }
    }
    if (!down && pending_single && uint32_t(now - released_at) > double_ms) {
      pending_single = false;
      return SINGLE;
    }
    if (!down && !pending_single && uint32_t(now - last_activity) >= idle_ms) {
      sleeping = true;
      page = HOME;
    }
    return NONE;
  }

  float hold_progress(uint32_t now) const {
    if (!down || waking || long_sent) return 0.0f;
    return fminf(1.0f, float(uint32_t(now - pressed_at)) / long_ms);
  }
};

}  // namespace kids_lamp
