/******************************************************************
  @file       Reefwing_xIMU3.cpp
  @brief      Arduino Library for the xIMU3 GUI
  @author     David Such
  @copyright  Please see the accompanying LICENSE file

  Code:        David Such
  Version:     1.0.0
  Date:        13/04/23

  1.0.0     Original Release.       13/04/23

  Credit - Derived from the x-IMU-Arduino-Example by Seb Madgwick
           (https://github.com/xioTechnologies/x-IMU-Arduino-Example)
           and using information in the xIMU3 User Manual 
           (https://x-io.co.uk/downloads/x-IMU3-User-Manual-v1.1.pdf).
           Uses the Madgwick Quaternion Class.

******************************************************************/

#include "Reefwing_xIMU3.h"

/******************************************************************
 *
 * Reefwing_xIMU3 Implementation - 
 * 
 ******************************************************************/

Reefwing_xIMU3::Reefwing_xIMU3() { }

/******************************************************************
 *
 * Data Messages - 
 * 
 ******************************************************************/

void Reefwing_xIMU3::sendInertial(InertialMessage msg) {
  // Inertial Message Format: "I,timeStamp (µs),gx,gy,gz,ax,ay,az\r\n"
  Serial.print("I,");
  Serial.print(msg.timeStamp);
  Serial.print(",");
  Serial.print(msg.gx, 4);
  Serial.print(",");
  Serial.print(msg.gy, 4);
  Serial.print(",");
  Serial.print(msg.gz, 4);
  Serial.print(",");
  Serial.print(msg.ax, 4);
  Serial.print(",");
  Serial.print(msg.ay, 4);
  Serial.print(",");
  Serial.print(msg.az, 4);
  Serial.print("\r\n");
}

void Reefwing_xIMU3::sendMag(ScaledData data) {
  // Magnetometer Message Format: "M,timeStamp (µs),mx,my,mz\r\n"
  Serial.print("M,");
  Serial.print(data.timeStamp);
  Serial.print(",");
  Serial.print(data.sx, 4);
  Serial.print(",");
  Serial.print(data.sy, 4);
  Serial.print(",");
  Serial.print(data.sz, 4);
  Serial.print("\r\n");
}

void Reefwing_xIMU3::sendTemperature(TempData data) {
  // Temperature Message Format: "T,timeStamp (µs),temperature (°C)\r\n"
  Serial.print("T,");
  Serial.print(data.timeStamp);
  Serial.print(",");
  Serial.print(data.celsius, 4);
  Serial.print("\r\n");
}

void Reefwing_xIMU3::sendQuaternion(Quaternion quaternion) {
  // Quaternion Message Format: "Q,timeStamp (µs),w,x,y,z\r\n"
  Serial.print("Q,");
  Serial.print(quaternion.timeStamp);
  Serial.print(",");
  Serial.print(quaternion.q[0], 4);
  Serial.print(",");
  Serial.print(quaternion.q[1], 4);
  Serial.print(",");
  Serial.print(quaternion.q[2], 4);
  Serial.print(",");
  Serial.print(quaternion.q[3], 4);
  Serial.print("\r\n");
}

void Reefwing_xIMU3::sendEulerAngles(EulerAngles euler) {
  // Euler Angles Message Format: "A,timeStamp (µs),roll,pitch,yaw\r\n"
  Serial.print("A,");
  Serial.print(euler.timeStamp);
  Serial.print(",");
  Serial.print(euler.roll, 4);
  Serial.print(",");
  Serial.print(euler.pitch, 4);
  Serial.print(",");
  Serial.print(euler.yaw, 4);
  Serial.print("\r\n");
}

void Reefwing_xIMU3::sendBattery(BatteryData data) {
  // Battery Message Format: "B,timeStamp (µs),percentCharged,voltage,status\r\n"
  Serial.print("B,");
  Serial.print(micros());
  Serial.print(",");
  Serial.print(data.percentCharged, 4);
  Serial.print(",");
  Serial.print(data.voltage, 4);
  Serial.print(",");
  Serial.print((float)data.status, 4);
  Serial.print("\r\n");
}

void Reefwing_xIMU3::sendRSSI(RSSIData data) {
  // RSSI Message Format: "W,timeStamp (µs),percent,power (dBm)\r\n"
  Serial.print("W,");
  Serial.print(micros());
  Serial.print(",");
  Serial.print(data.percentage, 4);
  Serial.print(",");
  Serial.print(data.power, 4);
  Serial.print("\r\n");
}

void Reefwing_xIMU3::sendNotification(char *note) {
  // Notification Message Format: "N,timeStamp (µs),note\r\n"
  Serial.print("N,");
  Serial.print(micros());
  Serial.print(",");
  Serial.print(note);
  Serial.print("\r\n");
}

void Reefwing_xIMU3::sendError(char *error) {
  // Error Message Format: "F,timeStamp (µs),errorMessage\r\n"
  Serial.print("F,");
  Serial.print(micros());
  Serial.print(",");
  Serial.print(error);
  Serial.print("\r\n");
}

void Reefwing_xIMU3::sendNetworkAnnouncement(NetworkAnnouncement na) {
  Serial.print("{\"sync\":");
  Serial.print(na.sync);
  Serial.print(",\"name\":\"");
  Serial.print(na.displayName);
  Serial.print("\",\"sn\":\"");
  Serial.print(na.serialNumber);
  Serial.print("\",\"ip\":\"");
  Serial.print(na.ipAddress);
  Serial.print("\",\"port\":");
  Serial.print(na.portTCP);
  Serial.print(",\"send\":");
  Serial.print(na.sendUDP);
  Serial.print(",\"receive\":");
  Serial.print(na.receiveUDP);
  Serial.print(",\"rssi\":");
  Serial.print(na.rssiPercentage);
  Serial.print(",\"battery\":");
  Serial.print(na.batteryPercentage);
  Serial.print(",\"status\":");
  Serial.print(na.chargingStatus);
  Serial.print("}\r\n");
}

/******************************************************************
 *
 * Command Responses - 
 * 
 ******************************************************************/

void Reefwing_xIMU3::sendPing(Ping ping) {
  Serial.print("{\"ping\":{\"interface\":\"");
  Serial.print(ping.interface);
  Serial.print("\",\"deviceName\":\"");
  Serial.print(ping.dName);
  Serial.print("\",\"serialNumber\":\"");
  Serial.print(ping.sNumber);
  Serial.print("\"}}\r\n");
}

void Reefwing_xIMU3::sendResponse(char *key, char *value) {
  Serial.print("{\"");
  Serial.print(key);
  Serial.print("\":\"");
  Serial.print(value);
  Serial.print("\"}\r\n");
}

void Reefwing_xIMU3::sendResponse(char *key, int value) {
  Serial.print("{\"");
  Serial.print(key);
  Serial.print("\":\"");
  Serial.print(value);
  Serial.print("\"}\r\n");
}

/******************************************************************
 *
 * Helper Methods - 
 * 
 ******************************************************************/

const unsigned long Reefwing_xIMU3::hash(const char *str) {
  //  djb2 Hash Function
    unsigned long hash = 5381;  
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

ValueType Reefwing_xIMU3::parseValueType(char c) {
  switch(c) {
    case 'n':
      return JSON_NULL;
    break;
    case 't':
    case 'f':
      return JSON_BOOL;
    break;
    case '-': case '0': case '1': case '2': case '3': case '4': 
    case '5': case '6': case '7': case '8': case '9':
      return JSON_NUMBER;
    break;
    case '\"':
      return JSON_STRING;
    break;
    default:
      return JSON_UNDEFINED;
    break;
  }
}

void Reefwing_xIMU3::checkForCommand() {
  char buffer[BUFFER_SIZE];
  
  //  Check for xIMU3 Command Messages
  if (Serial.available() > 0) {
    // read the incoming bytes:
    int blen = Serial.readBytesUntil(TERMINAL, buffer, BUFFER_SIZE);
    TokenState state = START_JSON;
    
    //  Command & Value character count
    int index = 0;
    
    //  Parse JSON Command String, extract command, value,
    //  and value type using a state machine.
    for(int i = 0; i < blen; i++) {

      switch(state) {
        case START_JSON:
          if (buffer[i] == STRING_DELIM) { 
            state = START_CMD; 
          }
        break;
        case START_CMD:
          if (buffer[i] == STRING_DELIM) { 
            state = END_CMD; 
            _cmd[index] = NULL_TERMINATOR;
            index = 0;
          }
          else {
            _cmd[index] = buffer[i];
            index++;
          }
        break;
        case END_CMD:
          if (buffer[i] == COLON) { state = START_VALUE; }
        break;
        case START_VALUE:
          if (index == 0) {  
            //  Check 1st char of value to detect type
            _valueType = parseValueType(buffer[i]);
          }
          if (buffer[i] == END_OBJ) { 
            state = END_VALUE; 
            _value[index] = NULL_TERMINATOR;
          }
          else {
            _value[index] = buffer[i];
            index++;
          }
        break;
        default:
        break;
      }

    }
  }
  else {
    _cmd[0] = NULL_TERMINATOR;
    _value[0] = NULL_TERMINATOR;
  }
}

bool Reefwing_xIMU3::newCommand() {
  if (_cmd[0] == NULL_TERMINATOR) {
    return false;
  }

  return true;
}

char* Reefwing_xIMU3::getCommand() {
  char *cmdPtr = _cmd;

  return cmdPtr;
}

char* Reefwing_xIMU3::getValue() {
  char *valuePtr = _value;

  return valuePtr;
}

ValueType Reefwing_xIMU3::getValueType() {
  return _valueType;
}