#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include <DHT.h>

//DHT
#define DHTPIN D2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

//WIFI
const char* ssid     = "ProjectBig";
const char* password = "warapolkang";

//WiFiServer server(88); // ประกาศสร้าง TCP Server ที่พอร์ต 88

//THINGSPEAK
String apiKey = "YWQ0THIW220BSOTO";
const char* server = "api.thingspeak.com";

//NETPIE
#define APPID   "KangSmartHome"
#define KEY     "kfHTjp8WB2e1ira"
#define SECRET  "Nqyk0LBleDpZdl7in1SGC888k" 
#define ALIAS   "ESP8266"

//LAMP_PIN
#define D0 16
#define lamp1 D0

#define D0 16
#define lampIn D0

#define pressed LOW

WiFiClient client;
MicroGear microgear(client);

void setup(){
  Serial.begin(115200);
  dht.begin();
  pinMode(lamp1, OUTPUT);
  pinMode(lampIn, INPUT_PULLUP);
  setup_wifi();
  setup_microgear();
}

void loop(){
  if (microgear.connected()) {
    //dht11();
    microgear.loop();
  }else {
    Serial.println("การเชื่อมต่อล้มเหลม, กำลังเชื่อมต่อใหม่...");
    microgear.connect(APPID);
  }
}

void lampSwitch(char msg){
  if (msg == '1') {
    digitalWrite(lamp1, HIGH);
  }else if(msg == '0'){
    digitalWrite(lamp1, LOW);    
  }
  lampStatus();
}

void lampStatus(){
  bool ReadStat = digitalRead(lampIn);
  if(ReadStat == pressed){
    Serial.println("Status : OFF");
    microgear.chat("Dashboard1","Status : OFF");
  }else{
    Serial.println("Status : ON");
    microgear.chat("Dashboard1","Status : ON");
  }
}

void dht11(){
  dht.read();
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  // ตัววัดเสีย เลยต้องประมาณเอา
  float tem = t/27.5; //ประมาณให้เป็นค่าที่น่าจะเป็น
  float hum = h/26; //ประมาณให้เป็นค่าที่น่าจะเป็น
  microgear.chat("Dashboard1",tem);
  //microgear.chat("Dashboard1", hum);
  
  if (client.connect(server,80)) { 
    String postStr = apiKey;
           postStr +="&field1=";
           postStr += String(tem);
           postStr +="&field2=";
           postStr += String(hum);
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
           
     Serial.print("Temperature: ");
     Serial.print(tem);
     Serial.print(" degrees Celcius Humidity: "); 
     Serial.print(hum);
     Serial.println("% send to Thingspeak");    
  }
  client.stop();
}

void setup_wifi(){
  if (WiFi.begin(ssid, password)) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
    }
    Serial.println("เชื่อมต่อ Wifi สำเร็จ");  
    Serial.print("IP address : ");
    Serial.print(WiFi.localIP());
    Serial.println("");
}

void setup_microgear(){
  microgear.on(MESSAGE,onMsghandler);
  microgear.on(CONNECTED,onConnected);    
  microgear.init(KEY,SECRET,ALIAS);
  microgear.connect(APPID);
}

void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("เชื่มต่อไปยัง NETPIE สำเร็จ");
  microgear.setAlias(ALIAS);
  digitalWrite(lamp1, LOW);
  lampStatus();
}

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  char m = *(char *)msg;

  Serial.print("ข้อความที่ได้รับ : ");
  msg[msglen] = '\0';
  Serial.println((char *)msg);

  if(m == '0' || m == '1'){
    lampSwitch(m);
  }else if(m == '2'){
    dht11();
  }else if(m == '?'){
    lampStatus();
  }
}

