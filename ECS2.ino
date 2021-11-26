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
#include <dht.h>
#include <LiquidCrystal.h>
#include <Servo.h>

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
#define LED1 13
#define LED2 2
#define LED3 3

// Initialize Objects
dht              DHT;
LiquidCrystal    LCD  (RS, E, D4, D5, D6, D7);
Servo            SM;

// Control Variables
bool STATE_DISABLED, STATE_IDLE, STATE_ERROR, STATE_RUNNING;

// Global Variables
int pos;                    // Servo Motor Position
char buf[80];               // Serial Input Buffer
float TEMP = 20;            // Control Temperature
int value;                  // Water Level Reading

//-------------------------------------------------------------------------------------------------------//

void setup() {

    // DC Motor
    pinMode(ENABLE,OUTPUT);                     // Illegal
    pinMode(DIRA,OUTPUT);                       // Illegal
    pinMode(DIRB,OUTPUT);                       // Illegal

    // Servo Motor
    SM.attach(SM_PIN);
    SM.write(90);

    // Start Systems
    Serial.begin(9600);                           // Start Serial
    LCD.begin(16, 2);                             // 16 characters, 2 line

    // Delay Program
    delay(1000);

}

//-------------------------------------------------------------------------------------------------------//

void loop(){

    // Set Servo Motor Position from Serial Input
    if (readline(Serial.read(), buf, 80) > 0) {
      Serial.print("Servo Position: ");
      Serial.println(buf);
      pos = atoi(buf);                              
      SM.write(pos);                               
    }

    // Water Level Detection Sensor
    value = analogRead(WLD_SNS_PIN);                                // Illegal
//    Serial.print("Water Level: ");
//    Serial.println(value);
    if (value == 0) 
      Serial.println("Water Level too Low!");

    // Print Temperature and Humidity Values to LCD Screen
    DHT.read11(DHT_PIN);                            
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

    // Adjust DC Fan Motor according to Temperature
    if (DHT.temperature > TEMP) {                           // Turn On
      digitalWrite(ENABLE,HIGH);                            // Illegal
      digitalWrite(DIRA,HIGH);                              // Illegal
      digitalWrite(DIRB,LOW);                               // Illegal
    }
    else if (DHT.temperature < TEMP) {                      // Turn Off
      digitalWrite(ENABLE,LOW);                             // Illegal
    }

    // Delay
    delay(3000);
    
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
