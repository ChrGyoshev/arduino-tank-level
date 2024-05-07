#include <NewTone.h>

#include <NewTone.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LcdBarGraphRobojax.h>
#include <NewTone.h>

byte lcdNumCols = 16; // Number of columns in the LCD
byte lcdLine = 2;     // Number of lines in the LCD

LiquidCrystal_I2C lcd(0x27, lcdNumCols, lcdLine); // I2C address for the LCD

LcdBarGraphRobojax rjx(&lcd, 16, 0, 0); // Creating 16 character long bargraph starting at char 0 of line 0

#include <NewPing.h>

#define TRIGGER_PIN 11    // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN 12       // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 60   // Maximum distance we want to ping for (in centimetres).
#define BUZZER_PIN 8

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup()
{
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);



  // Initializing the LCD
  lcd.init();
  lcd.backlight();
  lcd.begin(lcdNumCols, lcdLine);
  lcd.clear();
  lcd.print("*Gyoshev  Trans*");

  // Delay to stabilize visualization
  delay(2000);
  lcd.clear();
}

void loop()
{
  
  unsigned long distance = sonar.ping_cm(); // Get distance in cm
 
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  int duration = pulseIn(ECHO_PIN, HIGH);
  
 
  distance = (duration/2) / 29.1;
 
 

  // Map the distance to a percentage
  int distancePercentage = map(distance, 20, MAX_DISTANCE, 100, 0);
  Serial.print(distancePercentage);

  rjx.clearLine(1); // Clear line 1 to display fresh voltage value

  // Draw bar graph from the distance percentage
  rjx.drawValue(distancePercentage, 100);

  // Check if tank is full or empty

  if (distancePercentage >= 95) {
    lcd.setCursor(0,1);
    lcd.print("Fuel Tank Full!");

  }  else if (distancePercentage <=10) {
    //  NewTone( BUZZER_PIN,1000, 500);
     
    
    
    
    rjx.clearLine(1);
    rjx.drawValue(0,100);
    lcd.setCursor(0,1);
    lcd.print("Fuel Tank Empty!");
  }
  
  else {
     lcd.setCursor(0,1);
    lcd.print("Fuel Level: ");
    lcd.print(distancePercentage);
    lcd.print("%");
  }
 
  
   
 
  
   
   
  
  

  delay(1000); // Delay for stability
}
