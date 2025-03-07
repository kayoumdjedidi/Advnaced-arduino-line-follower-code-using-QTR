#include <TFT.h>

#define cs   48
#define dc   46
#define rst  47

TFT screen = TFT(cs, dc, rst);

int senPosX[16] = {3, 13, 23, 33, 43, 53, 63, 73, 83, 93, 103, 113, 123, 133, 143, 153};
int senPosY = 114;
int sensorValue = 0;
char sensorvalueChar[2];
String valueStr;


void setup() {
  // initialize the screen
  screen.begin();

  // make the background black
  screen.background(0,0,0);
  // set the stroke color to white
  screen.stroke(255,255,255);
  screen.setTextSize(1);
  //screen.text(text, screen.width()/2-3, screen.height()/2-3);
  screen.line(1, 108, 159, 108);
  screen.line(1, 127, 159, 127);

  screen.stroke(255,0,0);
  screen.fill(255,0,0);
  screen.rect(screen.width()/2-9, 107,19,3);

  // add message saying calibration mode
}

void loop() {

for (int i=0; i<16 ;i++){
    
    sensorValue=random(2);
    valueStr = String(sensorValue);
    valueStr.toCharArray(sensorvalueChar, 2);
    for (int i=0; i<16 ;i++){
    screen.stroke(255,255,255);
    screen.text(sensorvalueChar, senPosX[i], senPosY);
    }
    for (int i=0; i<16 ;i++){
    screen.stroke(0,0,0);
    screen.text(sensorvalueChar, senPosX[i], senPosY);
    }

  }
}


