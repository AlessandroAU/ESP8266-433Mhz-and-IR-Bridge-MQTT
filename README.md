# ESP8266-433Mhz-and-IR-Bridge-MQTT
ESP8266 based bridge that allows you bridge any 433Mhz ASK/OOK and IR devices to a network via MQTT over WiFi

This project uses an ESP8266 to bridge 433/315Mhz OOK/ASK RF modules as well as 38khz based IR devices to a LAN network.
The primary intended form of communication is over MQTT to allow for easy integration into home automation systems such as OpenHAB or HomeAssistant

This means you can send and recieve from ANY 433Mhz/315Mhz OOK sensor, switch etc.
It also supports any 38 khz IR device. At the moment packets are raw format and excoded as pulse lengths, 
there is no packet decoding implimented yet.

The bridge can be as 'polished' or rudmentary as you like, the bridge can be built on a breadboard or integrated into a custom housing.
STLs will be provided for a 3D printed enclosure that fits a WeMos 8266 along with a daughter board to mount the RF modules. See images.

The board is compatible with most cheap 433/315Mhz OOK/ASK modules that are avaiable cheaply from various online sources. 

Usage:


License: CC BY-NC-SA 3.0