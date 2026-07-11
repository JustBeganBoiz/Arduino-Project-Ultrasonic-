#include <Servo.h> //calling upon the servo library 

// defining the pin no's and constants
const int LED = 3;
int value = 0;
int fadeAmount = 5;  

//initialising the object myservo
Servo myservo;

//defining the rest of the constants and pin no's
const int RED = A0;
const int GREEN = A1;
const int BLUE = A2;

const int button = 2;
const int buttoninterval = 50;
unsigned long previousbutton = 0;
bool lastbuttonstate = HIGH;
bool buttonstate = LOW;

int buzzvalue = 0;

unsigned long objectsensed = 0;
unsigned long blinktime = 0;
int ledvalue = 0;
int state = 0;

//creating an array to use moving average method for maximum accuracy
long readings[4] = {0,0,0,0};
int readIndex;

const int Buzz = 6;

const int Echo = 7;
const int Trig = 4;

long duration;
int distance = 999;

bool syson = true;

unsigned long int trigtime = 0;
const unsigned long int sensorinterval = 50;

int previousDistance = 0;

int angle = 0;
int direction = 1;
unsigned long previousServo = 0;
const int servointerval = 15;

void setup() {

  //estabilishing the pins
  myservo.attach(11);

  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);

  pinMode(button, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
  pinMode(Buzz, OUTPUT);
   
  //starting animation 
  analogWrite(LED, 0);
  delay(100);

  analogWrite(LED, 255);
  delay(500);

  analogWrite(LED, 0);
  delay(500);

  Serial.begin(115200);
}

void loop() {
    
    //reading the button
    bool currentState = digitalRead(button);
    
    //checking if the button was pressed or not
    if(currentState != lastbuttonstate){
      previousbutton = millis();
    }

    //providing a small debounce window of 50ms to prevent floating inputs
    if((millis() - previousbutton) > buttoninterval){
      if(currentState != buttonstate){
        buttonstate = currentState;
        if(buttonstate == HIGH){
        syson = !syson;
        }
      }
  }

  //if the button had been pressed then only will our system turn off otherwise it by default stays on
  lastbuttonstate = currentState;
  
  if(syson){

    /*This program has three defined states which are as follows:
    - state 0 which is the idling state and also the scanning mode
    - state 1 in which the sensor is in sensing mode
    - state 2 which is a state that gets triggered briefly if an object stays even after 10 seconds*/
    if(state == 0 || state == 2){
    if(millis() - previousServo >= servointerval){
      previousServo = millis();

      myservo.write(angle);

      // servo movement code
      angle += direction;

      if(angle >= 160){
        angle = 160;
        direction = -1;
      }
      
      if(angle <= 0){
        angle = 0;
        direction = 1;
      }
    }
    }

    if(millis() - trigtime >= sensorinterval){
      
      //ultrasonic sensor code
      trigtime = millis();

      digitalWrite(Trig, LOW);
      delayMicroseconds(2);

      digitalWrite(Trig, HIGH);
      delayMicroseconds(10);

      digitalWrite(Trig, LOW);

      
      long currentDuration = pulseIn(Echo, HIGH, 10000); //timeout of 10000 microseconds to prevent false readings as well as to give it a fixed distance of around 150cm
      long currentDistance = 999;

      if(currentDuration > 0){
        currentDistance = (currentDuration * 0.0343) / 2; // distance = speed * time formula
      }
      if(currentDistance != 999 && (previousDistance == 999 || abs(currentDistance - previousDistance) < 40)){
        readings[readIndex] = currentDistance; //moving average previously discussed. this throws away the garbage values 
        }
      else{
        readings[readIndex] = -1;
      }

      readIndex++;

      if(readIndex >= 4){ //filter code 

        readIndex = 0;

        int validreadings = 0;
        long totalDistance = 0;

        for(int i=0; i<4; i++){
          if(readings[i] != -1){
            totalDistance += readings[i]; 
            validreadings++;
          }
        }

      if(validreadings > 0){
      distance = totalDistance / validreadings; //averaging function 
      previousDistance = distance;
      }

      else{
      distance = 999;
      previousDistance = 999; //defaulting scenario in case the sensor times out
      }
      }
      Serial.print("Distance: ");
      Serial.println(distance); 
    }

    if(distance <= 50){ //sensor sensing mode gets triggered under 50cm
      buzzvalue = map(distance, 50, 0, 1500, 4000);  //the buzzer has been mapped from 1500hz to 4000hz

      /*the RGB LED has three modes when the object is under 50cm
      GREEN if its from 50cm to 30cm
      YELLOW if its from 30cm to 15cm
      RED if its less than 15cm*/
      if(state == 0){
        objectsensed = millis();
        blinktime = millis();
        digitalWrite(RED, LOW);
        digitalWrite(BLUE, LOW);
        digitalWrite(GREEN, LOW); 
        state = 1;
      }
      if(state == 1 && millis() - objectsensed < 10000){
        if(millis() - blinktime >= 300){
          blinktime = millis();
          if(distance <= 50 && distance > 30){
            digitalWrite(RED, LOW);
            digitalWrite(BLUE, LOW);
            digitalWrite(GREEN, HIGH);
          }
          if(distance <= 30 && distance > 15){
            digitalWrite(RED, HIGH);
            digitalWrite(BLUE, LOW);
            digitalWrite(GREEN, HIGH);
          }
          if(distance <= 15){
            digitalWrite(RED, HIGH);
            digitalWrite(BLUE, LOW);
            digitalWrite(GREEN, LOW);
          }
          if(ledvalue == 0){
            ledvalue = 255;
            tone(Buzz, buzzvalue); //buzzer buzzing code
          }
          else{
            ledvalue = 0;
            tone(Buzz, buzzvalue);
          }
          analogWrite(LED, ledvalue);
          
        }
      }
      else if(state == 1){ //in the instance that the object stays within 50 cm post 10 seconds 
        state = 2;
        noTone(Buzz);
      }
      if(state == 2){
        static unsigned long ledtimer = 0;
        if(millis() - ledtimer >= 15){
          ledtimer = millis();
          analogWrite(LED, value);
          value += fadeAmount;
          if(value <= 0 || value >= 255){
            fadeAmount = -fadeAmount;
          }
        }
        digitalWrite(RED, LOW);
        digitalWrite(BLUE, LOW);
        digitalWrite(GREEN, HIGH);
      }
    }
    else{
      noTone(Buzz); //once the object exits the space before 10 seconds 

      static bool exitTimerStarted = false;

      if(state != 0 && !exitTimerStarted){
        blinktime = millis();
        exitTimerStarted = true;
        analogWrite(LED, 0);
      }

      if(exitTimerStarted && (millis() - blinktime >= 200)){
        state = 0;             
        exitTimerStarted = false; 
      }

      if(state == 0){
        static unsigned long ledtimer = 0;
        if(millis() - ledtimer >= 15){
          ledtimer = millis();
          analogWrite(LED, value);
          value += fadeAmount;
          if(value <= 0 || value >= 255){
            fadeAmount = -fadeAmount;
          }
        } // the blue led blinks to signal the state change 
        digitalWrite(RED, LOW);
        digitalWrite(BLUE, LOW);
        digitalWrite(GREEN, LOW);
      }
    }
  }
  else{ //default case if everything fails
    analogWrite(LED, 0);
    state = 0;
    noTone(Buzz);
  }
}