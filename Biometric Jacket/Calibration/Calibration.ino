/*
This is the Accelerometer Threshold Calibration program written for the Biometric Jacket created by team 
"It Worked Yesterday, I Swear" from Rice University, as part of the ELEC-220 course.

This code is run first before running the Main Framework.

To determine the proper threshold value for the accelerometer, do the following:
1. Put on the jacket and turn it on. 
2. With the current threshold, take 20 steps; 
  if the value displayed on the step count is more than 20, increase the threshold value
  if the value displayed on the step count is less than 20, decrease the threshold value
3. Repeat until the number of steps you make matches that displayed on the screen.


Code Written By:
  Savva Morozov

Other Contributors:
  Bobby Bao
  Sam Ramirez
  
*/




/*CONNECTIONS:

pin 9 - increase the threshold button pin
pin 10 - decrease the threshold button pin

A4 - SDA
A5 - SCL

5V
GND

*/


#include <Wire.h>
#include <EEPROM.h>
#include <MPU6050_tockn.h>

int checkButton = 0;



// ------------------------------------------DISPLAY

#include <Adafruit_GFX.h>        //OLED libraries
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET    -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Declaring the display name (display)


int displayRefreshRate = 2000; //in ms
unsigned long tOfLastDisplay = 0;
// ------------------------------------------ACCELEROMETER
MPU6050 mpu6050(Wire);

byte threshUpPin = 9;
byte lu; byte u;

byte threshDownPin = 10;
byte ld; byte d;

byte thresholdAddress = 57;
float threshold = 0.45;

float xval = 0;
float yval = 0;
float zval = 0;


float totvect = 0;
float totvect_last = 0;
float totave = 0;
float xaccl = 0;
float yaccl = 0;
float zaccl = 0;

float xavg, yavg, zavg;

int flag = 0;
int stepCount = 0;

unsigned long totalcheck = 0;


void setup()
{
  Wire.begin();
  //----------------------------------TURN ON THE DISPLAY
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Start the OLED display
  display.display();
  delay(3000);
  display.clearDisplay();
  drawDisplay();
  tOfLastDisplay = millis();

  pinMode(threshUpPin, INPUT);
  pinMode(threshDownPin, INPUT);
  // ------------------------------------------INITIALIZING ACCELEROMETER
  mpu6050.begin();
  calibrate();
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.write(thresholdAddress, threshold);   //update eeprom
  display_Routine();
  tOfLastDisplay = millis();
}





void loop()
{
  routineStuff();

  //do something if the button was pressed
  if ( checkButton != 0 ){
    //increase the threshold button was pressed
    if (checkButton == 1)
      threshold += 0.05;
    // decrease the threshold button was pressed
    else if (checkButton == 2)
      threshold-=0.05;
    resetSteps();
    EEPROM.write(thresholdAddress, threshold);   //update eeprom
    display_Routine();                           //update the display
  }
}



// fetch all routinely collected data
void routineStuff(){
  imu_Routine();
  button_Routine();
  checkButton = buttonPressed();
  display_Routine();
}
