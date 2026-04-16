# Hajimi(哈圾米） — Product Requirements

<!-- WHAT IS THIS FILE?
This file describes what SmartBin is, who uses it, and what it must do.
-->

---

## What it is

**Hajimi** is an intelligent waste classification system designed for community waste stations.

It combines **voice interaction, sensor detection, and automatic control** to help users classify garbage more conveniently, while also assisting staff in managing garbage bins more efficiently and safely.

The system runs entirely on **embedded hardware (STM32)** and does not rely on cloud services.

---

## The problem

Current waste stations rely heavily on **manual inspection and human judgment**, which leads to three key issues:

1. **Low efficiency**  
   Attendants must walk around to check each bin’s status.

2. **Incorrect waste sorting**  
   Residents often don’t know how to classify waste properly.

3. **Delayed fire risk detection**  
   Dangerous items (e.g., cigarettes) may cause fire hazards that are not detected in time.

The result:  
Higher workload, lower sorting accuracy, and potential safety risks.

---

## Users

| Role | Primary goal | Context |
|---|---|---|
| **Resident / User** | Quickly dispose of waste correctly | At waste station |
| **Waste Station Attendant** | Monitor bin status and respond to risks efficiently | On-site worker |

---

## Core requirements

These MUST work end-to-end before adding any extra features.

| # | Requirement | Role |
|---|---|---|
| R1 | User can speak a waste name and system identifies category | Resident |
| R2 | Correct bin lid opens automatically after recognition | Resident |
| R3 | Each bin is equipped with a button for manual control | All users |
| R4 | When a bin is full, a red LED turns on or flashes as an alert | Attendant |
| R5 | When a flame is detected, the system immediately triggers a buzzer alarm | Attendant |
| R6 | Flame detection and alarm function works even without network connection | Attendant |
| R7 |System responds within defined time limits (voice + action) | All users |

---

## Constraints

| Constraint | Detail |
|---|---|
| **Offline-first** | System must work without internet |
| **Real-time response** | Voice + action must complete within ~3 seconds |
| **Low cost** | Must use affordable components (student budget) |
| **Hardware-based** | Runs on STM32, no cloud dependency |
| **Simple interaction** | Designed for non-technical users |

---

## Out of scope

Do NOT build these:

| Out of scope | Reason |
|---|---|
| Mobile app / web dashboard | Not required for this prototype |
| Cloud storage / database | System is fully local |
| User accounts / login system | No need for authentication |
| Remote notifications | Only local alerts required |
| Complex AI models | Use predefined voice commands only |

---

## Definition of done

The product is ready when:

- [ ] All 7 core requirements (R1–R7) work end-to-end on a real mobile device
- [ ] Voice → lid response < 3 seconds
- [ ] Overflow detection works reliably
- [ ] Fire alarm triggers correctly
- [ ] Tested with at least 5 users
- [ ] At least 5 validation sessions are documented in the Validation Report
- [ ] The Technical Documentation describes the architecture, stack choices, and data model
- [ ] The physical prototype is fully assembled and can be demonstrated in person to any assessor
