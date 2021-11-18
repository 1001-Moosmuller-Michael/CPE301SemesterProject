/*---------------------------------------------------------------------------------------------------------
  FILENAME:      DCServoMotor.ino
  DESCRIPTION:   Controls fan speed and rotates fan.
  COMPILER:      Arduino
  AUTHORS: 
  
  KIT: "The Most Complete Starter Kit MEGA 2560 Project" by ELEGOO
    
  CONFIGURATION:    
  --------------
  Fan Blade and 3-6V  DC Motor
  Power Pin:   L293D Pin 3
  Ground Pin:  L293D Pin 6  

  Servo Motor SG90
  Sensor Pin:  9 
  Power Pin:   PWR
  Ground Pin:  GND

  L293D Chip
  Motor 1     Bottom
  1     :     Arduino 10
  2     :     Arduino 22
  3     :     DC Motor PWR Pin 
  4     :     GND
  5     :     Empty
  6     :     DC Motor GND Pin
  7     :     Arduino 6
  8     :     PWR

  Power Supply Module


  *** Needs to be customized to project, and DC motor settings without library functions. ***
  
  
---------------------------------------------------------------------------------------------------------*/

#include <Servo.h>

#define ENABLE 10
#define DIRA 6
#define DIRB 22

Servo myservo;

// DC Motor Port Manipulation
volatile unsigned char *portDDRB = (unsigned char *) 0x24;
volatile unsigned char *portB =    (unsigned char *) 0x23;

int i;


//-------------------------------------------------------------------------------------------------------//

void setup() {
                                    
  //*portDDRB |= 0b00000100;                                     // Set PB5 (pin 8) to Output

  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  
  myservo.attach(9);                                             // Attach Servo Motor to pin 9
  myservo.write(90);                                             // Start at center position (90*)
  
  Serial.begin(9600);                                           // Start the UART

}

//-------------------------------------------------------------------------------------------------------//

void loop() {

  //---back and forth example
  Serial.println("One way, then reverse");
  digitalWrite(ENABLE,HIGH); // enable on
  
  for (i=0;i<5;i++) {
    digitalWrite(DIRA,HIGH); //one way
    digitalWrite(DIRB,LOW);
    delay(500);
    digitalWrite(DIRA,LOW);  //reverse
    digitalWrite(DIRB,HIGH);
    delay(500);
  }
  
  digitalWrite(ENABLE,LOW); // disable
  delay(2000);

  Serial.println("fast Slow example");
  //---fast/slow stop example
  digitalWrite(ENABLE,HIGH); //enable on
  digitalWrite(DIRA,HIGH); //one way
  digitalWrite(DIRB,LOW);
  delay(3000);
  digitalWrite(ENABLE,LOW); //slow stop
  delay(1000);
  digitalWrite(ENABLE,HIGH); //enable on
  digitalWrite(DIRA,LOW); //one way
  digitalWrite(DIRB,HIGH);
  delay(3000);
  digitalWrite(DIRA,LOW); //fast stop
  delay(2000);

  Serial.println("PWM full then slow");
  //---PWM example, full speed then slow
  analogWrite(ENABLE,255); //enable on
  digitalWrite(DIRA,HIGH); //one way
  digitalWrite(DIRB,LOW);
  delay(2000);
  analogWrite(ENABLE,180); //half speed
  delay(2000);
  analogWrite(ENABLE,128); //half speed
  delay(2000);
  analogWrite(ENABLE,50); //half speed
  delay(2000);
  analogWrite(ENABLE,128); //half speed
  delay(2000);
  analogWrite(ENABLE,180); //half speed
  delay(2000);
  analogWrite(ENABLE,255); //half speed
  delay(2000);
  digitalWrite(ENABLE,LOW); //all done
  delay(10000);

  myservo.write(90);            // move servos to center position -> 90°
  delay(500);
  myservo.write(30);            // move servos to center position -> 60°
  delay(500);
  myservo.write(90);            // move servos to center position -> 90°
  delay(500);
  myservo.write(150);           // move servos to center position -> 120°
  delay(500);

  delay(1000);

}
