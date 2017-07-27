
void IRin_ISR() { //trigger to detect when there is a falling edge on IR input pin
  digitalWrite(redPin, HIGH);
  detachInterrupt(D5);
  IRin_detected = true;
}

void IR_cleanPacket() { //convert raw data into pulse length format
  bool currBit = false;
  bool prevBit = false;
  int absIndex = 0;
  int prevAbsIndex = 0;
  int irOutputIndex = 1;
  irDataIn[0] = 0;
  irDataDecoded[0] = 0;

  for (int i = 0; i < IRrecvPinBuffSize; i++) {
    for (int j = 0; j < 8; j++) {
      byte currByte = irDataIn[i];
      currBit = bitRead(currByte, j);
      //Serial.println(j);

      if (currBit == !prevBit) {
        //Serial.println(absIndex,DEC);

        irDataDecoded[irOutputIndex] = absIndex - prevAbsIndex;
//        if ((absIndex - prevAbsIndex)>255){
//          if ((absIndex - prevAbsIndex)<10000){
//          Serial.println("wtferr");
//          Serial.println(absIndex - prevAbsIndex);
        //}
       // }

        irOutputIndex = irOutputIndex + 1;
        prevAbsIndex = absIndex;
      }
      prevBit = currBit;
      absIndex = absIndex + 1;
      //Serial.println(absIndex);

     if ((absIndex - prevAbsIndex) > 200 and (irOutputIndex > 10)) {
      //if (absIndex==IRrecvPinBuffSize*8-1){
      goto exitloop;
    }
    }

  }
exitloop:
  irDataDecoded[0] = irOutputIndex;
  printDataByte(irDataDecoded, irOutputIndex);
  //printDataHuman(irDataDecoded);
  pubMQTT(irDataDecoded, "ESP/IRrecv");
  digitalWrite(redPin, LOW);
  //printDataBits();
}

void IR_sample() { //funciton to sample and store the IR pin data
  if (IRin_detected == true) {
    if (bitIRindex == 8) {
      bitIRindex = 0;
      IRindex += 1;
      if (IRindex == IRrecvPinBuffSize - 1) {
        IRindex = 0;
        //printData();
        IRin_detected = false;
        //checkForData(irDataIn, IRrecvPinBuffSize, true);
        Serial.print("Raw IR Packet Recieved: ");
        IR_cleanPacket();
        //Serial.println("");
        //client.publish("outBridge", "Raw IR Packet Recieved: ");
        attachInterrupt(D5, IRin_ISR, FALLING);
        reEnableIR = millis();
        digitalWrite(redPin, LOW);
      }
    }
    if (digitalRead(IRrecvPin)) {
      bitSet(irDataIn[IRindex], bitIRindex);
    } else {
      bitClear(irDataIn[IRindex], bitIRindex);
    }
    bitIRindex = bitIRindex + 1;
  }
}

void IRsend_ISR() { //routine for sending IR pulses at 38khz.

  if (IR_on == true) {
    IRsendPin_state = !IRsendPin_state; //toggle IR pin state to generate carrier signal
    digitalWrite(IRsendPin, IRsendPin_state);
    //Serial.print(IR_on);
  } else {
    digitalWrite(IRsendPin, LOW);
    //Serial.print(IR_on);
  }
}


//void IR_SendFunc(){
//
//  IRsendIndex=IRsendIndex+1;
//
//      if(!testdata[IRsendIndex]==1){
//        IR_on=true;
//        Serial.print("1");
//      }else{
//        IR_on=false;
//        Serial.print("1");
//      }
//    //Serial.println(IRsendIndex);
//    if(IRsendIndex>=testdata[0]){
//      Serial.println("Sent IR Data Packet");
//      IRsendIndex=0;
//      IR_NeedtoSend=false;
//      IR_on=false;
//
//  }
//}


void IR_SendFuncPWM() {
  //  int packetLength = testdata[0];


  if (IRsendSubIndex == irDataOut[IRsendIndex]) {
    IR_on = !IR_on;
    IRsendSubIndex = 0;
    IRsendIndex = IRsendIndex + 1;
    //Serial.print("1");
  }
  IRsendSubIndex = IRsendSubIndex + 1;
  //
  if (IRsendIndex == irDataOut[0]) {
    if (IRpacketCounter < IRpacketRepeats) {
      IRsendIndex = 0;
      IRsendSubIndex = 0;
      IRpacketCounter = IRpacketCounter + 1;
      //Serial.println("sent part data");
    } else {
      IR_NeedtoSend = false;
      IR_on = false;
      IRsendIndex = 0;
      IRsendSubIndex = 0;
      Serial.print("Sent Packet over IR (repeats," + String(IRpacketCounter));
      Serial.print("): ");
      printDataByte(irDataOut, irDataOut[0]);
      Serial.println("");
      IRpacketCounter = 0;
    }
  }

}



