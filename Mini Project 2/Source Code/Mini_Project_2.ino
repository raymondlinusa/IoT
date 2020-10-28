#include "DHT.h"
#define DHTPIN 16
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//pin led
int Merah = 27;
int Kuning = 26;
int Hijau = 25;

//pin button
int button = 33;

//btn status
int stat = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(Merah, OUTPUT);
  pinMode(Kuning, OUTPUT);
  pinMode(Hijau, OUTPUT);
  pinMode(button, INPUT);
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  int pencet = digitalRead(button);
  
  //jika tombol di pencet maka mengubah status btn
  //ini kondisi dari off ke on
  if(pencet == HIGH && stat == 0){
    stat++;
    Serial.println("button ditekan, mulai membaca suhu....");
    delay(1000);
  }
  //ini dari kondisi on ke off
  else if(pencet == HIGH && stat == 1){
    stat--;
    Serial.println("button ditekan, mematikan....");
    delay(3000);
    digitalWrite(Hijau, LOW);
    digitalWrite(Kuning, LOW);
    digitalWrite(Merah, LOW);
  }
  //setelah kondisi on, selalu cek suhu
  else if(stat == 1){
    bacaSuhu();
  }
}

void bacaSuhu(){
  delay(10000); // delay 10 detik, sebab sensor perlu waktu 10 detik untuk mengukur suhu
  
  float suhu = dht.readTemperature();
  //jika terjadi error saat membaca suhu
  if(isnan(suhu)){
    Serial.println("baca suhu gagal");
  }
  //jika berhasil
  else{
    Serial.print("suhu : ");
    Serial.print(suhu);
    Serial.println("° Celcius");
    nyalaLed(suhu); 
  }
}

void nyalaLed(float suhu){
  //nyala 3 LED jika suhu dibawah 35 derajat celcius
  if(suhu < 35){
    digitalWrite(Hijau, HIGH);
    digitalWrite(Kuning, LOW);
    digitalWrite(Merah, LOW);
  }
  //nyala 2 LED jika suhu diatas 35 dan dibawah 50 derajat celcius
  else if(suhu >= 35 && suhu <= 50){
    digitalWrite(Hijau, HIGH);
    digitalWrite(Kuning, HIGH);
    digitalWrite(Merah, LOW);
  }
  //nyala 3 LED jika suhu diatas 50 derajat celcius
  else if(suhu >= 50){
    digitalWrite(Hijau, HIGH);
    digitalWrite(Kuning, HIGH);
    digitalWrite(Merah, HIGH);
  }
}
