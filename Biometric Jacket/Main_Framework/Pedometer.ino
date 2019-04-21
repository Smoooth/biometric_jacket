/*

This file contains functions that service the pedometer.

*/

//check if we made a step. Contains a bunch of math that checks the accleration, and if the acceleration is higher than a certain threshold - count it as a step
void imu_Routine(){
  unsigned long delay120 = millis();
  unsigned long totalcheck = millis();
  int acc = 0;
  totvect = 0;
  totvect_last = 0;
  totave = 0;
  xaccl = 0;
  yaccl = 0;
  zaccl = 0;
    mpu6050.update();
    xaccl = float(mpu6050.getAccX());

    yaccl = float(mpu6050.getAccY());

    zaccl = float(mpu6050.getAccZ());

    totvect = sqrt(((xaccl - xavg) * (xaccl - xavg)) + ((yaccl - yavg) * (yaccl - yavg)) + ((zval - zavg) * (zval - zavg)));

    totave = (totvect + totvect_last) / 2 ;

   delay120 = millis();
  while (millis() - delay120 <= 100){
    hrt_Routine();
  }
   
    //cal stepCount
    if (totave > threshhold && flag == 0) {
      stepCount = stepCount + 1;
      flag = 1;

    } else if (totave < threshhold   && flag == 1) {
      flag = 0;
    }

    if (stepCount < 0) {
      stepCount = 0;
    }
}


// this function resets the step count
void resetSteps(){
  stepCount = 0;
}



//this function takes in the initial values of the accelerometer, and averages out the values. This is kin fo like our average reference point.
void calibrate(){
  float sum = 0;
  float sum1 = 0;
  float sum2 = 0;

  for (int i = 0; i < 100; i++) {
    sum += float(mpu6050.getAccX());
  }
  xavg = sum / 100.0;

  for (int j = 0; j < 100; j++){
    sum1 += float(mpu6050.getAccY());
  }
  yavg = sum1 / 100.0;

  for (int q = 0; q < 100; q++)
  {
    sum2 += float(mpu6050.getAccZ());
  }
  zavg = sum2 / 100.0;
}
