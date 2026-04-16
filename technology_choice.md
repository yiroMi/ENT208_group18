Technology Stack — Smart Waste Sorting Bin
The big picture
The Smart Waste Sorting Bin is an integrated embedded system:

text
┌─────────────────────────────────────────────────────────────────────────────┐
│  SMART BIN (Embedded System)                                                  │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐    │
│  │ Voice input  │→│ Microcontrol- │→│ Servo output │→│ Display      │    │
│  │ (SU-03T)     │  │ ler (STM32)  │  │ (SG90)       │  │ (OLED)       │    │
│  └──────────────┘  └──────────────┘  └──────────────┘  └──────────────┘    │
│         ↑                ↓                    ↑                ↑            │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐    │
│  │ Overflow     │←│ Overflow     │  │ Fire         │  │ Manual       │    │
│  │ detection    │  │ indicator    │  │ detection    │  │ control      │    │
│  │ (IR beam)    │  │ (Red/green   │  │ (Flame       │  │ (Push        │    │
│  │              │  │  LED)        │  │  sensor)     │  │  buttons)    │    │
│  └──────────────┘  └──────────────┘  └──────────────┘  └──────────────┘    │
│                            ↓                                                │
│                    ┌──────────────┐                                         │
│                    │ Local alarm  │                                         │
│                    │ (Buzzer+LED) │                                         │
│                    └──────────────┘                                         │
└─────────────────────────────────────────────────────────────────────────────┘
Everything runs offline — no cloud, no API, no Wi-Fi required. The bin listens, decides, and acts locally.

Main controller — STM32F103C8T6
The STM32 is the brain of the bin. It reads sensor data, processes voice commands, and controls servos, LEDs, and the display.

We chose the STM32F103C8T6 because:

72MHz clock speed — fast enough to drive 4 servos, 4 IR sensors, a voice module, 4 indicator LEDs, an OLED, and 4 push buttons simultaneously without lag

Rich peripherals — multiple PWM channels (for servos), UART (for voice module), I2C (for OLED), and GPIO (for sensors, LEDs, and buttons)

Student-friendly — abundant tutorials, libraries, and example code available online

What we considered instead:

Arduino Nano (16MHz, fewer PWM pins) — would struggle to control 4 servos while handling UART and sensor polling. The servo control would become jittery under load.

ESP32 (240MHz, built-in Wi-Fi) — overkill for an offline system. The Wi-Fi module would be unused, and ESP32 has higher power draw (~240mA idle vs ~50mA on STM32).

The trade-off: STM32 requires a separate programmer (ST-Link) and a slightly steeper learning curve than Arduino. But the performance gain justified this trade-off.

Docs: https://www.st.com/en/microcontrollers-microprocessors/stm32f103c8.html

Voice recognition — SU-03T (offline module)
The SU-03T converts spoken waste names (e.g., "milk tea bottle") into a digital command code (e.g., 0x01 for recyclable) and sends it to the STM32 via UART.

We chose the SU-03T because:

Fully offline — no network, no API key, no ongoing cost. The bin works anywhere, even in basements or areas without Wi-Fi.

Customisable — we program our own wake words ("HaJiMi") and waste name commands into the module's flash memory. We can map "milk tea bottle" → recyclable, "banana peel" → kitchen waste, etc.

Fast — recognition takes <1 second. The user doesn't wait.

What we considered instead:

LD3320 (another offline chip) — similar specs, but documentation is mostly in Chinese and community support is smaller. The SU-03T has better English resources.

Cloud-based (Google Speech, Baidu API) — would require Wi-Fi, an internet connection, and ongoing API fees. Also introduces latency (2–5 seconds) and privacy concerns (sending audio to a server). Our user story requires offline operation.

The trade-off: The SU-03T only recognises pre-programmed phrases — it cannot understand arbitrary sentences. That is fine for our use case: waste names are a fixed, small set. The trade-off (fixed vocabulary) gives us speed, privacy, and zero recurring cost.

Docs: SU-03T datasheet (supplied with module)

Overflow detection — Infrared beam sensor (separate emitter/receiver)
An IR emitter shines a beam toward a receiver. When a bin is full, waste blocks the beam. The receiver's output changes, and the STM32 detects this.

We chose the IR beam sensor because:

Non-contact — no moving parts, no physical trigger to jam

Environmentally immune — unaffected by dust, humidity, or temperature changes (unlike ultrasonic sensors, which can give false readings near reflective surfaces)

High accuracy — triggered only when the beam is physically blocked. No calibration drift.

What we considered instead:

Ultrasonic sensor (HC-SR04) — measures distance by sound waves. Can give false readings if the bin has angled walls or reflective surfaces (e.g., a shiny bottle cap).

Laser ranging (TOF sensor) — very accurate but overkill for a binary "full / not full" detection.

The trade-off: IR beam requires precise alignment during assembly (emitter must point directly at receiver). We mitigated this with mounting brackets and hot glue to lock alignment. The reliability after calibration justified the manual setup time.

Docs: Generic IR beam sensor datasheet (supplied with component)

Lid actuation — SG90 servo
The SG90 servo rotates the bin lid open (90°) when a valid voice command is received, then closes it after 3 seconds.

We chose the SG90 because:

Simple control — standard PWM signal (50Hz, pulse width 1–2ms). The STM32's timer peripherals generate this directly.

Enough torque — 1.8 kg·cm at 5V. Bin lids are lightweight plastic — this is plenty.

What we considered instead:

MG995 / MG996R (metal-gear, high-torque servos) — stronger (10+ kg·cm) but draw more current (~500mA vs ~200mA). A USB power bank cannot supply the peak current for four MG995s. Also overkill for small lids.

Solenoid with spring return — would require a separate driver circuit and draws continuous current to hold open. Less energy-efficient.

The trade-off: The SG90 has plastic gears that can strip if the lid jams. We added software protection (limited PWM duty cycle, open-failure detection) to reduce this risk. The low power draw made this trade-off acceptable.

Docs: TowerPro SG90 datasheet

Fire detection — Flame sensor (digital output)
The flame sensor detects a lighter or candle flame (760–1100nm wavelength) and outputs a digital signal to the STM32, which triggers the buzzer and LED alarm.

We chose the flame sensor because:

Fast response — <1 second detection time

Simple interface — digital output (HIGH when flame detected). No ADC or calibration required.

What we considered instead:

Smoke sensor (MQ-2) — detects smoke, not flame. A fire can produce smoke, but smoke detection takes 10–30 seconds. Too slow for our "immediate alarm" requirement.

CO sensor (MQ-7) — detects carbon monoxide. Requires preheating (60 seconds) and analogue calibration. Overkill for a simple flame alarm.

The trade-off: The flame sensor requires the flame to be within ~1 meter and directly visible. It cannot detect a fire behind an object. We accepted this limitation because the bin is typically placed in an open area (e.g., a dorm hallway), and the requirement is "detect an open flame near the bin" — not a full fire detection system. For a student project, this is sufficient.

Docs: Flame sensor (KY-026) datasheet

Local alarm — Buzzer + High-power LED
When the flame sensor detects a fire, the STM32 turns on a buzzer (audible alarm) and a bright LED (visual alarm).

We chose this combination because:

Audible + visual — works for people who are nearby (hear the buzzer) or across the room (see the flashing LED)

Offline — works even if the building's network is down

What we considered instead:

Passive buzzer — requires a PWM signal to produce a tone. An active buzzer (our choice) only needs a HIGH output — simpler firmware.

Speaker + voice playback — would require pre-recorded audio and an amplifier circuit. More expensive and complex.

The trade-off: A buzzer is annoying by design — that is the point. There is no way to send a silent notification to a phone because the bin has no network. For a public bin, a loud local alarm is appropriate.

Overflow indicator — Red/green LED (one per bin)
Each bin has a red/green LED. When the bin is not full, it shows green. When the bin is full, the STM32 switches the LED to red and makes it flash. Workers can see at a glance which bin needs emptying during walkthroughs.

We chose the red/green LED because:

Intuitive — red = full / needs emptying, green = normal. Workers can check status without approaching or operating anything.

Low power — each LED draws ~20mA, far less than an LCD or screen solution.

Low GPIO usage — each LED uses only 2 GPIO pins (one for red, one for green).

What we considered instead:

Shared LCD display — could show detailed status for each bin, but requires workers to walk up and read it. The LCD backlight also draws continuous power (~100mA).

Single-colour LED — using just a red LED (on = full, off = not full) cannot distinguish between "not full" and "system fault". A bi-colour LED provides extra information: green = working and not full, red = full, off = fault.

Wireless notification (App push) — would require a Wi-Fi module and cloud server, adding cost and power draw. Workers would also need to constantly check their phones. For a walkthrough inspection scenario, LEDs are the most direct solution.

The trade-off: LEDs cannot show which waste category is full (workers need to walk closer to read the bin label). We consider this acceptable: workers pass each bin one by one during inspection and can take action when they see a red LED. Compared to LCD or wireless solutions, the advantages in power and simplicity are clear.

Status display — 0.96" OLED (I2C)
The OLED screen shows the bin type ("Recyclable", "Kitchen waste", etc.) and example waste names ("Milk tea bottle", "Banana peel") to help residents sort correctly.

We chose the OLED because:

Clear text — high contrast, readable in any lighting

I2C interface — uses only 2 pins (SDA, SCL) on the STM32, leaving more GPIO for sensors

Low power — ~20mA when on

Small size — fits on the bin's front panel

What we considered instead:

LCD 1602 (character LCD) — cheaper but requires 6–8 pins (parallel interface) and has poor contrast in dim light. Also larger — would not fit on the bin's front panel easily.

TFT screen — full colour, but requires more complex graphics code. Overkill for simple text.

The trade-off: The OLED is more expensive than LCD 1602, but the better readability and smaller size justified the choice for this project.

Docs: SSD1306 (OLED driver) datasheet

Manual control — Tactile push buttons (one per bin)
Each bin has a tactile push button. When a worker presses the button, the STM32 detects the GPIO level change. If the bin is not full, it drives the servo to open the lid (same logic as voice control). If the bin is full, the button has no effect.

We chose tactile push buttons because:

Simple and reliable — mechanical switch, no pull-up/pull-down resistor needed (STM32 internal configuration available)

Intuitive operation — workers need no training; press to open

Reuses existing logic — shares the same lid-opening function as voice control, giving high code reusability

What we considered instead:

Capacitive touch buttons — no mechanical wear, but require专门的 touch detection circuitry and are sensitive to humidity (wet hands may cause false triggers or fail to trigger).

Mobile App control (Bluetooth) — would require adding a Bluetooth module and developing an App. Workers would need to take out their phone, open the App, connect, and tap — too many steps for a quick inspection scenario.

Infrared remote control — requires a remote and IR receiver. Workers would need to carry the remote, which is easy to lose.

The trade-off: Tactile push buttons are mechanical parts and may wear out over time. However, our use case involves workers inspecting a few times per day — not high-frequency operation. For a 4-week project demonstration, they are completely reliable. Simplicity far outweighs durability concerns.

Power supply — USB power bank (5V)
The entire system runs from a standard USB power bank (5V output). Peak current is ~200mA when multiple servos move simultaneously.

We chose a USB power bank because:

Off-the-shelf — no custom power supply design required

Safe — built-in overcurrent, overvoltage, and short-circuit protection

Portable — the bin can be placed anywhere, not near a wall outlet

Rechargeable — the power bank can be swapped or recharged without opening the bin

What we considered instead:

Li-ion battery + 5V regulator — would require a charging circuit, over-discharge protection, and a custom enclosure. Adds significant assembly time.

AA batteries (4×1.5V) — 6V nominal, but voltage drops as batteries drain. Servos would slow down and eventually stop working correctly. Also creates waste from disposable batteries.

The trade-off: A USB power bank adds a small recurring cost (electricity for recharging) but eliminates the complexity of building a battery management system. For a student project, the simplicity is worth it.

Summary
Layer	Component	Why
Main controller	STM32F103C8T6	72MHz, rich peripherals, student-friendly
Voice recognition	SU-03T	Offline, customisable, <1s response
Overflow detection	IR beam sensor	Non-contact, low cost, environment-proof
Lid actuation	SG90 servo	Simple PWM control, enough torque
Fire detection	Flame sensor	Fast response, simple digital output
Local alarm	Buzzer + LED	Audible + visual, works offline
Overflow indicator	Red/green LED (one per bin)	Intuitive (red=full), low power, walkthrough-friendly
Status display	0.96" OLED (I2C)	Clear text, 2-pin interface, low power
Manual control	Tactile push button (one per bin)	Simple, reliable, reuses lid-open logic
Power supply	USB power bank	Off-the-shelf, safe, rechargeable, portable
This is the complete 10-component English version without cost/pricing information.
