void powerESPupdate() {
  // int relayState = digitalRead(RELAY_PIN);

  // Read the analog value from LM35
  int analogValue = analogRead(LM35_PIN);

  // Convert the analog value to temperature in Celsius
  float temperatureC = analogValue * (5.0 / 1023.0) * 100.0;

  // Update moving average filter
  temperatureSum -= temperatureHistory[currentIndex];
  temperatureHistory[currentIndex] = temperatureC;
  temperatureSum += temperatureC;
  currentIndex = (currentIndex + 1) % numSamples;
  float averageTemperatureC = temperatureSum / numSamples;

  // Compute the estimated power based on temperature change
  if (isAmbientTemperatureSet) {
    float temperatureChange = averageTemperatureC - ambientTemperatureC;

    float det = 1.2702 * temperatureChange + 0.6135;
    if (det>0) {
      //Previous models
      //float current = fabs(0.2503 * log(0.9927 * temperatureChange + 0.9582)); // curvefit2, second data gathering
      //float current  = fabs(0.1752 * log(5.277 * temperatureChange - 9.1166)); //curvefit3, third data gathering

      //Working model
      current = fabs(0.2512 * log(det)); //
    } else {
      current = 0;
    }
    
    // Display the power on the LCD
    // float op_voltage = 220; 
    // power = op_voltage * current;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Current: ");
    lcd.print(power);
    lcd.print(" A");
    delay(2000);
    lcd.clear();
  }

  // Send data to Thingspeak
//   int net_acceleration=0; // insert here


//    String getData="GET /update?api_key="+ API+"&field1="+current+
//    "&field2="+power +"&field3="+ net_acceleration +"&field4="+relayState; //relay state here

//    sendCommand("AT+CIPMUX=1",5,"OK");
//    sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
//    sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
//    esp8266.println(getData);

//  countTrueCommand++;

//  sendCommand("AT+CIPCLOSE=0",5,"OK");
}