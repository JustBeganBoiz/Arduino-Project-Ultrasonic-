#ifndef GLOBALS_H
#define GLOBALS_H

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

#endif