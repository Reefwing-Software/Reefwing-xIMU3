![version](https://img.shields.io/github/v/tag/Reefwing-Software/Reefwing-xIMU3) ![license](https://img.shields.io/badge/license-GPL-green) ![release](https://img.shields.io/github/release-date/Reefwing-Software/Reefwing-xIMU3?color="red") ![open source](https://badgen.net/badge/open/source/blue?icon=github)

# Reefwing xIMU3 GUI

 An Arduino Library to facilitate serial communication with the xIMU3 GUI data visualisation software. The Reefwing xIMU3_GUI Library is available from the Arduino IDE Library Manager or from our GitHub repository.

## x-IO Technologies & the x-IMU3 GUI

[x-io Technologies](https://x-io.co.uk/) Limited is a UK-based engineering company that specialises in the design and production of IMUs. It was founded in 2012 by Seb Madgwick during his PhD at the University of Bristol. The [Madgwick algorithm](https://github.com/xioTechnologies/Fusion) in its various incarnations, is used in a LOT of AHRS libraries (including ours).

In addition to their sensor fusion software, x-io also have an IMU data visualisation tool called the x-IMU3 GUI. This free open source software is available for all major operating systems (Windows, macOS, and Linux). You can download a copy from the [x-io web site](https://x-io.co.uk/x-imu3/). The API for the x-IMU3 is described in the user manual. We will use this API information to write an Arduino library which makes communicating with the xIMU3 GUI easy.

## The x-IMU3 API

The API largely consists of command and data messages. An extract from the x-IMUs user manual follows.

```
The communication protocol consists of two message types:

- Command messages 
- Data messages 

All messages are terminated by a Line Feed (LF) control character. 
This termination byte will not appear anywhere else in a message and 
so can be used to divide a byte stream into individual messages. 

Some messages are terminated with an additional Carriage Return (CR) 
control character.

The first byte of a message indicates the message type. Command messages start with the character "{" (0x7B in hex, 123 in decimal). 
Data messages start with either an uppercase character or a byte value greater than 0x80 (128 in decimal) depending on the message.
You could use the command messages to control your Arduino from the x-IMU3 GUI, and we will demonstrate how to read and parse the commands from the serial port, but first let's look at data messages.
```

You could use the command messages to control your Arduino from the x-IMU3 GUI, and we will demonstrate how to read and parse the commands from the serial port, but first let's look at data messages.

## Data Messages

One of the key things that you will note when looking at the data message format (Figure 1), is that they all include a time stamp (in microseconds). This means that you need an IMU library which returns a time stamp with each sensor reading. Some IMUs have a dedicated time stamp register but the LSM9DS1, is not one of them. Consequently, we updated the Reefwing LSM9DS1 Library to include a time stamp with each reading. We record the time stamp just after the data register is read and use the Arduino `micros()` function, so the time wont be 100% correct but should be good enough for demonstration purposes.

| MSG ID | TIME STAMP (µs) |         PAYLOAD         | STOP BYTE |
|:------:|:---------------:|:-----------------------:|:---------:|

*Figure 1. x-IMU3 Data Message Format*

Data messages can be either ASCII encoded or binary, depending on the device settings. We will use ASCII. Each value in an ASCII data message is comma separated.

The `MSG_ID` is the first byte in a data message and needs be a single uppercase character indicating the message type (e.g., `I`). The next value is the time stamp in microseconds. The API specifies a 64-bit unsigned integer for the time stamp, but 32-bits works, and this is what `micros()` returns.

The data payload data type depends on the message. For example, the magnetometer message (`MSG_ID = M`) expects three floating point numbers printed to four decimal places. The values expected from the IMU are the scaled output (i.e., celcius for temperature, DPS for gyroscope, g for acceleration, and gauss for the magnetometer).

``` c++
Serial.print("M,");
Serial.print(imu.data.mTimeStamp);
Serial.print(",");
Serial.print(imu.data.mx, 4);
Serial.print(",");
Serial.print(imu.data.my, 4);
Serial.print(",");
Serial.print(imu.data.mz, 4);
Serial.print("\r\n");
```

All messages are terminated by a Line Feed (`\n`) control character. This termination byte will not appear anywhere else in a message and so it can be used to divide a byte stream into individual messages. Most messages are also terminated with an additional Carriage Return (`\r`) control character.

The inertial message (gyroscope and accelerometer data), looks like the following. You could just follow this pattern and refer to the API to work out the message ID and expected payload contents.

``` c++
Serial.print("I,");
Serial.print(imu.data.gTimeStamp);
Serial.print(",");
Serial.print(imu.data.gx, 4);
Serial.print(",");
Serial.print(imu.data.gy, 4);
Serial.print(",");
Serial.print(imu.data.gz, 4);
Serial.print(",");
Serial.print(imu.data.ax, 4);
Serial.print(",");
Serial.print(imu.data.ay, 4);
Serial.print(",");
Serial.print(imu.data.az, 4);
Serial.print("\r\n");
```

However, to make things easier we have created wrapper methods for the common data message types. For example, to send the inertial message above you can use our method, `sendInertial(InertialMessage msg)`. The `InertialMessage` struct is already included in our [LSM9DS1 library](https://github.com/Reefwing-Software/Reefwing-LSM9DS1), and it contains all the information required for this message type. If you are not using our LSM9DS1 Library, then just declare the structure yourself or you can include the [Reefwing_imuTypes Library](https://github.com/Reefwing-Software). We have a number of libraries which use the same structures and classes and they include imuTypes so that they are only defined once.

``` c++
struct InertialMessage {
  float ax, ay, az;
  float gx, gy, gz;
  uint32_t timeStamp;
};
```

The available xIMU3 data message methods in our library are:

``` c++
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
  ```

If there is a data message you would like added, just let us know.

## Network Announcement Message

The x-IMU3 GUI expects a connected device to send a network announcement message once per second. This message is a JSON object that contains the information shown below.  

``` JSON
{
  "sync": 0,
  "name": "Raven",
  "sn": "0123-4567-89AB-CDEF",
  "ip": "192.168.1.1",
  "port": 7000,
  "send": 8000,
  "receive": 9000,
  "rssi": 100,
  "battery": 100,
  "status": 0
}
```

We tried sending a JSON object with these fields every second but were unsuccesful in updating the device settings. The library includes a sendNetworkAnnouncement() method and example sketch, if you want to experiment with this. Let us know if you get it working!

## Command Messages

Command messages are a JSON object containing a single key/value pair, terminated by the usual `\r\n`. A sample command message, requesting the device serial number looks like:

``` JSON
{"serialNumber":null}\r\n
```

To understand what command messages that the GUI was sending to the Arduino, we started by echoing these back to the x-IMU3 GUI as notifications. You can bring up the Notification and Error Message window by clicking on the chat bubble in the bottom right of the GUI.

The code for echoing commands to the x-IMU3 GUI is straight forward. We call `checkForCommand()` in `loop()`, and then use `sendNotification(cmd)`, to complete the round trip.

``` c++
checkForCommand();
if (newCommand()) {  
  sendNotification(cmd);
}
```

The `checkForCommand()` and `newCommand()` functions are detailed below.

``` c++
void checkForCommand() {
  char buffer[BUFFER_SIZE];
  
  //  Check for xIMU3 Command Messages
  if (Serial.available() > 0) {
    // read the incoming bytes:
    int blen = Serial.readBytesUntil(STOP_BYTE, buffer, BUFFER_SIZE);
    bool cmdFound = false;
    
    //  Command character count
    int index = 0;
    
    for(int i = 0; i < blen; i++) {
      //  Extract Command
      if (buffer[i] == CMD_BYTE) {
        cmdFound = !cmdFound;
      }
      else if (cmdFound) {
        cmd[index] = buffer[i];
        index++;
      }
    }
    cmd[index] = NULL_TERMINATOR;
  }
  else {
    cmd[0] = NULL_TERMINATOR;
  }
}

bool newCommand() {
  if (cmd[0] == NULL_TERMINATOR) {
    return false;
  }

  return true;
}
```

Next up we stripped the command key from the JSON command message. This allowed us to parse the received commands and reply to selected ones. 

## Parsing Command Messages

The obvious approach to parsing our received command messages is to use a `switch()` statement. Unfortunately, C/C++ doesn't treat strings as first class citizens, so we can't switch on a string or use them in case statements. This is not a new problem and there are a number of ways that you can crack this egg. In our Reefwing MSP library (A light weight Arduino implementation of the MultiWii Serial Protocol), we used a command lookup table as the solution.

``` c++
#define BAD_ID -1

typedef struct { 
  char *key; 
  int val; 
} msp_commands_t;

static msp_commands_t lookupTable[] = {
    { "MSP_API_VERSION", MSP_API_VERSION }, { "MSP_FC_VARIANT", MSP_FC_VARIANT }, { "MSP_FC_VERSION", MSP_FC_VERSION }, { "MSP_BOARD_INFO", MSP_BOARD_INFO },
    { "MSP_BUILD_INFO", MSP_BUILD_INFO }, { "MSP_NAME", MSP_NAME }, { "MSP_SET_NAME", MSP_SET_NAME }, { "MSP_IDENT", MSP_IDENT }, { "MSP_STATUS", MSP_STATUS },
    { "MSP_RAW_IMU", MSP_RAW_IMU }, { "MSP_SERVO", MSP_SERVO }, { "MSP_MOTOR", MSP_MOTOR}, { "MSP_RC", MSP_RC }, { "MSP_ATTITUDE", MSP_ATTITUDE }, { "MSP_ALTITUDE", MSP_ALTITUDE },
    { "MSP_PID", MSP_PID }, { "MSP_BOX", MSP_BOX }, { "MSP_BOXNAMES", MSP_BOXNAMES }, { "MSP_PIDNAMES", MSP_PIDNAMES }, { "MSP_BOXIDS", MSP_BOXIDS },
    { "MSP_SET_RAW_RC", MSP_SET_RAW_RC }, { "MSP_SET_RAW_GPS", MSP_SET_RAW_GPS }, { "MSP_SET_PID", MSP_SET_PID }, { "MSP_SET_BOX", MSP_SET_BOX },
    { "MSP_SET_RC_TUNING", MSP_SET_RC_TUNING }, { "MSP_ACC_CALIBRATION", MSP_ACC_CALIBRATION }, { "MSP_MAG_CALIBRATION", MSP_MAG_CALIBRATION }, { "MSP_SET_MISC", MSP_SET_MISC },
    { "MSP_RESET_CONF", MSP_RESET_CONF }, { "MSP_SET_WP", MSP_SET_WP }, { "MSP_SELECT_SETTING", MSP_SELECT_SETTING }, { "MSP_SET_HEAD", MSP_SET_HEAD }
};

#define NKEYS (sizeof(lookupTable)/sizeof(msp_commands_t))

int idLookup(char *key) {
  for (int i = 0; i < NKEYS; i++) {
      msp_commands_t sym = lookupTable[i];

      if (!strcmp(sym.key, key))
          return sym.val;
  }

  return BAD_ID;
}
```

## Hash Tables and Functions

To mix things up, for this library we will use a hash function to uniquely identify each command. A hash function is any function that can be used to map data of arbitrary size to fixed-size values. The values returned by a hash function are called hash values, hash codes, or simply hashes. The values are usually used to index a hash table. The hash function that we will use is called djb2, an algorithm (with k=33) written by Dan Bernstein.

``` c++
unsigned long hash(unsigned char *str) {
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}
```

A hash function should have the following characteristics:

1. It should be fast.
2. It shouldn't return the same index for different strings. This is called a collision and different hash functions will have different index distributions. The other key determinant of collisions is the hash table size, bigger is better but that takes more memory.
3. It needs to be deterministic. For a given string it should return the same index every time.

If we just use the hash result, then we don't get any collisions for the command keys that we tested. However, these keys are large numbers and the usual approach is to modulo the hash result with the size of your hash table (e.g., `hashes[i] = hash(xIMU3Commands[i]) % HASH_SIZE`).

A common rule of thumb is to set the hash table size to be 1.3 times the number of keys. Normally collisions in a hash table are not a problem and you just need to handle them, but we want to avoid this additional work if possible. To test this heuristic, we put together a sketch, which is included in our library, called `xIMU3_Command_Hashes.ino`.

The constant `HASH_SIZE`, represents the hash table size. If you make this value equal to one, the original hash function results are used. Our sketch includes 31 of the xIMU3 command keys, so we tested `HASH_SIZE = 31 x 1.3 = 41`. This size table has a number of collisions.

To minimize collisions, we want to reduce the number of common factors between `HASH_SIZE` and the hashes of the xIMU3 command keys. This can be done by choosing a `HASH_SIZE` which has very few factors (i.e., a prime number). For the 31 command strings tested, the smallest `HASH_SIZE` that we found, which is a prime number and doesn't cause collisions is 751.

Now there are a lot more command keys than the 31 that we tested, so this `HASH_SIZE` is no guarantee of zero collisions. In fact, if you try a `HASH_SIZE` of 1511, you will get collisions again. For our application, the consequences of a collision are not a big deal. At worst we may send a reply to a command that wasn't sent, which will then be ignored. So let's move on…

## Back to Parsing Command Messages

With a hash table of the command keys that we want to respond to, we can now receive these and send replies to the x-IMU3 GUI. In the Library example folder, there is a sketch called `parseCommand.ino`, which shows how you could handle requests from the GUI. The `parseCommand()` function is called whenever we receive a new command from the x-IMU3 GUI. To compare the string command in the `switch()` statement, we take its hash and then modulo that with `HASH_SIZE` (i.e., 751).

``` c++
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
```

Most x-IMU3 command responses consist of the string command key and a response value, which might be a string, number or boolean. In our library we have included an overloaded `sendResponse()` method to assist with replying to command messages.

``` c++
void sendResponse(char *key, char *value) {
  Serial.print("{\"");
  Serial.print(key);
  Serial.print("\":\"");
  Serial.print(value);
  Serial.print("\"}\r\n");
 }

 void sendResponse(char *key, int value) {
  Serial.print("{\"");
  Serial.print(key);
  Serial.print("\":\"");
  Serial.print(value);
  Serial.print("\"}\r\n");
 }
 ```

The delay in serial startup on the Nano 33 BLE causes it to miss the command messages when you initially connect. You can get the GUI to repeat these commands by clicking on the download icon in the bottom left of the GUI. The tool tip for this icon is `Read Settings from Device`. 

## Connecting x-IMU3 GUI to your Arduino

Plug you Arduino into a USB port on your PC and work out the name of the port it is connected to. You can see this in the Arduino IDE. Then open up the x-IMU3 GUI app, click on `Connection` (top left) -> `New USB Connection`. In the dialogue box that pops up, select the port with the Arduino connect and click on Connect.

## Using the Reefwing x-IMU3 Library

To utilise the Reefwing xIMU3 GUI Library, you will need to include it at the start of your sketch, and instantiate a new `Reefwing_xIMU3` object. This library uses the `Reefwing_imuTypes` Library, and you will also need it installed.

``` c++
#include <Reefwing_xIMU3.h>

Reefwing_xIMU3 rx;
```

In `setup()`, you have to start Serial and for Arduino boards with a direct USB connection (e.g., the Nano 33 BLE family), you need to wait until there is a connection, otherwise you may miss some of the serial data transmission.

``` c++
void setup() {
  //  Start Serial and wait for connection
  Serial.begin(115200);
  while (!Serial);

  rx.sendNotification("Arduino Connected");
}
```

The code, `while (!Serial)`, is not required for boards with an FT232 chip or other USB to Serial bridge (e.g., Uno, and Mega2560). This is because these boards will reset when they receive a DTR serial command.

The code within `loop()` will depend on your sketch objectives. You will either be sending data messages, responding to commands or a combination of both. Refer to the library examples.

## Library Examples

In the Library examples we have tried to demonstrate the typical applications. 

### Command Hashes ###

This is more of a utility than a example of the x-IMU3 Library. We used it to work out a `HASH_SIZE` that didn't have any collisions for the 31 command keys of interest to us. If you want to add command keys, place them in the `xIMU3Commands[]` array, increase the maximum number of commands constant (`MAX_CMDS`), and run the sketch. You can then tweak `HASH_SIZE` to find a number that doesn't create collisions.

To allow the library to handle your new commands, you will need to update the hash table, and `HASH_SIZE` value, in `xIMU3_Protocol.h`.

### Echo Command ###

This sketch waits until it receives a command from the x-IMU3 GUI, and then sends it back as a notification. The code for echoing commands to the x-IMU3 GUI is straight forward. We call `checkForCommand()` in `loop()`, and then use `sendNotification(cmd)`, to complete the round trip.

``` c++
rx.checkForCommand();
if (rx.newCommand()) {
  char *cmdPtr = rx.getCommand();

  //  Echo Command to x-IMU3 GUI
  rx.sendNotification(cmdPtr);
}
```

### Network Announcement ###

The x-IMU3 GUI expects a connected device to send a network announcement message once per second. This message is a JSON object that contains the information shown below.

``` JSON
{
  "sync": 0,
  "name": "Raven",
  "sn": "0123-4567-89AB-CDEF",
  "ip": "192.168.1.1",
  "port": 7000,
  "send": 8000,
  "receive": 9000,
  "rssi": 100,
  "battery": 100,
  "status": 0
}
```

This sketch demonstrates sending a Network Announcement every second (non-blocking), but for some reason it doesn't update the device settings. We have included it anyway in case someone else can work out what we are doing wrong.

### Parse Command ###

Command messages are a `JSON` object containing a single key/value pair, terminated by the usual `\r\n`. A sample command message, requesting the device serial number looks like:

``` JSON
{"serialNumber":null}\r\n
```

This sketch illustrates one method of handling those commands when received by your Arduino. Most of the hard work is done by the `parseCommand()` function. We have tried to provide a representative sample of the commands you would want to respond to.

You can get a heap of commands sent by clicking on the download icon in the bottom left of the x-IMU3 GUI. The tool tip for this icon is `Read Settings from Device`.

The other way to send commands is from the GUI tool bar. You can send a `shutdown` command, by clicking on the power button icon. The icon next to the power button icon, looks like: `{;}`. You can use this to send standard or custom commands.

Standard commands that are already in the hash table in `xIMU3_Protocol.h`, can be handled by including them in the `parseCommand()` example function. When your Arduino receives a command the GUI expects it to respond by sending back the command key and value sent. You can use the `sendResponse()` methods for this. If you don't respond to the command message, the GUI will send it again, for a total of three attempts.

If you want to send custom message types (i.e., ones not defined by the x-IMU3 API) from the GUI to your Arduino, you will first need to calculate the hash of your new command. You can use the `commandHashes` example sketch for this purpose. To demonstrate this capability, we added the custom command: `blinkLED` to the parseCommand sketch. If you send this command from the GUI with the value true (e.g., `{"blinkLED":true}`), the Arduino will start blinking its built in LED at one second intervals. To turn off blinking, send the custom command:

``` JSON
{"blinkLED":false}
```

All command messages have a key, and most have `null` for the value, but some (e.g., `note` and `heading`) include a string, number or boolean in addition to the key. The Reefwing_xIMU3 library provides three methods to assist with processing the JSON encoded messages. Two of these are used in the `parseCommand` sketch, to extract the command key and value.

``` c++
Reefwing_xIMU3 rx;

char *cmdPtr = rx.getCommand();
char *cmdValue = rx.getValue();
```

The other method that may be useful is `getValueType()`. This will return a ValueType, which indicates what type the value is. The value is stored as a c string, so you may need to convert it to this JSON type, before use. In the example sketch, when handling `blinkLED` we convert the string value to a bool.

``` c++
enum ValueType {
  JSON_STRING = 0,
  JSON_NUMBER,
  JSON_BOOL,
  JSON_NULL,
  JSON_UNDEFINED
};
```

### Stream IMU ###

This sketch demonstrates how to stream real-time IMU data to the x-IMU3 GUI. For the example, we have used our LSM9DS1 Library with an Arduino Nano 33 BLE. The [Reefwing LSM9DS1 Library](https://github.com/Reefwing-Software/Reefwing-LSM9DS1), already includes the [Reefwing-imuTypes Library](https://github.com/Reefwing-Software/Reefwing-imuTypes). The imuTypes Library has the structs and class required to pass data into the serial transmission methods.

If you are using a different IMU Library, then you can include the Reefwing-imuTypes Library, or just declare the structures you need in the sketch. For example, if you want to stream gyroscope and accelerometer data, you need the `InertialMessage` struct.

``` c++
struct InertialMessage {
  float ax, ay, az;
  float gx, gy, gz;
  uint32_t timeStamp;
};
```

If you want to send magnetometer data, then you need the `ScaledData` struct.

``` c++
struct ScaledData {
  float sx, sy, sz;
  uint32_t  timeStamp;
};
```
