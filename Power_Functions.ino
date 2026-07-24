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

    myservo1.write(0);  // Servo homing sequence
    angle[0] = 0;
    direction[0] = 1;  
    angle[1] = 160;
    direction[0] = -1;

    return true;  // System is OFF
  }
  return false;  // System is ON
}

void ShutDown() {
  noTone(Buzz);  //turning OFF all the components on the board
  analogWrite(RED, 0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);
  analogWrite(LED, 0);
  distance = 999;
  previousdistance = 999;
  myservo1.write(0);
  myservo2.write(0);
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
      servoReset = true;
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