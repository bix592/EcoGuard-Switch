void initMPU() {
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    lcd.print("MPU6050 connect");
    lcd.setCursor(0, 1);
    lcd.print("failed!");
    while (1);
  }
  
  // Configure MPU6050 motion detection
  mpu.setMotionDetectionThreshold(10);  // Set motion detection threshold (raw value)
  mpu.setMotionDetectionDuration(1);   // Set motion detection duration (1 indicates immediate response)
  mpu.setIntMotionEnabled(true);       // Enable motion interrupt
  mpu.setDMPEnabled(false);            // Disable DMP to use raw data and motion interrupts
  mpu.setIntEnabled(0x40);             // Enable motion detection interrupt
}