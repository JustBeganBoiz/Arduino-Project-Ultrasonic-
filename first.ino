#include <Servo.h>

const int LED = 3;
int value = 0;
int fadeAmount = 5;  

Servo myservo;
int pos = 0;

const int button = 2;
const int buttoninterval = 50;
unsigned long previousbutton = 0;
bool lastbuttonstate = HIGH;
bool buttonstate = LOW;
long timeidled = 0;

unsigned long timeidling = 0;

long readings[4] = {0,0,0,0};
int readIndex;

const int Buzz = 6;

const int Echo = 10;
const int Trig = 9;

long duration;
int distance;

bool Objectinside = false;
unsigned long timeinside;
bool done = false;

bool syson = true;

bool Ledstate = false;
unsigned long int previousblink;
const int blinkinterval = 300;

unsigned long int trigtime = 0;
const unsigned long int sensorinterval = 50;

int previousDistance = 0;
unsigned long int measureinterval = 10;

int angle = 0;
int direction = 1;
unsigned long previousServo = 0;
const int servointerval = 15;

void setup() {

  myservo.attach(11);

  pinMode(button, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
  pinMode(Buzz, OUTPUT);
   
  digitalWrite(LED, LOW);
  delay(100);

  digitalWrite(LED, HIGH);
  delay(500);

  digitalWrite(LED, LOW);
  delay(500);

  Serial.begin(115200);
}

void loop() {
    
    bool currentState = digitalRead(button);

    if(currentState != lastbuttonstate){
      previousbutton = millis();
    }

    if((millis() - previousbutton) > buttoninterval){
      if(currentState != buttonstate){
        buttonstate = currentState;
        if(buttonstate == HIGH){
        syson = !syson;
        }
      }
  }

  lastbuttonstate = currentState;

  if(!syson){

    if(millis() - previousServo >= servointerval){
      previousServo = millis();

      myservo.write(angle);

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

    if(millis() - trigtime >= sensorinterval){
      
      trigtime = millis();

      digitalWrite(Trig, LOW);
      delayMicroseconds(2);

      digitalWrite(Trig, HIGH);
      delayMicroseconds(10);

      digitalWrite(Trig, LOW);

      long currentDuration = pulseIn(Echo, HIGH, 15000);
      long currentDistance = 999;

      if(currentDuration > 0){
        currentDistance = (currentDuration * 0.0343) / 2;
      }

      if(currentDistance != 999 && abs(currentDistance - previousDistance) < 40 || previousDistance == 999){
        readings[readIndex] = currentDistance;
        }
      else{
        readings[readIndex] = -1;
      }

      readIndex++;

      if(readIndex >= 4){

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
      distance = totalDistance / validreadings;
      previousDistance = distance;
      }

      else{
      distance = 999;
      previousDistance = 999;
      }
      }
      Serial.print("Distance: ");
      Serial.println(distance); 
    }
    
    static unsigned long ledtimer = 0;
    if(millis() - ledtimer > 15){
      ledtimer = millis();
      analogWrite(LED, value);
      value += fadeAmount;

      if(value <= 0 || value >= 255){
        fadeAmount = -fadeAmount;
      }
    }

    if(distance > 80 || distance == 999){
      timeidling = millis();
      timeidled = 0;
    }
    if(distance <= 80){
      timeidled = millis() - timeidling;
    }

    if(timeidled >= 10000){
      distance = 999;
      previousDistance = 999;

      timeidling = millis();
      timeidled = 0;
    }
  }
  else{
      analogWrite(LED, 0);
  }
}