#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SimpleDHT.h>

//#include <DHT.h>

String apiKey = "QW4T1W0K";// calve del canal de thigspeak
const char* ssid = "vodaf";// nombre de la wifi
const char* password = "UY8";// clave de la wifi
const char* server = "api.thingspeak.com";
int pinDHT22 = D0;
SimpleDHT22 dht22;
WiFiClient client;
void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(A0, INPUT);
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}
void loop() {
  byte temperatura = 0;
  byte humedad = 0;
  float luminosidad = analogRead(A0);// ldr si quieres, si no quitar
  if (dht22.read(pinDHT22, &temperatura, &humedad, NULL)) {
    Serial.print("Read DHT22 failed.");
    return;
  }
  if (client.connect(server,80)) {
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(luminosidad);
    postStr +="&field2=";
    postStr += String((int)temperatura);
    postStr +="&field3=";
    postStr += String((int)humedad);
    postStr += "\r\n\r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    Serial.println("% send to Thingspeak");
  }
  client.stop();
  Serial.println("Waiting…");
  delay(20000);
}
