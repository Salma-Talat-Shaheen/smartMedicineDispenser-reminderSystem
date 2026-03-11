# 💊 Smart Medicine Dispenser & Reminder System

<p align="center">
  <img src="https://github.com/Salma-Talat-Shaheen/smartMedicineDispenser-reminderSystem/blob/main/assets/Circuit%20Diagram.png" alt="Project Logic" width="600">
</p>

## Project Description
An Arduino-Uno-based embedded system designed to automate medication schedules. It features a structured multi-mode interface and a physical dispensing mechanism, ensuring patients take their medication exactly on time through a **User-Acknowledgment** safety loop.

---

##  Hardware Core Components
* **Microcontroller:** Arduino Uno (ATmega328P)
* **Timekeeping:** DS1307 Real-Time Clock (RTC)
* **Mechanical Actuator:** SG90 Servo Motor (90° Rotation)
* **Display:** 16x2 LCD Interface
* **Indicators:** Bicolor LEDs (Flashing Alert)
* **Inputs:** 4x Push Buttons (Action, Mode, Up, Down)

---

##  System Logic (Flowchart)
<p align="center">
  <img src="https://github.com/Salma-Talat-Shaheen/smartMedicineDispenser-reminderSystem/blob/main/assets/Flowchart.png" alt="Project Logic" width="600">
</p>
The system follows a strict operational flow to ensure reliability:

1.  **Initialization:** Setup I/O, LCD, and calibrate Servo.
2.  **State Navigation:** Switch between 5 operational modes (M0-M4).
3.  **Time Calibration:** Set current time and alarm time using modulo constraints.
4.  **Active Monitoring:** Background time tracking using `millis()`.
5.  **Locked Alert Loop:** Actuators trigger and remain active until confirmed.
6.  **User Reset:** System clears alert only after physical button press.



---

##  Key Features
* **Safety Lock:** The alarm (Buzzer/LED/Servo) will NOT stop automatically; it requires manual confirmation.
* **Dynamic Messaging:** LCD toggles between the clock and health-encouraging messages (e.g., "Get Well, Salma!").
* **Absolute Positioning:** Fixed UI layout to prevent character overlap on the LCD.
* **Flicker-Free UI:** Smart screen updates for a smoother visual experience.

---

##  Repository Structure
* `/src`: Contains the `.ino` source code (Version 22.0).
* `/sim`: Proteus ISIS schematic files.
* `/docs`: Project report and high-level Flowchart.

