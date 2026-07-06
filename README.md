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

Version 1.5:
-

The following components are added(along with the afforementioned components already added):
- One(1) SG90 Servo Motor
- Cardboard

This version, added the functionality of automatically controlling the Ultrasonic sensor instead of manually detecting the object by hand. This is achieved using a cardboard attached to a servo motor which controls the ultrasonic sensor.

Working:
- Same as before except with the added functionality of a moving ultrasonic sensor via the servo motor.
- The motor will automatically stop when it encounters an obstacle for a timeframe of 10 seconds. 

Challenges Faced:
- Just trying to properly implement all the different timers properly in order to make sure that nothing interfered with anything
- understanding what is a debounce window when it comes to buttons and acoustics
- state machines to an extent and just how complicated things can ge


***Photos to be Uploaded***

Version 2.0(Work In Progress):
- 

The Following Components to be Added:
- One(1) SG90 Servo Motor
- One(1) HC-SR04 Ultrasonic Sensor
- One(1) RGB Led

Working:
- There are two sensors working in tandem now
- One servo motor keeps scanning for objects 
- The Other motor stays idle until the first sensor detects something in which case the sensor activates and then the servo motor will point towards the object in question
- the sensor will track the object for 10 seconds as accurately as possible due to sensor and motor limitations
- An RGB LED will also showcase how far the object is in two stages:
  - Green: Beyond 20cm of the sensor
  - Yellow: Within 20cm of the sensor
  - Red: Within 10cm of the sensor
