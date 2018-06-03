#include <ArduinoJson.h>
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <Hash.h>

ESP8266WiFiMulti WiFiMulti;

WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        webSocket.sendTXT(num, "{\"event\":\"init\",\"data\":{\"dataHumidity\":[69,53,73,60,68,61,59,64,65,50,60,73,55,76,57,75,60,65,57,56,73,53,73,60,53,57,77,73,54,51,63,53,61,58,74,77,61,68,76,63,53,57,77,73,54,51,63,53,58,56,71,61,58,74,77,61,68,76,63,53,57,77,73,54,51,63,53,58,56,71,61,58,74,77,61,68,76,63,53,58,56,71,61,58,74,77,61,68,76,63,53,57,77,73,54,51,63,53,58,56,71,61,58,74,77,61,68,76,63,53,57,77,73,54,51,63,53,58,56,71,61,58,74,77,61,68,76,63,68,76,63,53,57,77,73,54,51,63,53,58,56,71,61,58,74,77,61,68,76,63,68,76,63,53,57,77,73,56,71,61,58,74,77,61,68,76,61,68,76],\"dateWatering\":1526381147614,\"autotesting\":false,\"wateringMode\":true,\"sensorAnalysis\":false,\"energySavingMode\":true,\"automaticWatering\":true,\"microcontroller\":\"Arduino ESP8266\",\"humidity\":55}}");
      }
      break;
    case WStype_TEXT:
       //Serial.printf("[%u] get Text: %s\n", num, payload);
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(payload);
      Serial.print(root["event"].as<char*>());     
      //webSocket.broadcastTXT("message here");

      break;
  }

}

void setup() {
  Serial.begin(9600);

      Serial.setDebugOutput(true);

    Serial.println();
    Serial.println();
    Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

WiFiMulti.addAP("dlink9321", "19311933af");
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
}

