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
// Variable to read the display button
int button;
int lcdState;
// Altitude in Madrid
#define ALTITUDE 635
// Variables for the BMP180 data
char status;
double T,P,p0,a;

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
 Serial.begin(9600);
 lcd.begin(16, 2);
   // Inicia el sensor BMP180
  if (pressure.begin())
    Serial.println("BMP180 init success");
    else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail\n\n");
    while(1); // Pause forever.
  }
 lcd.setCursor(0,0);
}

void loop(){
 
 /* Read the temperature and humidity from tthe DHT22 */
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


/* Read the temperature and pressure from the BMP180 */
  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);
    
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:
      Serial.print("temperature: ");
      Serial.print(T,2);
      Serial.print(" deg C, ");

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("absolute pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");

          p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
          Serial.print("relative (sea-level) pressure: ");
          Serial.print(p0,2);
          Serial.print(" mb, ");

          a = pressure.altitude(P,p0);
          Serial.print("computed altitude: ");
          Serial.print(a,0);
          Serial.print(" meters, ");

             }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

/* Check if any button is pressed */
button = read_LCD_buttons();
switch(button){
 case btnLEFT:
  lcdState --;
 case btnRIGHT:
  lcdState ++;
}
if (lcdState == 0)
 lcdState = 2;
else if (lcdState == 3)
 lcdState = 1;

// DISPLAY DATA on the LCD
switch (lcdState){
 case 1: // Display temp and humidity from DHT22
  lcd.clear();
  lcd.setCursor(0,0); // Va a mostrar información en la segunda línea //
  lcd.print("Temp:");
  lcd.print(DHT.temperature,1); // texto a mostrar //
  lcd.write(223); // Degree symbol
  lcd.print("C ");
  lcd.setCursor(0,1);
  lcd.print("Humity:");                                             
  lcd.print(DHT.humidity,1);
  lcd.print("%");
 
 case 2: // Display the data from BMP180
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Presion: ");
  lcd.print(P,2);
  lcd.print(" mb");
  lcd.setCursor(0,1);
  lcd.print(T,2);
}

Serial.print(DHT.humidity, 1);
Serial.print(",\t");
Serial.println(DHT.temperature, 1);
}
