#include <Arduino.h>
#include <AntaresESP8266HTTP.h>   // Inisiasi library HTTP Antares

#define ACCESSKEY "017c7e810b75e05b:0932f32db0c81348"       // Ganti dengan access key akun Antares anda
#define WIFISSID "TP-LINK 2"         // Ganti dengan SSID WiFi anda
#define PASSWORD "asdf1234"     // Ganti dengan password WiFi anda

#define projectName "NewApp"   // Ganti dengan application name Antares yang telah dibuat
#define deviceName "smartHome"     // Ganti dengan device Antares yang telah dibuat

AntaresESP8266HTTP antares(ACCESSKEY);    // Buat objek antares

void setup() {
  Serial.begin(9600);     // Buka komunikasi serial dengan baudrate 115200
  antares.setDebug(true);   // Nyalakan debug. Set menjadi "false" jika tidak ingin pesan-pesan tampil di serial monitor
  antares.wifiConnection(WIFISSID,PASSWORD);  // Mencoba untuk menyambungkan ke WiFi
}

void loop() {
  // Mengambil data terakhir ke penampungan data
  antares.get(projectName, deviceName);

  // Mendapatkan data individu
  int temp = antares.getInt("temperature");
  int hum = antares.getInt("humidity");
  float windsp = antares.getFloat("wind_speed");
  float rainlv = antares.getFloat("rain_level");
  String lat = antares.getString("latitude");
  String lon = antares.getString("longitude");

  // Kirim ke serial monitor
  Serial.println("Temperature: " + String(temp));
  Serial.println("Humidity: " + String(hum));
  Serial.println("Wind speed: " + String(windsp));
  Serial.println("Rain level: " + String(rainlv));
  Serial.println("Latitude: " + lat);
  Serial.println("Longitude: " + lon);
  delay(10000);
}