#include <Arduino.h>
#include <WiFi.h>

String translateEncryptionType(wifi_auth_mode_t encryptionType);
void scanNetworks();
void connectToNetwork();

const char *ssid = "TP-LINK 2";
const char *pwd = "asdf1234";
void setup()
{
  Serial.begin(9600);
  scanNetworks();
  connectToNetwork();
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());
}

void loop()
{
  // put your main code here, to run repeatedly:
}

String translateEncryptionType(wifi_auth_mode_t encryptionType) // Check WiFi Encryption
{
  switch (encryptionType)
  {
  case WIFI_AUTH_OPEN:
    return "Open";
    break;
  case WIFI_AUTH_WEP:
    return "WEP";
    break;
  case WIFI_AUTH_WPA_PSK:
    return "WPA_PSK";
    break;
  case WIFI_AUTH_WPA2_PSK:
    return "WPA2_PSK";
    break;
  case WIFI_AUTH_WPA_WPA2_PSK:
    return "WPA_WPA2_PSK";
    break;
  case WIFI_AUTH_WPA2_ENTERPRISE:
    return "WPA2_ENTERPRISE";
    break;
  case WIFI_AUTH_MAX:

    break;

  default:
    return "Invalid Type Encryption";
    break;
  }
}
void scanNetworks() // Check Available WiFi
{
  int numberOfNetworks = WiFi.scanNetworks();
  Serial.println("Number of Networks Found");
  Serial.println(numberOfNetworks);
  for (int i = 0; i < numberOfNetworks; i++)
  {
    Serial.print("Network Name");
    Serial.println(WiFi.SSID(i));
    Serial.print("Signal Strength");
    Serial.println(WiFi.RSSI(i));
    Serial.print("Mac Address: ");
    Serial.println(WiFi.BSSIDstr(i));
    Serial.print("Encryption Type: ");
    String encryptionTypeDescription =
        translateEncryptionType(WiFi.encryptionType(i));
    Serial.println(encryptionTypeDescription);
    Serial.println("*****************");
  }
}
void connectToNetwork() //Connect to Specific Network
{
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print("Enstablishing Network");
    Serial.println(ssid);
  }
  Serial.println("Connected to Networks");
}