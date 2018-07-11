#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

const char* ssid     = "ProjectBig";
const char* password = "warapolkang";

#define APPID   "KangSmartHome"
#define KEY     "kfHTjp8WB2e1ira"
#define SECRET  "Nqyk0LBleDpZdl7in1SGC888k" 

#define ALIAS   "pieled"

#define D0 16
#define lamp1 D0

WiFiClient client;

char state = 0;
char stateOutdated = 0;
char buff[16];

int timer = 0;
char str[32];

#define DHTTYPE DHT11          //Define sensor type
#define DHTPIN D4              // Define sensor pin
DHT dht(DHTPIN, DHTTYPE, 15);   //Initialize DHT sensor 

MicroGear microgear(client);

int humid;
int temp;

void sendState(){
  if (state==0)
    microgear.publish("/pieled/state","0");
  else
    microgear.publish("/pieled/state","1");
  Serial.println("");
  stateOutdated = 0;
}

void updateIO(){
  if (state >= 1) {
    digitalWrite(lamp1, HIGH);
  }
  else {
    state = 0;
    digitalWrite(lamp1, LOW);
  }
}

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  char m = *(char *)msg;

  Serial.print("ค่าที่ได้รับ == ");
  msg[msglen] = '\0';
  Serial.println((char *)msg);

  if (m == '0' || m == '1') {
    state = m=='0'?0:1;
updateIO();
  }
  if (m == '0' || m == '1' || m == '?') stateOutdated = 1;
}

void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("เชื่มต่อไปยัง NETPIE สำเร็จ");
  microgear.setAlias(ALIAS);
  stateOutdated = 1;
}

void setup(){
    Serial.begin(115200);
    Serial.println("กำลังเริ่มระบบ...");

    pinMode(lamp1, OUTPUT);
    dht.begin();
    
    if (WiFi.begin(ssid, password)) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
    }

    Serial.println("เชื่อมต่อ Wifi สำเร็จ");  
    Serial.print("IP address : ");
    Serial.print(WiFi.localIP());
   
    microgear.on(MESSAGE,onMsghandler);
    microgear.on(CONNECTED,onConnected);    
    microgear.init(KEY,SECRET,ALIAS);
    microgear.connect(APPID);
}

void loop(){
  if (microgear.connected()) 
  {
    if (stateOutdated)
    {
      sendState();
      microgear.loop();
    }

    if (timer >= 1000) 
    {
        humid = dht.readHumidity();
        temp = dht.readTemperature();
        sprintf(str,"%d,%d",humid,temp);
        Serial.println(str);

        Serial.print("Sending -->");
        microgear.publish("/dht",str);

        timer = 0;
     } 
     else 
     {
      timer += 100;
     }
  }
  else 
  {
    Serial.println("การเชื่อมต่อล้มเหลม, กำลังเชื่อมต่อใหม่...");
     
    if (timer >= 5000) 
    {
      microgear.connect(APPID);
      timer = 0;
    }
    else 
    {
      timer += 100;
    }
  }
  delay(100);
}
