#include <ESP8266WiFi.h>
const char* ssid     = "HUAWEI-GR5";
const char* password = "0990016679pet";
#define D0 16
#define LED D0
void setup() 
{
   pinMode(LED,OUTPUT);
   Serial.begin(115200);
   Serial.println("Starting...");

   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) 
   {
      delay(250);
      Serial.print(".");
   }

   Serial.println("WiFi connected");  
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP());
}

void loop() 
{
}
