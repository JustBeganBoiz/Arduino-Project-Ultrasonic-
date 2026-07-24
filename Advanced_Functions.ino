void Dual_ServoSwerve() {
  if (modeShifted && servoReset == true) {
    servoReset = false;

    angle[0] = 0;
    angle[1] = 160;
    direction[0] = 1;
    direction[1] = -1;

    myservo1.write(angle[0]);
    myservo2.write(angle[1]);

    quickScanDone = false;
    servoTimer = millis();
    quickTimer = millis();
    return;
  }

  unsigned long currentInterval = quickScanDone ? servoInterval : (servoInterval * 2/3);

  if (!quickScanDone && (millis() - quickTimer >= quickInterval)) {
    quickScanDone = true;
  }

  if (millis() - servoTimer >= currentInterval) {
    servoTimer = millis();
    
    for (int i = 0; i < 2; i++) {
      angle[i] += direction[i];
      if (angle[i] <= 0) {
        angle[i] = 0;
        direction[i] = 1;
      } else if (angle[i] >= 160) {
        angle[i] = 160;
        direction[i] = -1;
      }
    }

    myservo1.write(angle[0]);
    myservo2.write(angle[1]);
  }
}