#include <Servo.h>

const int LED = 5;   // defining the LED Pin no's as well as the amount of glow
const int Buzz = 6;  //defining the pin no's for the buzzer, echo and trigger pin for the ultrasonic sensor
const int Trig = 2;
const int Echo = 4;
const int Button = 7;  // defining the Pin no. for the button and
const int RED = 9;     //defining the pins for the RGB LED
const int GREEN = 11;
const int BLUE = 10;

bool isReset = false;  //to keep track of resets(short button presses of less than 4s)

const int bufferInterval = 2000;  //This is the amount of time after Sensing has been initiated for which the SCANNING mode will NOT be disengaged
unsigned long bufferTimer = 0;    // to keep track of the buffer timer
bool bufferStarted = false;

int value = 0;  // to control the glow amount
int fade = 5;   // to control the amount by which the glow increases or decreases

bool isWakingUp = false;  //to keep track of the system as it Resets
bool Started = false;     //When the system has just been turned ON

Servo myservo;  //defining the servo object

unsigned long buttonPressed = 0;  //how long the button has been pressed for
const int buttonInterval = 50;    // debounce window
bool lastButtonState = HIGH;
bool buttonState = HIGH;  //the default state of the system is ON

unsigned long modeTimer = 0;    //to keep track of how long the button has been pressed for the Mode Switch
const int modeInterval = 4000;  // it takes 4 seconds of the button being held down to switch modes
const int resetInterval = 1500;
bool modeShifted = false;       //to keep track of Shifts
bool lastModeButton = HIGH;     // to keep track of Shifts

unsigned long flashTimer = 0;    // Tracks the 2-second flash window
const int flashInterval = 1800;  // 1.8 seconds

int stepRange = 255 * 3;
int rgbValue[3] = { 0, 0, 0 };
int stepCounter = 0;
unsigned long colorStepTimer = 0;

bool isFlashing = false;  // Lockout flag during the flash duration

int buzzvalue = 0;  //used for defining the frequency at which the buzzer buzzes at

unsigned long Objectsensed = 0;   //to keep track of how long the object has been in sensing range
unsigned long ledtimelapsed = 0;  //to keep track of the blinking of the LED

const byte Read_Num = 5;                                // the size of the array
long readings[Read_Num] = { 999, 999, 999, 999, 999 };  //creation of an array in order to store the data values
byte readIndex = 0;                                     //to keep track of the position of the array

long duration = 0;         //duration of the ultrasonic sensor signals
int distance = 999;        //initial default state
int previousdistance = 0;  //keeping a track of the previous distances

unsigned long pingTimer = 0;    // how long it has been since the previous ping
const int pingInterval = 1000;  // amount of time between pings
unsigned long pingPause = 0; // how long the current ping has started for
static int pingPusher = 50; //how much to increase the frequency of the buzzer
static int pingRange = 2000; // range of the pinging 
const int pingBounce = 100; //how long each ping lasts

unsigned long int trigtime = 0;                 //keeping track of the sensor triggers
const unsigned long int Scanninginterval = 65;  // interval for the ultrasonic sensor

int angle = 0;      // angle for servo
int direction = 1;  //controlling the direction of the servo
const int Servointerval = 15;
unsigned long previousServo = 0;  //keeping track of how long it has been since last servo movement

static unsigned long transtimer = 0;    //timer to keep track of how long it has been since 10 seconds have passed
static unsigned long ledtimer = 0;      // timer to keep track of the blinking of the LED post state change
static bool transitionStarted = false;  // state change operator
const int transInterval = 1200;         // time to transition from SCANNING to SENSING
const int ledInterval = 200;            //time between blinks

bool currentButton;  //to keep track of the current state of the button

unsigned long switchblinker = 0;
bool ledState;
bool flashStarted;
const int blinkInterval = 300;

enum SystemState {  //All the different states of the system
  SCANNING,
  SENSING,
  TRANSITION,
  BUFFER
};

enum Power {  // To control the Power Supply of the system
  TURN_ON = true,
  TURN_OFF = false
};

enum Modes {  // The two different modes of the system
  BASIC,
  ADVANCED
};

enum FlashColour {  // The different colours that the system can take
  PURPLE,
  VARY_COLOR,
  WHITE
};

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

void Turn_Off_RGB() {
  analogWrite(RED, 0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);
}

void Clearing() {
  noTone(Buzz);  //Keeping the buzzer  and the RGB LED's OFF while scanning
  analogWrite(RED, 0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);
}

bool CheckPowerOff() {
  if (Control == TURN_OFF) {
    ShutDown();
    isFlashing = false;
    flashStarted = false;
    isReset = true;

    myservo.write(0);  // Servo homing sequence
    angle = 0;
    direction = 1;  

    return true;  // System is OFF
  }
  return false;  // System is ON
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

void LED_Blink() {
  if (millis() - ledtimelapsed >= 500) {  // every half a second it blinks
    ledtimelapsed = millis();             //start the countdown
    value = (value == 0) ? 255 : 0;  // confining the values to HIGH or LOW
    analogWrite(LED, value); 
  }
}

void LED_Fade() {
  static unsigned long fadeTimer = 0;
  if (millis() - fadeTimer >= 30) {
    fadeTimer = millis();
    value = constrain(value + fade, 0, 255);
    if (value <= 0) {
      fade = 5;
    } else if (value >= 255) {
      fade = -5;
    }
    analogWrite(LED, value); // it takes 1.53 seconds for the led to go to max brightness and vice versa
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

void ShutDown() {
  noTone(Buzz);  //turning OFF all the components on the board
  analogWrite(RED, 0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);
  analogWrite(LED, 0);
  distance = 999;
  previousdistance = 999;
  myservo.write(0);
  CurrentState = SCANNING;  // to reset the whole system
}

void ProcessButton() {
  bool buttonReader = digitalRead(Button); //reads the button

  if (buttonReader != lastButtonState) {
    buttonPressed = millis(); 
    lastButtonState = buttonReader; // debounce filter
  }

  if (millis() - buttonPressed >= buttonInterval) {
    if (buttonReader != buttonState) { // if after 50ms the button is held down then the timer starts counting
      buttonState = buttonReader;
      if (buttonState == LOW) {
        modeTimer = millis();
        modeShifted = false;
      } 
      else if(buttonState == HIGH){
        unsigned long holdDuration = millis() - modeTimer; 

        if(holdDuration <= resetInterval && !modeShifted){
          PowerSwitcher(); //if the button is held down for less than 1.5 seconds than it toggles the power. 
        }
        else if(holdDuration > resetInterval && !modeShifted){
          // if button is held for more than 1.5 seconds but less than 4 seconds than it doesn't do anything(dead zone)
        }
      }
    }
  }

  if (Control == TURN_ON && buttonState == LOW && !modeShifted) {
    if (millis() - modeTimer >= modeInterval) { //mode shift after 4 seconds
      ModeSwitcher();
      modeShifted = true;
    }
  }
}

void PowerSwitcher() {
  if (Control == TURN_OFF) {
    Control = TURN_ON;
    isWakingUp = true; //to reset the reset switch
  } else {
    Control = TURN_OFF; //self- explanatory
  }
}

void ModeSwitcher() {
  if (Shifter == BASIC) {
    Shifter = ADVANCED;
  } else {
    Shifter = BASIC;
  }
}

void DangerSense() {
  //this function allows us to use the LED's to show how far the object is from the sensor
  if (distance <= 50 && distance > 30) {
    analogWrite(RED, 0);
    analogWrite(GREEN, 255);
    analogWrite(BLUE, 0);
  }
  if (distance <= 30 && distance > 15) {
    analogWrite(RED, 255);
    analogWrite(GREEN, 255);
    analogWrite(BLUE, 0);
  }
  if (distance < 15) {
    analogWrite(RED, 255);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, 0);
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

// Helper function: Converts a 0-255 Hue value into fully saturated RGB PWM values
void fillHue(byte hue, byte &r, byte &g, byte &b) {
  byte region = hue / 43;
  byte remainder = (hue - (region * 43)) * 6;

  byte q = 255 - remainder;
  byte t = remainder;

  switch (region) {
    case 0:
      r = 255;
      g = t;
      b = 0;
      break;  // Red -> Yellow
    case 1:
      r = q;
      g = 255;
      b = 0;
      break;  // Yellow -> Green
    case 2:
      r = 0;
      g = 255;
      b = t;
      break;  // Green -> Cyan
    case 3:
      r = 0;
      g = q;
      b = 255;
      break;  // Cyan -> Blue
    case 4:
      r = t;
      g = 0;
      b = 255;
      break;  // Blue -> Magenta
    default:
      r = 255;
      g = 0;
      b = q;
      break;  // Magenta -> Red
  }
}

void Startup() {
  if (!flashStarted) {
    flashStarted = true;
    flashTimer = millis();
    switchblinker = millis();
    colorStepTimer = millis();
    ledState = false;

    stepCounter = 0;  // Will represent Hue (0 to 255)
  }

  // 1. Blink timer for discrete modes (PURPLE / WHITE)
  if (millis() - switchblinker >= blinkInterval) {
    switchblinker = millis();
    ledState = !ledState;
  }

  // 2. Smooth Rainbow Hue Sweep for VARY_COLOR
  if (currentFlashColor == VARY_COLOR) {
    // Step hue every 6ms (255 steps * 6ms = ~1.5s rainbow sweep)
    if (millis() - colorStepTimer >= 6) {
      colorStepTimer = millis();
      if (stepCounter < 255) {
        stepCounter++;
      }
    }

    byte r, g, b;
    fillHue((byte)stepCounter, r, g, b);

    analogWrite(RED, r);
    analogWrite(GREEN, g);
    analogWrite(BLUE, b);
  }
  // 3. Discrete Blinking Colors
  else if (ledState) {
    switch (currentFlashColor) {
      case PURPLE:
        analogWrite(RED, 255);
        analogWrite(GREEN, 0);
        analogWrite(BLUE, 255);
        break;

      case WHITE:
        analogWrite(RED, 255);
        analogWrite(GREEN, 255);
        analogWrite(BLUE, 255);
        break;
    }
  } else {
    analogWrite(RED, 0);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, 0);
  }

  // 4. Timer Expiration
  unsigned long activeInterval = isReset ? resetInterval : flashInterval;

  if (millis() - flashTimer >= activeInterval) {
    isFlashing = false;
    flashStarted = false;
    isReset = false;

    analogWrite(RED, 0);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, 0);
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