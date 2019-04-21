/*

This file contains functions that service buttons.

*/



//checks if a button was pressed. 
//returns 0 if the asnwer is no, 
//        1 if the increase threshold value button was pressed, 
//        2 if the decrease threshold value button was pressed
int buttonPressed(){
  if (u == 1 && lu == 0){
    return 1;
  }
  if (d == 1 && ld == 0){
    return 2;
  }
  return 0;
}


// read the buttons and change the flags accordingly. 
// lu, ld - last value of the up/down buttons. u, d is the current value of these buttons. 
void button_Routine(){
  lu = u; ld = d;
  u = digitalRead(threshUpPin);
  d = digitalRead(threshDownPin);
}
