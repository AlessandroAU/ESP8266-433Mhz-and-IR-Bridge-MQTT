void LedAllOff() {
  digitalWrite(redPin, LOW);
  digitalWrite(bluPin, LOW);
  digitalWrite(grnPin, LOW);
}

void LedSet() {
  analogWrite(redPin, redVal);
  analogWrite(grnPin, grnVal);
  analogWrite(bluPin, bluVal);
}

///////////////////////////////////////////////////////////////////////

void MQTTleds(char* topic, byte* payload, unsigned int msglength) {
  String input;
  input.reserve(4);
  int ledVal = 0;

  for (int i = 1; i < msglength; i++) { //decode comma delimited byte array... hopefully...
    input.concat(String((char)payload[i]));
  }
  ledVal = input.toInt();
  if (ledVal > 100) {
    ledVal = 255;
  }

  if (topic[4] == 'R') {
    redVal = ledVal * 4;
  }
  if (topic[4] == 'G') {
    grnVal = ledVal * 4;
  }
  if (topic[4] == 'B') {
    bluVal = ledVal * 4;
  }
  LedSet();
}
//////////////////////////////////////////////////////////////////////
void MQTTsetColour(char* topic, byte* payload, unsigned int msglength){
  String input;
  input.reserve(4);
  if(msglength==6 && topic[0]==','){
    for(int i=0;topic[i]==!',';i++){
  }
}
}



