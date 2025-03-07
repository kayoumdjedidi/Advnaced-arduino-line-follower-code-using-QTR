int rightDirection = 12;
int rightPWM = 3;
int rightBrake = 9;
int leftDirection = 13;
int leftPWM = 11;
int leftBrake = 8;



//front right , to go forward blue in +, green in -
//back right, to go forwaed grey in +, purple in -
//front left, to go forward green in +, yellow in -
//back left, to go forward blue in +, green in -


void setup() {
  // put your setup code here, to run once:
pinMode(rightDirection, OUTPUT);
pinMode(leftDirection, OUTPUT);
pinMode(rightPWM, OUTPUT);
pinMode(leftPWM, OUTPUT);
pinMode(rightBrake, OUTPUT);
pinMode(leftBrake, OUTPUT);

analogWrite(rightPWM,250);
analogWrite(leftPWM,250);
}

void loop() {
digitalWrite(rightDirection, HIGH);
digitalWrite(leftDirection, HIGH);

  // put your main code here, to run repeatedly:
/*digitalWrite(rightBrake, LOW);
digitalWrite(rightDirection, HIGH);
digitalWrite(leftDirection, HIGH);
delay(1000);
digitalWrite(rightBrake, HIGH);
digitalWrite(leftDirection, HIGH);
delay(500);
digitalWrite(rightBrake, LOW);
digitalWrite(rightDirection, HIGH);
digitalWrite(leftDirection, HIGH);
delay(1000);
digitalWrite(rightBrake, HIGH);
digitalWrite(leftDirection, HIGH);
delay(500);*/

}
