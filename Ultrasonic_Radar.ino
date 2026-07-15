#include <Servo.h>

const int LED = 3; // defining the LED Pin no's as well as the amount of glow 
const int Buzz = 6; //defining the pin no's for the buzzer, echo and trigger pin for the ultrasonic sensor
const int Trig = 4;
const int Echo = 7;
const int Button = 2; // defining the Pin no. for the button and 
const int RED = A0; //defining the pins for the RGB LED
const int GREEN = A1;
const int BLUE = A2;

int value = 0; // to control the glow amount
int fade = 5; // to control the amount by which the glow increases or decreases 

//defining the servo object 
Servo myservo;

// defining the debounce window for the button
unsigned long Buttonpressed = 0; //how long the button has been pressed for
const int buttoninterval = 50;
bool lastbuttonstate = true;
bool buttonState = false; //the default state of the system is ON 

int buzzvalue = 0; //used for defining the frequency at which the buzzer buzzes at

unsigned long Objectsensed = 0; //to keep track of how long the object has been in sensing range
unsigned long ledtimelapsed = 0; //to keep track of the blinking of the LED

const byte Read_Num = 5; // the size of the array
long readings[Read_Num] = {999,999,999,999,999}; //creation of an array in order to store the data values
byte readIndex = 0; //to keep track of the position of the array

long duration = 0;  //duration of the ultrasonic sensor signals
int distance = 999; //initial default state
int previousdistance = 0; //keeping a track of the previous distances

unsigned long int trigtime = 0; //keeping track of the sensor triggers 
const unsigned long int Scanninginterval = 65; // interval for the ultrasonic sensor

int angle = 0; // angle for servo
int direction = 1; //controlling the direction of the servo 
const int Servointerval = 15; 
unsigned long previousServo = 0; //keeping track of how long it has been since last servo movement

static unsigned long transtimer = 0; //timer to keep track of how long it has been since 10 seconds have passed
static unsigned long ledtimer = 0; // timer to keep track of the blinking of the LED post state change
static bool transitionStarted = false; // state change operator
const int transInterval = 1200; // time to transition from SCANNING to SENSING
const int ledInterval = 200; //time between blinks

bool CurrentButton; //to keep track of the current state of the button

enum SystemState {
  SCANNING,
  SENSING,
  TRANSITION
};

enum Power {
  TURN_ON = true,
  TURN_OFF = false
};

Power Control = TURN_ON;
SystemState CurrentState = SCANNING;


void setup() {
  pinMode(LED, OUTPUT);

  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);

  pinMode(Buzz, OUTPUT);

  pinMode(Button, INPUT_PULLUP);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  myservo.attach(9);

  analogWrite(LED, 0); //blinking action of the blue LED on startup
  delay(100);

  analogWrite(LED, 255);
  delay(500);

  analogWrite(LED, 0);
  delay(500);

  Serial.begin(115200);
}

void loop() {

  ButtonReader(); //reads the state of the Button 

  switch(Control){
    case(TURN_ON):
      DistanceRead(); //Distance Reading of the Sensor
      StateSwitcher(); //to change the states when the distances are changed
      switch(CurrentState){
        case(SCANNING):
          LED_Fade(); //Fade function for the LED's
          ServoSwerve(); //Movement of the Servo
          Clearing(); //to keep only the Sensors and the servo turned ON
          break;

        case(SENSING):
          AlertTone(); // Controlling the buzzing of the buzzer
          LED_Blink(); // Controls the Blinking of the LED's
          DangerSense(); //Controls the Buzzing of the Buzzer
          break;
        
        case(TRANSITION):
          Clearing();
          break;
      }
      break;
    
    case(TURN_OFF):
      ShutDown(); //turning everything OFF
      break;
  }
}

void Clearing(){
  noTone(Buzz); //Keeping the buzzer  and the RGB LED's OFF while scanning
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
}

void ServoSwerve(){

  if((millis() - previousServo) >= Servointerval){ //servo scans for 0° - 160° in 15ms interval between each angle interval
  previousServo = millis();

  myservo.write(angle);

  angle += direction;
  
  if(angle <= 0){
    angle = 0;
    direction = 1;
  }
  if(angle >= 160){
    angle = 160;
    direction = -1;
  }
  }
}

void LED_Blink(){
  if(millis() - ledtimelapsed >= 500){ // every half a second it blinks
    ledtimelapsed = millis(); //start the countdown
    value = (value == 0) ? 255 : 0;
    analogWrite(LED, value);
  }
}

void LED_Fade(){
  static unsigned long fadeTimer = 0;
  if(millis() - fadeTimer >= 30){
    fadeTimer = millis();
    value = constrain(value + fade, 0, 255);
    if(value <= 0){
      fade = 5;
    }
    else if(value >= 255){
      fade = -5;
    }
    analogWrite(LED, value);
  }
}

void AlertTone(){
  static unsigned long toneTimer = 0;
  if(millis() - toneTimer >= 100){
    toneTimer = millis();
    buzzvalue = map(distance, 50, 0, 4500, 1500); //the buzzer buzzes from a distance of 50cm to 0cm with a variable frquency from 4500 to 1500hz
    if(distance < 50){
        tone(Buzz, buzzvalue);
    }
  }
}

void DistanceRead(){

  if(millis() - trigtime >= Scanninginterval){ //gives the sensor a total interval of 50 milliseconds (non blocking)

    trigtime = millis(); //starts the countdown of the sensor

    digitalWrite(Trig, LOW);
    delayMicroseconds(2);

    digitalWrite(Trig, HIGH);
    delayMicroseconds(10);

    digitalWrite(Trig, LOW); //sends out a ultrasonic pulse
    
    unsigned long CurrentDuration = pulseIn(Echo, HIGH, 15000); /*timeout of the reading if 
    more than 15000 microseconds have passed */ 
    
    long CurrentDistance = 999; // default

    if(CurrentDuration > 0){
      CurrentDistance = (0.0343 * CurrentDuration) / 2; // distance formula
    }

    readings[readIndex] = CurrentDistance; // putting the readings into the array
    readIndex++; //incrementation

    if(readIndex >= Read_Num){
      readIndex = 0; // resetting the position of adder
    }
    
    long sorted[Read_Num]; //sorted array definition

    int validreadings = 0; // internal variable to keep track of the median filter 
    for(int i=0; i<Read_Num; i++){
      if(readings[i] != 999){
        sorted[validreadings] = readings[i];
        validreadings++; //copying the normal readings into an unsorted array
      }
    }

    if(validreadings == 0){
      distance = 999; //edge case
    }

    else{ 
      for(int i = 1; i < validreadings; i++){ //insertion sorting
        long key = sorted[i];
        int j = i - 1;
        while(j >= 0 && sorted[j] > key){
          sorted[j+1] = sorted[j];
          j--;
        }
        sorted[j+1] = key;
      }
      if(validreadings % 2 == 1){
        distance = sorted[validreadings/2]; //median filtering by finding which is the median
      }
      else{
        distance = ((sorted[(validreadings/2) - 1]) + (sorted[validreadings/2])) / 2;
      }
    }
    Serial.print("Distance: ");
    Serial.println(distance); //printing of the distance values
  } 
}

void ShutDown(){
  noTone(Buzz); //turning OFF all the components on the board
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
  digitalWrite(LED, LOW);
  distance = 999;
  previousdistance = 999;
  CurrentState = SCANNING; // to reset the whole system
}

void ButtonReader(){
  CurrentButton = digitalRead(Button); //to read the current state of the button

  if(CurrentButton != lastbuttonstate){
    Buttonpressed = millis(); // if there's a change in the button it starts counting
  }

  if((millis() - Buttonpressed) >= 50){ //a small debounce window
    if(CurrentButton != buttonState){
      buttonState = CurrentButton; 
      if(buttonState == LOW){
        if(Control == TURN_ON){
          Control = TURN_OFF;
        } 
        else {
          Control = TURN_ON;
        }//changes the state of the system if the button is pressed down
      }
    }
  }
  lastbuttonstate = CurrentButton; //switches the past state to currentstate
}

void DangerSense(){
  //this function allows us to use the LED's to show how far the object is from the sensor
  if(distance <= 50 && distance > 30){
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, LOW);
  }
  if(distance <= 30 && distance > 15){
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, LOW);
  }
  if(distance < 15){
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
  }
}

void StateSwitcher(){
  if(distance <= 50){
    if(Objectsensed == 0){
      Objectsensed = millis();
    }
    if((millis() - Objectsensed) < 10000 && !transitionStarted){
      CurrentState = SENSING;
    }

    else{
      CurrentState = TRANSITION; // initiates state change if even after 10 seconds it's in range

      if(!transitionStarted){
        transtimer = millis();
        ledtimer = millis();
        transitionStarted = true;
      }

      if(millis() - transtimer < transInterval){
        if(millis() - ledtimer >= ledInterval){
          ledtimer = millis();
          value = (value == 0) ? 255 : 0; //every 2/10th a second LED blinks (totally twice)
          analogWrite(LED,value);
        }
      }

      else{
        CurrentState = SCANNING; //last switch
      }
    }
  }

  else if(distance > 50 && Objectsensed > 0){ //if object leaves before 10 seconds
    CurrentState = TRANSITION;

    if(!transitionStarted){
      transitionStarted = true;
      ledtimer = millis();
      transtimer = millis();
    }

    if((millis() - transtimer < transInterval)){
      if(millis() - ledtimer >= ledInterval){
        ledtimer = millis();
        value = (value == 0) ? 255 : 0;
        analogWrite(LED, value);
      }
    }
    else{
      Objectsensed = 0;
      transitionStarted = false;
      CurrentState = SCANNING; //clearing all the flags
    }
  }

  else{
    Objectsensed = 0;
    transitionStarted = false;
    CurrentState = SCANNING;
  }
}