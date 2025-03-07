#include <QTRSensors.h>
QTRSensors qtr;

// 8 -> 1 8 ->1
// A7, A5, A6, A4, A3, A2, A1, A0, A15, A14, A13, A12, A11, A10, A9, A8
// A8, A9, A10, A11, A12, A13, A14, A15, A0, A1, A2, A3, A4, A6, A5, A7

//53, 51, 49, 47, 45, 43, 41, 39
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

void setup()
{
  qtr.setTypeRC();
  //qtr.setSensorPins((const uint8_t[]){A8, A9, A10, A11, A12, A13, A14, A15}, SensorCount); // sensor right
  //qtr.setSensorPins((const uint8_t[]){A15, A14, A13, A12, A11, A10, A9, A8, A6, A1, A0}, SensorCount); // sensor left
  //qtr.setSensorPins((const uint8_t[]){37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22}, SensorCount); // sensor all
  qtr.setSensorPins((const uint8_t[]){37,36,35,34,33,32,31,30}, SensorCount); // sensor left

  //qtr.setEmitterPin(2);

  delay(2000);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH); // turn on Arduino's LED to indicate we are in calibration mode

  // analogRead() takes about 0.1 ms on an AVR.
  // 0.1 ms per sensor * 4 samples per sensor read (default) * 6 sensors
  // * 10 reads per calibrate() call = ~24 ms per calibrate() call.
  // Call calibrate() 400 times to make calibration take about 10 seconds.
  for (uint16_t i = 0; i < 300; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(10, LOW); // turn off Arduino's LED to indicate we are through with calibration
  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();
  // print the calibration maximum values measured when emitters were on
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
}                        
                
void loop() 
{
  // read calibrated sensor values and obtain a measure of the line position
  // from 0 to 5000 (for a white line, use readLineWhite() instead)
  uint16_t position = qtr.readLineBlack(sensorValues);

  // print the sensor values as numbers from 0 to 1000, where 0 means maximum
  // reflectance and 1000 means minimum reflectance, followed by the line
  // position
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println(position);

  delay(250);
}
