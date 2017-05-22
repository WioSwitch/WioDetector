/* 
  WioDetector V1.0 (ESP32 Version)
  Using a Magnet and a Hall Effect Magnetic Sensor Module To Monitor Garage Door Status
  Written by Chiming Lee 05-22-2017
*/

#include <WiFi.h>

WiFiServer server(80);

IPAddress ip(192, 168, 1, 201);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

const char* ssid = "SSID";
const char* password = "PASSWORD";

String readStrings = "";

int pinG5 = 5; // G5 = ESP8266 D1
int pinG15 = 15; // G15 = ESP8266 D8
int G15Status = 0;

void setup() {
  pinMode(pinG5, OUTPUT);
  Serial.begin(115200);
  WiFi.enableSTA(true);
  //WiFi.enableAP(false);
  delay(2000);
  WiFi.config(ip, gateway, subnet);
  Serial.print("Connecting ");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(800);
  }
  Serial.println(" OK");
  Serial.println("WioDetector started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("WioServer with a Hall Effect Magnetic Sensor Module = WioDetector");
  Serial.println("");
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }
  
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request
  if (request.indexOf("/") > 0 ) {
    readHallSensor();
    delay(1);
   
    if (G15Status == 0){ 
      //Return the response
      client.println("HTTP/1.1 200 OK\r\n");
      //client.println("Content-Type: text/html\r\n");      
      client.println("{\"vout\": 0}");
      digitalWrite(pinG5,HIGH);
      Serial.println("{\"vout\": 0}");
      Serial.println("Garage is Closed");
      Serial.println("");
      delay(1);
    }

    // The Hall Effect Magnetic Sensor Module default output is HIGH
    // No strong magnetic field around 
    if (G15Status == 1){
      //Return the response
      client.println("HTTP/1.1 200 OK\r\n");
      //client.println("Content-Type: text/html\r\n"); 
      client.println("{\"vout\": 1}"); // the sensor default output is HIGH
      digitalWrite(pinG5,LOW);
      Serial.println("{\"vout\": 1}");
      Serial.println("Garage is Opened");
      Serial.println("");
      delay(1);     
    } 
  }
}

void readHallSensor() {
  G15Status = digitalRead(pinG15);
}
