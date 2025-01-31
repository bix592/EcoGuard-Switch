void initializeSensors() {
  // initialize current sensing
  lcd.print("Current sensing");
  lcd.setCursor(0, 1);
  lcd.print("Initializing");
  initTemp();
  lcd.clear();

  // Initialize MPU
  initMPU();
  lcd.print("MPU connected");
  delay(1000);
  lcd.clear();

  // Map the room with distance sensor and servo
  lcd.setCursor(0, 0);
  lcd.print("Mapping room...");
  mapRoom();
  lcd.clear();

  // Initialize PIR sensor (allow it to stabilize for a minute)
  lcd.setCursor(0, 0);
  lcd.print("Initializing PIR...");
  delay(10000); // Wait for 1 minute
  lcd.clear();
}