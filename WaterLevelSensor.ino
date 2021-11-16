/*---------------------------------------------------------------------------------------------------------
  FILENAME:      WaterLevelSensor.ino
  DESCRIPTION:   Monitors water level within the evaporation cooling system.
  COMPILER:      Arduino
  AUTHORS: 
  
  KIT: "The Most Complete Starter Kit MEGA 2560 Project" by ELEGOO
    
  CONFIGURATION:    
  --------------
  Water Level Detection Sensor Module
  Power Pin:   13
  Sensor Pin:  A0  
---------------------------------------------------------------------------------------------------------*/
 
volatile unsigned char* my_ADMUX    = (unsigned char*) 0x7C;
volatile unsigned char* my_ADCSRB   = (unsigned char*) 0x7B;
volatile unsigned char* my_ADCSRA   = (unsigned char*) 0x7A;
volatile unsigned int*  my_ADC_DATA = (unsigned int*) 0x78;

volatile unsigned char *portDDRB = (unsigned char *) 0x24;
volatile unsigned char *portB    = (unsigned char *) 0x23;

unsigned int val = 0;
int adc_id = 0;

//-------------------------------------------------------------------------------------------------------//

void setup() {
  
  *portDDRB |= 0b10000000;                              // Set port 13 to Output
  *portB &= 0b01111111;                                 // Set port 13 to low
  
  adc_init();                                           // Setup the ADC

  Serial.begin(9600);                                   // Start the UART
  
}

//-------------------------------------------------------------------------------------------------------//

void loop() {
    
    unsigned int level = readSensor();

    // Serial.print("Water level: ");
    // Serial.println(level);
   
    if (level < 100)
      Serial.println("Water is low!");

    if (level > 140)
      Serial.println("Water is too high!");

    delay(1000);
    
}

//-------------------------------------------------------------------------------------------------------//

void adc_init() {
  
  // set up the A register
  *my_ADCSRA |= 0b10000000;                             // Enable the ADC (set bit 7 to 1)
  *my_ADCSRA &= 0b11011111;                             // Disable the ADC trigger mode (clear bit 5 to 0)
  *my_ADCSRA &= 0b11110111;                             // Disable the ADC interrupt (clear bit 3 to 0)
  *my_ADCSRA &= 0b11111000;                             // Set prescaler selection to slow reading (clear bit 2-0 to 0)
  
  // set up the B register
  *my_ADCSRB &= 0b11110111;                             // Reset the channel and gain bits (clear bit 3 to 0)
  *my_ADCSRB &= 0b11110111;                             // Set free running mode (clear bit 2-0 to 0)
  
  // set up the MUX Register
  *my_ADMUX  &= 0b00000000;                             // AVCC analog reference (clear bit 7 to 0)
  *my_ADMUX  |= 0b01000000;                             // AVCC analog reference (set bit 6 to 1)
  *my_ADMUX  &= 0b11011111;                             // Right adjust result (clear bit 5 to 0)
  *my_ADMUX  &= 0b11100000;                             // Reset the channel and gain bits (clear bit 4-0 to 0)
  
}

//-------------------------------------------------------------------------------------------------------//

unsigned int readSensor() {
  
  *portB |= 0b10000000;                                 // Drive PB7 (pin #13) to high (turn on)
  delay(10);                                            // wait 10 milliseconds
  
  val = adc_read(adc_id);                               // Get reading from the AOC   
  *portB |= 0b01111111;                                 // Drive PB7 (pin #13) to low (turn off)
  
  return val;     
  
}

//-------------------------------------------------------------------------------------------------------//

unsigned int adc_read(unsigned char adc_channel_num) {
  
  *my_ADMUX &= 0b11100000;                              // reset the channel and gain bits
  *my_ADCSRB &= 0b11110111;                             // clear the channel selection bits
  
  if(adc_channel_num > 7) {                             // set the channel number
    adc_channel_num -= 8;                               // set channel selection bits, remove most significant (bit 3)
    *my_ADCSRB |= 0b00001000;                           // set MUX bit 
  }

  *my_ADMUX  += adc_channel_num;                        // set channel selection bits
  *my_ADCSRA |= 0b01000000;                             // set ADCSRA bit to 1, start conversion
  
  while(*my_ADCSRA & 0b000000000);                      // wait for the conversion
  
  return *my_ADC_DATA;                                  // return result in the ADC data register
  
}
