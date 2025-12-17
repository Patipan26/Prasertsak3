#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

const int LCD_COLS = 16;
const int LCD_ROWS = 2;
LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);

// DHT sensor setup
#define DHTPIN 16 // Digital pin connected to the DHT sensor
#define LED_RED 18
#define LED_BLUE 5
#define LED_YELLOW 17
#define DHTTYPE DHT22 // or DHT11, etc., depending on your sensor
DHT dht(DHTPIN, DHTTYPE);

void initLCD()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starting...!");
  delay(2000);
}

void displayLCD(int col, int row, String msg, bool clear = true)
{
  if (clear)
    lcd.clear();
  lcd.setCursor(col, row);
  lcd.print(msg);
}
void setup()
{
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  Serial.begin(9600);
  initLCD();   // Initialize the LCD
  dht.begin(); // Initialize the DHT sensor
}

void loop()
{
  // Wait a few seconds between measurements.
  delay(2000);

  // Read humidity and temperature from the DHT sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early
  if (isnan(humidity) || isnan(temperature))
  {

    displayLCD(0, 0, "Failed to read!");
    return;
  }

  if (temperature > 35)
  {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_BLUE, LOW);
  }
  if (temperature < 15)
  {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_BLUE, HIGH);
  }
  if (humidity < 30)
  {
    digitalWrite(LED_YELLOW, HIGH);
  }
  else
    digitalWrite(LED_YELLOW, LOW);

  // Display the results on the LCD
  displayLCD(0, 0, "Hum: " + String(humidity) + " %", true);
  displayLCD(0, 1, "Temp: " + String(temperature) + " C", false);
}
