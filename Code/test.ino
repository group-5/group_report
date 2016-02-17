#include <Servo.h>

int pos = 0;
Servo myservo;

// the setup routine runs once when you press reset:
void setup() {
    myservo.attach(2);
}

// the loop routine runs over and over again forever:
void loop() {
    for (pos = 0; pos <= 180; pos += 1) { // go from 0-180 deg in steps of 1 deg
        myservo.write(pos);               // tell servo to go to position 'pos'
        delay(15);                        // wait 15ms for servo to reach position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // go from 180-0 deg
        myservo.write(pos);               // tell servo to go to position 'pos'
        delay(15);                        // wait 15ms for servo to reach position
    }
}
