# Hajimi[哈圾米] —— User Roles
<!--
WHAT IS A USER ROLE?
A user role defines who uses the system and what they are allowed to do.
Defining roles ensures that each function matches real user needs and prevents misuse.
-->

Two main roles use the Smart Waste Bin System. Every feature belongs to one or both.

---

## Role overview

| Role                        | Who they are                                                               | Typical number |
| --------------------------- | -------------------------------------------------------------------------- | -------------- |
| **Resident User**           | A person disposing of waste using the smart bin (e.g., community resident) | Many           |
| **Waste Station Attendant** | A worker responsible for managing bins and ensuring safety                 | 1–3            |

---

## Resident User

**Goal:** Dispose of waste quickly and correctly without needing to know classification rules.

**Can do:**

* Use voice input to describe waste (e.g., “milk tea bottle”)
* Trigger automatic lid opening for the correct bin
* View simple guidance on the OLED screen (e.g., waste examples)
* Dispose of waste without manual sorting knowledge

**Cannot do:**

* Access system settings or internal data
* Manually override bin status (e.g., open a full bin)
* View or modify system logs or sensor data
* Disable safety features such as fire detection

---

## Waste Station Attendant

**Goal:** Monitor bin status, reduce manual workload, and ensure operational safety.

The attendant interacts with the system regularly and relies on automated feedback.

**Can do:**

* Monitor bin fullness via LED indicators (red = full, green = not full)
* Receive fire alerts through buzzer and high-power LED
* Manually open bin lids using push buttons (if bin is not full)
* Observe system status and guidance on the OLED display
* Respond to abnormal situations (e.g., fire hazard, overflow)

**Cannot do:**

* Override safety constraints (e.g., force open a full bin)
* Modify voice recognition keyword mappings
* Access or modify embedded firmware directly

---

## Rules for the system

Apply these rules when designing or implementing features:

1. **Default to Resident User interaction.** The system should be simple and intuitive, requiring no prior knowledge.
2. **Safety overrides convenience.** If a bin is full or a fire is detected, the system must restrict normal operations.
3. **Attendant has monitoring, not control authority.** The system assists rather than relying on manual intervention.
4. **All processing is local.** No cloud, database, or external API is used — all decisions are made on-device.
5. **Real-time response is critical.** Actions such as lid opening and fire alarms must occur immediately.

