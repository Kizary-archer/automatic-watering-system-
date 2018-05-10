    #include <ESP8266WiFi.h>
     
    void setup()
    {
      Serial.begin(115200);
      Serial.println();
     
      WiFi.begin("Meizu", "qwerty11");
     
      Serial.print("Connecting");  //  "Подключение"
      while (WiFi.status() != WL_CONNECTED)
      {
        delay(500);
        Serial.print(".");
      }
      Serial.println();
     
      Serial.print("Connected, IP address: ");
               //  "Подключились, IP-адрес: "
      Serial.println(WiFi.localIP());
    }
     
    void loop() {}
