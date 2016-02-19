/*********************************************************
 * 2 motor, 3 servo test routine for Sand-E
 * by Alex Goodsell
 *
 *********************************************************/

#include <Servo.h>

//Create servo objects to control servos
Servo servoOne;
Servo servoTwo;
Servo servoThree;

//create variables to store servo positions
int servoPositionOne = 0;
int servoPositionTwo = 0;
int servoPositionThree = 0;

//create variable to exit loop
int numberOfPerformances = 1;

void setup() {
  //Setup Channel A for Motor 1
  pinMode(12, OUTPUT); //Inititates Motor Channel A pin
  pinMode(9, OUTPUT); //Inititates Brake Channel A pin

  //Setup Channel B for Motor 2
  pinMode(13, OUTPUT); //Inititates Motor Channel B pin
  pinMode(8, OUTPUT); //Inititates Brake Channel B pin

  //attach servos to pins
  servoOne.attach(5);
  servoTwo.attach(6);
  servoThree.attach(7);

}

void loop() {
  if (numberOfPerformances==1) {
    //full speed forward
  setMotors(HIGH,255,LOW,HIGH,255,LOW);
  delay(3000);

  //come to a stop
  setMotors(LOW,0,LOW,LOW,0,LOW);
  delay(1000);

  //full speed reverse
  setMotors(LOW,255,LOW,LOW,255,LOW);
  delay(3000);

  //turn on spot
  setMotors(HIGH,100,LOW,LOW,100,LOW);

  //make servos do a wave
  moveServo(servoOne,servoPositionOne);
  delay(1000);
  moveServo(servoTwo,servoPositionTwo);
  delay(1000);
  moveServo(servoThree,servoPositionThree);
  moveServo(servoOne,servoPositionOne);
  delay(1000);
  moveServo(servoTwo,servoPositionTwo);
  delay(1000);
  moveServo(servoThree,servoPositionThree);

  //stop motors
  setMotors(LOW,0,LOW,LOW,0,LOW);

  //perform sweep turn
  setMotors(HIGH,100,LOW,LOW,0,LOW);

  //move on a curve
  setMotors(HIGH,255,LOW,LOW,100,LOW);

  numberOfPerformances+=1;
  }


}

//void to move a given servo
void moveServo(Servo servoNumber, int servoPosition) {
  if (servoPosition == 0) {
    servoNumber.write(180);
    servoPosition = 180;
  }

  else {
    servoNumber.write(0);
    servoPosition = 0;
  }
}

//void to turn motors on and off, and vary speed
void setMotors(uint8_t channelADirection, int channelASpeed,uint8_t channelABrake,uint8_t channelBDirection, int channelBSpeed,uint8_t channelBBrake) {
  //set motor 1/channel A
  digitalWrite(12,channelADirection);
  digitalWrite(9,channelABrake);
  analogWrite(3,channelASpeed);

  //set motor 2/channel B
  digitalWrite(13,channelBDirection);
  digitalWrite(8,channelBBrake);
  analogWrite(11,channelBSpeed);
}
