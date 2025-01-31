void toggleRelay() {
  // Read the switch state
  volatile bool switchState = digitalRead(SWITCH_PIN);
  // Turn on relay when switch is high, turn off relay when switch is low
  digitalWrite(RELAY_PIN, switchState ? LOW : HIGH);
  relay_updateLCD = true;
}