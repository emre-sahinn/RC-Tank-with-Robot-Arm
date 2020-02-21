#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

const uint8_t in1 = 25;
const uint8_t in2 = 14;
const uint8_t in3 = 27;
const uint8_t in4 = 26;
const uint8_t en1 = 13;
const uint8_t en2 = 12;

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8; //0-255


void setup() {
  Serial.begin(115200);
  SerialBT.begin("RAS TANK");
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(en1, ledChannel);
  ledcAttachPin(en2, ledChannel + 2);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  ledcWrite(ledChannel, 200);
  ledcWrite(ledChannel + 2, 200);
}

String msg = "";
void loop() {
  
  while (SerialBT.available()>0) {
    msg = char(SerialBT.read());
  }
  Serial.println(msg);
  if (msg == "F") {
    digitalWrite(in1, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in4, LOW);
  } else if (msg == "B") {
    digitalWrite(in1, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in4, HIGH);
  } else if (msg == "R") {
    digitalWrite(in1, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in4, HIGH);
  } else if (msg == "L") {
    digitalWrite(in1, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in2, HIGH);
    digitalWrite(in4, LOW);
  }
  if  (msg == "S") {
    digitalWrite(in1, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in4, LOW);
    delay(20);
  }

  /*
    digitalWrite(in1, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in4, LOW);*/
}
