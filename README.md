# Arduino-Ultrasonic-Radar

This is an Arduino-based project that uses the Arduino UNO R4 Minima as the primary microcontroller, along with ultrasonic sensors, motors and displays, to function as a coherent radar-style scanning system.

**NOTE: Circuit diagrams were simulated in Tinkercad using an Arduino UNO R3 footprint, as Tinkercad does not currently support the UNO R4 Minima. The relevant pin layout and behaviour are equivalent for this project.**

## Table Of Contents:

**Version List:**

* [Version 1.0 (The Prototype)](#version-10)
  * [What's New?](#whats-new-in-version-10)
* [Version 1.5 (The First Fully Working System)](#version-15)
  * [What's New?](#whats-new-in-version-15)
* [Version 2.0 (A Key Jump)](#version-20)
  * [What's New?](#whats-new-in-version-20)
* [Version 2.1 (Minor Upgrade)](#version-21)
  * [What's New?](#whats-new-in-version-21)
* [Version 2.2](#version-22)
 
**Within Each Version:**

* Hardware Used
* Software Used
* Working
* Challenges Faced

**Now adding a whole new addition and that is the [Improvements Made In Every Version] as compared to the previous version.**

## Version 1.0

### Whats New In Version 1.0
- Nothing, this is the beginning of the project and is, frankly speaking, a prototype for the much more polished future versions with all the added features however in this version ther already was a base for:
- The Microcontroller(in this case the Arduino UNO R4 Minima) of course which handles all the processing and timer logic
- This version of the project is using delay() based timers and will be upgraded using millis() based non-blocking timers in later versions
- It includes the functionality of turning on the system using a button
- A buzzer to buzz depending on the distance from the object
- An ultrasnic sensor which is used to sense the distance from the object and works on simple distance formula measurements
- A Blue LED which is used to showcase the primitive states which is SCANNING and NON_SCANNING

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

### Whats New In Version 1.5
- A fully working state machine
- A servo motor in order to control the direction of the Sensor
- An RGB LED to alert the user of the various distances the object is from the sensor(RED, YELLOW, GREEN)
- An improved moving average based distance measurement instead of a standard direct measurement using distance formula

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

### Whats New In Version 2.0
- An upgraded and clearly defined finite state machine(SFM) with clearly defined states(SCANNING, SENSING, TRANSITION)
- A  further upgraded median filter which improves upon the moving average filter with faster, more robust readings as well as reducing the calculation overhead which was previously required
- DRASTICALLY improved the overall readability of the code
- VASTLY reduced the overall "jank" or dead code which was existing in the previous code with well placed and well named constants
- Improved the upgradeability of the project by SEVERAL folds by switching to a modular programming based approach
- Fixed the bugs which were present in the previous versions namely, the improper state switching and the irregular reading of the sensor

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
* The Working of the system is the same as the previous version but the significant difference comes in the extensive commenting and the use of clearly defined states as well as building upon the functional and switch case logic wherever possible

Challenges Faced:
* Understanding the key difference between procedural programming and modular programming using finite state machines (FSM's) as well as using function based programming
* Interlacing of all the various different functions and making sure the entire code block doesn't devolve into unknown states and is clearly defined with objectives and goals outlined

**NOTE: NO photos or videos were shot for this version nor were any changes to the circuit done hence only the code and the README is being modified**

## Version 2.1 

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
- The System works on the same basic principle that the previous version had except now additional functionality has been added or, to be precise, allocation for the functionality has been added onto the system.
- **ALL** the functions that the previous version did is now within the "BASIC" mode of the system and that mode has been successfully declared as *functionally* complete
- When you turn the system ON, the blue LED blinks twice (quicker than before) and then the RGB led will blink a PURPLE color 4 times
- The system starts with the power being turned OFF and must be turned on via a button
- After the button gets pressed, the RGB flashes WHITE once followed by running the system in BASIC mode (all the functionalities of Version 2.0).
- The button pressing and the white light flashing is now fixed regardless of how many times you reset
- When you reset via a single short press it puts the Servo motor(and by extension the Sensor) at the zero position.
- When you hold the button(whilst the control is ON) for 4 seconds, the mode will be switched to ADVANCED(feature set to be developed)
- Similarly you can switch back to BASIC mode
- Whilst the BASIC mode is running, during the SCANNING cycle the buzzer will buzz at set intervals similar to a radar

Challenges Faced:
- Further improving the use of non-blocking timers and State Machines especially since the program has evolved into a complicated 600+ lines program with several states WITHIN states.
- How exactly do we divide this GINORMOUS program properly so that it can be developed as efficiently as possible while not needlessly dividing the program
- What's the usage of "ifndef", "define" and "exitif" and the overall usage of .h files in order to store the global variables.
- How to stop one self from needlessly adding features when it's not needed
- How exactly does one maintain a huge program such as this when 600+ lines without running into roadblocks constantly
- When to omit a feature when the implementation of that particular feature would hinder the overall development siginificantly

## Whats New In Version 2.1
- The headline feature is the creation of two seperate MODES within the system called as BASIC and ADVANCED
- The new sonar-based pinging system using the Buzzer during the SCANNING cycle
- The new WHITE color blinking from the RGB LED to indicate the power shifting from OFF to ON
- Two different glow's of the LED when the modes get switched. The two different glow's being PURPLE and a multi-color glow
- Division of the program into several different files in order to implement proper segmentation and reduce clutter in the main program and improve developmental efficiency

## Version 2.2

Work in progress
