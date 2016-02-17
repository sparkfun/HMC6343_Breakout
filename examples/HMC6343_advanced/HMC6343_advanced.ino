/******************************************************************************
HMC6343_advanced.ino
Example to demo the extended features of the HMC6343 3-axis compass library.
Jordan McConnell @ SparkFun Electronics
17 July 2014
https://github.com/sparkfun/SparkFun_HMC6343_Arduino_Library

This example declares an SFE_HMC6343 object called compass. The object/sensor
is initialized and if the initialization fails, the sensor could not
be found and read from successfully.

Each time through the main loop(), the following features are demo'ed:

Reading/printing raw 3-axis magnetometer values
Reading/printing raw 'tilt' values (pitch, roll, and temperature)
Entering and exiting standby mode (4.5mA draw in run mode, 1.0mA in standby)
Entering and exiting sleep mode   (10uA draw in sleep mode)
Setting the physical orientation of the HMC6343 IC (which affects heading/tilt readings)
Reading the primary status register, OPMode1
Reading and writing the EEPROM registers of the sensor
Resetting the processor of the HMC6343 (commented out by default)
Entering and exiting user calibration mode (commented out by default)

The HMC6343 breakout board needs to be powered with 3.3V and uses I2C to talk
to the microcontroller. If you're using a 5V microcontroller board, such as 
the standard Arduino UNO, you'll need a Logic Level Converter for the I2C lines,
such as SparkFun's BOB-12009.

Developed/Tested with:
Arduino Uno
Arduino IDE 1.0.5 & 1.5.2

Requires:
SFE_HMC6343_Library

This code is beerware.
Distributed as-is; no warranty is given. 
******************************************************************************/

// Libraries for I2C and the HMC6343 sensor
#include <Wire.h>
#include "SFE_HMC6343.h"

SFE_HMC6343 compass; // Declare the sensor object

void setup()
{
  // Start serial communication at 115200 baud
  Serial.begin(115200);
  
  // Give the HMC6343 a half second to wake up
  delay(500); 
  
  // Initialize the HMC6343 and verify its physical presence
  if (!compass.init())
  {
    Serial.println("Sensor Initialization Failed\n\r"); // Report failure, is the sensor wiring correct?
  }
}

void loop()
{
  // Read and print the raw magnetometer values for the x, y, and z axis of the sensor
  compass.readMag();
  printMagData();
  
  // Read and print the raw tilt values (pitch, roll, and temperature) of the sensor
  compass.readTilt();
  printTiltData();
  
  
  // Enter and exit standby mode (4.5mA draw in run mode, 1.0mA in standby)
  // HMC6343 requires 1 ms before it can receive commands after switching modes
  compass.enterStandby();
  delay(1);
  compass.exitStandby(); // Exit standby, enter run mode (default)
  delay(1);
  
  // Enter and exit sleep mode (4.5mA draw in run mode, 10uA in sleep)
  // HMC6343 requires 1ms after entering sleep and 20ms after exiting before it's able to receive new commmands
  compass.enterSleep();
  delay(1);
  compass.exitSleep();
  delay(20);
  
  // Set orientation of the HMC6343 IC so heading, pitch, and roll calculations are correct
  // Possible orientations:
  // LEVEL      X = forward, +Z = up (default)
  // SIDEWAYS   X = forward, +Y = up
  // FLATFRONT  Z = forward, -X = up
  compass.setOrientation(LEVEL);
  delay(1);  // 1 ms before sensor can receive commands after setting orientation
  
  // Read the OPMode1 status register of the HMC6343
  // The register informs you of current calculation status, filter status, modes enabled and the orientation selected
  // Refer to the HMC6343 datasheet for bit specifics
  byte opmode1 = compass.readOPMode1();
  Serial.print("OP Mode 1: ");
  Serial.println(opmode1,BIN);
  
  // Read and print the Serial Number of the HMC6343 IC from its EEPROM
  // EEPROM register list can be found in SFE_HMC6343.h or the datasheet
  // EEPROM reads require 10ms afterward before the sensor is able to receive another command
  int sn = compass.readEEPROM(SN_LSB);
  delay(10);
  sn |= (compass.readEEPROM(SN_MSB) << 8);
  delay(10);
  Serial.print("HMC6343 IC Serial Number: ");
  Serial.println(sn,HEX);
  Serial.println();
  
  // Write the filter value (used for weighted averages of sensor reads) to the EEPROM of the HMC6343
  // FILTER_LSB register can be set between 0x00 and 0x0F
  // It averages the last X sensor readings (filter register value) with the latest reading
  // Example: if set to 4 (instead of default 0), it would average the last 4 readings with the latest reading 
  //  for sensor values such as heading with the last 4 for a total of a second average (5Hz)
  compass.writeEEPROM(FILTER_LSB, 0); // 0x00-0x0F, default is 0x00
  delay(10); // 10ms before sensor can accept commands after writing to the EEPROM
  // To actually enable the filter, the filter bit must be set in OPMode1 register: 0bxx1xxxxx
  //compass.writeEEPROM(OP_MODE1, 0x31); // Enable filter (default OPMODE1 + filter enable bit set)
  //compass.writeEEPROM(OP_MODE1, 0x11); // Default, filter disabled (0bxx010001)
  //delay(10);
  
  /*
  // Reset the processor of the HMC6343
  // Sensor requires 500ms before it can receive more commands after a reset
  compass.reset();
  delay(500);
  */
  
  /*
  // Enter and exit user calibration mode
  // Refer to datasheet for further instructions of how to use this mode
  // Requires 1ms after entering and 50ms after exiting calibration before sensor can receive new commands
  compass.enterCalMode();
  delay(1);
  compass.exitCalMode();
  delay(50);
  */
  
  delay(2000); // Wait an arbitrary amount of seconds
}

// Print the raw magnetometer values for the x, y, and z axis of the sensor
void printMagData()
{
  Serial.println("Raw Magnetometer Data:");
  Serial.print("X: ");
  Serial.println(compass.magX);
  Serial.print("Y: ");
  Serial.println(compass.magY);
  Serial.print("Z: ");
  Serial.println(compass.magZ);
  Serial.println();
}

// Print the raw tilt values (pitch, roll, and temperature) of the sensor
void printTiltData()
{
  Serial.println("Raw Tilt Data:");
  Serial.print("Pitch: ");
  Serial.println(compass.pitch);
  Serial.print("Roll: ");
  Serial.println(compass.roll);
  Serial.print("Temperature: ");
  Serial.println(compass.temperature);
  Serial.println();
}
