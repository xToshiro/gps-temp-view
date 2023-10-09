//Coded by Jairo Ivo

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <TinyGPS++.h>
#include <ESP32Time.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>

#define SEALEVELPRESSURE_HPA (1013.25)

#define LED 2

// ESP32Time rtc;
ESP32Time rtc(-10800);  // offset in seconds GMT-3

#define GPS_BAUDRATE 9600  // the default baudrate of NEO-6M is 9600

TinyGPSPlus gps;  // the TinyGPS++ object

String dataMessage;  // save data to sdcard

File dataFile;

// Inicia o objeto do sensor de temperatura
Adafruit_BME280 bme;

// Variveis do RTC interno
int rtcdia, rtcmes, rtcano, rtchora, rtcminuto, rtcsegundo{ 0 };
// Variaveis do gps
int gpsdia, gpsmes, gpsano, gpshora, gpsminuto, gpssegundo{ 0 };
float gpslat, gpslong{ 0 };
char latitudeStr[15];
char longitudeStr[15];
double gpsalt = 0;
float gpsvel = 0;
// Variaveis de leitura dos sensores
// BME
float temp, hum, pres, alt{ 0 };

void setup() {
  Serial.begin(9600);
  Serial2.begin(GPS_BAUDRATE);

  Serial.println(F("AirTemp View - Coded by Jairo Ivo"));

  pinMode(LED, OUTPUT);

  initSDCard();
  checkSDFile();  // Checa o arquivo data.csv no cartão de memoria ou cria se ele não existir

  Serial.println(F("Iniciando sincronização do RTC interno com o gps!"));
  delay(500);
  while (rtc.getYear() < 2001) {
    Serial.println(F("."));
    rtcSyncWithGps();
    //delay(50);
  }

  if (!bme.begin(0x76, &Wire)) {
    delay(500);
    Serial.println("Não foi possivel encontrar o sensor BME280, checar os fios!");
    while (1)
      ;
  }
}

void loop() {
  //delay(500);
  if (Serial2.available() > 0) {
    if (gps.encode(Serial2.read())) {
      digitalWrite(LED, LOW);
      if (gps.location.isValid()) {
        dtostrf(gps.location.lat(), 12, 8, latitudeStr);
        dtostrf(gps.location.lng(), 12, 8, longitudeStr);
        if (gps.altitude.isValid()) {
          gpsalt = (gps.altitude.meters());
        } else {
          Serial.println(F("- alt: INVALID"));
          delay(150);
        }
      } else {
        Serial.println(F("- location: INVALID"));
        delay(150);
      }
      if (gps.speed.isValid()) {
        gpsvel = (gps.speed.kmph());
      } else {
        Serial.println(F("- speed: INVALID"));
        delay(150);
      }
      if (gps.date.isValid() && gps.time.isValid()) {
        gpsano = (gps.date.year());
        gpsmes = (gps.date.month());
        gpsdia = (gps.date.day());
        gpshora = (gps.time.hour());
        gpsminuto = (gps.time.minute());
        gpssegundo = (gps.time.second());
      } else {
        Serial.println(F("- gpsDateTime: INVALID"));
        delay(150);
      }
      //Serial.println();
    }
  }
  if ((rtc.getSecond()) != rtcsegundo) {
    digitalWrite(LED, HIGH);
    Serial.print(F("- RTC date&time: "));
    Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));  // (String) returns time with specified format

    rtcmes = rtc.getMonth();
    rtcdia = rtc.getDay();
    rtcano = rtc.getYear();
    rtchora = rtc.getHour(true);
    rtcminuto = rtc.getMinute();
    rtcsegundo = rtc.getSecond();

    Serial.print(F("- GPS date&time: "));
    Serial.print(gpsano);
    Serial.print(F("-"));
    Serial.print(gpsmes);
    Serial.print(F("-"));
    Serial.print(gpsdia);
    Serial.print(F(" "));
    Serial.print(gpshora);
    Serial.print(F(":"));
    Serial.print(gpsminuto);
    Serial.print(F(":"));
    Serial.println(gpssegundo);
    Serial.print(F("- latitude: "));
    Serial.println(latitudeStr);
    Serial.print(F("- longitude: "));
    Serial.println(longitudeStr);
    Serial.print(F("- altitude: "));
    Serial.println(gpsalt);
    Serial.print(F("- speed: "));
    Serial.print(gpsvel);
    Serial.println(F(" km/h"));

    temp = bme.readTemperature();
    hum = bme.readHumidity();
    pres = (bme.readPressure() / 100.0F);
    alt = (bme.readAltitude(SEALEVELPRESSURE_HPA));

    Serial.print("- temperature = ");
    Serial.print(temp);
    Serial.println(" °C");
    Serial.print("- pressure = ");
    Serial.print(pres);
    Serial.println(" hPa");
    Serial.print("- approx. Altitude = ");
    Serial.print(alt);
    Serial.println(" m");
    Serial.print("- humidity = ");
    Serial.print(hum);
    Serial.println(" %");

    saveData();
    Serial.println();
  }
}

void rtcSyncWithGps() {
  if (Serial2.available() > 0) {
    //delay(150);
    if (gps.encode(Serial2.read())) {
      delay(150);
      Serial.print(F("- GPS date&time: "));
      if (gps.date.isValid() && gps.time.isValid()) {
        Serial.print(gps.date.year());
        Serial.print(F("-"));
        Serial.print(gps.date.month());
        Serial.print(F("-"));
        Serial.print(gps.date.day());
        Serial.print(F(" "));
        Serial.print(gps.time.hour());
        Serial.print(F(":"));
        Serial.print(gps.time.minute());
        Serial.print(F(":"));
        Serial.println(gps.time.second());
        rtc.setTime((gps.time.second()), (gps.time.minute()), (gps.time.hour()), (gps.date.day()), (gps.date.month()), (gps.date.year()));  // 17th Jan 2021 15:24:30
        //rtc.setTime(1609459200);  // 1st Jan 2021 00:00:00
        //rtc.offset = 7200; // change offset value
        Serial.print(F("- RTC date&time: "));
        Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));  // (String) returns time with specified format
        // formating options  http://www.cplusplus.com/reference/ctime/strftime/
      } else {
        Serial.println(F("Sem dados de data e hora validos!"));
      }

      Serial.println();
    }
  }
  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("Sem dados de gps validos: verificar conexão"));
}