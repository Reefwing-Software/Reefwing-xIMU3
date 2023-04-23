/******************************************************************
  @file       commandHashes.ino
  @brief      Calculate hashes for the xIMU3 GUI command keys
  @author     David Such
  @copyright  Please see the accompanying LICENSE file

  Code:        David Such
  Version:     1.0.0
  Date:        13/04/23

  1.0.0     Original Release.       13/04/23

  Notes:
    - The Hash Table is displayed on the Arduino IDE Serial Monitor,
      not the x-IMU3 GUI.
    - You can adjust the HASH_SIZE constant to see the effect that
      this has on collisions.
    - If you want to use additional command keys then you can test
      them here first for collisions before adding them to the
      x_IMU3_Protocol.h header file.

******************************************************************/

#include "Reefwing_xIMU3.h"

#define HASH_SIZE  751
#define MAX_CMDS   32

Reefwing_xIMU3 rx;

const char *xIMU3Commands[MAX_CMDS] = {"ping", "serialNumber", "serialMode", "serialBaudRate", "deviceName",
                                 "default", "apply", "save", "time", "reset", "shutdown", "strobe",
                                 "colour", "heading", "accessory", "note", "format", "test", "bootloader",
                                 "factory", "erase", "firmwareVersion", "bootloaderVersion", "hardwareVersion",
                                 "serialRtsCtsEnabled", "wirelessMode", "wirelessFirmwareVersion", 
                                 "externalAntennaeEnabled", "wiFiRegion", "wiFiMacAddress", "synchronisationEnabled"
                                };
    
char buffer[50];
unsigned long hashes[MAX_CMDS];

unsigned long collision() {
  int index = sizeof(hashes) / sizeof(hashes[0]);

  for (int i = 0; i < index - 1; i++) { 
      for (int j = i + 1; j < index; j++) {
          if (hashes[i] == hashes[j]) {
              return hashes[i];
          }
      }
  }

  return NAN;
}

void setup() {
  //  Start Serial and wait for connection
  Serial.begin(115200);
  while (!Serial);

  //  Print Hash Table Header
  Serial.println("  ______________________________________");
  sprintf(buffer, " |     x-IMU3 Command      \t  Hash  |");
  Serial.println(buffer);
  Serial.println("  ______________________________________");

  //  Generate & print hashes for the x-IMU3 command messages
  for (int i = 0; i < MAX_CMDS; i++) {
    hashes[i] = rx.hash(xIMU3Commands[i]) % HASH_SIZE;
    sprintf(buffer, " | %25s \t%6lu  |", xIMU3Commands[i], hashes[i]);
    Serial.println(buffer); 
   }

  //  Print Hash Table Footer
  Serial.println("  ______________________________________");

   // Check for hash table collisions
   unsigned long index = collision();

   Serial.print("\n    HASH_SIZE = ");
   Serial.println(HASH_SIZE);
   
   if (index != NAN) {
    Serial.print("\n    Duplicate hash found: ");
    Serial.println(index);
   }

}

void loop() {  }
