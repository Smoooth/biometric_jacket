/*

This file contains functions that service the display.

*/

// draw the display and print out individual values.
void drawDisplay(){
   display.clearDisplay();
   display.setTextSize(1.5);                    
   display.setTextColor(WHITE);             
   display.setCursor(5,2);  
   display.print("TEMP") ;
   display.setCursor(40,2);
   display.print("STEP ");
   display.setCursor(80,2);
   display.print("BPM ");
   display.setTextSize(2.5);                    
   display.setTextColor(WHITE);
   display.setCursor(5,12);
   display.println(tempC); 
   display.setCursor(40,12);
   display.println(stepCount);
   display.setCursor(80,12);
   display.print(hrtRate);
//   display.setTextSize(1);
//   display.setCursor(80,24);
//   display.print(blinkState);
//   display.setCursor(100,24);
//   display.print(lightsOn);
   display.display();
}


// initiates the display drawing.
// we don't want to update the display every time we run our routines - because it takes a lot of time to redraw the display
// instead we would do it every displayRefreshRate. 
// as well as when the buttons are pressed.
void display_Routine(){
  if (millis() - tOfLastDisplay >= displayRefreshRate){
    tOfLastDisplay = millis();
    temp_Routine();
    hrt_Out();
    drawDisplay();
    
  }
}
