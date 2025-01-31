void initTemp() {
  // Initialize temperature history for moving
  for (int i = 0; i < numSamples; i++) {
    temperatureHistory[i] = 0;
  }
  // Take multiple readings over 10 seconds
  int numReadings = 20;
  float tempSum = 0;
  for (int i = 0; i < numReadings; i++) {
    int analogValue = analogRead(LM35_PIN);
    float temperatureC = (analogValue * (5.0 / 1023.0)) * 100.0;
    tempSum += temperatureC;
    delay(500);  // Wait for 0.5 seconds between readings
  }
  ambientTemperatureC = tempSum / numReadings;
  isAmbientTemperatureSet = true;
}