#include <Servo.h>


int pos = 0;
Servo myservo;


// the setup routine runs once when you press reset:
void setup() {
    myservo.attach(2);
}

// the loop routine runs over and over again forever:
void loop() {

    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees in steps of 1 degree
        myservo.write(pos);               // tell servo to go to position in variable 'pos'
        delay(15);                        // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
        myservo.write(pos);               // tell servo to go to position in variable 'pos'
        delay(15);                        // waits 15ms for the servo to reach the position
    }

}
