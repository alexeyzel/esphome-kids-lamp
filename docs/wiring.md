# Wiring / Схема комутації та підключення

[English README](../README.md) · [Український README](../README.uk.md)

## Prototype data and power / Дані та живлення прототипу

This documents the current GPIO1 firmware, not an approved final power board. Rings share parallel power but receive serial data. Disconnect power before wiring.
Це схема для поточної прошивки GPIO1, а не затверджена фінальна плата живлення. Живлення кілець паралельне, сигнал послідовний. Монтаж виконуйте без живлення.

```text
DATA / ДАНІ
AtomS3R GPIO1 ── [3.3 → 5 V buffer*] ── DI Ring 1 (LED 0–7)
                                           DO ── DI Ring 2 (LED 8–15)
                                                    DO: unused / вільний

LED POWER / ЖИВЛЕННЯ LED
External +5 V ──┬── Ring 1: 5V
Зовнішні +5 В   └── Ring 2: 5V

COMMON GROUND / СПІЛЬНА ЗЕМЛЯ
External GND ──┬── AtomS3R GND
               ├── Ring 1: GND
               ├── Ring 2: GND
               └── Buffer GND (if fitted / за наявності)

CONTROLLER POWER / ЖИВЛЕННЯ КОНТРОЛЕРА
USB-C supply ───── AtomS3R USB-C
Grove red +5 V: isolated / ізольовано
```

| Net / Вузол | Connections / З'єднання |
| --- | --- |
| LED +5 V | External +5 V → ring 1 5V + ring 2 5V + buffer VCC if fitted / зовнішні +5 В → 5V обох кілець та VCC буфера за наявності |
| Common GND / Спільна GND | External GND + AtomS3R GND + both ring GND + buffer GND / GND джерела, AtomS3R, обох кілець і буфера |
| Data / Дані | GPIO1 → buffer input; buffer output → ring 1 DI; ring 1 DO → ring 2 DI / GPIO1 → вхід буфера; вихід → DI першого; DO першого → DI другого |
| Unused / Вільні | Ring 2 DO, Grove GPIO2; Grove red +5 V isolated in this two-supply bench setup / DO другого, GPIO2; червоний +5 В Grove ізолювати в цьому стендовому варіанті |

### AtomS3R HY2.0-4P

| Wire / Провід | Signal / Сигнал | Use / Використання |
| --- | --- | --- |
| Black / Чорний | GND | Common ground / Спільна земля |
| Red / Червоний | 5 V | Leave isolated in diagram above / Ізолювати для схеми вище |
| Yellow / Жовтий | GPIO2 | Unused / Не використовується |
| White / Білий | GPIO1 | LED data / Дані LED |

Verify connector orientation against the official pin map; wire colors alone are insufficient.
Звірте орієнтацію роз'єму з офіційною розпіновкою; не покладайтеся лише на кольори.

## Signal compatibility / Сумісність сигналу

*The buffer is a recommended reliability option, not a newly mandated component. ESP32-S3 outputs 3.3 V; LED power is 5 V. The ring board marking does not identify the silicon revision, so direct 3.3 V control is not guaranteed. A short direct bench connection can be evaluated separately; it is not proof of robust final operation. Never feed 5 V back into the GPIO.

*Буфер рекомендовано для надійності, але остаточний вибір ще не погоджено. GPIO має 3,3 В, LED живляться від 5 В. Маркування плати не визначає ревізію кристалів, тому пряме керування не гарантоване. Коротке пряме з'єднання можна окремо перевірити на стенді; це не підтверджує надійність фінального виробу. Не подавайте 5 В на GPIO.

SN74AHCT125 supports TTL-compatible inputs at 5 V supply. A buffer implementation must specify VCC/GND, active-low OE, unused inputs and local decoupling according to its datasheet. This functional diagram intentionally does not assign IC package pins, resistor/capacitor values, fuse ratings or final connectors; select these together with the final power design.

SN74AHCT125 має TTL-сумісні входи при живленні 5 В. Реалізація потребує визначення VCC/GND, активного низьким рівнем OE, невикористаних входів і локального розв'язування за datasheet. Ця функціональна схема не фіксує ніжки корпусу, номінали резисторів/конденсаторів, запобіжник чи фінальні роз'єми: їх слід погодити разом із живленням.

## Power and commissioning / Живлення та перший запуск

- Never power LEDs from a GPIO. Do not join independent USB/Grove 5 V supplies: the diagram uses separate positive rails and common GND. Do not assume the Grove port can deliver the full LED current.
- A conservative planning assumption of 60 mA per RGB LED gives 16 × 60 mA = 0.96 A for rings alone at full white. This is not a measured specification of these boards; add controller overhead if sharing a supply and measure actual demand before final sizing.
- Start with LEDs off, then a low brightness command from HA; local presets include 100% and there is no firmware current limiter. Check polarity, current, heating, color order and flicker. Disconnect on abnormal heating.
- One-cable final power distribution remains undecided. Keep mains outside the enclosure; use an appropriate external low-voltage supply. Chain Base pin conflicts require a separate review before adding it.

- Не живіть LED від GPIO. Не з'єднуйте незалежні +5 В USB/Grove: у схемі окремі плюсові шини та спільна GND. Допустимий струм Grove для всіх LED не підтверджено.
- Консервативне припущення 60 мА на RGB LED дає 16 × 60 мА = 0,96 А лише для кілець на повному білому. Це не виміряна специфікація плат; для спільного джерела додайте споживання контролера й виміряйте фактичний струм перед остаточним вибором.
- Починайте з вимкнених LED, потім задайте малу яскравість через HA. Локальні рівні включають 100%, програмного ліміту струму немає. Перевірте полярність, струм, нагрівання, порядок кольорів і мерехтіння. За ненормального нагрівання вимкніть живлення.
- Фінальне живлення одним кабелем ще не визначено. Мережеву напругу залишайте поза корпусом, використовуйте відповідне зовнішнє низьковольтне джерело. Перед додаванням Chain Base окремо перевірте конфлікти GPIO.

## Official references / Офіційні джерела

Verified / перевірено 2026-10-29:

- [M5Stack AtomS3R pin map and schematics](https://docs.m5stack.com/en/core/AtomS3R)
- [ESPHome RMT LED strip](https://esphome.io/components/light/esp32_rmt_led_strip/)
- [TI SN74AHCT125 datasheet and electrical requirements](https://www.ti.com/product/SN74AHCT125)
