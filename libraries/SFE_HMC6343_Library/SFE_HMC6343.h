/******************************************************************************
SFE_HMC6343.h
Core header file for the HMC6343 3-axis compass library.
Jordan McConnell @ SparkFun Electronics
17 July 2014
https://github.com/sparkfun/HMC6343_Breakout

This header file declares the SFE_HMC6343 sensor class as well as its various
functions and variables. It also defines sensor specifics including register
addresses and sensor commands.

Developed/Tested with:
Arduino Uno
Arduino IDE 1.0.5 & 1.5.2

This code is beerware; if you see me (or any other SparkFun employee) at the 
local, and you've found our code helpful, please buy us a round!
Distributed as-is; no warranty is given. 
******************************************************************************/

#ifndef SFE_HMC6343_h
#define SFE_HMC6343_h

#include "Arduino.h"
#include "Wire.h"

// HMC6343 I2C Address (0x32 >> 1 = 0x19)
#define HMC6343_I2C_ADDR 0x19

// HMC6343 Registers
#define SLAVE_ADDR 0x00
#define SW_VERSION 0x02
#define OP_MODE1 0x04
#define OP_MODE2 0x05
#define SN_LSB 0x06
#define SN_MSB 0x07
#define DATE_CODE_YY 0x08 
#define DATE_CODE_WW 0x09 
#define DEVIATION_LSB 0x0A
#define DEVIATION_MSB 0x0B
#define VARIATION_LSB 0x0C
#define VARIATION_MSB 0x0D
#define XOFFSET_LSB 0x0E
#define XOFFSET_MSB 0x0F
#define YOFFSET_LSB 0x10
#define YOFFSET_MSB 0x11
#define ZOFFSET_LSB 0x12
#define ZOFFSET_MSB 0x13
#define FILTER_LSB 0x14
#define FILTER_MSB 0x15

// HMC6343 Commands
#define POST_ACCEL 0x40
#define POST_MAG 0x45
#define POST_HEADING 0x50
#define POST_TILT 0x55
#define POST_OPMODE1 0x65
#define ENTER_CAL 0x71
#define ORIENT_LEVEL 0x72
#define ORIENT_SIDEWAYS 0x73
#define ORIENT_FLATFRONT 0x74
#define ENTER_RUN 0x75
#define ENTER_STANDBY 0x76
#define EXIT_CAL 0x7E
#define RESET 0x82
#define ENTER_SLEEP 0x83
#define EXIT_SLEEP 0x84
#define READ_EEPROM 0xE1
#define WRITE_EEPROM 0xF1

// HMC6343 Orientations
#define LEVEL 0     // X = forward, +Z = up (default)
#define SIDEWAYS 1  // X = forward, +Y = up
#define FLATFRONT 2 // Z = forward, -X = up

class SFE_HMC6343
{
 public:
  SFE_HMC6343();
  ~SFE_HMC6343();
  
  int heading, pitch, roll;
  int magX, magY, magZ;
  int accelX, accelY, accelZ;
  int temperature;
  
  bool init();
  
  void readMag();
  void readAccel();
  void readHeading();
  void readTilt();

  void enterStandby();
  void exitStandby();
  
  void enterSleep();
  void exitSleep();
  
  void enterCalMode();
  void exitCalMode();
  
  void setOrientation(uint8_t orientation);
  
  void reset();
  
  uint8_t readOPMode1();
  
  uint8_t readEEPROM(uint8_t reg);
  void writeEEPROM(uint8_t reg, uint8_t data);

 private:
  uint8_t _addr;
  
  uint8_t rawData[6];
  void clearRawData();
  
  void sendCommand(uint8_t command);
  void readGeneric(uint8_t command, int* first, int* second, int* third);
};

#endif