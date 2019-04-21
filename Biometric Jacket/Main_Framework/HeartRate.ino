/*

This file contains functions that service the heart rate.

*/

//check the heart rate sensor
void hrt_Routine(){

  irValue = particleSensor.getIR();
  
  if (checkForBeat(irValue) == true) {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
      overTime += beatAvg;
      cnt++;
    }
  }
}



//update temperature readings
void temp_Routine(){
  tempC = particleSensor.readTemperature();
  tempF = particleSensor.readTemperatureF();
}

//based on the average heart rate, we change the up time/down time for the blinking state of the lights (when the lights are blinking at heart rate)
void hrt_Out(){
  if (cnt!= 0)
    hrtRate =  overTime / cnt;
  else
    hrtRate = -1;
    
  if (blinkState == 2){
    if (hrtRate != -1){
      upTime = 60000 / hrtRate * 0.6;
      downTime = 60000 / hrtRate* 0.4;    
    }
    else {
      upTime = 1000;
      downTime = 0;
    }
  }
  
  overTime = 0;
  cnt = 0;
}
