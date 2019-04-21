/*

This file contains functions that service buttons.

*/

//checks if a button was pressed. 
//returns 0 if the asnwer is no, 
//        1 if the light switch button was pressed, 
//        2 if the step reset button was pressed
int buttonPressed(){
  if (s == 1 && ls == 0){
    return 1;
  }
  if (r == 1 && lr == 0){
    return 2;
  }
  return 0;
}


// read the buttons and change the flags accordingly. 
// ls is last value of the swithc button. s is the current value.
void button_Routine(){
  ls = s; lr = r;
  r = digitalRead(stepResetPin);
  s = digitalRead(lightSwitchPin);
}
