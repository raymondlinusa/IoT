#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char auth[] = "s6mDwHZYmA2e7QZC4JFpTnbuaZfesss0";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Zn";
char pass[] = "zn123zn456";

#define DHTPIN 33
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define COLOR_GREEN "#00FF2F"
#define COLOR_RED "#FF0000"

//inisialisasi pin yang digunakan
WidgetLED lSuhu(V7);
WidgetLED lCahaya(V6);

int merah1 = 27;
int merah2 = 26;
int LDR = 32;

int suhu = 0;
int cahaya = 0;

int paramSuhu = 0;
int paramCahaya = 0;

int kondisi = 0;

//TH cahaya
BLYNK_WRITE(V4)
{
  paramCahaya = param.asInt();
  Serial.print("TH cahaya berubah dalam satuan analog : ");
  Serial.println(paramCahaya);
  paramCahaya = (param.asInt()*0.009768) + 10;
  Serial.print("TH cahaya berubah : ");
  Serial.println(paramCahaya);
}
//TH suhu
BLYNK_WRITE(V5)
{
  paramSuhu = param.asInt();
  Serial.print("TH suhu berubah : ");
  Serial.println(paramSuhu);
}

BLYNK_WRITE(V8)
{
  int pinValue = param.asInt();
  //jika btn start dalam kondisi ditekan / on
  if (pinValue == 1) {
    kondisi = 1;
  }
  //jika btn dalam kondisi mati / off
  else {
    kondisi = 0;
    digitalWrite(merah1, LOW);
    digitalWrite(merah2, LOW);
  }
}

void bacaSuhu() {
  delay(10000);
  suhu = dht.readTemperature();
  if (isnan(suhu)) {
    Serial.println("gagal baca nilai suhu");
  }
  else {
    Serial.print("nilai suhu : ");
    Serial.println(suhu);
    Blynk.virtualWrite(V0, suhu);
    Blynk.virtualWrite(V3, suhu);
  }
}

void bacaCahaya() {
  cahaya = analogRead(LDR);
  if (isnan(cahaya)) {
    Serial.println("gagal baca nilai cahaya");
  }
  else {
    cahaya = (0.009758 * cahaya) + 10;
    Serial.print("nilai cahaya : ");
    Serial.println(cahaya);
    Blynk.virtualWrite(V1, cahaya);
    Blynk.virtualWrite(V2, cahaya);
  }
}

void nyalaLed(int suhu, int cahaya) {
  //pengecekan TH suhu
  lSuhu.on();
  if (suhu > paramSuhu) {
    lSuhu.setColor(COLOR_RED);
    Serial.println("LED suhu : MERAH");
    digitalWrite(merah1, HIGH);
  }
  else {
    lSuhu.setColor(COLOR_GREEN);
    Serial.println("LED suhu : HIJAU");
    digitalWrite(merah1, LOW);
  }

  //pengecekan TH cahaya
  lCahaya.on();
  if (cahaya > paramCahaya) {
    lCahaya.setColor(COLOR_RED);
    Serial.println("LED cahaya : MERAH");
    digitalWrite(merah2, HIGH);
  }
  else {
    lCahaya.setColor(COLOR_GREEN);
    Serial.println("LED cahaya : HIJAU");
    digitalWrite(merah2, LOW);
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  dht.begin();
  pinMode(merah1, OUTPUT);
  pinMode(merah2, OUTPUT);


  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 100, 7), 8080);
}

void loop()
{
  Blynk.run();
  if (kondisi == 1) {
    bacaSuhu();
    bacaCahaya();
    nyalaLed(suhu, cahaya);
  }
}
