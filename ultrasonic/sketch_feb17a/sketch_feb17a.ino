/*
********************************************
14CORE ULTRASONIC DISTANCE SENSOR CODE TEST
********************************************
*/
#define TRIGGER 5
#define ECHO 4

#define D0 16
#define LED D0


// NodeMCU Pin D1 > TRIGGER | Pin D2 > ECHO

void setup() {
  
  Serial.begin (9600);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  data();
  delay(1000);
}

void data(){
  long duration;
  float distance;
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = duration * 0.034 / 2;
  
  Serial.print(distance);
  Serial.println("Centimeter:");

  if(distance <= 15)
  {
    for(int i=0; i<15; i++){
      digitalWrite(LED, HIGH);
      delay(70);
      digitalWrite(LED, LOW);
      delay(70);
    }
  }
  else if(distance <= 30)
  {
    for(int i=0; i<10; i++){
      digitalWrite(LED, HIGH);
      delay(100);
      digitalWrite(LED, LOW);
      delay(100);
    }
  }
  else if(distance <= 45)
  {
     for(int i=0; i<5; i++){
      digitalWrite(LED, HIGH);
      delay(200);
      
      digitalWrite(LED, LOW);
      delay(200);
     } 
  }
  else if(distance <= 50)
  {
     for(int i=0; i<3; i++){
      digitalWrite(LED, HIGH);
      delay(300);
      digitalWrite(LED, LOW);
      delay(300);
     } 
  }
  else
  {
    digitalWrite(LED, LOW);
  }
}

