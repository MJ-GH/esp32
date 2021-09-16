
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <WiFi.h>
#include <Wire.h>

#define ds1621 0x90 >> 1

const char* ssid = "A34Ybtt6";                // Only supports 2.4 GHz WiFi
const char* password = "MilleErHurtig";

AsyncWebServer server(80);                    // Creates an AsyncWebServer object on port 80.

int LED_BUILTIN = 2;
//bool fahrenheit = false;

void setup()
{
  pinMode (LED_BUILTIN, OUTPUT);

  Serial.begin(115200);

  Wire.begin();
  Wire.beginTransmission(ds1621);             // Connect to DS1621
  Wire.write(0xAC);                           // Access config
  Wire.write(0x02);                           // Set for continuous conversion
  Wire.beginTransmission(ds1621);             // Restart
  Wire.write(0xEE);                           // Start conversions
  Wire.endTransmission();

  //WiFi.mode(WIFI_AP);                         // Configures the ESP32 as a soft Access Point, "soft" because it doesn't connect further to a wired network like a router.
  //WiFi.softAP(ssid, password);                // "ssid" is the name of the ESP32, "password" is the password required to connect to the AP. Set to NULL if no password is required.

  server.on("/temp", HTTP_GET, [](AsyncWebServerRequest * req) {
    req->send(200, "text/plain", String(readTemp()));
  });

  //  server.on("/change", HTTP_GET, [](AsyncWebServerRequest * req) {
  //    req->send(200, "text/plain", changeDegrees());
  //  });

  // Connect to WiFi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);                 // Connecting to home WiFi
  while (WiFi.status() != WL_CONNECTED) {     // While it's trying to connect do this:
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(20);
    digitalWrite(LED_BUILTIN, LOW);
  }

  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());             // Print IP
  Serial.println();

  server.begin();                             // Starts the server
}

void loop()
{
  Serial.println(readTemp());
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);
  digitalWrite(LED_BUILTIN, LOW);
}

float readTemp() {

  int8_t firstByte;
  int8_t secondByte;
  float temp = 0;

  delay(1000);                                // Give time for measurement

  Wire.beginTransmission(ds1621);
  Wire.write(0xAA);                           // Read temperature
  Wire.endTransmission();
  Wire.requestFrom(ds1621, 2);                // Request two bytes from DS1621 (0.5 deg. resolution)

  firstByte = Wire.read();                    // Get first byte
  secondByte = Wire.read();                   // Get second byte

  temp = firstByte;

  if (secondByte)                             // If there is a 0.5 deg. difference
    temp += 0.5;

  //  if (fahrenheit) {
  //    temp = temp * 1.8 + 32;
  //  }

  return temp;
}

//String changeDegrees() {
//  if (fahrenheit == false) {
//    fahrenheit = true;
//    return String(fahrenheit);
//  }
//  else {
//    fahrenheit = false;
//    return String(fahrenheit);
//  }
//}
