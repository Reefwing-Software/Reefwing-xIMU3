/******************************************************************
  @file       xIMU3_Types.h
  @brief      Types for use with the xIMU3 GUI Arduino Library
  @author     David Such
  @copyright  Please see the accompanying LICENSE file

  Code:        David Such
  Version:     1.1.0
  Date:        13/04/23

  1.0.0     Original Release.       13/04/23
  1.1.0     Updated Serial Msg      09/08/23

  Credit - Derived from the x-IMU-Arduino-Example by Seb Madgwick
           (https://github.com/xioTechnologies/x-IMU-Arduino-Example)
           and using information in the xIMU3 User Manual 
           (https://x-io.co.uk/downloads/x-IMU3-User-Manual-v1.1.pdf).
           Uses the Madgwick Quaternion Class.

******************************************************************/

#ifndef xIMU3_Types_h
#define xIMU3_Types_h

/******************************************************************
 *
 * xIMU3 Structs - from XimuReceiver.h
 * (https://github.com/xioTechnologies/x-IMU-Arduino-Example)
 * 
 ******************************************************************/

typedef enum {
    ERR_NO_ERROR,
    ERR_FACTORY_RESET_FAILED,
    ERR_LOW_BATTERY,
    ERR_USB_RECEIVE_BUFFER_OVERRUN,
    ERR_USB_TRANSMIT_BUFFER_OVERRUN,
    ERR_BLUETOOTH_RECEIVE_BUFFER_OVERRUN,
    ERR_BLUETOOTH_TRANSMIT_BUFFER_OVERRUN,
    ERR_SD_CARD_WRITE_BUFFER_OVERRUN,
    ERR_TOO_FEW_BYTES_IN_PACKET,
    ERR_TOO_MANY_BYTES_IN_PACKET,
    ERR_INVALID_CHECKSUM,
    ERR_UNKNOWN_HEADER,
    ERR_INVALID_NUM_BYTES_FOR_PACKET_HEADER,
    ERR_INVALID_REGISTER_ADDRESS,
    ERR_REGISTER_READ_ONLY,
    ERR_INVALID_REGSITER_VALUE,
    ERR_INVALID_COMMAND,
    ERR_GYROSCOPE_AXIS_NOT_AT_200DPS,
    ERR_GYROSCOPE_NOT_STATIONARY,
    ERR_ACCELEROMETER_AXIS_NOT_AT_1G,
    ERR_MMAGNETOMETER_SATURATION,
    ERR_INCORRECT_AUXILIARY_PORT_MODE,
    ERR_UART_RECEIVE_BUFFER_OVERRUN,
    ERR_UART_TRANSMIT_BUFFER_OVERRUN,
} ErrorCode;

typedef struct {
    float battery;      /* battery voltage in V */
    float thermometer;  /* thermometer in degC  */
} BattAndThermStruct;

/******************************************************************
 *
 * xIMU3 Additional Enums and Structs - 
 * 
 ******************************************************************/

enum ChargingStatus {
  DISCONNECTED = 0,
  CHARGING,
  CHARGED
};

enum SerialMode {
  DISABLED = 0,
  STANDARD,
  ACCESSORY
};

enum TokenState {
  START_JSON = 0,
  START_CMD,
  END_CMD,
  START_VALUE,
  END_VALUE,
  END_JSON
};

enum CommandTokens {
  START_OBJ = '{',
  STRING_DELIM = '\"',
  COLON = ':',
  END_OBJ = '}',
  TERMINAL = '\n'
};

enum ValueType {
  JSON_STRING = 0,
  JSON_NUMBER,
  JSON_BOOL,
  JSON_NULL,
  JSON_UNDEFINED
};

struct BatteryData {
  float percentCharged; 
  float voltage;
  ChargingStatus status;
};

struct RSSIData {
  float percentage;
  float power;
};

struct Ping {
  char *interface;
  char *dName;
  char *sNumber;
};

struct NetworkAnnouncement {
  uint16_t sync;
  char *displayName;
  char *serialNumber;
  char *ipAddress;
  uint16_t portTCP;
  uint16_t sendUDP;
  uint16_t receiveUDP;
  uint8_t rssiPercentage;
  uint8_t batteryPercentage;
  uint8_t chargingStatus;
};

#endif