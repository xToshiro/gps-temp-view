#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Wire.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>
#include <ESP32Time.h>

#define SEALEVELPRESSURE_HPA (1013.25)

//ESP32Time rtc;
ESP32Time rtc(-10800);  // offset in seconds GMT+1


const int ledVerdePin = 2;
const int ledVermelhoPin = 12;
const int ledAmareloPin = 15;
// Definição dos pinos RX e TX para o GPS
static const int RXPin = 16, TXPin = 17;

static const uint32_t GPSBaud = 9600;

File dataFile;

// Tempo de atualização do GPS em milissegundos
const unsigned long intervaloAtualizacao = 1000;
unsigned long ultimaAtualizacao = 0;

// Inicialização do objeto GPS
TinyGPSPlus gps;
// Inicia o objeto do sensor de temperatura
Adafruit_BME280 bme;

// Inicialização do objeto SoftwareSerial
SoftwareSerial ss(RXPin, TXPin);

String dataMessage;

unsigned long dia = 0;
unsigned long mes = 0;
unsigned long ano = 0;
unsigned long hora = 0;
unsigned long minuto = 0;
unsigned long segundo = 0;

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

// Função para ligar o LED Verde
void ligarLedVerde() {
  digitalWrite(ledVerdePin, HIGH);
  digitalWrite(ledVermelhoPin, LOW);
  digitalWrite(ledAmareloPin, LOW);
}

// Função para ligar o LED Vermelho
void ligarLedVermelho() {
  digitalWrite(ledVerdePin, LOW);
  digitalWrite(ledVermelhoPin, HIGH);
  digitalWrite(ledAmareloPin, LOW);
}

void ligarLedAmarelo() {
  digitalWrite(ledVerdePin, LOW);
  digitalWrite(ledVermelhoPin, LOW);
  digitalWrite(ledAmareloPin, HIGH);
}

void rtcSyncWithGps() {

  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}


void gpsprint() {
  Serial.print("Data: ");
  Serial.print(gps.date.month());
  Serial.print("/");
  Serial.print(gps.date.day());
  Serial.print("/");
  Serial.print(gps.date.year());
  Serial.print(" Hora: ");
  Serial.print(gps.time.hour());
  Serial.print(":");
  Serial.print(gps.time.minute());
  Serial.print(":");
  Serial.print(gps.time.second());
  Serial.print(" Latitude: ");
  Serial.print(gps.location.lat(), 6);
  Serial.print(" Longitude: ");
  Serial.print(gps.location.lng(), 6);
  Serial.print(" Altitude: ");
  Serial.print(gps.altitude.meters());
  Serial.print("m");
  Serial.print(" Velocidade: ");
  Serial.print(gps.speed.kmph());
  Serial.println("km/h");
}

void setup() {
  // Inicialização dos pinos dos LEDs
  pinMode(ledVerdePin, OUTPUT);
  pinMode(ledVermelhoPin, OUTPUT);
  pinMode(ledAmareloPin, OUTPUT);
  ligarLedVermelho();
  delay(1000);
  ligarLedAmarelo();
  delay(1000);
  ligarLedVerde();
  delay(1000);
  // Inicialização da comunicação serial com o monitor serial
  Serial.begin(115200);

  // Inicialização da comunicação serial com o GPS
  ss.begin(GPSBaud);

  // Verifica se o GPS está conectado corretamente
  unsigned long startTime = millis();
  while (millis() - startTime < 5000) {
    if (ss.available()) {
      // GPS conectado corretamente
      ligarLedVerde();
      Serial.println("GPS conectado corretamente.");
      delay(5000);
      break;
    }
  }

  if (!ss.available()) {
    // GPS não encontrado

    ligarLedVermelho();
    delay(500);
    Serial.println("GPS não encontrado ou falha na conexão.");
  }

  if (!bme.begin(0x76, &Wire)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1)
      ;
  }

  initSDCard();
  checkSDFile();  // Checa o arquivo data.csv no cartão de memoria ou cria se ele não existir
}

void bmeRead() {
  if (!bme.begin(0x76)) {
    //bmeLogSum += 1;
    //bmeStatus = false;
    //Serial.print("DHT ERROR! ");
  } else {
    temp = bme.readTemperature();
    hum = bme.readHumidity();
    pres = bme.readPressure();
    alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
    //bmeStatus = true;
    //bmeSampleCount++;
    //Serial.print("SHT OK! ");
  }
}

void gpsRead() {
  // Verifica se há dados disponíveis na porta serial do GPS
  while (ss.available() > 0)
    // Lê os dados do GPS
    if (gps.encode(ss.read())) {
      // Verifica se os dados do GPS são válidos
      if (gps.location.isValid()) {
        saveData();
        // Atualiza a cada intervalo definido
        if (millis() - ultimaAtualizacao > intervaloAtualizacao) {
          ultimaAtualizacao = millis();
          // Imprime os dados no monitor serial
          gps.encode(ss.read());

          mes = gps.date.month();
          dia = gps.date.day();
          ano = gps.date.year();
          hora = gps.time.hour();
          minuto = gps.time.minute();
          segundo = gps.time.second();
          //gpslat = gps.location.lat();
          //gpslong = gps.location.lng();

          dtostrf(gps.location.lat(), 12, 8, latitudeStr);
          dtostrf(gps.location.lng(), 12, 8, longitudeStr);

          Serial.println(gps.altitude.meters());
          gpsalt = gps.altitude.meters();
          gpsvel = gps.speed.kmph();
          bmeRead();
          delay(500);
          ligarLedVerde();
        }
      } else {
        // Dados inválidos do GPS
        Serial.println("Dados inválidos do GPS!");
        //delay(1000);
        // Ligar o LED Vermelho
        ligarLedAmarelo();
      }
    }
}

void loop() {
  
}

