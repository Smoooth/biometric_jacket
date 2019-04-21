/*
This is the State Machine framework written for the Biometric Jacket created by team 
"It Worked Yesterday, I Swear" from Rice University, as part of the ELEC-220 course.

The purpose of the code is to effeciently collect the data from each individual sensor, 
agregate it, and display it on the OLED screen on the wrist. It also operates the EL wires and buttons.

This is the Main Framework of the jacket. Make sure to runthe calibration first.

Code Written by:
  Savva Morozov

Other contributors:
  Bobby Bao
  Sam Ramirez
  
*/


/*CONNECTIONS:

pin 9 - step count reset pin
pin 10 - light state switch bin
pin 11 - light mosfet control pin

A4 - SDA
A5 - SCL

*/

//importing libraries
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <MPU6050_tockn.h>


int checkButton = 0; // flag for checking buttons


//--------------------------------------------HEART RATE SENSOR VARIABLES
MAX30105 particleSensor; //that's the sensor itself
const byte RATE_SIZE = 4; //some averaging
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
unsigned long lastBeat = 0; //Time at which the last beat occurred
long irValue = 0;           // value red from IR sensor
float beatsPerMinute;
int beatAvg;
//temperatures
int tempC = 0;
int tempF = 0;

unsigned long overTime = 0;
unsigned long cnt = 0;        //count of heartbeats for averaging
int hrtRate;                  



//--------------------------------------------LIGHTS
int blinkState = 0;           // state of the lights. 0 - the lights are off, 1 - lights are on; 2 - blink with rate of the heart rate mode
byte lightMosfetPin = 11;     //output mosfet switch for siwtching lihgts on/off
byte lightSwitchPin = 10;     //input pin to a button that controls current blink state

byte ls; byte s;  //lights buttons state. these are flags for the buttons

boolean lightsOn = false;       // true if lights are on, false if lights are off
unsigned long tOfLastBlink;     //variables I use to blink the lights when i'm in state 2
int upTime;                     
int downTime;

unsigned long tOfLastDisplay;


// ------------------------------------------DISPLAY
//bunch of more libraries
#include <Adafruit_GFX.h>        //OLED libraries
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET    -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Declaring the display name (display)


int displayRefreshRate = 2000; //in ms



// ------------------------------------------ACCELEROMETER
MPU6050 mpu6050(Wire);

byte stepResetPin = 9;    //input pin for the button that resets your step count
byte lr; byte r;          // flag for the step reset button

#include <EEPROM.h>
byte thresholdAddress = 57;
float threshhold = 0.5;    // threshold value for the step counter. Higher threshold - more movement needd to trigger a step

//some values needed to do math to get the pedometer working
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

  pinMode(stepResetPin, INPUT);
  
  particleSensor.begin(Wire, I2C_SPEED_FAST);

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
  particleSensor.enableDIETEMPRDY();
  
  // ------------------------------------------INITIALIZING LIGHTS
  pinMode(lightMosfetPin, OUTPUT);
  pinMode(lightSwitchPin, INPUT);
  blinkState = 0;
  digitalWrite(lightMosfetPin, LOW);
  // ------------------------------------------INITIALIZING ACCELEROMETER

  //assuming that you first ran the calibration code, this would fetch the threshold value from EEPROM 
//  threshold = EEPROM.read(thresholdAddress);
  
  mpu6050.begin();
  calibrate(); // a bit more calibration 
}





void loop()
{
  //copmlete the routines - take inputs from the sensors, etc
  routineStuff();
  //so something if the button was pressed
  if ( checkButton != 0 ){
    //lights button pressed
    if (checkButton == 1)
      switchLightState();
    //step reset request
    if (checkButton == 2)
      resetSteps();
    hrt_Routine();
    display_Routine();
  }
}



// fetch all routinely collected data
//the reason why heart rate is collected so frequently is to ensure that we don't miss any heart bits. 
//It's probably an overkill, but it's like myt father told me - it's better to be safe than sorry.

void routineStuff(){
  hrt_Routine();
  imu_Routine();
  hrt_Routine();
  button_Routine();
  hrt_Routine();
  checkButton = buttonPressed();
  hrt_Routine();
  if (blinkState == 2)
    blinking();
  hrt_Routine();
  display_Routine();
  hrt_Routine();
}
