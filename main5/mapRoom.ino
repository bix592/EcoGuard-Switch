void mapRoom() {
  for (int i = 0; i < NUM_SAMPLES; i++) {
    int angle = map(i, 0, NUM_SAMPLES - 1, SERVO_ANGLE_START, SERVO_ANGLE_END);
    servo.write(angle);
    delay(500); // Allow the servo to move to the position
    referenceDistances[i] = sonar.ping_cm();
    delay(200); // Short delay between pings
  }
}
