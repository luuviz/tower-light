#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "Tower-Light";
const char* password = "sa123456";
ESP8266WebServer server(80);

const int led = 4;
const int relay=5; /*wemos di 1*/

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "It's work");
  digitalWrite(led, 0);
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void){
  pinMode(led, OUTPUT);
  pinMode(relay,OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);
  Serial.println("");


  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

 

  server.on("/", handleRoot);

  server.on("/red", [](){
    server.send(200, "application/json", "{\"status\":\"ok\"}");
    digitalWrite(relay,HIGH);
  });
  server.on("/yellow", [](){
    server.send(200, "application/json", "{\"status\":\"ok\"}");
    digitalWrite(relay,LOW);
  });
  server.on("/status", [](){
    char buffer[50];
    if(digitalRead(relay)==1){
      sprintf(buffer,"%s%s%s", "{\"status\":\"ok\",\"result\":","red","}");
    }else{
      sprintf(buffer,"%s%s%s", "{\"status\":\"ok\",\"result\":","yellow","}");
    }    
    server.send(200, "application/json",buffer );
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}

