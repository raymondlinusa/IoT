//pin led
int Merah = 27;
int Kuning = 26;
int Hijau = 25;

//parameter untuk waktu
unsigned long waktu = 0;
//parameter lampu
int param = 0;

//durasi lampu hidup
const long dMerah = 8000;     //8 detik
const long dKuning = 2000;    //2 detik
const long dHijau = 4000;     //4 detik

//3 fungsi untuk nyalain lampu sesuai judul
void nyalaMerah(){
  digitalWrite(Merah, HIGH);
  digitalWrite(Kuning, LOW);
  digitalWrite(Hijau, LOW);
}
void nyalaKuning(){
  digitalWrite(Merah, LOW);
  digitalWrite(Kuning, HIGH);
  digitalWrite(Hijau, LOW);
}
void nyalaHijau(){
  digitalWrite(Merah, LOW);
  digitalWrite(Kuning, LOW);
  digitalWrite(Hijau, HIGH);
}

void setup() {
  // set digital pin Merah, Kuning, Hijau jadi OUT
  pinMode(Merah, OUTPUT);
  pinMode(Kuning, OUTPUT);
  pinMode(Hijau, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //variabel yang menyimpan waktu terbaru
  unsigned long sekarang = millis();
  
  //awal-awal nyalain merah dulu
  if(param == 0){
    nyalaMerah();
    param++;
    waktu += dMerah;
  }
  else{
    //state kuning
    if(param == 1 && waktu - sekarang == 0){
      //kondisi kuning
      nyalaKuning();
      waktu += dKuning;
      param++;
    }
    //state hijau
    else if(param == 2 && waktu - sekarang == 0){
      //kondisi hijau
      nyalaHijau();
      waktu += dHijau;
      param++;
    }
    //state merah
    else if(param == 3 && waktu - sekarang == 0){
      //kondisi merah
      nyalaMerah();
      waktu += dMerah;
      param = 1;
    }
  }
}
