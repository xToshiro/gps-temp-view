#include <TinyGPS++.h>
#include <ESP32Time.h>

//ESP32Time rtc;
ESP32Time rtc(-10800);  // offset in seconds GMT+1

#define GPS_BAUDRATE 9600  // The default baudrate of NEO-6M is 9600

TinyGPSPlus gps;  // the TinyGPS++ object

String dataMessage;

int gpsdia = 0;
int gpsmes = 0;
int gpsano = 0;
int gpshora = 0;
int gpsminuto = 0;
int gpssegundo = 0;

int rtcdia = 0;
int rtcmes = 0;
int rtcano = 0;
int rtchora = 0;
int rtcminuto = 0;
int rtcsegundo = 0;

char gpslat = 0;
char gpslong = 0;

char latitudeStr[15];
char longitudeStr[15];

double gpsalt = 0;
float gpsvel = 0;


// Variaveis de leitura dos sensores
//BME
float temp = 0;
float hum = 0;
float pres = 0;
float alt = 0;


void setup() {
  Serial.begin(9600);
  Serial2.begin(GPS_BAUDRATE);

  Serial.println(F("ESP32 - GPS module"));

  while (rtc.getYear() < 2001) {
    Serial.println(F("Iniciando sincronização do RTC interno com o gps"));
    rtcSyncWithGps();
    delay(150);
  }
}

void loop() {
  if ((rtc.getSecond()) != rtcsegundo) {
    while (Serial2.available() > 0) {
      if (gps.encode(Serial2.read())) {
        //delay(1000);
        if (gps.location.isValid()) {
          Serial.print(F("- latitude: "));
          Serial.println(gps.location.lat());

          Serial.print(F("- longitude: "));
          Serial.println(gps.location.lng());

          Serial.print(F("- altitude: "));
          if (gps.altitude.isValid())
            Serial.println(gps.altitude.meters());
          else
            Serial.println(F("INVALID"));
        } else {
          Serial.println(F("- location: INVALID"));
        }

        Serial.print(F("- speed: "));
        if (gps.speed.isValid()) {
          Serial.print(gps.speed.kmph());
          Serial.println(F(" km/h"));
        } else {
          Serial.println(F("INVALID"));
        }

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
          delay(500);

        } else {
          Serial.println(F("INVALID"));
        }

        Serial.println();
      }
    }

    if (millis() > 5000 && gps.charsProcessed() < 10)
      Serial.println(F("Sem dados de gps validos: verificar conexão"));


    Serial.print(F("- RTC date&time: "));
    Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));  // (String) returns time with specified format

    rtcmes = rtc.getMonth();
    rtcdia = rtc.getDay();
    rtcano = rtc.getYear();
    rtchora = rtc.getHour(true);
    rtcminuto = rtc.getMinute();
    rtcsegundo = rtc.getSecond();

    delay(150);
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