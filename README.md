# Arduino-Ultrasonic-Radar

This is an Arduino-based project that uses the Arduino UNO R4 Minima as the primary microcontroller, along with ultrasonic sensors, motors and displays, to function as a coherent radar-style scanning system.

**NOTE: Circuit diagrams were simulated in Tinkercad using an Arduino UNO R3 footprint, as Tinkercad does not currently support the UNO R4 Minima. The relevant pin layout and behaviour are equivalent for this project.**

## Table Of Contents:

**Version List:**

* [Version 1.0 (The First Prototype)](#version-10)
* [Version 1.5 (The First Fully Working Product)](#version-15)
* [Version 2.0 (Currently a Work in Progress)](#version-20)

**Within Each Version:**

* Hardware Used
* Software Used
* Working
* Challenges Faced

## Version 1.0

Hardware Used:

* One (1) Arduino UNO R4 Minima
* One (1) Blue LED
* One (1) Button
* One (1) Passive Buzzer
* One (1) HC-SR04 Ultrasonic Sensor

Software Used:

* Arduino IDE

Working:

* The system is ON by default. Pressing the button turns the system OFF, while pressing it again turns the system back ON.
* Once the system is turned ON, the blue LED blinks once to indicate that the system has started.
* Following this, the ultrasonic sensor begins taking distance measurements. Since this version does not use a motor or an automated rotating platform, the sensor must be manually rotated to scan for objects and measure their distance.
* When an object enters the sensing range of less than 50 cm, the LED continuously blinks for 10 seconds to indicate that an object has been detected.
* The buzzer also produces a varying frequency depending on the measured distance of the object from the sensor.
* This version of the code uses a simple distance measurement formula to determine the object's distance, unlike the upgraded and more accurate moving-average method with additional filtering used in later versions.
* Once 10 seconds have elapsed, the sensor value briefly defaults back to 999. This causes the detected object to be temporarily ignored, even if it remains within the 50 cm detection range.
* The LED then returns to its idle fading state until the object leaves the detection perimeter. If an object enters the sensing range again, the LED begins blinking and the process repeats.

Challenges Faced:

* Understanding the basic principles of wiring components to an Arduino.
* Understanding the purpose of PWM signals and how to use them effectively.
* Implementing non-blocking timers using `millis()`.
* Understanding and implementing the distance formula required to use the ultrasonic sensor.
* Implementing a basic state machine to control the LED behaviour.

## Version 1.5

Hardware Used:

* One (1) Arduino UNO R4 Minima
* One (1) Blue LED
* One (1) Button
* One (1) Passive Buzzer
* One (1) HC-SR04 Ultrasonic Sensor
* One (1) SG90 Servo Motor with a servo horn
* Cardboard
* One (1) RGB LED

Software Used:

* Arduino IDE

Working:

* The system starts in a similar manner to the previous version. The system is ON by default and can then be controlled using the button.
* Once the system has been turned ON, State 0 is engaged and the servo begins moving between 0° and 160° at a steady pace. The ultrasonic sensor is mounted on top of the servo motor and moves alongside it. Whenever the sensor detects an object within 50 cm, the system transitions into State 1 and the servo motor stops.
* The servo motor remains stationary for a total duration of 10 seconds, during which the ultrasonic sensor continuously measures the object's distance and outputs the measurements to the Serial Monitor.
* When the detected object is between 30 cm and 50 cm from the sensor, the RGB LED remains GREEN. At distances between 15 cm and 30 cm, the RGB LED turns YELLOW. Finally, at distances below 15 cm, the RGB LED turns fully RED. These colours represent the SAFE, CAUTION and DANGER distance states respectively.
* After 10 seconds have elapsed, the sensor value defaults back to 999 and the system returns to State 0, its normal scanning state. The detection cycle repeats if the object is detected again.
* If the object remains within the detection range after the 10-second sensing period, the system transitions into State 2. In this state, the blue LED blinks before the system returns to State 0.
* During the sensing stage, the buzzer continuously changes its frequency depending on the measured distance. The blue LED also continues blinking to indicate that the system is actively in its sensing mode.

Challenges Faced:

* Implementing `millis()`-based non-blocking timers for multiple components, each with their own timing intervals and subtimers, proved challenging.
* Understanding the movement and behaviour of the servo motor, particularly why it would occasionally slow down, speed up or become jittery. These issues could be caused by blocking timers within the code, faulty wiring or wires being stretched too tightly.
* Understanding which components require PWM signals and which do not, as well as learning to use the analog pins as digital pins (`A0`, `A1` and `A2`) to preserve PWM-capable pins for future upgrades.
* Properly implementing a significantly more complex state machine compared to [Version 1.0](#version-10), while also interfacing multiple components and ensuring that they work together as seamlessly as possible.
* Understanding the power requirements of the servo motor in order to account for future project enhancements.

## Version 2.0

Work in Progress
