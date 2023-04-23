/******************************************************************
  @file       networkAnnouncement.ino
  @brief      Send a network announcement to the x-IMU3 GUI every second
  @author     David Such
  @copyright  Please see the accompanying LICENSE file

  Code:        David Such
  Version:     1.0.0
  Date:        13/04/23

  1.0.0     Original Release.       13/04/23

  Dependency - Requires that the Reefwing_imuTypes Library is also
               installed.

******************************************************************/

#include "Reefwing_xIMU3.h"

Reefwing_xIMU3 rx;

const long notificationPeriod = 1000;
unsigned long previousMillis = 0;
NetworkAnnouncement na;

void setup() {
  //  Start Serial and wait for connection
  Serial.begin(115200);
  while (!Serial);

  rx.sendNotification("Arduino Connected");

  //  Init Network Announcement Packet
  na.sync = 0;
  na.displayName = "Arduino";
  na.serialNumber = "0123-4567-89AB";
  na.ipAddress = "198.168.1.1";
  na.portTCP = 6000;
  na.sendUDP = 7000;
  na.receiveUDP = 8000;
  na.rssiPercentage = 85;
  na.batteryPercentage = 50;
  na.chargingStatus = DISCONNECTED;   //  ChargingStatus type in xIMU3_Types.h
}

void loop() {
  if (millis() - previousMillis >= notificationPeriod) {
    //  Send Network Announcement every notificationPeriod, non-blocking
    rx.sendNetworkAnnouncement(na);
    rx.sendNotification("Network Announcement Broadcast");
    previousMillis = millis();
  }
}
