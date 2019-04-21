/*

This file contains functions that service the lights.

*/

//changes the light state
void switchLightState(){
  blinkState++;
  blinkState %=3;
  actOnLightState();
}



//if the light state was changed, that's how we intialize the lights
void actOnLightState(){
  
  if (blinkState == 0 || blinkState == 1)
    switchLights();
    
  else
    tOfLastBlink = millis();
    
}



//this is what we do if the lights are blinking at your heart rate. 
void blinking(){
  if (blinkState == 2){
    //if the average heart rate is -1 - that is, the heart rate sensor isn't recording anything, just turn the lights on
    if (hrtRate == -1){
      turnLightsOn();
      return;
    }

    //if the lights are on and the uptime is over - turn them off
    if ( lightsOn && tOfLastBlink >= upTime ){
       switchLights();
       tOfLastBlink = millis();
    }
    //if the lights are off and the downtime is over - turn them on
    else if (  !lightsOn && tOfLastBlink >= downTime ){
      switchLights();
      tOfLastBlink = millis();
    }
  }
}



//chagne currrent light state: from off to on, or from off to on
void switchLights(){
  if (lightsOn)
    turnLightsOff();
  else
    turnLightsOn();
}


void turnLightsOn(){
  lightsOn = true;
  digitalWrite(lightMosfetPin, HIGH);
}


void turnLightsOff(){
  lightsOn = false;
  digitalWrite(lightMosfetPin, LOW);
}
