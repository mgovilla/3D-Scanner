# 3D-Scanner

This is a project to create a 3D scanner. Heavily inspired from https://www.youtube.com/watch?v=-qeD2__yK4c.

## Hardware
The main body and various components were all designed in Solidworks and 3D printed. The motors, steel rods, threaded rod, an carriage nut were sourced from a broken 3D printer. The design looks like so:

![Scanner CAD](/images/ScannerCAD.PNG)

## Electronics
The brain of the system is the Arduino UNO running a state machine (details in Software section) which has a CNC shield on it that houses 2 A4988 stepper motor drivers. Since all of the electronics pull significant current, it is powered by a 12v 5a wall power supply. For now, the Arduino is powered through USB, but in the future I plan to power it using a buck converter running parallel from the power supply in the future. 

The sensor I'm using is a Adafruit VL6180X ToF sensor. I wanted to experiment with a new kind of sensor, and I found that it is pretty reliable and accurate, more so than cheap ultrasonic sensors I have tried before. 

To store the values from the sensor during scans, I used a microSD card arduino module and the SD library in arduino. There is also an HC-05 for communication with the desktop program (details in Software section). 

## Software
There are two main components to the code: arduino program and the desktop program.

The Arduino is running a state machine with states for scanning, waiting, calibrating, and jogging:
* Scanning: Perform the scan (rotate the object and store point values)
* Calibrating: Set 0 position of the sensor
* Jogging: Manually move the stepper motors by some increments
* Waiting: Idle while waiting for commands

The desktop program is the interface to send commands to the arduino through Serial communication along a Bluetooth connection. The GUI for the program was created using PAGE which generated the boilerplate code, and then functionality was added with python. 

![Desktop GUI](/images/DesktopGUI.PNG)
