/* Library used by this sketch */

#include <LiquidCrystal.h>
#include <dht.h>
#include <SFE_BMP180.h>
#include <Wire.h>

/* Variables and constants */

// Pin to read the DHT22
#define DHT22_PIN 13
// Variable to read the DHT status
int chk;
// Altitude in Madrid
#define ALTITUDE 635

// define some values used by the LCD panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  
 
 return btnNONE;  // when all others fail, return this...
}

/* Create the objects */

dht DHT; // Crea el objeto DHT (temperatura y humedad)
SFE_BMP180 pressure; // Crea el objeto pressure (Sensor BMP180)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  // Crea el objeto LCD //

void setup(){
}

void loop(){
 Serial.print("DHT22, \t");
 chk = DHT.read22(DHT22_PIN);
 switch (chk)
 {
  case DHTLIB_OK:  
   Serial.print("OK,\t"); 
   break;
  case DHTLIB_ERROR_CHECKSUM: 
   Serial.print("Checksum error,\t"); 
   break;
  case DHTLIB_ERROR_TIMEOUT: 
   Serial.print("Time out error,\t"); 
   break;
  default: 
   Serial.print("Unknown error,\t"); 
   break;
}
 
 +  // DISPLAY DATA on the LCD
 +  Serial.print(DHT.humidity, 1);
 +  Serial.print(",\t");
 +  Serial.println(DHT.temperature, 1);
}
