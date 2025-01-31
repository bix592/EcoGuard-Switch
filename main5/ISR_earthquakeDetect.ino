void earthquakeDetect() {
  mpu.getIntStatus();   // Clear the motion interrupt status by reading the interrupt status register
  digitalWrite(RELAY_PIN, HIGH); // turn off relay when earthquake is detected
  earthquakeDetected = true;
}  
  
