// The following is a simple stepper motor control procedures
# define EN 8 // stepper motor enable , active low
# define X_DIR 5 // theta -axis stepper motor direction control
# define Y_DIR 6 // z -axis stepper motor direction control
# define X_STP 2 // theta -axis stepper control
# define Y_STP 3 // z -axis stepper control
# define rotations 10 // Max rotations to reach Z height

#include <Wire.h>
#include <SPI.h>
#include<SD.h>
#include <VL53L0X.h>

VL53L0X sensor;
bool scanning = true;
File ScanData;

void setup () {   // The stepper motor used in the IO pin is set to output
    Serial.begin(9600);
    Wire.begin();

    pinMode (X_DIR, OUTPUT); pinMode (X_STP, OUTPUT);
    pinMode (Y_DIR, OUTPUT); pinMode (Y_STP, OUTPUT);
    pinMode (EN, OUTPUT);
    digitalWrite (EN, LOW);

    sensor.init();
    sensor.setTimeout(500);
    sensor.setMeasurementTimingBudget(200000);

    if (!SD.begin(4)) {
      Serial.println("initialization failed!");
      scanning = false;
    }
    
    if(scanning) {
       ScanData = SD.open("Scan.txt", FILE_WRITE);
       if(ScanData) {
          ScanData.println("Heading");
       }
    }
}

void loop () {
    if(scanning) {
    
      for(int i = 0; i < 3; i++) {
        for(int r = 1; r < 10; r++) {      // 1 step is 1.8 degrees, 200 for a full revolution before z axis moves
          step (false, X_DIR, X_STP, 1.0); // Move that 1 step (subject to change) 
          
          ScanData.print(r*1.8);            // Theta (in degrees)
          ScanData.print(',');
          ScanData.print(sensor.readRangeSingleMillimeters());
          if (sensor.timeoutOccurred()) { ScanData.print(0.0); }    // R (in mm)
          ScanData.print(',');
          ScanData.print((i*1.8)/16.0); // Z (in inches) Must be changed based on Threads per inch of actual threaded rod, and to make units consistent
          ScanData.println();

          delayMicroseconds(750);
          
        }
        
        step (false, Y_DIR, Y_STP, 25); //Calibrate value to move specific distance
      }
  
      scanning = false;
    
  } else {
     if(ScanData) {
        ScanData.close();
     }
  }
} 


/*
 * Function : step . function: to control the direction of the stepper motor , the number of steps .
 * Parameters : dir direction control , dirPin corresponding stepper motor DIR pin , stepperPin corresponding stepper motor " step " pin , Step number of step of no return value.
 * 
 */

void step (boolean dir, byte dirPin, byte stepperPin, int steps)  
{
    digitalWrite (dirPin, dir);
    delay (50);
    for (int i = 0; i < steps; i++) 
    {
        digitalWrite (stepperPin, HIGH);
        delayMicroseconds (800);
        digitalWrite (stepperPin, LOW);
        delayMicroseconds (800);
    }
}
