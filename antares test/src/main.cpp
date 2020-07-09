#include<Arduino.h>
#include<AntaresESP8266HTTP.h>   // Inisiasi library HTTP Antares

#define ACCESSKEY "017c7e810b75e05b:0932f32db0c81348"       // Ganti dengan access key akun Antares anda
#define WIFISSID "TP-LINK 2"         // Ganti dengan SSID WiFi anda
#define PASSWORD "asdf1234"     // Ganti dengan password WiFi anda

#define projectName "NewApp"   // Ganti dengan application name Antares yang telah dibuat
#define deviceName "smartHome"     // Ganti dengan device Antares yang telah dibuat

AntaresESP8266HTTP antares(ACCESSKEY);    // Buat objek antares

void setup() {
  Serial.begin(115200);     // Buka komunikasi serial dengan baudrate 115200
  antares.setDebug(true);   // Nyalakan debug. Set menjadi "false" jika tidak ingin pesan-pesan tampil di serial monitor
  antares.wifiConnection(WIFISSID,PASSWORD);  // Mencoba untuk menyambungkan ke WiFi
}

void loop() {
  // Isi variabel dengan nilai acak, dengan tipe data yang berbeda
  int temp = random(25,30) ;
  int hum = random(75,90);
  float windsp = float(random(20, 30))/3.33;
  float rainlv = float(random(0, 20))/6.99;
  String lat = "-6.8718189";
  String lon = "107.5872477";

  // Memasukkan nilai-nilai variabel ke penampungan data sementara
  antares.add("temperature", temp);
  antares.add("humidity", hum);
  antares.add("wind_speed", windsp);
  antares.add("rain_level", rainlv);
  antares.add("latitude", lat);
  antares.add("longitude", lon);

  // Kirim dari penampungan data ke Antares
  antares.send(projectName, deviceName);
  delay(10000);
}