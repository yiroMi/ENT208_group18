
# Hajimi [哈基米] — User Stories

---

## Story 1 — Voice-based waste sorting

**As a recycling worker at a waste station,**
I want to call “HaJiMi” and say the name of a waste item (e.g., “milk tea bottle”),
so that the system can automatically identify the category and open the correct bin, allowing fast and accurate disposal.

**Acceptance criteria:**

* [ ] The system recognizes the command within **2 seconds** after “HaJiMi + waste name”
* [ ] The correct bin opens within **1 second** after recognition
* [ ] If recognition fails, the system provides feedback: **“Please say it again”**
* [ ] No bin opens when recognition fails
* [ ] The system correctly identifies **at least 20 common waste items**
* [ ] Recognition accuracy is **≥ 90%** during testing

---

## Story 2 — Bin fullness indication

**As a waste station attendant,**
I want to know immediately when a bin is full,
so that I can go directly to the correct bin instead of checking all bins manually.

**Acceptance criteria:**

* [ ] When a bin is full, a **red LED turns on and flashes continuously**
* [ ] The indication is clearly visible from a distance
* [ ] After the bin is emptied, the system resets within **10 seconds**
* [ ] No further alerts are triggered until the bin becomes full again

---

## Story 3 — Fire detection and alarm

**As a waste station attendant,**
I want the system to detect fire instantly and trigger a local alarm,
so that I can respond immediately even without network connectivity.

**Acceptance criteria:**

* [ ] When flame is detected, **buzzer and red LED activate within 1 second**
* [ ] The alarm continues for at least **30 seconds**, or stops automatically when the flame disappears
* [ ] The attendant can manually silence the alarm
* [ ] The system works **fully offline** (no Wi-Fi / cloud dependency required)
