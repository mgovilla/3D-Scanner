#define MOTOR_A_ENABLE_PIN 7
#define MOTOR_A_STEP_PIN 2
#define MOTOR_A_DIR_PIN 5
#define MOTOR_B_ENABLE_PIN 8
#define MOTOR_B_STEP_PIN 3
#define MOTOR_B_DIR_PIN 6
#define pinCS 10
#define rotations 10
#define THREADS_PER_IN 16

#include <Wire.h>
#include <AccelStepper.h>
#include <VL53L0X.h>
#include <SD.h>


VL53L0X sensor;
AccelStepper motorA(1, MOTOR_A_STEP_PIN, MOTOR_A_DIR_PIN);
AccelStepper motorB(1, MOTOR_B_STEP_PIN, MOTOR_B_DIR_PIN);
File myFile;
bool scanning = true;


void setup()
{
  motorA.setEnablePin(MOTOR_A_ENABLE_PIN);
  motorA.setPinsInverted(false, false, true);

  motorB.setEnablePin(MOTOR_B_ENABLE_PIN);
  motorB.setPinsInverted(false, false, true);
  
  motorA.setAcceleration(20);
  motorA.setMaxSpeed(120);
  
  motorB.setAcceleration(20);
  motorB.setMaxSpeed(120);
  
  pinMode(MOTOR_A_ENABLE_PIN, OUTPUT);
  pinMode(MOTOR_B_ENABLE_PIN, OUTPUT);
  motorA.enableOutputs();
  motorB.enableOutputs();

  Serial.begin(9600);
  Wire.begin();

  sensor.init();
  sensor.setTimeout(500);

  sensor.setMeasurementTimingBudget(200000);

  pinMode(pinCS, OUTPUT);
  
  // SD Card Initialization
  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
  
  // Create/Open file 
  myFile = SD.open("test.txt", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.println("Writing to file...");
    // Write to file
    myFile.println("Testing text 1, 2 ,3...");
    myFile.close(); // close the file
    Serial.println("Done.");
  }
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening test.txt");
  }
}

void loop()
{
  if(scanning) {
    
    for(int i = 1; i < rotations; i++) {
      
      for(int r = 1; r < 200; r++) {
        motorA.move(1.0);
        motorA.run();
        
        myFile.print(r*1.8);
        myFile.print(',');
        myFile.print(sensor.readRangeSingleMillimeters());
        if (sensor.timeoutOccurred()) { myFile.print(0.0); }
        myFile.print(toZ(i));
        myFile.print(',');
        myFile.println();
        
      }
      
      motorB.move(20);
      motorB.run();
    }

    scanning = false;
    
  } else {

    myFile.close();
    motorA.stop();
    motorB.stop();
    
  }

}

double toZ(int steps) {
  return steps*1.8*THREADS_PER_IN;
}
