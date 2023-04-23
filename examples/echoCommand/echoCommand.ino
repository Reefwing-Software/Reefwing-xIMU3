/******************************************************************
  @file       echoCommand.ino
  @brief      Send commands from the x-IMU3 GUI back as Notifications
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

void setup() {
  //  Start Serial and wait for connection
  Serial.begin(115200);
  while (!Serial);

  rx.sendNotification("Arduino Connected");
}

void loop() {
  rx.checkForCommand();
  if (rx.newCommand()) {
    char *cmdPtr = rx.getCommand();

    //  Echo Command to x-IMU3 GUI
    rx.sendNotification(cmdPtr);
  }
}