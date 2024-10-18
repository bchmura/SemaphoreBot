//
// Created by bchmura on 10/14/24.
//
#include <Arduino.h>
#include <ArduinoLog.h>
#include <map>
#include <string>
#include <cstring>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <ios>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


#define SERVOMIN  102 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  500 // This is the 'maximum' pulse length count (out of 4096)
//#define USMIN  500 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
//#define USMAX  2500 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// our servo # counter
uint8_t servonum = 0;

std::map<std::string, std::tuple<uint16_t, uint16_t, uint16_t>> semaphore;

char messageToSend[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
int messagePointer = 0;


void TestServos();
void LoadSemaphore();
void MoveServoTo(int servoNumber, int angle);

void setup() {
    Serial.begin(9600);
    while(!Serial && !Serial.available()){}
    randomSeed(analogRead(0));
    Log.begin(LOG_LEVEL_VERBOSE, &Serial);
    LoadSemaphore();
    Log.verboseln("Setting up PWM");
    pwm.begin();
    pwm.setOscillatorFrequency(27000000);
    pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

    Log.verboseln("Moving servos to 135 start");
    MoveServoTo(0, 135);
    MoveServoTo(1,135);
    delay(5000);
    //MoveServoTo(0, 90);
    //MoveServoTo(1,90);
    //delay(5000);
    //TestServos();


  };


void MoveServoTo(const int servoNumber, const int angle)
{
    int pulseWidth = 0;

    if (servoNumber == 1 )
    {
        pulseWidth = map(angle, 0, 270, SERVOMAX, SERVOMIN);
    } else if (servoNumber == 0)
    {
        pulseWidth = map(angle, 0, 270, SERVOMIN, SERVOMAX);

    } else
    {
        Serial.println("No such servo");
    }
    Log.verboseln("Moving servo # %d to %d degrees, using p/w %d", servoNumber, angle, pulseWidth);
    pwm.setPWM(servoNumber, 0, pulseWidth);
}


void TestServos()
{
    Log.verboseln("TestServos");
    for (int angle = 0; angle <= 270; angle += 45 )
    {
        Log.verboseln("...Both servos to %d degrees", angle);
        MoveServoTo(0, angle);
        MoveServoTo(1, angle);
        delay(2000);
    }

    for (int angle = 270; angle >= 0; angle -= 45 )
    {
        Log.verboseln("...Both servos to %d degrees", angle);
        MoveServoTo(0, angle);
        MoveServoTo(1, angle);
        delay(2000);
    }

}




void loop() {

    if (messagePointer > strlen(messageToSend))
    {
        messagePointer = 0;
        delay(5000); 
    }

    auto & [type, leftpos, rightpos] = semaphore[std::string(1,messageToSend[messagePointer])];
    Log.verboseln("Flagging %c, which is left %d right %d", messageToSend[messagePointer], leftpos, rightpos);
    MoveServoTo(0,leftpos);
    MoveServoTo(1,rightpos);

    messagePointer++;
    delay(500);




    //
    // Serial.println("Action 0");
    // pwm.setPWM(1, 0,500);
    //
    // delay(5000);
    //
    // Serial.println("Action 1");
    // pwm.setPWM(1, 0,100);
    //
    // delay(5000);
    //
    // Serial.println("Action 2");
    // pwm.setPWM(1, 0,500);
    //
    // delay(5000);
    //
    // Serial.println("Action 2");
    // pwm.setPWM(1, 0,200);
    //
    // delay(5000);


    // Drive each servo one at a time using setPWM()
    //Serial.println(servonum);
    //Serial.println("Increasing");
    //Serial.println(servonum);
    // for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX+1; pulselen += 5) {
    //      Serial.println(pulselen);
    //      pwm.setPWM(1, 0, pulselen);
    //      delay(200);
    // }
    // pwm.setPWM(1, 0, 50);
    // delay(500);

    //Serial.println("Decreasing");

    // for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
    //     Serial.println(pulselen);
    //
    //     pwm.setPWM(servonum, 0, pulselen);
    //     delay(2000);
    // }

    delay(500);

    // Drive each servo one at a time using writeMicroseconds(), it's not precise due to calculation rounding!
    // The writeMicroseconds() function is used to mimic the Arduino Servo library writeMicroseconds() behavior.
   // for (uint16_t microsec = USMIN; microsec < USMAX; microsec++) {
   //     pwm.writeMicroseconds(servonum, microsec);
    //}

  //  delay(500);
  //  for (uint16_t microsec = USMAX; microsec > USMIN; microsec--) {
 //       pwm.writeMicroseconds(servonum, microsec);
  //  }

    // delay(500);

    // servonum++;
    // if (servonum > 2) servonum = 0; // Testing the first 8 servo channels
};

void LoadSemaphore()
{
    Log.verboseln("Loading semaphore");
    // type, left, right
    semaphore["A"] = {0, 180, 225};
    semaphore["B"] = {0, 135, 225};
    semaphore["C"] = {0, 90, 225};
    semaphore["D"] = {0, 45, 225};
    semaphore["E"] = {0, 225, 90};
    semaphore["F"] = {0, 225, 135};
    semaphore["G"] = {0, 225, 180};
    semaphore["H"] = {0, 135, 270};
    semaphore["I"] = {0, 90, 270};
    semaphore["J"] = {0, 45, 135};
    semaphore["K"] = {0, 180, 45};
    semaphore["L"] = {0, 180, 90};
    semaphore["M"] = {0, 180, 135};
    semaphore["N"] = {0, 180, 180};
    semaphore["O"] = {0, 135, 0};
    semaphore["P"] = {0, 135, 45};
    semaphore["Q"] = {0, 134, 90};
    semaphore["R"] = {0, 135, 135};
    semaphore["S"] = {0, 135, 180};
    semaphore["T"] = {0, 90, 45};
    semaphore["U"] = {0, 90, 90};
    semaphore["V"] = {0, 45, 180};
    semaphore["W"] = {0, 0, 135};
    semaphore["X"] = {0, 0, 180};
    semaphore["Y"] = {0, 90, 135};
    semaphore["Z"] = {0, 270, 135};





}
