#include <QTRSensors.h>
#include <TFT.h>

#define cs   48
#define dc   46
#define rst  47

QTRSensors qtr;

TFT screen = TFT(cs, dc, rst);
int senPosX[16] = {3, 13, 23, 33, 43, 53, 63, 73, 83, 93, 103, 113, 123, 133, 143, 153};
int senPosY = 114;

int sensorValue = 0;  // for int to char conversion
char sensorvalueChar[2]; // for int to char conversion
String valueStr; // for int to char conversion

const uint8_t SensorCount = 16;
uint16_t values[SensorCount];
int maxOutputValue = 1000;
const int maxAllowedSpeed = 255;
const int minAllowedSpeed = 0;
const int baseSpeed = 160;
const int baseLeftSpeed = baseSpeed;
const int baseRightSpeed = baseSpeed;
const int rightSideCorrection = baseRightSpeed * 0.04;

uint16_t position;
const int centerPosition = 450;
int line = 1;
int& lineRef = line;
int counter = 0;
int& counterRef = counter;
int error = 0;
int lastError = 0;
int integral = 0;
int derivative;
const float KP = 1.5;  // Overshoot = reduce KP, slugish = increase KP  0.87
const float KD = 0.5;  // increase to reduce wobling   0.45
const float KI = 0;    // aditive error, must test with real runs


int leftSpeed;
int rightSpeed;
int speedModifier = 1;
int mb;

int rightDirection = 12;
int rightPWM = 3;
int rightBrake = 9;
int leftDirection = 13;
int leftPWM = 11;
int leftBrake = 8;
int ping = 42;
int echo = 43;
int barrier;

int barrierDetection();
void handBrake();
void IOConfig();
int lineType(uint16_t (&values)[SensorCount], int& lineRef, int& counterRef);
void calibrate(uint8_t SensorCount);
void printCalibratedValues(uint8_t SensorCount);
void printValues(const uint8_t SensorCount, uint16_t values[]);
void forward(int rightSpeed, int leftSpeed);
void backward(int rightSpeed, int leftSpeed);
void screenSetup();
void showSensorValues();


void setup() {
  Serial.begin(9600);
  IOConfig();
  screenSetup();
  // configure the sensors
  qtr.setTypeRC();
  //qtr.setSensorPins((const uint8_t[]){A8, A9, A10, A11, A12, A13, A14, A15}, SensorCount); // sensor right
  //qtr.setSensorPins((const uint8_t[]){A15, A14, A13, A12, A11, A10, A9, A8, A6, A1, A0}, SensorCount); // sensor left
  //qtr.setSensorPins((const uint8_t[]){37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22}, SensorCount); // sensor all
  qtr.setSensorPins((const uint8_t[]){37,36,35,34,33,32,31,30}, SensorCount); // sensor left

  rightSpeed = baseRightSpeed;
  leftSpeed = baseLeftSpeed;
  analogWrite(leftBrake, HIGH);
  analogWrite(rightBrake, HIGH);
  // turn on Arduino's LED to indicate we are in calibration mode

  calibrate(SensorCount);
  printCalibratedValues(SensorCount);
}


void loop() {
  showSensorValues();
  barrier = barrierDetection();
  position = qtr.readLineBlack(values);

 

    line = lineType(values, lineRef, counterRef);
    if (line == 1) {
      position = qtr.readLineBlack(values);
    } else if (line == 0)
      position = qtr.readLineWhite(values);

    printValues(SensorCount,values,line);



    //PID control

    error = ((position / 10) - centerPosition);
    derivative = error - lastError;
    integral = integral + error;

    lastError = error;

    speedModifier = (error * KP + integral * KI + derivative * KD);
    leftSpeed = baseLeftSpeed - speedModifier;
    rightSpeed = (baseRightSpeed + rightSideCorrection) + speedModifier;

    if (leftSpeed <= minAllowedSpeed)
      leftSpeed = minAllowedSpeed;
    if (leftSpeed >= maxAllowedSpeed)
      leftSpeed = maxAllowedSpeed;
    if (rightSpeed <= minAllowedSpeed)
      rightSpeed = minAllowedSpeed;
    if (rightSpeed >= maxAllowedSpeed)
      rightSpeed = maxAllowedSpeed;

    //----------------------------------- START OF CONDITIONS REGION ---------------------------//

    /*if (position == 0 && counter ==0)
      forward(baseSpeed,baseSpeed);
    else if (position == 0 && counter !=0)
      handBrake();*/
    if (position == 0)
      forward(baseSpeed, baseSpeed);
    else if (position == 3500)
      hardLeft(maxAllowedSpeed);
    else if (position == 5500)
      hardRight(maxAllowedSpeed);
    else if (position == 9000 && line == 0) {
      mb = 1;
      forward(baseRightSpeed, baseLeftSpeed);
    } else if (values[2] == 1 && values[3] == 1 && values[4] == 1 && values[5] == 1 && values[6] == 1 && values[7] == 1 && mb ==1)
      handBrake();
      else {


        //----------------------------------- END OF CONDITIONS REGION ---------------------------//

        forward(rightSpeed, leftSpeed);
      }
}





/*///////////////////////////////////////////////////////////*/

void showSensorValues(){
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

void screenSetup(){
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

}

int barrierDetection() {
  digitalWrite(ping, LOW);
  delayMicroseconds(2);
  digitalWrite(ping, HIGH);
  delayMicroseconds(10);
  digitalWrite(ping, LOW);
  long duration = pulseIn(echo, HIGH);
  int distance = duration * 0.034 / 2;
  int barrier = (distance < 20) ? barrier = 1 : barrier = 0;
  return barrier;
}

void calibrate(uint8_t SensorCount) {
  // 2.5 ms RC read timeout (default) * 10 reads per calibrate() call
  // = ~25 ms per calibrate() call.
  // Call calibrate() 400 times to make calibration take about 10 seconds.
  for (uint16_t i = 0; i < 200; i++) {
    qtr.calibrate();
  }
  // say that its done calibrating 
}



void printCalibratedValues(uint8_t SensorCount) {
  // print the calibration minimum values measured when emitters were on
  for (uint8_t i = 0; i < SensorCount; i++) {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  for (uint8_t i = 0; i < SensorCount; i++) {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  delay(1000);
}



void printValues(const uint8_t SensorCount, uint16_t values[], int line) {
  // print the sensor values as numbers from 0 (max) to 1000 (min) then the position
  for (uint8_t i = 0; i < SensorCount; i++) {
    Serial.print(values[i]);
    Serial.print('\t');
  }
  Serial.print(position);
  Serial.write('  error =');
  Serial.print(error);
  Serial.write('  L =');
  Serial.print(leftSpeed);
  Serial.write('  R =');
  Serial.print(rightSpeed);
  Serial.write('  S =');
  Serial.print(line);
  Serial.write('  C =');
  Serial.println(counter);
  delay(500);
}


void forward(int rightSpeed, int leftSpeed) {
  analogWrite(leftBrake, LOW);
  analogWrite(rightBrake, LOW);
  digitalWrite(rightDirection, HIGH);
  analogWrite(rightPWM, rightSpeed);
  digitalWrite(leftDirection, HIGH);
  analogWrite(leftPWM, leftSpeed);
}


void backward(int rightSpeed, int leftSpeed) {
  analogWrite(leftBrake, LOW);
  analogWrite(rightBrake, LOW);
  digitalWrite(rightDirection, LOW);
  analogWrite(rightPWM, rightSpeed);
  digitalWrite(leftDirection, LOW);
  analogWrite(leftPWM, leftSpeed);
}

void hardRight(int maxAllowedSpeed) {
  analogWrite(leftBrake, LOW);
  analogWrite(rightBrake, LOW);
  digitalWrite(rightDirection, HIGH);
  analogWrite(rightPWM, maxAllowedSpeed);
  digitalWrite(leftDirection, LOW);
  analogWrite(leftPWM, maxAllowedSpeed);
}

void hardLeft(int maxAllowedSpeed) {
  analogWrite(leftBrake, LOW);
  analogWrite(rightBrake, LOW);
  digitalWrite(rightDirection, LOW);
  analogWrite(rightPWM, maxAllowedSpeed);
  digitalWrite(leftDirection, HIGH);
  analogWrite(leftPWM, maxAllowedSpeed);
}

void handBrake() {
  digitalWrite(rightBrake, HIGH);
  digitalWrite(leftBrake, HIGH);
}

void IOConfig(){
  // configure the motors
  pinMode(rightDirection, OUTPUT);
  pinMode(leftDirection, OUTPUT);
  pinMode(rightPWM, OUTPUT);
  pinMode(leftPWM, OUTPUT);
  pinMode(rightBrake, OUTPUT);
  pinMode(leftBrake, OUTPUT);
  pinMode(ping, OUTPUT);
  pinMode(echo, INPUT);
}

int lineType(uint16_t (&values)[SensorCount], int& lineRef, int& counterRef) {
  if (values[0] == maxOutputValue && values[1] == maxOutputValue && values[2] == maxOutputValue && /*values[3] == maxOutputValue &&*/ values[4] == 0 && values[5] == 0 && /*values[6] == maxOutputValue &&*/ values[7] == maxOutputValue && values[8] == maxOutputValue && values[9] == maxOutputValue)
    line = 1;  // black line

  else
    line = line;
  if (values[0] == 0 && values[1] == 0 && values[2] == 0 && /* values[3] == 0 && */ values[4] == maxOutputValue && values[5] == maxOutputValue && /* values[6] == 0 &&*/ values[7] == 0 && values[8] == 0 && values[9] == 0) {
    line = 0;  // white line
    counter++;
  } else
    line = line;
  return line;
}