#pragma once

#include "lamp_controls.h"
#include "esphome/components/display/display.h"
#include "esphome/components/light/light_state.h"

namespace kids_lamp {

inline void draw(esphome::display::Display &it, const Controls &ui,
                 esphome::light::LightState *left,
                 esphome::light::LightState *right, uint32_t now) {
  using esphome::Color;
  const Color ink(190, 190, 190), muted(45, 45, 45), accent(255, 217, 179), black(0, 0, 0);
  it.fill(black);

  // Vector icons and seven-segment numbers avoid font/image downloads.
  auto arrow = [&](int x, int y, bool rightwards, int size, Color c) {
    int dx = rightwards ? size : -size;
    it.line(x, y, x + dx, y + size, c);
    it.line(x + dx, y + size, x + dx, y + size - size / 2, c);
    it.line(x + dx, y + size, x + dx - dx / 2, y + size, c);
    it.line(x + 1, y, x + dx + 1, y + size, c);
  };
  auto bulb = [&](int x, int y, int r, Color c, bool on) {
    if (on) it.filled_circle(x, y, r, c);
    else it.circle(x, y, r, c);
    it.rectangle(x - r / 2, y + r, r + 1, 6, c);
    it.horizontal_line(x - r / 2 + 1, y + r + 8, r - 1, c);
  };
  auto sun = [&](int x, int y, int r, Color c) {
    it.circle(x, y, r, c);
    for (int i = 0; i < 8; i++) {
      float a = i * 0.785398f;
      it.line(x + int((r + 4) * cosf(a)), y + int((r + 4) * sinf(a)),
              x + int((r + 8) * cosf(a)), y + int((r + 8) * sinf(a)), c);
    }
  };
  auto power = [&](int x, int y, int r, Color c, bool on) {
    it.circle(x, y, r, c);
    it.filled_rectangle(x - 5, y - r - 2, 11, r + 2, black);
    it.filled_rectangle(x - 1, y - r - 4, 3, r + 5, c);
    if (on) it.filled_circle(x, y + 5, 3, c);
  };
  auto palette = [&](int x, int y, int r) {
    for (int i = 0; i < 5; i++) {
      const auto &p = PALETTE[i];
      float a = i * 1.256637f - 1.570796f;
      it.filled_circle(x + int(r * cosf(a)), y + int(r * sinf(a)),
                       r < 15 ? 3 : 6, Color(p.r * 255, p.g * 255, p.b * 255));
    }
  };
  auto digit = [&](int x, int y, int n) {
    static const uint8_t mask[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
    uint8_t m = mask[n];
    if (m & 1) it.filled_rectangle(x + 3, y, 12, 3, ink);
    if (m & 2) it.filled_rectangle(x + 15, y + 3, 3, 12, ink);
    if (m & 4) it.filled_rectangle(x + 15, y + 18, 3, 12, ink);
    if (m & 8) it.filled_rectangle(x + 3, y + 30, 12, 3, ink);
    if (m & 16) it.filled_rectangle(x, y + 18, 3, 12, ink);
    if (m & 32) it.filled_rectangle(x, y + 3, 3, 12, ink);
    if (m & 64) it.filled_rectangle(x + 3, y + 15, 12, 3, ink);
  };

  if (ui.sleeping) {
    bulb(64, 62, 7, Color(65, 55, 45), false);
    return;
  }

  auto *lamp = ui.selected == 0 ? left : right;
  const auto &v = lamp->remote_values;
  Color actual(v.get_red() * 255, v.get_green() * 255, v.get_blue() * 255);
  bool on = v.is_on();
  if (ui.page == HOME) {
    for (int side = 0; side < 2; side++) {
      int x = side == 0 ? 32 : 96;
      if (ui.selected == side) it.rectangle(x - 29, 12, 58, 97, accent);
      arrow(x + (side == 0 ? 10 : -10), 24, side == 1, 20, ink);
      auto *zone = side == 0 ? left : right;
      const auto &z = zone->remote_values;
      Color c(z.get_red() * 255, z.get_green() * 255, z.get_blue() * 255);
      bulb(x, 73, 13, z.is_on() ? c : muted, z.is_on());
    }
  } else {
    arrow(64 + (ui.selected == 0 ? 7 : -7), 8, ui.selected == 1, 14, accent);
    it.filled_circle(112, 12, 4, on ? actual : muted);
    if (ui.page == MENU) {
      palette(22, 60, 12);
      sun(64, 60, 7, ink);
      power(106, 60, 10, on ? accent : ink, on);
      it.rectangle(2 + ui.menu_item * 42, 35, 40, 53, accent);
      it.filled_rectangle(10, 100, int(v.get_brightness() * 108), 5, actual);
    } else if (ui.page == COLOR) {
      it.filled_circle(64, 55, 22, actual);
      int current = palette_index(v.get_red(), v.get_green(), v.get_blue());
      for (int i = 0; i < 5; i++) {
        const auto &p = PALETTE[i];
        int x = 16 + i * 24;
        it.filled_circle(x, 99, 7, Color(p.r * 255, p.g * 255, p.b * 255));
        if (i == current) it.circle(x, 99, 10, ink);
      }
    } else {
      sun(64, 40, 8, accent);
      int percent = int(lroundf(v.get_brightness() * 100));
      int x = percent == 100 ? 22 : (percent >= 10 ? 32 : 43);
      if (percent == 100) { digit(x, 65, 1); x += 22; }
      if (percent >= 10) { digit(x, 65, (percent / 10) % 10); x += 22; }
      digit(x, 65, percent % 10);
      it.circle(103, 69, 2, ink);
      it.line(101, 88, 113, 70, ink);
      it.circle(111, 89, 2, ink);
      it.rectangle(10, 107, 108, 6, muted);
      it.filled_rectangle(11, 108, int(v.get_brightness() * 106), 4, actual);
    }
  }
  float progress = ui.hold_progress(now);
  if (progress > 0) it.filled_rectangle(0, 0, int(progress * 128), 5, accent);
}

}  // namespace kids_lamp
