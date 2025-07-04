#include <WiFi.h>
#include "DHT.h"
#include <HTTPClient.h>
#include <Wire.h>

int LDR_val = 0;
int sensor = 34;
#define soil_moisture_pin 4 



DHT dht(13, DHT11);
float hum,temp;

const char* ssid ="vivo 1919";
const char* password ="123456789";
const char* serverName ="http://api.thingspeak.com/update";
String apikey="HM3I80676Y9YOVR0";

void setup()
{
  Serial.begin(115200);

  dht.begin();

  WiFi.begin(ssid, password);
  while(WiFi.status()!= WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("connected to wifi with IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  if(WiFi.status()==WL_CONNECTED)
  {
    WiFiClient client;
    HTTPClient http;
    delay(1000);
    DHT_data();

    if(isnan(temp))
    {
      Serial.print(F("failed to read DHT sensor!"));
      return;
    }

    http.begin(client,serverName);
    http.addHeader("Content-type","application/x-www-form-unlencoded");
    String httpRequestData = "api_key="+apikey+"&field1="+String(temp);
    int httpResponseCode = http.POST(httpRequestData);

    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);

    http.end();
  }
  LDR_val = analogRead(sensor);
  Serial.print("light percentage: ");
  Serial.print(LDR_val/50);
  Serial.println("%");
  Serial.print("humdity: ");
  Serial.println(hum);
  Serial.print("temperature: ");
  Serial.println(temp);
  Serial.print("soil moisture: ");
  Serial.print(analogRead(soil_moisture_pin)/100);
  Serial.println("%");
  Serial.println("  ");
  delay(1000);
}

void DHT_data()
{
  hum = dht.readHumidity();
  temp = dht.readTemperature(); 
}
