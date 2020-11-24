#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ESP32Servo.h>
#include <HCSR04.h>

#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

Servo myservo;
int pinServo = 27;
int pos = 0;
UltraSonicDistanceSensor sensorJarak(26,25);

FirebaseData firebaseData;

String path = "/node1";
int jarakLama;
int jarakBaru;            

int on_off = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  myservo.attach(pinServo);
  initWifi();
  jarakLama = sensorJarak.measureDistanceCm();
}

void loop() {
  //cek status sistem pada firebase, jika 1 maka hidup dan jika 0 maka mati
  Firebase.getInt(firebaseData, path + "/statusSistem");
  on_off = firebaseData.intData();
  Serial.print("kondisi sistem ");
  Serial.println(on_off);
  if(on_off == 1){
    //ambil nilai dari sensor ultrasonic dan kirim ke firebase
    jarakBaru = sensorJarak.measureDistanceCm();
    Serial.println(jarakBaru);

    if(jarakBaru != jarakLama){
      Firebase.setInt(firebaseData, path + "/jarak", jarakBaru);
      jarakLama = jarakBaru;
      
      //lakukan pengecekan jika jarak kurang dari 10cm maka akan menggerakan servo
      if(jarakLama < 10 && jarakLama != -1){
        if(pos == 0) {
          for(pos = 0; pos <= 180; pos += 1){
            myservo.write(pos); //prosedur penulisan data PWM ke motor servo
            delay(15); //waktu tunda 15 ms                 
          }
           pos = 180;
        }
        Firebase.setInt(firebaseData, path + "/kondisiServo", 1);
        Serial.println("Servo bergerak");
        delay(2000);
      }
      else {
        if(pos == 180) {
          for(pos = 180; pos>=0; pos-=1){                              
            myservo.write(pos);                 
            delay(15);                        
           }
           pos = 0;
        }
        Firebase.setInt(firebaseData, path + "/kondisiServo", 0);
        Serial.println("Servo idle");
      }
    }
    else {
      Firebase.setInt(firebaseData, path + "/kondisiServo", 0);
      Serial.println("Servo idle");
    }
  }
  else{
    
  }
}

void initWifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}
