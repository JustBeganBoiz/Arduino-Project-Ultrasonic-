#include <Servo.h>

const int LED = 3;
int value = 0;
int fadeAmount = 5;  

Servo myservo;
int pos = 0;

const int button = 2;

long readings[4] = {0,0,0,0};
int readIndex;

const int Buzz = 6;

const int Echo = 10;
const int Trig = 9;

long duration;
int distance;
long currentDistance = 0;

bool Objectinside = false;
unsigned long timeinside;
bool done = false;

bool syson = true;
bool lastbuttonstate = HIGH;

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
  if(syson){
    
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

      if(currentDuration > 0){
        currentDistance = (currentDuration * 0.0343) / 2;
      }

      if(abs(currentDistance - previousDistance) < 30 || previousDistance == 999){
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

    /*

    analogWrite(LED, value);                 

    value = value + fadeAmount; 

    if (value >= 255 || value <= 0) {
      fadeAmount = -fadeAmount;              
    }

    if (distance <= 150 && !Objectinside) {
        Objectinside = true;
        done = false;
        timeinside = millis();
    }

    if (Objectinside && !done) {

        if (millis() - timeinside < 10000) {

            if(millis() - previousblink >= blinkinterval){
              previousblink = millis();

              Ledstate = !Ledstate;

              if(Ledstate){
                analogWrite(LED, 255);
              }
              else{
                analogWrite(LED, 0);
              }
            }

            int freq = map(distance, 0, 150, 2500, 1000);
            freq = constrain(freq, 1000, 2500);
            tone(Buzz, freq);
        }
        else {
            done = true;
            analogWrite(LED, 0);
            delay(100);

            analogWrite(LED, 255);
            delay(200);

            analogWrite(LED, 0);
            delay(200);
            
            analogWrite(LED, 250);
            delay(200);

            analogWrite(LED, 0);
            delay(200);

            noTone(Buzz);
        }
    }

      if (distance > 150 && Objectinside) {
          Objectinside = false;
          done = false;

          analogWrite(LED, 0);
          delay(100);

          analogWrite(LED, 255);
          delay(200);

          analogWrite(LED, 0);
          delay(200);
            
          analogWrite(LED, 250);
          delay(200);

          analogWrite(LED, 0);
          delay(200);

          noTone(Buzz);
      }

    delay(100);
  }
  bool currentstate = digitalRead(button);

  if (currentstate == LOW && lastbuttonstate == HIGH) {
      syson = !syson;
      delay(50);    
  }

  lastbuttonstate = currentstate;

  if (!syson) {
    noTone(Buzz);
    analogWrite(LED, 0);
    return;
    */
  }
}
}