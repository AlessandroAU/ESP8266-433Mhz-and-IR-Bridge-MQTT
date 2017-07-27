//void HandleTelnet() {
//  uint8_t i;
//  //check if there are any new clients
//  if (server.hasClient()) {
//    PrintIntroMessageTelnet();
//    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
//      //find free/disconnected spot
//      if (!serverClients[i] || !serverClients[i].connected()) {
//        if (serverClients[i]) serverClients[i].stop();
//        serverClients[i] = server.available();
//        Serial1.print("New Telnet client: "); Serial1.print(i);
//        continue;
//      }
//    }
//    //no free/disconnected spot so reject
//    WiFiClient serverClient = server.available();
//    serverClient.stop();
//  }
//  //check clients for data
//  for (i = 0; i < MAX_SRV_CLIENTS; i++) {
//    if (serverClients[i] && serverClients[i].connected()) {
//      if (serverClients[i].available()) {
//        //get data from the telnet client and push it to the UART
//        while (serverClients[i].available()) Serial.write(serverClients[i].read());
//      }
//    }
//  }
//}
//
//void printTelent(String inStr) {
//  int strlength = inStr.length();
//  if (strlength > 1024) {
//    Serial.println("Telnet Packet too long to send");
//    return;
//  } else {
//    inStr.getBytes(TelnetBuff, strlength);
//    byte i = 0;
//    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
//      if (serverClients[i] && serverClients[i].connected()) {
//        serverClients[i].write(&TelnetBuff[0], strlength); //write byte too all connected clients.
//
//      }
//    }
//  }
//}

