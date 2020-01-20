#include <stdio.h>
#include <Wire.h>
#include <SPI.h>
#include <ESP8266WiFi.h>              // Include main library
//------------------- Connect ----------
const char* ssid     = "TiFF";    // Set your SSID
const char* password = "12345678";  // Set your password
const char* thing = "Gbp_Project";   // Set Thing name
const char* host = "dweet.io";        // Set host  

//------------------ ตรวจจับการเอียง-สั่น --------
const int switchTilt = D2;
String Tilteds ="";
//------------------ วัดระยะทาง --------------
long distance = 0;
#define trigPin D5
#define echoPin D6
//------------------- LED & Sound------------
const int LedR = D1; 
const int LedG = D3;
const int LedY = D4; 
#define Sound D8 
int val = 0;
void setup(){
   analogWrite(Sound,255);
   pinMode (LedR,OUTPUT);
   pinMode (LedY,OUTPUT);
   pinMode (LedG,OUTPUT);
   pinMode (switchTilt,INPUT);
   pinMode(trigPin, OUTPUT);
   pinMode(echoPin, INPUT);;
   delay(500);
   Serial.begin(9600);
   delay(10);
   Serial.println();
   Serial.println();
   Serial.print("Connecting to ");
   Serial.println(ssid);
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) 
   {
     delay(500);
     Serial.print(".");                // Print WiFi status
   }
   Serial.println("");
   Serial.println("WiFi connected");   // Print connect status
   Serial.println("IP address: ");     
   Serial.println(WiFi.localIP());     // Print IP address
}


void loop(){
  String Tilteds_value =Tilted();
  Serial.println(Tilteds_value);
//  distances();
  String Distances_value = distances(); 
  Serial.println(distance + " Cm.");
  WiFiClient client;
  const int httpPort = 80;            // Set HTTP port
//  Serial.println(host);
//  Serial.println(httpPort);
  if (!client.connect(host, httpPort))// Test connection 
  {
    Serial.println("connection failed"); // Print connection fail message
    return;
  }else{
     Serial.println("connection");
  }
  String url =  String("/dweet/for/") + thing + "?"; // Set message
  url += "Tilted=";
  url += Tilteds_value;
  url += "&Distance=";
  url += Distances_value;
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: keep-alive\r\n" +
               "Cache-Control: max-age=0\r\n\r\n");
  Serial.print("URL : ");         
  Serial.println(url);              // Print URL 
//  
//  while (client.available()) 
//  {
//    String line = client.readStringUntil('\r');
//    Serial.print(line);
//  }
//  Serial.println();
//  Serial.println("closing connection"); // Print closing status
  delay(3000);
}

String Tilted(){
   val = digitalRead(switchTilt);
   if (val == HIGH){
//      digitalWrite(LedR,HIGH);
      Tilteds ="No_Tilted";
//      Serial.println(Tilteds);
   }
   else {
//   digitalWrite(LedR,LOW);
      Tilteds ="Tilted";
//      Serial.println(Tilteds);
   }
   return Tilteds;
}

String distances(){
  distance = read_hc_sr04();
  if (distance <= 200 && distance >= 0){
      Serial.print("Distance: ");
      Serial.println(String(distance) + " Cm.");
      if (distance <= 44){
          LOW_distance(distance);
      }
      else if (distance <= 88) {
          Medium_distance(distance);
      }
      else if (distance > 88) {
          High_distance(distance);
      }
  }
  return String(distance);
}

long read_hc_sr04(){
  long duration, distance = 0;
  digitalWrite(trigPin, LOW);
  delay(500);
  digitalWrite(trigPin, HIGH);
  delay(500);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  return distance;
}

void LOW_distance(long distance){
  digitalWrite(LedG,1);
  delay(1000);
  digitalWrite(LedG,0);
}

void Medium_distance(long distance){
  digitalWrite(LedY,1);
  delay(1000);
  digitalWrite(LedY,0);
}

void High_distance(long distance){
  digitalWrite(LedR,1);
  delay(1000);
  digitalWrite(LedR,0);
}
