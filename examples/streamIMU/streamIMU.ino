/******************************************************************
  @file       streamIMU.ino
  @brief      Real-time data from LSM9DS1 IMU, displayed on the x-IMU3 GUI.
  @author     David Such
  @copyright  Please see the accompanying LICENSE file

  Code:        David Such
  Version:     1.0.0
  Date:        13/04/23

  1.0.0     Original Release.       13/04/23

  Dependency - Requires that the Reefwing_LSM9DS1 Library is also
               installed. If you use your own IMU library then
               the Reefwing_imuTypes Library also needs to be
               installed.

******************************************************************/

#include <Reefwing_xIMU3.h>
#include <ReefwingLSM9DS1.h>

Reefwing_xIMU3 rx;
ReefwingLSM9DS1 imu;

void setup() {
  // Initialise the LSM9DS1 IMU
  imu.begin();

  //  Start Serial and wait for connection
  Serial.begin(115200);
  while (!Serial);

  if (imu.connected()) { 
    imu.start();
    imu.calibrateGyro();
    imu.calibrateAccel();
    imu.calibrateMag();
    delay(20);
    
    //  Flush the first readings
    imu.readGyro();
    imu.readAccel();
    imu.readMag();
  } 
  else {
    rx.sendError("LSM9DS1 IMU Not Connected");
    while(1);
  }

  rx.sendNotification("LSM9DS1 IMU Connected");
}

void loop() {
  //  Read IMU Sensor Data
  InertialMessage msg = imu.readInertial();
  ScaledData magData = imu.readMag();

  imu.updateTempData();
  
  //  Send data messages to xIMU3 GUI
  rx.sendInertial(msg);
  rx.sendMag(magData);
  rx.sendTemperature(imu.gyroTemp);
}