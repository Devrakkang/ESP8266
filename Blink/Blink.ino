#define D0 16
#define led D0
void setup() {
  pinMode(led, OUTPUT);  
  Serial.begin(115200);   
}

void loop() {
  
  char data = Serial.read();
  
  if(data == '0'){
    Serial.println(data);
    digitalWrite(led, LOW);
  }else if(data == '1'){
    Serial.println(data);
    digitalWrite(led, HIGH); 
  }else{
    digitalWrite(led, LOW);                                            
  }
                  
}
