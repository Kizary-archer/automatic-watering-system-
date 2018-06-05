#include <ArduinoJson.h>
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <Hash.h>

ESP8266WiFiMulti WiFiMulti;

WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  StaticJsonBuffer<1024> jsonBuffer;
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        JsonObject& initSend = jsonBuffer.createObject();
        initSend["event"] = "init";
        JsonObject& data = initSend.createNestedObject("data");
        JsonArray& dataHumidity = data.createNestedArray("dataHumidity");
        for (unsigned short i = 0; i < 20; i++)
        dataHumidity.add(i);
        /*Serial.print("dataHumidity");
        unsigned long ts = millis();
        while (Serial.available() == 0)
        {
          unsigned long currentMillis = millis();
          if (currentMillis - ts > 1000)break;
        }
        if (Serial.available() > 0)
        {
          int countlog = Serial.parseInt();
          while (countlog)
          {
            ts = millis();
            while (Serial.available() == 0)
            {
              unsigned long currentMillis = millis();
              if (currentMillis - ts > 1000)break;
            }
            int val = Serial.parseInt();
            dataHumidity.add(val);
            Serial.print(val);
            countlog--;
          }
        }*/




        data["dateWatering"] = 1526381147614;
        data["automaticWatering"] = true;
        data["autotesting"] = true;
        data["sensorAnalysis"] = true;
        data["wateringMode"] = true;
        data["microcontroller"] = "Arduino";
        data["humidity"] = 60;
        char JSONmessageBuffer[1024];
        initSend.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
        webSocket.sendTXT(num, JSONmessageBuffer);
      }
      break;
    case WStype_TEXT:
      //Serial.printf("[%u] get Text: %s\n", num, payload);
      JsonObject& espSend = jsonBuffer.parseObject(payload);
      Serial.print(espSend["event"].as<char*>());
      delay(200);
      if (Serial.find('@')) Serial.write(espSend["data"].as<char*>());
      //webSocket.broadcastTXT("message here");

      break;
  }

}

void setup() {
  Serial.begin(9600);

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
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

