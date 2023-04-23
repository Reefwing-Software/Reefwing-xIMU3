/******************************************************************
  @file       xIMU3_Protocol.h
  @brief      Commands & MSG_IDs for the xIMU3 GUI
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

******************************************************************/

#ifndef xIMU3_Protocol_h
#define xIMU3_Protocol_h

#define xIMU3_API_VERSION_MAJOR   1
#define xIMU3_API_VERSION_MINOR   0

/******************************************************************
 *
 * xIMU3 Command Message Hash Table - Not all keys included
 * 
 ******************************************************************/

#define xIMU3_ping                      100  
#define xIMU3_serialNumber              353  
#define xIMU3_serialMode                445  
#define xIMU3_serialBaudRate            211  
#define xIMU3_deviceName                168  
#define xIMU3_default                   555  
#define xIMU3_apply                     80  
#define xIMU3_save                      329  
#define xIMU3_time                      372  
#define xIMU3_reset                     639  
#define xIMU3_shutdown                  69  
#define xIMU3_strobe                    367  
#define xIMU3_colour                    499  
#define xIMU3_heading                   185  
#define xIMU3_accessory                 500  
#define xIMU3_note                      293  
#define xIMU3_format                    97  
#define xIMU3_test                      735  
#define xIMU3_bootloader                259  
#define xIMU3_factory                   582  
#define xIMU3_erase                     624  
#define xIMU3_firmwareVersion           19  
#define xIMU3_bootloaderVersion         687  
#define xIMU3_hardwareVersion           421  
#define xIMU3_serialRtsCtsEnabled       434  
#define xIMU3_wirelessMode              36  
#define xIMU3_wirelessFirmwareVersion   178  
#define xIMU3_externalAntennaeEnabled   616  
#define xIMU3_wiFiRegion                70  
#define xIMU3_wiFiMacAddress            306  
#define xIMU3_synchronisationEnabled    453  
#define xIMU3_unknownKey                120 

/******************************************************************
 *
 * xIMU3 Global Constants
 * 
 ******************************************************************/

#define BUFFER_SIZE 100
#define CMD_SIZE 24
#define NOTE_SIZE 127
#define HASH_SIZE 751

const char NULL_TERMINATOR  = '\0';

#endif