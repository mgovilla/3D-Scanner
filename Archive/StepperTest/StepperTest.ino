#define MOTOR_A_ENABLE_PIN 8
#define MOTOR_A_STEP_PIN 2
#define MOTOR_A_DIR_PIN 5

#define MOTOR_Y_STEP_PIN 3
#define MOTOR_Y_DIR_PIN 6

#define rotations 10

#include <AccelStepper.h>
#include <Wire.h>

AccelStepper motorA(1, MOTOR_A_STEP_PIN, MOTOR_A_DIR_PIN);
AccelStepper motorY(1, MOTOR_Y_STEP_PIN, MOTOR_Y_DIR_PIN);

bool scanning = true;

void setup()
{

  Serial.begin(9600);
  
  motorA.setEnablePin(MOTOR_A_ENABLE_PIN);
  motorA.setPinsInverted(false, false, true);

   
  motorA.setMaxSpeed(170);
  
  motorY.setEnablePin(MOTOR_A_ENABLE_PIN);
  motorY.setPinsInverted(false, false, true);

  motorY.setMaxSpeed(170);
  
  pinMode(MOTOR_A_ENABLE_PIN, OUTPUT);
  motorA.enableOutputs();
  motorY.enableOutputs();

}

void loop()
{
  
  if(scanning) {
    
    for(int i = 1; i < rotations; i++) {
      
      for(int r = 1; r < 200; r++) {
        motorA.move(5);
        motorA.run();
        
        Serial.print(r*1.8);
        Serial.print(',');
        Serial.print(0.0);
        Serial.print(',');
        Serial.print(toZ(i));
        Serial.println();
        
      }
      
      motorY.move(20);
      motorY.run();
    }

    scanning = false;
    
  } else {

    motorA.stop();
    motorY.stop();
    
  }

}

double toZ(int steps) {
  return steps*1.8*16;
}
