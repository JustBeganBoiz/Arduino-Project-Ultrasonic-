# Arduino-Project-Ultrasonic
This project is an Arduino based prototype military radar which will be able to detect object using an ultrasonic sensor and also be able to track it providing Live details on it.

Version 1.0:
-

This version was the first prototype which involved the following components:
- One(1) HC SR04 Ultrasonic Sensor
- One(1) Passive Buzzer
- One(1) Blue LED
- One(1) Button

This version was very primitive as it didnt do anything much. The sensor was hand guided and whenever it detected an object, The LED blinked twice, followed by the Buzzer turning on and getting louder or quieter depending on the distance from the sensor. 

Exact Working:
1. When the button is presse the entire apparatus turns on and the LED blinks once
2. LED is in idle state where it just glows by itself in a cycle.
3. If an object comes into the sensing range of the sensor then the led will start blinking and the buzzer buzzes based on the distance and, when the object is in this range for more than 5 seconds the LED will stop blinking and the apparatus is back into idle mode

Challenges Faced:
- Understanding just how the basic connections in an Arduino even work?
- Whats the difference between Active and Passive Buzzer?
- Why do we need PWM?
- How do we interface all these various components together?

Version 1.5(currently work in progress):
-

The following components are added(along with the afforementioned components already added):
- One(1) SG90 Servo Motor
- Cardboard

This version, added the functionality of automatically controlling the Ultrasonic sensor instead of manually detecting the object by hand. This is achieved using a cardboard attached to a servo motor which controls the ultrasonic sensor.

Working:
- Same as before except with the added functionality of a moving ultrasonic sensor via the servo motor.

