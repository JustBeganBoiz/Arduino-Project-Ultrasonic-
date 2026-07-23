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

void Turn_Off_RGB() {
  analogWrite(RED, 0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);
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