/*---------------------------------------------------------------------------------------------------------
  FILENAME:      TempHumidityLCD.ino
  DESCRIPTION:   Displays temperature and humidity on the LCD screen.
  COMPILER:      Arduino
  AUTHORS: 
  
  KIT: "The Most Complete Starter Kit MEGA 2560 Project" by ELEGOO
    
  CONFIGURATION:    
  --------------
  DHT11 Temperature Humidity Module
  Sensor Pin:    7      left
  Power Pin:     PWR    center
  Ground Pin:    GND    right
  
  LCD1602 Module
  LiquidCrystal(RS, E, D4, D5, D6, D7)
  lcd.begin(16, 2)
  VSS   : GND
  VDD   : PWR
  V0    : Potentiometer Sensor Pin (back)
  RS    : 12
  RW    : GND
  E     : 11
  D0-D3 : Empty 
  D4    : 5
  D5    : 4
  D6    : 3
  D7    : 2

  Potentiometer 10k
  Sensor Pin:   V0    back
  Power Pin:    PWR   front right
  Ground Pin:   GND   front left
  
---------------------------------------------------------------------------------------------------------*/

#include <dht_nonblocking.h>
#include <LiquidCrystal.h>

#define DHT_SENSOR_TYPE DHT_TYPE_11
static const int DHT_SENSOR_PIN = 7;

DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//-------------------------------------------------------------------------------------------------------//

void setup( ) {
  
  Serial.begin( 9600);
  lcd.begin(16, 2);
  
}

//-------------------------------------------------------------------------------------------------------//

static bool measure_environment( float *temperature, float *humidity ) {
  
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul ) {
    if( dht_sensor.measure( temperature, humidity ) == true ) {
      measurement_timestamp = millis( );
      return( true );
    }
  }
  return( false );
}

//-------------------------------------------------------------------------------------------------------//

void loop( ) {
  
  float temperature;
  float humidity;

  if( measure_environment( &temperature, &humidity ) == true ) {
/*    
    Serial.print( "T = " );
    Serial.print( temperature, 1 );
    Serial.print( " deg. C, H = " );
    Serial.print( humidity, 1 );
    Serial.println( "%" );
    delay(10);
*/
    lcd.setCursor(0,0); 
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print((char)223);
    lcd.print("C");
    lcd.setCursor(0,1);
    lcd.print("Humidity: ");
    lcd.print(humidity);
    lcd.print("%");
  }

  delay(1000);
}
