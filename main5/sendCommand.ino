// void sendCommand (String command, int maxTime, char readReplay[]) {
//   Serial.print(countTrueCommand);
//   Serial.print(". at command => ");
//   Serial.print(command);
//   Serial.print(" ");

//   while(countTimeCommand < (maxTime*1)){
//     esp8266.println(command);//at+cipsend
//     if(esp8266.find(readReplay))//ok
//     {
//       found = true;
//       break;
//     }
//     countTimeCommand++;
//   }

//   if(found == true)
//   {
//     Serial.println("OK");
//     countTrueCommand++;
//     countTimeCommand = 0;
//   }

//   if(found == false)
//   {
//     Serial.println("Fail");
//     countTrueCommand = 0;
//     countTimeCommand = 0;
//   } 

//   found = false;

//  }