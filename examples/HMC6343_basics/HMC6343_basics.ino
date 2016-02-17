/******************************************************************************
HMC6343_basics.ino
Simple example for using the HMC6343 3-axis compass library.
Jordan McConnell @ SparkFun Electronics
17 July 2014
https://github.com/sparkfun/SparkFun_HMC6343_Arduino_Library

This example declares an SFE_HMC6343 object called compass. The object/sensor
is initialized and if the initialization fails, the sensor could not
be found and read from successfully.

Each time through the loop, heading values (heading, pitch, and roll) and
accelerometer values (accelX, accelY, accelZ) are read from the sensor. They
are then printed to the Serial Monitor at 115200 baud. The raw sensor values 
are printed as well as scaled values in readable units (degrees and g forces).

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
  // Read, calculate, and print the heading, pitch, and roll from the sensor
  compass.readHeading();
  printHeadingData();
  
  // Read, calculate, and print the acceleration on the x, y, and z axis of the sensor
  compass.readAccel();
  printAccelData();
  
  // Wait for two seconds
  delay(2000); // Minimum delay of 200ms (HMC6343 has 5Hz sensor reads/calculations)
}

// Print both the raw values of the compass heading, pitch, and roll
// as well as calculate and print the compass values in degrees
// Sample Output:
// Heading Data (Raw value, in degrees):
// Heading: 3249  324.90°
// Pitch:   28    2.80°
// Roll:    24    2.40°
void printHeadingData()
{
  Serial.println("Heading Data (Raw value, in degrees):");
  Serial.print("Heading: ");
  Serial.print(compass.heading); Serial.print("  "); // Print raw heading value
  Serial.print((float) compass.heading/10.0);Serial.write(176);Serial.println(); // Print heading in degrees
  Serial.print("Pitch: ");
  Serial.print(compass.pitch); Serial.print("  ");
  Serial.print((float) compass.pitch/10.0);Serial.write(176);Serial.println();
  Serial.print("Roll: ");
  Serial.print(compass.roll); Serial.print("  ");
  Serial.print((float) compass.roll/10.0);Serial.write(176);Serial.println();
  Serial.println();
}

// Print both the raw values of the compass acceleration measured on each axis
// as well as calculate and print the accelerations in g forces
// Sample Output:
// Accelerometer Data (Raw value, in g forces):
// X: -52    -0.05g
// Y: -44    -0.04g
// Z: -1047  -1.02g
void printAccelData()
{
  Serial.println("Accelerometer Data (Raw value, in g forces):");
  Serial.print("X: ");
  Serial.print(compass.accelX); Serial.print("  "); // Print raw acceleration measurement on x axis
  Serial.print((float) compass.accelX/1024.0);Serial.println("g"); // Print x axis acceleration measurement in g forces
  Serial.print("Y: ");
  Serial.print(compass.accelY); Serial.print("  ");
  Serial.print((float) compass.accelY/1024.0);Serial.println("g");
  Serial.print("Z: ");
  Serial.print(compass.accelZ); Serial.print("  ");
  Serial.print((float) compass.accelZ/1024.0);Serial.println("g");
  Serial.println();
}
