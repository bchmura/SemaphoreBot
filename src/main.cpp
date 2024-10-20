//
// Created by bchmura on 10/14/24.
//
#include <Arduino.h>
#include <ArduinoLog.h>
#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <ios>
#include <SemaphoreFlags.hpp>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

SemaphoreFlags FlagBrain;

#define SERVOMIN  102 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  500 // This is the 'maximum' pulse length count (out of 4096)
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

constexpr unsigned int SCREEN_OLED_WIDTH = 128;
constexpr unsigned int SCREEN_OLED_HEIGHT = 64;
constexpr int SCREEN_OLED_RESET_PIN = -1; // Reset pin # (or -1 if sharing Arduino reset pin)
constexpr unsigned int SCREEN_OLED_ADDRESS  = 0x3C;

Adafruit_SSD1306 display(SCREEN_OLED_WIDTH, SCREEN_OLED_HEIGHT, &Wire, SCREEN_OLED_RESET_PIN);


char messageToSend[] = "I AM WORKING!";
int messagePointer = 0;

void TestServos();
void MoveServoTo(int servoNumber, int angle);

void setup() {
    Serial.begin(9600);
    while(!Serial && !Serial.available()){}
    randomSeed(analogRead(0));

    Log.begin(LOG_LEVEL_VERBOSE, &Serial);

    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_OLED_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }

    display.display();
    display.clearDisplay();

    Log.verboseln("Setting up PWM");
    pwm.begin();
    pwm.setOscillatorFrequency(27000000);
    pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

    Log.verboseln("Moving servos to 135 start");
    MoveServoTo(0, 135);
    MoveServoTo(1,135);
    delay(1000);

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

    std::vector<std::tuple<std::string, FlagType, int, int>> commands = FlagBrain.ConvertToFlagSequence(messageToSend);

    for (const std::tuple<std::string, FlagType, int, int>& command : commands) {
        Log.verboseln("Printing %s", std::get<0>(command).c_str());
        display.clearDisplay();
        display.setTextColor(SSD1306_WHITE);
        display.setTextWrap(false);

        display.setTextSize(6);
        display.setCursor(50, 17);
        display.println(F(std::get<0>(command).c_str()));

        display.setTextSize(1);
        display.setCursor(2, 1);
        display.print(messageToSend);

        display.display();
        MoveServoTo(0,std::get<2>(command));
        MoveServoTo(1,std::get<3>(command));
        delay(1000);
    }
    Log.verboseln("------------");
    delay(4000);
};

