/*
  Send and receive data from a LoRaWAN network
  This sketch demonstrates how to send and receive data with the MKR WAN 1300/1310 board.
  This example code is in the public domain.
*/

#include <MKRWAN.h>
#include "arduino_secrets.h"

LoRaModem modem;

void setup() {
  // Serial port initialization
  Serial.begin(115200);
  while (!Serial);
  
  // LoRa module initialization
  // Initialize the modem with your regional band (eg. US915, AS923,...)
  if (!modem.begin(EU868)) {
    Serial.println("- Failed to start module");
    while (1) {}
  };

  // With the module initialized, we can get its version and device EUI
  // Your network server provider requires device EUI information to connect to their network
  Serial.print("- Your module version is: ");
  Serial.println(modem.version());
  Serial.print("- Your device EUI is: ");
  Serial.println(modem.deviceEUI());

  // Join procedure to the network server
  // OTAA method need appEUI and appKey, this information is provided by the network server
  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println("- Something went wrong; are you indoor? Move near a window and retry...");
    while (1) {}
  }

  // Set poll interval to 60 secs.
  modem.minPollInterval(60);

  // NOTE: independent of this setting, the modem will not allow sending more than one message every 2 minutes
  // This is enforced by firmware and can not be changed.
}

void loop() {
  Serial.println();
  Serial.println("- Enter a message to send to network");
  Serial.println("(make sure that end-of-line 'NL' is enabled)");

  // Get message from Serial Monitor
  while (!Serial.available());
  String msg = Serial.readStringUntil('\n');

  // Show the sent message to the network in HEX format
  Serial.println();
  Serial.print("- Sending: " + msg + " - ");
  for (unsigned int i = 0; i < msg.length(); i++) {
    Serial.print(msg[i] >> 4, HEX);
    Serial.print(msg[i] & 0xF, HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Check if the message was sent correctly or if there was an error
  int err;
  modem.beginPacket();
  modem.print(msg);
  err = modem.endPacket(true);
  if (err > 0) {
    Serial.println("- Message sent correctly!");
  } else {
    Serial.println("- Error sending message :(");
    Serial.println("(- You may send a limited amount of messages per minute, depending on the signal strength");
    Serial.println("- It may vary from one message every couple of seconds to one  message every minute)");
  }

  // Wait and check if there's a message sent from the network
  delay(1000);
  if (!modem.available()) {
    Serial.println("- No downlink message received at this time");
    return;
  }

  // If there's a message available, store it
  char rcv[64];
  int i = 0;
  while (modem.available()) {
    rcv[i++] = (char)modem.read();
  }

  // Decode and show the received message from the network
  Serial.print("- Received: ");
  for (unsigned int j = 0; j < i; j++) {
    Serial.print(rcv[j] >> 4, HEX);
    Serial.print(rcv[j] & 0xF, HEX);
    Serial.print(" ");
  }
  Serial.println();
}
