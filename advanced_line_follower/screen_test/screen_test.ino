#include <TFT.h> // Hardware-specific library
#include <SPI.h>

#define CS   48
#define DC   46
#define RESET  47


TFT myScreen = TFT(CS, DC, RESET);

// variable to keep track of the elapsed time
int counter = 0;
// char array to print time
char printout[4];

void setup(){

  myScreen.begin();

  myScreen.background(0,0,0); // clear the screen

  myScreen.stroke(255,0,255);

  // static text

  myScreen.text("Running for",0,0);

  myScreen.text("seconds",0,30);

  // increase font size for text in loop()

  myScreen.setTextSize(3);
}

void loop(){

   /* // get elapsed time

    counter = millis();

    // convert to a string

    String elapsedTime = String(counter/1000);

    // add to an array

    elapsedTime.toCharArray(printout,4);

    // print out and erase

    myScreen.stroke(255,255,255);
    myScreen.setTextSize(5);

    myScreen.text(printout,0,10);

    delay(1000);

    myScreen.stroke(0,0,0);

    myScreen.text(printout,0,10); */

    myScreen.point(myScreen.width()/2, myScreen.height()/2);
    int i,j =0;

    for (i=0 ; i<160 ; i++){
      for(j=0 ; j<128 ; j++){
        myScreen.stroke(i,i,j);
        myScreen.point(i,j);
        myScreen.stroke(j,j,i);

      }
    }
    
}