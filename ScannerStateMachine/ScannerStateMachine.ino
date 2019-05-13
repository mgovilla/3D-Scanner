// The following is a simple stepper motor control procedures
# define EN 8 // stepper motor enable , active low
# define X_DIR 5 // theta -axis stepper motor direction control
# define Y_DIR 6 // z -axis stepper motor direction control
# define X_STP 2 // theta -axis stepper control
# define Y_STP 3 // z -axis stepper control
# define rotations 10 // Max rotations to reach Z height

#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <VL53L0X.h>

VL53L0X distanceSensor;

File ScanData;
enum States{WAIT, SLEEP, SCANNING, CALIBRATE, JOG};
enum States state = WAIT;

int THREADS_INCH = 16.0;
int FileCount = 0;

double offset = 0;

void setup () {   // The stepper motor used in the IO pin is set to output
    Serial.begin(9600);
    Wire.begin();

    pinMode (X_DIR, OUTPUT); pinMode (X_STP, OUTPUT);
    pinMode (Y_DIR, OUTPUT); pinMode (Y_STP, OUTPUT);
    pinMode (EN, OUTPUT);
}

void loop () {
    RunStateMachine(state);
} 


void RunStateMachine(uint8_t s) {
  switch(s) {
    case SCANNING:
      FileCount++;
      String fileName;
      fileName = ("ScanData%d.txt", FileCount);
      ScanData = SD.open(fileName);
      if(ScanData) {
         ScanData.println("Heading");
      }

      Scan();
      state = WAIT;
      break;
    case CALIBRATE:
      // Change constants (offset, TPI) here, not sure if necessary
      
      break;
    case JOG:
      // Manually move motors here with user inputs until cancelled, or timer runs out
      digitalWrite (EN, LOW);
      
      if(Serial.available() > 0) {
        int inByte = Serial.read(); 
      
        switch(inByte) {
          case 'a':
            step (false, X_DIR, X_STP, 10);
            break;
          case 'd':
            step (true, X_DIR, X_STP, 10);
            break;
          case 'w':
            step (false, Y_DIR, Y_STP, 10);
            break;
          case 's':
            step (true, Y_DIR, Y_STP, 10);
            break;
          case 'c':
            state = WAIT;
            break;
          default:
            break;  
        }
      }
      
      break;
    case WAIT:
      // Code for menu items here, transitions to Scan, Calibrate, and Jog
      if(Serial.available() > 0) {
        int inByte = Serial.read(); 
      
        switch(inByte) {
          case 'c':
            state = CALIBRATE;
            break;
          case 'j':
            state = JOG;
            break;
          case 's':
            state = SLEEP;
            break;
          case 'r':
            state = SCANNING;
            break;
          default:
            state = WAIT;
            break;
        }
      }
        
      break;
    case SLEEP:
      // Idle motors and shutoff Arduino/Display
      digitalWrite (EN, HIGH);

      if(Serial.available() > 0) {
        state = WAIT;
      }
      break;
    default: // Debug
      Serial.println("Error Occured in State Machine");
      break;
  }
}

/*
 * Function : Scan . function: Run for loops to gain every data point. Stops if requested, the sensor sees nothing for some time, or max height is reached. Saves to file f
 * Parameters : dir direction control , dirPin corresponding stepper motor DIR pin , stepperPin corresponding stepper motor " step " pin , Step number of step of no return value.
 * 
 */
void Scan () {
  distanceSensor.init();
  distanceSensor.setTimeout(500);
  distanceSensor.setMeasurementTimingBudget(200000);

  digitalWrite (EN, LOW);
  
  for(int i = 0; i < 3; i++) {
        for(int j = 1; j < 10; j++) {      // 1 step is 1.8 degrees, 200 for a full revolution before z axis moves
          step (false, X_DIR, X_STP, 1.0); // Move that 1 step (subject to change) 
          
          double scanDist; 
          scanDist = distanceSensor.readRangeSingleMillimeters() - offset;
          if (distanceSensor.timeoutOccurred()) { scanDist = 0.0; } 
          
          printToSD(j, scanDist, i);
          delayMicroseconds(750);
        }
        
  step (false, Y_DIR, Y_STP, 25); //Calibrate value to move specific distance
  }
}

/*
 * Function : PrintToSD . function: Calculates xyz point based on the for loop values, 
 * Parameters : theta = angle of the plate, r = distance from sensor, 
 * 
 */
void printToSD (double theta, double r, double z) {
          theta = theta * 1.8;
          z = (z/200.0) / (THREADS_INCH);
          double x = r*cos(theta);
          double y = r*sin(theta);
          
          ScanData.print(x);            
          ScanData.print(',');
          ScanData.print(y);
          ScanData.print(',');
          ScanData.print(z); // Z (in inches) Must be changed based on Threads per inch of actual threaded rod, and to make units consistent
          ScanData.println();
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
