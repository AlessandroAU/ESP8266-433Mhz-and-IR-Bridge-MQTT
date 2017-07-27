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

Basic Usage:

Pre-req:
You will need a properly setup and working MQTT server on your local network (WAN servers have not been tested)

After flashing firmware you should see the device appear in your wireless network list. Connect to the device and browse to 192.168.4.1
Enter the Wifi Credentials for your network. The WifiManager Librairy is used for the moment. After connection there will not be an active webserver
all comms will occur over MQTT.

Assuming everything is working correctly the bridge will now use pub/sub messages for communication.

The following topics are used for communication:

<b>ESP/RFtoSend</b>  ---MQTT Sub Topic, Will relay any messages here over RF

<b>ESP/IRtoSend</b>  ---MQTT Sub Topic, Will relay any messages here over IR

<b>ESP/RFrecv</b>  ---MQTT Pub Topic, Will publish any RF packets recieved here

<b>ESP/IRrecv</b> ---MQTT Pub Topic, Will publish any IR packets recieved here

<b>ESP/RLED</b>      ---MQTT Sub Topic, Set RED Led Brightness (0-255) (Optional led Topic)

<b>ESP/GLED</b>      ---MQTT Sub Topic, Set RED Led Brightness (0-255) (Optional led Topic)

<b>ESP/BLED</b>     ---MQTT Sub Topic, Set RED Led Brightness (0-255) (Optional led Topic)

<b>ESP/Status</b>   ---MQTT Pub Topic, Publishes Debug Messages to here

Example:
Lets say you want to control a 433mhz switch from your home automation system.

I assume you are running the MQTT server on a linux based device. 

SSH into the device, make sure you set the correct device IP address in the code.
My MQTT server exists on 192.168.1.103, this is currently hardcoded into the source
but of course you can edit the variable and recompile

Subscribe to the topic "ESP/RFrecv" (make sure you have mosquitto and mosquitto-clients installed and configured!)

mosquitto_sub -t "ESP/RFrecv"

If there is some RF noise in your location you may already see some messages come through. Press button on your RF device.
You should see packets stream through.

![alt text](https://raw.githubusercontent.com/AlessandroAU/ESP8266-433Mhz-and-IR-Bridge-MQTT/Images/example/sub.PNG)


License: CC BY-NC-SA 3.0