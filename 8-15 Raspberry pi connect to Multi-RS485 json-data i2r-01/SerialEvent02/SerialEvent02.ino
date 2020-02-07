#include <ArduinoJson.h>
//json을 위한 설정
StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;

int no=2,power=0;
const int ledPin =  D3;// the number of the LED pin
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 3000;           // interval at which to blink (milliseconds)
// rs232 통신을 위한 설정
String inputString = "";         // 수신 데이타 저장
boolean stringComplete = false;  // 수신 완료되면 true
String s;  // 보내는 문자열
char payload[100];

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 0);
  // initialize serial:
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("start");
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    //Serial1.println("test");
  }
  serialEvent();
}

void serialEvent() {
  if (stringComplete) {
    for(int i=0;i<inputString.length();i++)
          payload[i]=inputString.charAt(i);
    //Serial.println(payload);
    deserializeJson(doc,payload);
    root = doc.as<JsonObject>();
    int noIn = root["no"];
    int powerIn = root["power"];
    if(no==noIn) {
      digitalWrite(ledPin, powerIn);
      Serial1.println("{\"no\":2, \"msg\":\"ok\"}");
      char buff[50];
      sprintf(buff,"no:%d, power:%d",noIn,powerIn);
      Serial.println(buff);
    }
    // clear the string:
    inputString = "";
    stringComplete=false;
  }
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    inputString += inChar;
    //Serial.println(inChar);
    if (inChar == '}') {
      stringComplete = true;
    }
  }
}
