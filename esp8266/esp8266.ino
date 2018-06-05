#include <ArduinoJson.h>
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <EasyTransfer.h>

EasyTransfer ET;


struct DATA_STRUCTURE {
  long dateWatering;
  int humidity;
  bool automaticWatering;
  bool autotesting;
  bool sensorAnalysis;
  bool wateringMode;
};
DATA_STRUCTURE sdata;
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
        Serial.print("init");
        JsonObject& initSend = jsonBuffer.createObject();
        initSend["event"] = "init";
        JsonObject& data = initSend.createNestedObject("data");
        JsonArray& dataHumidity = data.createNestedArray("dataHumidity");
        for (unsigned short i = 0; i < 20; i++)
          dataHumidity.add(i);
        delay(500);

        if (ET.receiveData()) {
          Serial.println(sdata.dateWatering);
          data["dateWatering"] = sdata.dateWatering;
          data["automaticWatering"] = sdata.automaticWatering;
          data["autotesting"] = sdata.autotesting;
          data["sensorAnalysis"] = sdata.sensorAnalysis;
          data["wateringMode"] = sdata.wateringMode;
          data["humidity"] = sdata.humidity;
        }
        data["microcontroller"] = "Arduino";
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

      break;
  }

}

void setup() {
  Serial.begin(9600);

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
  }

  ET.begin(details(sdata), &Serial);
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

