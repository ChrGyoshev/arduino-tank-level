#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LcdBarGraphRobojax.h>

byte lcdNumCols = 16; // Number of columns in the LCD
byte lcdLine = 2;     // Number of lines in the LCD

LiquidCrystal_I2C lcd(0x27, lcdNumCols, lcdLine); // I2C address for the LCD

LcdBarGraphRobojax rjx(&lcd, 16, 0, 0); // Creating 16 character long bargraph starting at char 0 of line 0

#include <NewPing.h>

#define TRIGGER_PIN 11    // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN 12       // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 30   // Maximum distance we want to ping for (in centimetres).

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup()
{
  Serial.begin(115200);

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
  int distanceCM = sonar.ping_cm(); // Get distance in cm

  // Map the distance to a percentage
  int distancePercentage = map(distanceCM, 0, MAX_DISTANCE, 100, 0);

  rjx.clearLine(1); // Clear line 1 to display fresh voltage value

  // Draw bar graph from the distance percentage
  rjx.drawValue(distancePercentage, 100);

  // Check if tank is full or empty
  if (distancePercentage >= 99) {
    // Tank is considered full
    rjx.drawValue(0, 100);
    lcd.setCursor(1, 1);
    lcd.print("Tank is Full!");
  } else if (distancePercentage <= 3) {
    // Tank is considered empty
    lcd.setCursor(1, 1);
    lcd.print("Tank is Empty!");
  } else {
    // Tank is neither full nor empty
    lcd.setCursor(0, 1);
    lcd.print("Fuel Level: ");
    lcd.print(distancePercentage); // Print the distance percentage
    lcd.print("%");
  }
  Serial.print(distancePercentage);

  delay(1000); // Delay for stability
}
