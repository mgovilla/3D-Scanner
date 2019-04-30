#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMSbot; // Rightmost jumper closed

// Connect two steppers with 200 steps per revolution (1.8 degree) to the top shield
Adafruit_StepperMotor *myStepper1 = AFMSbot.getStepper(200, 1);
Adafruit_StepperMotor *myStepper2 = AFMSbot.getStepper(200, 2);


// wrappers for the first motor!
void forwardstep1() {myStepper1->onestep(FORWARD, DOUBLE);}
void backwardstep1() {myStepper1->onestep(BACKWARD, DOUBLE);}

// wrappers for the second motor!
void forwardstep2() {myStepper2->onestep(FORWARD, DOUBLE);}
void backwardstep2() {myStepper2->onestep(BACKWARD, DOUBLE);}


// Now we'll wrap the 2 steppers in an AccelStepper object
AccelStepper stepper1(forwardstep1, backwardstep1);
AccelStepper stepper2(forwardstep2, backwardstep2);

void setup()
{  
  AFMSbot.begin(); // Start the bottom shield
   
  stepper1.setSpeed(200.0);  //setMaxSpeed
  stepper1.setAcceleration(25.0);
  stepper1.moveTo(10000); 
  
  stepper2.setSpeed(100.0);
  stepper2.setAcceleration(125.0);
  stepper2.moveTo(10000);
}

void loop()
{
    stepper1.run();
    stepper2.run();
}
