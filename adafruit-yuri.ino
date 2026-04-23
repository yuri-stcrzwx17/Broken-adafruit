#include "AdafruitIO_WiFi.h"
#include <DHT.h>

// ====== ADAFRUIT IO ======
#define IO_USERNAME  "Yurxx17"
#define IO_KEY       "adafruit"

// ====== WIFI ======
#define WIFI_SSID "Davi"
#define WIFI_PASS "davi1234"

// ====== DHT11 ======
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// ====== CONEXÃO ADAFRUIT ======
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

// ====== FEEDS ======
AdafruitIO_Feed *temperatura = io.feed("temperatura");
AdafruitIO_Feed *umidade = io.feed("umidade");

// ====== VARIÁVEIS ======
float ultimaTemp = -1000;
float ultimaUmidade = -1000;

void setup() {
Serial.begin(115200);
delay(1000);

dht.begin();

Serial.println("Iniciando conexão com Adafruit IO...");
io.connect();

// Loop com diagnóstico
while (io.status() < AIO_CONNECTED) {
Serial.print("Status: ");
  Serial.println(io.statusText());
delay(1000);
}

  Serial.println("Conectado ao Adafruit IO!");
}

void loop() {
io.run();
// verifica conexão continuamente
if(io.status() != AIO_CONNECTED) {
Serial.print("Desconectado: ");
Serial.println(io.statusText());
return;
}

float temp = dht.readTemperature();
float hum = dht.readHumidity();

if (isnan(temp) || isnan(hum)) {
Serial.println("Erro ao ler o DHT11");
delay(2000);
return;
}

if (abs(temp - ultimaTemp) >= 0.2) {
Serial.print("Temperatura: ");
Serial.print(temp);
Serial.println(" °C");
temperatura->save(temp);
ultimaTemp = temp;
}

if (abs(hum - ultimaUmidade) >= 1.0) {
Serial.print("Umidade: ");
Serial.print(hum);
Serial.println(" %");
umidade->save(hum);
ultimaUmidade = hum;
}

delay(3000);
}