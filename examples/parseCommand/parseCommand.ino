/******************************************************************
  @file       parseCommand.ino
  @brief      Example of handling commands from the x-IMU3 GUI
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

//  Custom Command Hashes
#define blinkLED  507

Ping pingPacket;
bool strobe = false, blink = false;
const long strobePeriod = 5000, blinkPeriod = 1000;       
unsigned long previousMillis = 0;

void parseCommand() {
  char *cmdPtr = rx.getCommand();

  switch(rx.hash(cmdPtr) % HASH_SIZE) {
    case xIMU3_ping:
      rx.sendPing(pingPacket);
      break;
    case xIMU3_deviceName:
      rx.sendResponse("deviceName", "Arduino");
      break;
    case xIMU3_serialNumber:
      rx.sendResponse("serialNumber", "0123-4567");
      break;
    case xIMU3_firmwareVersion:
      rx.sendResponse("firmwareVersion", "v1.0");
      break;
    case xIMU3_bootloaderVersion:
      rx.sendResponse("bootloaderVersion", "Rel: 4.0.2");
      break;
    case xIMU3_hardwareVersion:
      rx.sendResponse("hardwareVersion", "v1.0");
      break;
    case xIMU3_serialMode:
      rx.sendResponse("serialMode", STANDARD);
      break;
    case xIMU3_serialBaudRate:
      rx.sendResponse("serialBaudRate", 115200);
      break;
    case xIMU3_serialRtsCtsEnabled:
      rx.sendResponse("serialRtsCtsEnabled", "false");
      break;
    case xIMU3_note:
      rx.sendResponse("note", rx.getValue());
      break;
    case xIMU3_shutdown:
      //  Shutdown Arduino command received
      rx.sendResponse("shutdown", "true");
      break;
    case xIMU3_strobe:
      rx.sendResponse("strobe", "null");
      strobe = true;
      digitalWrite(LED_BUILTIN, HIGH);
      previousMillis = millis();
      break;
    case blinkLED: {
      char *cmdValue = rx.getValue();
      char msg[100] = "Custom Command Received - blinkLED - ";

      rx.sendResponse("blinkLED", cmdValue);
      rx.sendNotification(strcat(msg, cmdValue));

      if (strcasecmp("true", cmdValue) == 0) {
        blink = true;
        digitalWrite(LED_BUILTIN, HIGH);
        previousMillis = millis(); 
      }
      else {
        blink = false;
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
      break;
    default:
      char msg[100] = "Unhandled x-IMU3 command - ";
      
      rx.sendError(strcat(msg, cmdPtr));
      break;
  }
  
}

void setup() {
  //  Start Serial and wait for connection
  Serial.begin(115200);
  while (!Serial);

  rx.sendNotification("Arduino Connected");

  //  Init Ping Packet - struct defined in Reefwing_imuTypes.h
  pingPacket.interface = "USB";
  pingPacket.dName = "Arduino";
  pingPacket.sNumber = "0123-4567"; 

  //  Used for strobe and blink command
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  rx.checkForCommand();
  if (rx.newCommand()) {
    //Parse cmd string
    parseCommand();
  }

  if (strobe) {
    if (millis() - previousMillis >= strobePeriod) {
      digitalWrite(LED_BUILTIN, LOW);
      strobe = false;
    }
  }

  if (blink) {
    if (millis() - previousMillis >= blinkPeriod) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      previousMillis = millis();
    }
  }
}