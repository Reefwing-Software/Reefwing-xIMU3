/******************************************************************
  @file       Reefwing_xIMU3.h
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

#ifndef Reefwing_xIMU3_h
#define Reefwing_xIMU3_h

#include <Arduino.h>

#include "Reefwing_imuTypes.h"
#include "xIMU3_Types.h"
#include "xIMU3_Protocol.h"

/******************************************************************
 *
 * Reefwing_xIMU3 Class Definition - 
 * 
 ******************************************************************/

class Reefwing_xIMU3 {
public:
  Reefwing_xIMU3();

  void sendInertial(InertialMessage msg);
  void sendMag(ScaledData data);
  void sendTemperature(TempData data);
  void sendQuaternion(Quaternion quaternion);
  void sendEulerAngles(EulerAngles euler);
  void sendBattery(BatteryData data);
  void sendRSSI(RSSIData data);
  void sendNotification(char *note);
  void sendError(char *error);
  void sendNetworkAnnouncement(NetworkAnnouncement na);
  void sendPing(Ping ping);
  void sendResponse(char *key, char *value);
  void sendResponse(char *key, int value);

  const unsigned long hash(const char *str);
  void checkForCommand();
  bool newCommand();
  char* getCommand();
  char* getValue();
  ValueType getValueType();
  
private:
  ValueType parseValueType(char c);
  
  char _cmd[CMD_SIZE], _value[NOTE_SIZE];
  ValueType _valueType;
};

#endif