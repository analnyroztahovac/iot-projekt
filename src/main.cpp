/* Import kniznic pre komunikaciu s RTC */
#include <Wire.h>
#include <RTClib.h>

/* Import kniznic pre komunikaciu s displejom */
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

/* Import kniznic pre komunikaciu s teplomerom */
#include <OneWire.h>
#include <DallasTemperature.h>

RTC_DS1307 rtc;

// TFT Konstanty
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// Teplomer konstanty
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress teplomer = {
        0x28, 0xFF, 0x47, 0x09, 
        0xE9, 0x03, 0x00, 0x9A};

void setup() {
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  sensors.begin();
  sensors.setResolution(teplomer, 9);

  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(4);
}

void loop() {
  DateTime now = rtc.now();
  sensors.requestTemperatures();
  float tempC = sensors.getTempC(teplomer);
  
  // Datum a rok (dd.MM.yyyy)
  tft.setCursor(50, 25);
  tft.print(now.day());
  tft.print(".");
  tft.print(now.month());
  tft.print(".");
  tft.print(now.year());

  // Cas (HH:mm:ss)
  tft.setCursor(50, 50);
  tft.print(now.hour());
  tft.print(":");
  tft.print(now.minute());
  tft.print(":");
  tft.print(now.second());

  // Teplota
  tft.setCursor(50, 75);
  tft.print("Teplota: ");
  tft.print(tempC);
  tft.print("°C");
  
  delay(1000);
}
