# Arduino-Ultrasonic-Radar  
This is an Arduino based project which uses the Arduino UNO R4 Minima as the microcontroller head along with ultrasonic sensors, motors and displays in order to work as a coherent radar system.

## Table Of Contents:
Version's List
- [Version 1.0(The First Prototype)](#version-10)
- [Version 1.5(The First Fully Working Product)](version-15)
- [Version 2.0(Currently Work in Progress)](#version-20)
 
**1.0**
- Hardware Used
- Software Used
- Working
- Challenges Faced

**1.5**
- Hardware Used
- Software Used
- Working
- Challenges Faced

**2.0**
- Hardware Used
- Software Used
- Working
- Challenges Faced

## Version 1.0
Hardware Used:
- One(1) Arduino UNO R4 Minima
- One(1) Blue LED
- One(1) Button
- One(1) Passive Buzzer
- One(1) HC-SR04 Ultrasonic Sensor

Software Used:
 - Arduino IDE

Working:
- The system's default state is ON. When you press the button it turns it OFF and when you press it again you can turn it ON again
- Once it's turned on, the blue LED blinks once to indicate the starting of the system
- Post that, the sensor starts sensing and since there's no motor to automatically turn it around or an automatic turning platform the sensor has to be manually rotated and turned in order to sense objects and distances
- when something enters the sensing range of less than 50cm then the LED continuosly blinks for 10 seconds in order to signify that an object has been detected.
- The buzzer also buzzes with varying frequency depending on the distance from the sensor.
- This version of the code uses a simple distance formula measurement to gauge the distance unlike the upgraded, more accurate moving average method with filters.
- If 10 seconds have elapsed then the sensor defaults back to 999 for a brief moment because then the object will be ignored even if it's within 50cm
- the led then goes back to it's idle state of fading until the object leaves the perimeter in which the led blinks again and this process repeats

Challenges Faced:
- Understanding the basic principles of wiring in an Arduino
- What's the use of PWM in digital signals and how to use it effectively
- How to implement nonblocking timers in the code using millis()
- How to use the distance formula to use the ultrasonic sensor
- How to implement a very small state machine to program the LED

## Version 1.5
Hardware Used:
- One(1) Arduino UNO R4 Minima
- One(1) Blue LED
- One(1) Button
- One(1) Passive Buzzer
- One(1) HC-SR04 Ultrasonic Sensor
- One(1) SG90 Servo Motor with a servo horn
- Some Cardboard
- One(1) RGB LED

Software Used:
 - Arduino IDE

Working:
- The starting of this system is the same as the previous version. The system starts at ON state and is then controlled by the button.
- Once the system has been turned ON, state 0 is engaged and the servo starts moving from 0°-160° at a steady pace. The ultrasonic sensor is mounted on top of the servo motor and moves alongside it and, whenever the sensor detects an object within 50cm, the system goes into state 1 and the servo motor stops.
- The servo motor stops for a total duration of 10 seconds during which the sensor constantly gauages the distance and outputs it on the serial monitor.
- When the servo is at a distance from 50cm to 30cm, the RGB LED stays at a GREEN state. When the servo is at a distance from 30cm to 15cm it turns to YELLOW and lastly if its less than 15cm then the RGB turns fully RED indicating SAFE, CAUTION and DANGER states of the distance.
- Following 10 seconds, the sensor value defaults back to 999 and goes into state 0 again which is the natural scanning state and then the cycle repeats if the object is again at the same place.
- If during the 10 seconds the object leaves the place then the system goes into state 2 which blinks the blue LED followed by going back into state 0.
- During the sensing stage, the buzzer constantly changes its frequency depending on the distance and the blue LED keeps blinking to also indicate the sensing mode

Challenges Faced:
- The millis() nonblocking timer for so many components and their subtimers proved to be quite a challenge to implement properly.
- The movement of the servo motor and why it would slow down or speed up or sometimes even become very jittery either due to some blocking timers which were invloved during the making the code or due to faulty wiring or the wires being stretched taut.
- Understanding which components will require PWM signals and which do not as well as finding out the use of analog pins as digital pins (A0,A1,A2) to save the PWM pins for further upgrades. 
- Properly implementing a far more complex state machine as compared to [Version 1.0](#version-10) and also properly interfacing all these various components together to work as seamlessly as possible.
- Understanding the power draw of the servo motor in order to keep track of future project enhancements

## Version 2.0

Work in Progress
