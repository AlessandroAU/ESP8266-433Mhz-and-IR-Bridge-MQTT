
///// This a clusterfuck of a function and should probably be rewritten, I would like to impliment a circular buffer but this works for now /////

void RF_sample() {
  if (rfTriggerCondition == false) {
    if (digitalRead(RFrecvPin) == true) {
      RFtrigger1 = false;
      RFtrigger2 = false;
      return;
    }

    if (digitalRead(RFrecvPin) == false) {
      if (RFtrigger1 == false) {
        lastLowRFsampleTime = ESP.getCycleCount();
        RFtrigger1 = true;
        return;
      } else {
        unsigned long delta = ESP.getCycleCount() - lastLowRFsampleTime;
        if ((delta > 640000)) {
          //if(digitalRead(RFrecvPin) == true){
          //if//640000 cycles @ 160mhz = 4ms
          //Serial.println(delta);
          RFtrigger1 = false;
          //RFtrigger2 = false;
          rfTriggerCondition = true;
          RFclearArray();



          //lastLowRFsampleTime=ESP.getCycleCount();
          //}
        }
      }
    }
  } else {
    //else {
    //    if (RFtrigger2 == false) {
    //      if (digitalRead(RFrecvPin) == true) {
    //        RFtrigger2 = true;
    //        return;
    //      }
    //    } else {

    ////////////////Top Half of function waits for the RF signal to stay low for 4ms, this indicates that the reciever is about to get a packet of data
    ////////////////Bottom half then starts sampling the data

    if (bitRFindex == 8) {
      bitRFindex = 0;
      RFindex += 1;
      if (RFindex == RFrecvPinBuffSize) {
        RFindex = 0;
        //printData();
        //RF_cleanPacket();
        //printDataHuman(rfDataIn);
        rfTriggerCondition = false;
        rfDataIn[0] = 0; //first bit is always a 1, clear this.
        //printDataBits(rfDataIn, RFrecvPinBuffSize);
        RF_cleanPacket();
        //boolean rc = compare_arrays(test1,rfDataDecoded);
        //Serial.println("Result is:");
        //Serial.println(rc);
        //        if(rc){
        //          Serial.print("WE GOT ITTTTT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        //        }
        ////This branch is called when the function fills the buffer, the data is then passed on for analysis.
      }
    }
    if (digitalRead(RFrecvPin)) {
      bitSet(rfDataIn[RFindex], bitRFindex);
    } else {
      bitClear(rfDataIn[RFindex], bitRFindex);
    }
    bitRFindex = bitRFindex + 1;

  }
}
//}

void RFclearArray() {
  analogWrite(grnPin, 0);
  for (int i = 0; i < RFrecvPinBuffSize - 1; i++) {
    rfDataIn[i] = 0xFF;
  }
}

void RF_cleanPacket() { //convert raw data into pulse length format
  bool currBit = false;
  bool prevBit = false;
  int absIndex = 0;
  int prevAbsIndex = 0;
  int rfOutputIndex = 1;
  rfDataIn[0] = 0;
  rfDataDecoded[0] = 0;

  for (int i = 0; i < RFrecvPinBuffSize; i++) {
    for (int j = 0; j < 8; j++) {
      byte currByte = rfDataIn[i];
      currBit = bitRead(currByte, j);
      //Serial.println(j);

      if (currBit == !prevBit) {
        //Serial.println(absIndex,DEC);

        rfDataDecoded[rfOutputIndex] = absIndex - prevAbsIndex;

        rfOutputIndex = rfOutputIndex + 1;
        prevAbsIndex = absIndex;
      }
      prevBit = currBit;
      absIndex = absIndex + 1;
      //Serial.println(absIndex);

      if ((absIndex - prevAbsIndex) > 100 and (rfOutputIndex > 3)) {
        goto exitloop;
      }
    }

  }
exitloop:
  rfDataDecoded[0] = rfOutputIndex;
  lastRFrecv = millis();
  //Serial.println(rfDataDecoded[1]);
  int newval = rfDataDecoded[1] + 76; //magic const 67
  if (newval < 255) {
    rfDataDecoded[1] = newval;
  } else {
    rfDataDecoded[1] = 255;
  }

  if (rfDataDecoded[0] > 1 and CheckforValidity(rfDataDecoded) == true) {
    Serial.print("Raw RF Packet Recieved:");
    printDataByte(rfDataDecoded, rfOutputIndex);
    pubMQTT(rfDataDecoded, "ESP/RFrecv");

    //printDataHuman(rfDataDecoded);

    //printDataBits();
  }
      digitalWrite(grnPin, HIGH);
}

// This is another hard to follow function, basically it toggles the RF pin as it loops through the data array
// This is not as bad as the IR send function as we do not have to generate the 38khz carrier signal.

void RF_SendPacket() {
  analogWrite(grnPin, LOW);

  //  if (rfDelayTime == true) {
  //    digitalWrite(RFsendPin, HIGH);
  //    RF_on = true;
  //    rfDelayCounter = rfDelayCounter + 1;
  //    if (rfDelayCounter == 100) {
  //      rfDelayTime = false;
  //      rfDelayCounter = 0;
  //      //RF_on = true;
  //      //digitalWrite(RFsendPin,HIGH);
  //      //RF_on=true;
  //    }
  //  } else {
  //
  //  if(firstpacket){
  //  digitalWrite(RFsendPin, HIGH);
  //  firstpacket=false;
  //  RF_on = false;
  //  }else{
  if (RFsendSubIndex == rfDataOut[RFsendIndex + 1]) {
    RF_on = !RF_on;
    digitalWrite(RFsendPin, RF_on);
    //Serial.print(RF_on);
    RFsendSubIndex = 0;
    RFsendIndex = RFsendIndex + 1;
    //Serial.println(rfDataOut[RFsendIndex]);
  }
  RFsendSubIndex = RFsendSubIndex + 1;
  if (RFsendIndex == rfDataOut[0] - 1) {
    digitalWrite(RFsendPin, LOW);
    if (RFpacketCounter < RFpacketRepeats) {
      //digitalWrite(RFsendPin, LOW);
      RF_on = false;
      RFsendIndex = 0;
      RFsendSubIndex = 0;
      RFpacketCounter = RFpacketCounter + 1;
      //Serial.println("sent part data");
      //RF_on = true;
      //digitalWrite(RFsendPin, LOW);
      rfDelayTime = true;
      firstpacket = true;

    } else {
      RF_on = false;
      RFsendIndex = 0;
      RFsendSubIndex = 0;
      Serial.print("Sent Packet over RF (repeats," + String(RFpacketCounter));
      Serial.print("): ");
      printDataByte(rfDataOut, rfDataOut[0]);
      RFpacketCounter = 0;
      digitalWrite(RFsendPin, LOW);
      RF_NeedtoSend = false;
      rfDelayTime = true;
      firstpacket = true;
      rfDelayCounter = 0;
    }
  }
  analogWrite(grnPin, HIGH);
}
//}
//}


