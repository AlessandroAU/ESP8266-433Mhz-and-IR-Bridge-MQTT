byte argsIndex[20]; //index for storing input arguments via MQTT {}{}{}... gives posistion of {} in input;
int argsIndexCounter = 0; //counter for storing where we are in the argsIndex array
int argsCount = 0; //counter for the number of input arguments
int IndexInByte = 0; //search index in the input array

void extractArgs(byte* payload, unsigned int length) {
  while (extactBackets) {
  }
  IndexInByte = 0;
}

bool extactBackets(byte* inputPayload, unsigned int inlength) {  //maybe fix the input args naming scheme //finds locations of {}{} in input byte.
  for (int i = IndexInByte; i < inlength; i++) {
    if ((char) inputPayload[i] == '{') {
      argsIndex[argsIndexCounter] = i;
      argsIndexCounter = argsIndexCounter + 1;
      for (int j = i; j < inlength - i; j++) {
        if ((char) inputPayload[i] == '}') {
          argsIndex[argsIndexCounter] = j;
          argsIndexCounter = argsIndexCounter + 1;
          argsCount = argsCount + 1;
          IndexInByte = j;
          if (inlength == IndexInByte) {
            return true;
          } else {
            return false;
          }
        }
      }
    }
  }
}

void MQTTcallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("MQTT arrived [");
  Serial.print(topic);
  Serial.print("] ");
  //noInterrupts();

  String input;
  input.reserve(10);
  byte val = 0;
  int Index = 0;

  if (topic[5] == 'L') { //LED state change req, process subroutine
    MQTTleds(topic, payload, length);
  } else {

    if ((char)payload[0] == 123) { // { bracket

      for (int i = 1; i < length; i++) { //decode comma delimited byte array... hopefully...
        if ( (char)payload[i] == 44 || (char)payload[i] == 125) { //comma char.
          //Serial.print("comma");
          //Serial.println(input);
          if (topic[5] == 'F') {
            //Serial.print("I made it");
            rfDataOut[Index] = input.toInt();
            //Serial.println(rfDataOut[Index]);
          }

          if (topic[5] == 'R') {

            irDataOut[Index] = input.toInt();

          }
          //Serial.println(val);
          //Serial.println(input);
          input = "";
          Index = Index + 1;
          //        //Serial.println(Index);
        } else {
          input.concat(String((char)payload[i]));

        }
        if ((char)payload[i] == 125) {
          //Serial.println("Complete Packet Recev IR");
          if (topic[5] == 'R') {
            sendIRpuls();
            //Serial.println("Sending IR");
            return;
            //Serial.println("Sending IR");

          }
          if (topic[5] == 'F') {
            if (RFaddtoQue == false) {
              RFaddtoQue = true;
            }
            //sendRFpuls();
            //delay(5);
            //sendRFpuls();
            //delay(5);
            //sendRFpuls();
            //Serial.println("Sending RF");
            return;
          }
        }
      }

    } else {
      Serial.println("INVALID MQTT MSG RECV");
    }
    // interrupts();
  }
}






void pubMQTT(byte * inputArray, char* topic) {
  if (client.connected()) {
    mqttbuff.concat("{");
    for (int i = 0; i < inputArray[0]; i++) {
      mqttbuff.concat(String((inputArray[i])));
      if (i < inputArray[0] - 1) {
        mqttbuff.concat(",");
      }
    }
    mqttbuff.concat("}");
    mqttbuff.toCharArray(mqttmsg, mqttbuff.length() + 1);
    client.publish(topic, mqttmsg);
    Serial.print("pub MQTT, topic:" + String(topic) + " msg: ");
    Serial.println(mqttbuff);
    mqttbuff = "";
  }
}

void HandleMQTT() {
  //Serial.println("Going to Ping MQTT Broker");
  // if (Ping.ping("192.168.1.1")) {
  //Serial.println(client.connected());
  if (!client.connected()) {
    Serial.println("MQTT Client not connected...");
    long now = millis();
    if (now - lastReconnectAttempt > 2500) {
      lastReconnectAttempt = now;
      Serial.println("Attempting reconnect to MQTT Broker");
      if (reconnect()) {
        Serial.println("Sucessfully reconnected to MQTT Broker!");
      }
    }
  }else{
    client.loop();
  }
}




//Serial.println("Not connected to MQTT broker but can still ping it...");
//    if (!client.connected()) {
//      Serial.println("MQTT Client not connected...");
//      long now = millis();
//      if (now - lastReconnectAttempt > 2500) {
//        Serial.println("Something is wrong with your MQTT broker!");
//        lastReconnectAttempt = now;
//        MQTTConAttempts = MQTTConAttempts + 1;
//        if (MQTTConAttempts == MQTTConAttemptsMax) {
//          Serial.println("Something is wrong with your MQTT broker!...Rebooting....");
//          ESP.reset();
//        }
//        // Attempt to reconnect
//        //if (Ping.ping(mqtt_server)) {
//          //if (reconnect()) {
//            //lastReconnectAttempt = 0;
//          //}
//        } else {
//
//        }
//      }
//    } else {
//      Serial.println("Something is wrong with your MQTT broker! Can't Connect!, Can't Ping!");
//    }
//  //} else {
//    //if (MQTTConAttempts != 0) {
//    //MQTTConAttempts = 0;
//    //client.loop();
//  }
//}

boolean reconnect() {
  client.setServer(mqtt_server, 1883);
  client.setCallback(MQTTcallback);
  Serial.println("Attempting Reconnect");
 if (client.connect(getMacAddress().c_str(), mqtt_username, mqtt_password)) {
 //if (client.connect("esp")) {
 
    //Serial.print(getMacAddress().c_str() + mqtt_username.c_str() + mqtt_password.c_str()+'\n');
    Serial.println("ESP/status: ESP8266 Network Bridge reconnected....");
    client.publish("ESP/status", "ESP8266 Network Bridge reconnected....");
    client.subscribe("ESP/RFtoSend");
    client.subscribe("ESP/IRtoSend");
    client.subscribe("ESP/RLED");
    client.subscribe("ESP/GLED");
    client.subscribe("ESP/BLED");
    return client.connected();
  } else {
    return false;
  }

}


