# Operating logic / Логіка роботи

[English README](../README.md) · [Український README](../README.uk.md)

## English

The firmware models one 16-LED data chain as two independent RGB lights: zone 1 (LED 0–7, left arrow) and zone 2 (LED 8–15, right arrow). The transport strip is internal and hidden from HA. Changing one zone leaves the other unchanged.

| Screen | Short press | Double press | Hold ≥700 ms |
| --- | --- | --- | --- |
| Zone selection | Select other zone | Both off if either is on; otherwise both on | Open selected zone menu |
| Zone menu | Cycle color → brightness → power | Return to zone selection | Open setting; on power, toggle selected zone |
| Color setting | Next palette color | Return to zone selection | Return to zone menu |
| Brightness setting | Next brightness preset | Return to zone selection | Return to zone menu |

A short action waits for the 350 ms double-click window. Long/double gestures do not also trigger a short action. Input debounce is 20 ms and gesture polling is every 10 ms. Holding shows a progress indicator along the top edge.

Entering color/brightness turns the selected zone on using its existing values; entry itself does not advance a value. Changes apply immediately, with no save/cancel step. Leaving a setting leaves the light on. Browsing menu items does not turn a light on.

Brightness presets: **30 → 50 → 75 → 100 → 30%**. A value set by HA advances to the next strictly higher preset (42 → 50%). HA can select brightness below 30%; there is no enforced maximum below 100%.

Palette: **warm white → yellow → green → light blue → purple**, wrapping to white. RGB fractions: `(1,.85,.70)`, `(1,.8,0)`, `(0,1,0)`, `(0,.6,1)`, `(.6,0,1)`. Arbitrary HA colors are supported; the next local color action after an off-palette value selects warm white. RGB white is an approximation, not a calibrated color temperature.

Light transitions default to 1 second. No local sleep countdown, scheduled fade, alarm animation or Chain Encoder control is implemented.

After 30 seconds without button activity, a stationary dim moon appears. The entire first wake gesture is consumed and only opens zone selection. HA commands update light values without waking the display, changing navigation or resetting its idle timer. Display PWM is active 80/255, idle 128/255; a dim drawing does not imply lower backlight power.

On reboot both zones are off, navigation starts at zone selection, and saved individual color/brightness values are retained for the next switch-on. Without saved values the defaults are 30% and warm white. Flash writes are batched at 5 seconds: wait at least that long before power removal; the last change can be lost on abrupt power loss.

HA exposes two RGB lights and two buttons (both on/both off). Local and HA commands affect the same light state in arrival order. Wi-Fi/API reboot timeouts are disabled, so loss of connection alone does not restart the lamp. Local controls remain available offline; remote control requires connectivity.

## Українська

Один ланцюжок із 16 LED поділено на два незалежні RGB-світильники: зона 1 (LED 0–7, ліва стрілка) і зона 2 (LED 8–15, права стрілка). Транспортний ланцюжок прихований від HA. Зміна однієї зони не змінює другу.

| Екран | Коротке | Подвійне | Утримання ≥700 мс |
| --- | --- | --- | --- |
| Вибір зони | Вибрати іншу зону | Вимкнути обидві, якщо хоча б одна ввімкнена; інакше ввімкнути обидві | Меню вибраної зони |
| Меню зони | Колір → яскравість → живлення | До вибору зони | Відкрити налаштування; на живленні — перемкнути вибрану зону |
| Колір | Наступний колір | До вибору зони | До меню зони |
| Яскравість | Наступний рівень | До вибору зони | До меню зони |

Одиночна дія очікує завершення вікна подвійного натискання 350 мс. Довге й подвійне не виконують додаткової короткої дії. Фільтр брязкоту — 20 мс, опитування — кожні 10 мс. Утримання показує смуга вгорі екрана.

Вхід у колір/яскравість вмикає вибрану зону з поточними значеннями, не перемикаючи їх. Зміни застосовуються одразу, без підтвердження чи скасування. Вихід залишає світло ввімкненим. Гортання пунктів меню світло не вмикає.

Рівні: **30 → 50 → 75 → 100 → 30%**. Після довільного значення з HA обирається наступний строго більший рівень (42 → 50%). Через HA доступні значення нижче 30%. Максимум програмно не обмежено нижче 100%.

Палітра: **теплий білий → жовтий → зелений → блакитний → фіолетовий**, далі знову білий. Частки RGB: `(1,.85,.70)`, `(1,.8,0)`, `(0,1,0)`, `(0,.6,1)`, `(.6,0,1)`. Після кольору HA поза палітрою наступна локальна дія вибирає теплий білий. Це наближений RGB-відтінок, а не калібрована колірна температура.

Типовий перехід світла — 1 с. Локального таймера засинання, згасання за розкладом, тривожної анімації та керування Chain Encoder поки немає.

Через 30 с без кнопки з'являється нерухомий тьмяний місяць. Перший жест повністю поглинається й лише відкриває вибір зони. HA оновлює значення без пробудження, зміни навігації чи скидання таймера. PWM екрана: активний 80/255, очікування 128/255; темна картинка не означає нижчу потужність підсвічування.

Після перезапуску обидві зони вимкнені, відкрито вибір зони; індивідуальні колір і яскравість відновлюються для наступного ввімкнення. Без збережених значень — 30% і теплий білий. Запис flash відкладено до 5 с: зачекайте щонайменше цей час перед відключенням; за раптової втрати живлення остання зміна може загубитися.

HA має два RGB-світильники та кнопки ввімкнення/вимкнення обох. Локальні та HA-команди змінюють спільний стан у порядку надходження. Таймаути перезапуску Wi-Fi/API вимкнені. Локальне керування працює без мережі; віддалене потребує з'єднання.
