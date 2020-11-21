#include <WiFi.h> 
#include <MQTT.h>
#include <ESP32Servo.h>

const char ssid[] = "Zn";
const char pass[] = "zn123zn456";

Servo myservo;
int servo = 27;

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("ESP32s", "2a036196", "5add527a47f4ae11")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/servoCommand");
  client.subscribe("/servoStatus");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  if(topic.equals("/servoCommand")){
    //mengubah nilai payload dari string ke int
    int nilaiSudut = payload.toInt();
    myservo.write(nilaiSudut);
    String infoServo = "Servo berputar hingga : "+payload+" derajat";
    Serial.println(infoServo);
    client.publish("/servoStatus", infoServo);
}


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  myservo.attach(servo);
  client.begin("broker.shiftr.io", net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }
}
