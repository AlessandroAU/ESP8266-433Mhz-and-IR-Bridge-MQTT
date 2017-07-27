const char IntroMessage[] PROGMEM  = 
{"ESP/RFtoSend  ---MQTT Sub Topic, Will relay any messages here over RF\n"
"ESP/IRtoSend  ---MQTT Sub Topic, Will relay any messages here over IR\n\n"
"ESP/IRtoSend  ---MQTT Pub Topic, Will publish any RF packets recieved here\n"
"ESP/IRtoSend  ---MQTT Pub Topic, Will publish any IR packets recieved here\n\n"
"ESP/RLED      ---MQTT Sub Topic, Set RED Led Brightness (0-255)\n"
"ESP/GLED      ---MQTT Sub Topic, Set RED Led Brightness (0-255)\n"
"ESP/BLED      ---MQTT Sub Topic, Set RED Led Brightness (0-255)\n"
"ESP/Status    ---MQTT Pub Topic, Publishes Debug Messages to here\n"};

//void PrintIntroMessageTelnet(){
//  strcpy_P(mqttmsg, (char*)pgm_read_word(&(IntroMessage[0])));
//  printTelent(mqttmsg);
//}

void PrintIntroMessageSerial(){
  strcpy_P(mqttmsg, (char*)pgm_read_word(&(IntroMessage[0])));
  Serial.print(mqttmsg);
}



