#include "Globals.h"

Modes Shifter = BASIC;                   //this controls the Shifting of the States
Power Control = TURN_OFF;                //this controls the Power Supply (default: POWERED OFF)
SystemState CurrentState = SCANNING;     // this controls the States
Modes lastShifterState = BASIC;          // keeping track of states
FlashColour currentFlashColor = PURPLE;  // the glow for the RGB when state changes occur

void setup() {
  pinMode(LED, OUTPUT);  //defining the pin outs

  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);

  pinMode(Buzz, OUTPUT);

  pinMode(Button, INPUT_PULLUP);
  buttonState = digitalRead(Button);
  lastButtonState = buttonState;

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  myservo.attach(3);

  analogWrite(LED, 0);  //blinking action of the blue LED on startup
  delay(150);

  analogWrite(LED, 255);
  delay(300);

  analogWrite(LED, 0);
  delay(150);

  analogWrite(LED, 255);
  delay(300);

  analogWrite(LED, 0);
  delay(600);

  Serial.begin(115200);  //the baud rate
}

void loop() { 

  if (!Started) {
    isFlashing = true;  // to show that the RGB will now start flashing
    currentFlashColor = PURPLE;
    lastShifterState = Shifter;
    Started = true;
  }

  if (isFlashing) {
    Startup();            //this function controls the RGB LED
    analogWrite(LED, 0);  //to keep the blue LED when the RGB is flashing
    noTone(Buzz);         //keeping the Buzzer off
    return;               //basically a reset
  }

  ProcessButton();  //self-explanatory

  if (CheckPowerOff()) {
    return;
  }

  else if (isWakingUp) {  //after a reset(clicking the button but not holding it)
    isWakingUp = false;
    isFlashing = true;
    currentFlashColor = WHITE;
    lastShifterState = Shifter;
  }

  else if (Shifter != lastShifterState) {
    isFlashing = true;
    if (Shifter == BASIC) {
      currentFlashColor = PURPLE;
    } else if (Shifter == ADVANCED) {
      currentFlashColor = VARY_COLOR; // A varied color to highlight the significantly different mode from the BASIC mode
    }
    lastShifterState = Shifter;
  }

  DistanceRead(); // Reads the distance and displays it on the serial monitor
  StateSwitcher(); //used to switch the States from BASIC to ADVANCED

  switch (Shifter) {
    case BASIC:
      switch (CurrentState) {
        case (SCANNING):
          LED_Fade(); // fades the LED's nonstop
          ServoSwerve(); // controls the movement of the Servo
          Turn_Off_RGB(); // self-understood
          Sonar(); //A pinging sonar like sound
          break;
        case (SENSING):
          AlertTone(); // a distance based pinging noise which varies from 1500 to 4500 hz in frequency
          LED_Blink(); // a constant blinking of the LED every half a second
          DangerSense(); // Distance based LED Color shift
          break;
        case (TRANSITION):
          Clearing(); // to clear up all the variables
          break;
        case BUFFER: // A two second buffer window in which SENSING can't be triggered
          Turn_Off_RGB();
          ServoSwerve();
          LED_Fade();
          Sonar();
          break;
      }

      break;
    case ADVANCED:
      myservo.write(0); //yet to be developed fully
      break;
  }
}

void ServoSwerve() {

  if ((millis() - previousServo) >= Servointerval) {  //servo scans for 0° - 160° in 15ms interval between each angle interval
    previousServo = millis();

    myservo.write(angle); //to move the servo

    angle += direction; // to increment the angle

    if (angle <= 0) { // constrains the servo to a fixed angle of 0° - 160°
      angle = 0;
      direction = 1;
    }

    if (angle >= 160) {
      angle = 160;
      direction = -1;
    }
  }
}

void AlertTone() {
  static unsigned long toneTimer = 0;
  if (millis() - toneTimer >= 100) {
    toneTimer = millis();
    buzzvalue = map(distance, 50, 0, 4500, 1500);  //the buzzer buzzes from a distance of 50cm to 0cm with a variable frquency from 4500 to 1500hz
    if (distance < 50) {
      tone(Buzz, buzzvalue);
    }
  }
}

void DistanceRead() {

  if (millis() - trigtime >= Scanninginterval) {  //gives the sensor a total interval of 50 milliseconds (non blocking)

    trigtime = millis();  //starts the countdown of the sensor

    digitalWrite(Trig, LOW);
    delayMicroseconds(2);

    digitalWrite(Trig, HIGH);
    delayMicroseconds(10);

    digitalWrite(Trig, LOW);  //sends out a ultrasonic pulse

    unsigned long CurrentDuration = pulseIn(Echo, HIGH, 15000); /*timeout of the reading if 
    more than 15000 microseconds have passed */

    long CurrentDistance = 999;  // default

    if (CurrentDuration > 0) {
      CurrentDistance = (0.0343 * CurrentDuration) / 2;  // distance formula
    }

    readings[readIndex] = CurrentDistance;  // putting the readings into the array
    readIndex++;                            //incrementation

    if (readIndex >= Read_Num) {
      readIndex = 0;  // resetting the position of adder
    }

    static long sorted[Read_Num];  //sorted array definition

    int validreadings = 0;  // internal variable to keep track of the median filter
    for (int i = 0; i < Read_Num; i++) {
      if (readings[i] != 999) {
        sorted[validreadings] = readings[i];
        validreadings++;  //copying the normal readings into an unsorted array
      }
    }

    if (validreadings == 0) {
      distance = 999;  //edge case
    }

    else {
      for (int i = 1; i < validreadings; i++) {  //insertion sorting
        long key = sorted[i];
        int j = i - 1;
        while (j >= 0 && sorted[j] > key) {
          sorted[j + 1] = sorted[j];
          j--;
        }
        sorted[j + 1] = key;
      }
      if (validreadings % 2 == 1) {
        distance = sorted[validreadings / 2];  //median filtering by finding which is the median
      } else {
        distance = ((sorted[(validreadings / 2) - 1]) + (sorted[validreadings / 2])) / 2;
      }
    }
    Serial.print("Distance: ");
    Serial.println(distance);
  }
}

void StateSwitcher() {
  if (bufferStarted) {
    CurrentState = BUFFER;
    if (millis() - bufferTimer >= bufferInterval) { //a buffer window for a duration of 2 seconds in which SENSING can't be triggered at all
      bufferStarted = false;
      Objectsensed = 0;
      transitionStarted = false;
      CurrentState = SCANNING;
    }
    return;
  }

  if (distance <= 50) { //if the object is within 50 cm
    if (Objectsensed == 0) { 
      Objectsensed = millis(); // timer starts
    }

    if ((millis() - Objectsensed) < 10000 && !transitionStarted) {
      CurrentState = SENSING; // for 10 seconds, the sensor tracks the distance from the sensor
    } else { // if the object stays after 10 seconds
      if (!transitionStarted) {
        transtimer = millis(); //keep track of the transition
        ledtimer = millis(); //keep track of LED blinking
        transitionStarted = true;
        CurrentState = TRANSITION;
      }

      CurrentState = TRANSITION;
      if (millis() - transtimer < transInterval) {
        if (millis() - ledtimer >= ledInterval) {
          ledtimer = millis();
          value = (value == 0) ? 255 : 0;
          analogWrite(LED, value); // blinking action, which means it's going back into SCANNING mode
        }
      } else {
        transitionStarted = false; 
        bufferStarted = true;
        bufferTimer = millis();
        CurrentState = BUFFER; //buffer window starts
      }
    }
  }

  else if (distance > 50 && Objectsensed > 0) { // if the object leaves within 10 seconds 
    if (!transitionStarted) { 
      transitionStarted = true;
      transtimer = millis();
      ledtimer = millis();
      CurrentState = TRANSITION;
    }

    CurrentState = TRANSITION;
    if (millis() - transtimer < transInterval) {
      if (millis() - ledtimer >= ledInterval) {
        ledtimer = millis();
        value = (value == 0) ? 255 : 0;
        analogWrite(LED, value);
      }
    } else {
      transitionStarted = false;
      bufferStarted = true;
      bufferTimer = millis();
      CurrentState = BUFFER; //same logic as before
    }
  }

  else {
    Objectsensed = 0;
    transitionStarted = false;
    CurrentState = SCANNING; //default window
  }
}

void Sonar() {
  /*this function works as follows, assuming starting time is 0, it will buzz at a frequency of 1500. then, after 100ms 
  it will go silent for 900ms and then start buzzing at 1000ms and so on*/
  if ((millis() - pingTimer) >= pingInterval) { 
    pingTimer = millis(); 
    pingRange += pingPusher; 
    if (pingRange >= 2100) {
      pingRange = 2100;
      pingPusher = -50;
    } else if (pingRange <= 2000) {
      pingRange = 2000;
      pingPusher = 50;
    }
    tone(Buzz, pingRange);
    pingPause = millis();
  }

  if (millis() - pingPause >= pingBounce) {
    noTone(Buzz);
  }
}