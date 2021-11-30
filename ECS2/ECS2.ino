//---------------------------------------------------------------------------------------------------------
// FILENAME:      ECS.ino
// DESCRIPTION:   Driver file for Evaporation Cooling System.
// COMPILER:      Arduino Mega2560
// AUTHORS:       Marissa Floam         CPE301.1001  (Fall 2021)
//                Michael Moosmuller    CPE301.1001  (Fall 2021)
//                Jacob Wilt            CPE301.1001  (Fall 2021)
// NOTES:         
//---------------------------------------------------------------------------------------------------------

// Libraries
#include <ezButton.h>
#include <dht.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include "LED.h"
//#include "Button.h"

// Water Level Sensor Pins
#define WLD_SNS_PIN A0

// Temperature Humidity Sensor Pins
#define DHT_PIN A1

// LCD Screen Pins
#define RS 7
#define E  8
#define D4 9
#define D5 10
#define D6 11
#define D7 12

// DC Motor Pins
#define ENABLE 5
#define DIRA   6
#define DIRB   4

// Servo Motor Pin
#define SM_PIN 22

// LED Pins
#define GR 13
#define BL 2
#define RD 3
#define YW 26

// Button Pin
#define B 24

// Initialize Objects
dht              DHT;
LiquidCrystal    LCD  (RS, E, D4, D5, D6, D7);
Servo            SM;
ezButton         Button(B);
LED              Red(RD);
LED              Green(GR);
LED              Blue(BL);
LED              Yellow(YW);

// Control Variables
bool ST_DISABLED, ST_IDLE, ST_ERROR, ST_RUNNING;

// Global Variables
int STATE;                  // State Variable
int pos;                    // Servo Motor Position
char buf[80];               // Serial Input Buffer
float TEMP = 21;            // Control Temperature
int value;                  // Water Level Reading

// Prototypes
void readTempHumLCD();
void readWaterLvl();
void runDCMotor();

//-------------------------------------------------------------------------------------------------------//

void setup() {

    // States
    ST_DISABLED = true;
    ST_IDLE = false;
    ST_ERROR = false;
    ST_RUNNING = false;
    
    // LED'S
    Red.init("Output");
    Green.init("Output");
    Blue.init("Output");
    Yellow.init("Output");

    // DC Motor
    pinMode(ENABLE,OUTPUT);                     // Illegal
    pinMode(DIRA,OUTPUT);                       // Illegal
    pinMode(DIRB,OUTPUT);                       // Illegal

    // Servo Motor
    SM.attach(SM_PIN);
    SM.write(90);

    // Start Systems
    Serial.begin(9600);                         // Start Serial
    LCD.begin(16, 2);                           // 16 characters, 2 line

    // Delay Program
    delay(1000);

}

//-------------------------------------------------------------------------------------------------------//

void loop() {

    Button.loop();
    
    // State Control
    STATE = getState();
    switch(STATE) {
        case 1:
            Disabled();
            break;
        case 2:
            servoPosition();
            Idle();
            break;
        case 3:
            Error();
            break;
        case 4:
            servoPosition();
            Running();
            break;
        default:
            ST_DISABLED = true;
            break;
    }

    // Delay
    delay(3000);
    
}

//-------------------------------------------------------------------------------------------------------//

int getState() {
    int temp;
    if (ST_DISABLED)
        temp = 1;
    else if(ST_IDLE)
        temp = 2;
    else if(ST_ERROR)
        temp = 3;
    else if(ST_RUNNING)
        temp = 4;
    return temp;
}

//-------------------------------------------------------------------------------------------------------//

void Disabled() {
    Yellow.on();
    int btnState = Button.getState();
    //Serial.println(btnState);
    if (btnState == 0) {
        Yellow.off();
        ST_DISABLED = false;
        ST_IDLE = true;
    }
}

//-------------------------------------------------------------------------------------------------------//

void Idle() {
    Green.on();
    readTempHumLCD();
    readWaterLvl();
    if (DHT.temperature > TEMP) {
        Green.off();
        ST_IDLE = false;
        ST_RUNNING = true;
    }
    if (value < 100) {
        Green.off();
        ST_IDLE = false;
        ST_ERROR = true;
    }
}

//-------------------------------------------------------------------------------------------------------//

void Error() {
    Red.on();
    readWaterLvl();
    Serial.println("Water Level Low!");
    LCD.clear();
    LCD.print("Water Level Low!");
    if (value > 200) {
        Red.off();
        ST_ERROR = false;
        ST_IDLE = true;
    }
}

//-------------------------------------------------------------------------------------------------------//

void Running() {
    Blue.on();
    readTempHumLCD();
    runDCMotor();
    readWaterLvl();
    if (DHT.temperature < TEMP) {
        digitalWrite(ENABLE,LOW);                   // Illegal
        Blue.off();
        ST_RUNNING = false;
        ST_IDLE = true;
    }
    if (value < 100) {
        digitalWrite(ENABLE,LOW);                   // Illegal
        Blue.off();
        ST_RUNNING = false;
        ST_ERROR = true;
    }
}

//-------------------------------------------------------------------------------------------------------//

void servoPosition() {
    // Set Servo Motor Position from Serial Input
    if (readline(Serial.read(), buf, 80) > 0) {
        Serial.print("Servo Position: ");
        Serial.println(buf);
        pos = atoi(buf);
        SM.write(pos);
    }
}

//-------------------------------------------------------------------------------------------------------//

int readline(int readch, char *buffer, int len) {
    static int pos = 0;
    int rpos;

    if (readch > 0) {
        switch (readch) {
            case '\r':                                      // Ignore CR
                break;
            case '\n':                                      // Return on new-line
                rpos = pos;
                pos = 0;                                    // Reset position index ready for next time
                return rpos;
            default:
                if (pos < len-1) {
                    buffer[pos++] = readch;
                    buffer[pos] = 0;
                }
        }
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------------//

void readTempHumLCD() {

    // Print Temperature and Humidity Values to LCD Screen
    
    DHT.read11(DHT_PIN);
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("Temp: ");
    LCD.print(DHT.temperature);
    LCD.print((char)223);
    LCD.print("C");
    LCD.setCursor(0,1);
    LCD.print("Humidity: ");
    LCD.print(DHT.humidity);
    LCD.print("%");
/*
    Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature);
    Serial.println("C  ");
*/

}

//-------------------------------------------------------------------------------------------------------//

void readWaterLvl() {

    value = analogRead(WLD_SNS_PIN);                                // Illegal
  Serial.print("Water Level: ");
  Serial.println(value);

}

//-------------------------------------------------------------------------------------------------------//

void runDCMotor() {

    digitalWrite(ENABLE,HIGH);                            // Illegal
    digitalWrite(DIRA,HIGH);                              // Illegal
    digitalWrite(DIRB,LOW);                               // Illegal

}
