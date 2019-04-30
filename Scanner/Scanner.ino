// The following is a simple stepper motor control procedures
# define EN 8 // stepper motor enable , active low
# define X_DIR 5 // theta -axis stepper motor direction control
# define Y_DIR 6 // z -axis stepper motor direction control
# define X_STP 2 // theta -axis stepper control
# define Y_STP 3 // z -axis stepper control
# define rotations 10

/*
// Function : step . function: to control the direction of the stepper motor , the number of steps .
// Parameters : dir direction control , dirPin corresponding stepper motor DIR pin , stepperPin corresponding stepper motor " step " pin , Step number of step of no return value.
*/

#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;
bool scanning = true;

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
   
}

void loop () {
    if(scanning) {
    
      for(int i = 0; i < rotations; i++) {
        
        for(int r = 1; r < 2; r++) {
          step (false, X_DIR, X_STP, 10.0);
          
          Serial.print(r*1.8);
          Serial.print(',');
          Serial.print(sensor.readRangeSingleMillimeters());
          if (sensor.timeoutOccurred()) { Serial.print(0.0); }
          Serial.print(',');
          Serial.print((i*1.8)/16.0);
          Serial.println();

          delayMicroseconds(750);
          
        }
        
        step (false, Y_DIR, Y_STP, 25);
      }
  
      scanning = false;
    
  } 
} 


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
