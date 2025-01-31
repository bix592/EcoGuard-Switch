void compareDistanceMap() {
  bool changeDetected = false;

  // DISTANCE SENSOR + SERVO SURVEY THE ROOM
  for (int i = 0; i < NUM_SAMPLES; i++) {
    int angle = map(i, 0, NUM_SAMPLES - 1, SERVO_ANGLE_START, SERVO_ANGLE_END);
    servo.write(angle);
    delay(500); // Allow the servo to move to the position
    int currentDistance = sonar.ping_cm();
    delay(200); // Short delay between pings
    
    // Compare current distance to the reference distance
    // if change detected
    if (abs(currentDistance - referenceDistances[i]) > 30) { // Threshold of 10 cm
      changeDetected = true;
    }
  }

  if (changeDetected) {
    lcd.setCursor(0, 0);
    lcd.print("Change detected");
    delay(3000);
    lcd.clear();
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Relay Open");
    digitalWrite(RELAY_PIN, HIGH);  // Turn off the relay (if using an active-low relay)
    lcd.clear();
    lcd.noBacklight(); // turn off lcd
  }
}


