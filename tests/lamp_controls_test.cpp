#include "../components/kids_lamp/lamp_controls.h"
#include <assert.h>
#include <stdio.h>

using namespace kids_lamp;

int main() {
  // Single clicks wait for the double-click window to close.
  {
    Controls ui;
    assert(ui.poll(100, true) == NONE);
    assert(ui.poll(200, false) == NONE);
    assert(ui.poll(550, false) == NONE);
    assert(ui.poll(551, false) == SINGLE);
    assert(ui.poll(600, false) == NONE);
  }
  // Double clicks emit no single, including the exact boundary.
  {
    Controls ui;
    assert(ui.poll(100, true) == NONE);
    assert(ui.poll(200, false) == NONE);
    assert(ui.poll(550, true) == NONE);
    assert(ui.poll(600, false) == DOUBLE);
    assert(ui.poll(1000, false) == NONE);
  }
  // A held button emits one long action, never a short on release.
  {
    Controls ui;
    assert(ui.poll(100, true) == NONE);
    assert(ui.poll(799, true) == NONE);
    assert(ui.poll(800, true) == LONG);
    assert(ui.poll(2000, true) == NONE);
    assert(ui.poll(2100, false) == NONE);
    assert(ui.poll(2500, false) == NONE);
  }
  // Even a delayed polling loop recognizes duration on the release edge.
  {
    Controls ui;
    assert(ui.poll(100, true) == NONE);
    assert(ui.poll(900, false) == LONG);
    assert(ui.poll(1300, false) == NONE);
  }
  // A long second press does not leak the first pending short click.
  {
    Controls ui;
    ui.poll(100, true);
    ui.poll(200, false);
    assert(ui.poll(300, true) == NONE);
    assert(ui.poll(1000, true) == LONG);
    assert(ui.poll(1100, false) == NONE);
    assert(ui.poll(1500, false) == NONE);
  }
  // A second press after the deadline starts a separate single click.
  {
    Controls ui;
    ui.poll(100, true);
    ui.poll(200, false);
    assert(ui.poll(560, true) == SINGLE);
    assert(ui.poll(650, false) == NONE);
    assert(ui.poll(1001, false) == SINGLE);
  }
  // Idle returns to HOME. Wake consumes a complete double tap.
  {
    Controls ui;
    ui.page = COLOR;
    ui.selected = 1;
    assert(ui.poll(30000, false) == NONE);
    assert(ui.sleeping && ui.page == HOME);
    assert(ui.poll(31000, true) == WAKE);
    assert(!ui.sleeping && ui.selected == 1);
    assert(ui.poll(31100, false) == NONE);
    assert(ui.poll(31200, true) == NONE);
    assert(ui.poll(31300, false) == NONE);
    assert(ui.poll(31651, false) == NONE);
    assert(ui.poll(32000, true) == NONE);
    assert(ui.poll(32100, false) == NONE);
    assert(ui.poll(32451, false) == SINGLE);
  }
  // Wake by holding also performs no long action; held buttons don't idle.
  {
    Controls ui;
    ui.sleeping = true;
    assert(ui.poll(100, true) == WAKE);
    assert(ui.poll(40000, true) == NONE);
    assert(!ui.sleeping);
    assert(ui.poll(41000, false) == NONE);
    assert(ui.poll(41351, false) == NONE);
    assert(ui.poll(71351, false) == NONE);
    assert(ui.sleeping);
  }
  // millis() rollover cannot turn a short click into a long press.
  {
    Controls ui;
    assert(ui.poll(UINT32_MAX - 50, true) == NONE);
    assert(ui.poll(49, false) == NONE);
    assert(ui.poll(400, false) == SINGLE);
  }
  assert(fabsf(next_brightness(0.42f) - 0.50f) < 0.0001f);
  assert(fabsf(next_brightness(0.30f) - 0.50f) < 0.0001f);
  assert(fabsf(next_brightness(0.50f) - 0.75f) < 0.0001f);
  assert(fabsf(next_brightness(0.75f) - 1.00f) < 0.0001f);
  assert(fabsf(next_brightness(1.00f) - 0.30f) < 0.0001f);
  assert(fabsf(next_brightness(0.05f) - 0.30f) < 0.0001f);
  assert(palette_index(1, 0, 0) == -1);
  for (int i = 0; i < 5; i++)
    assert(palette_index(PALETTE[i].r, PALETTE[i].g, PALETTE[i].b) == i);
  puts("PASS: gestures, wake, idle, rollover, brightness and palette");
}
