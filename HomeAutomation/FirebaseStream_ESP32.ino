#include <IOXhop_FirebaseESP32.h>
//#include <WebServer.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#define WIFI_SSID "ARHAM7A2"
#define WIFI_PASSWORD "arham@1207"
#define FIREBASE_HOST "home-1823d.firebaseio.com"
#define FIREBASE_AUTH "uCyg16wYUvJy2kq3gORudSWSnsGTzkpUj86eCjzy"
#include "time.h"

#include <PubSubClient.h>
const char* mqttServer = "m16.cloudmqtt.com";
const int mqttPort = 15034;
const char* mqttUser = "dtvunaab";
const char* mqttPassword = "JVvPM5IvTY5B";
WiFiClient espClient;
PubSubClient client(espClient);

const char* ntpServer = "1.asia.pool.ntp.org";
const long  gmtOffset_sec = 60*60*4.5;
const int   daylightOffset_sec = 3600;
char timeStringBuff1[25]; //50 chars should be enough
char timeStringBuff2[25]; //50 chars should be enough
//#define NAME_TO_RESOLVE "esp32.local"
//using namespace mDNSResolver;
//WiFiClient wifiClient;ss
//WiFiUDP udp;
//Resolver resolver(udp);
AsyncWebServer server(80);
int pin1 = 4;
int pin2 = 15;
int read1 = 2;
int read2 = 5;
bool verify1=true;
bool verify2=true;
int led2=0;
int led1=0;
int led=0;
char JSONMessage_true[] = " {\"fan\": true, \"light\": true}"; //Original message
char JSONMessage_false[] = " {\"fan\":false, \"light\": false}"; //Original message
bool device0=false;
bool device1=false;
bool device10=false;
bool device11=false;
bool device20=false;
bool device21=false;
bool counter=false;
int counterNo=0;
StaticJsonBuffer<300> JSONBuffer;                         //Memory pool
JsonObject& parsed_true = JSONBuffer.parseObject(JSONMessage_true); //Parse message
JsonObject& parsed_false = JSONBuffer.parseObject(JSONMessage_false); //Parse message

void printLocalTime()
{
  time_t rawtime;
  struct tm timeinfo1;
  struct tm timeinfo2;
  if(!getLocalTime(&timeinfo1))
  {
    Serial.println("Failed to obtain time");  
    return;
  }
  if(!getLocalTime(&timeinfo2))
  {
    Serial.println("Failed to obtain time");  
    return;
  }
  
  strftime(timeStringBuff1, sizeof(timeStringBuff1), "%H:%M:%S", &timeinfo1);
  strftime(timeStringBuff2, sizeof(timeStringBuff2), "%y-%m-%d", &timeinfo2);
  //print like "const char*"
//  Serial.println(timeStringBuff);

  //Optional: Construct String object 
  String asString(timeStringBuff1);
//  String asString1(timeStringBuff2);
//  return {timeStringBuff1,timeStringBuff2};
return;
  
}

void setup() {
  Serial.begin(115200);
  pinMode(pin1,OUTPUT);
  pinMode(pin2,OUTPUT);
  pinMode(read1,INPUT);
  pinMode(read2,INPUT);

  // connect to wifi.
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.setHostname("esp32_host");
//  MDNS.begin("esp32", WiFi.localIP());
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
      Serial.println("connected");  
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.subscribe("mesg");  
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.getHostname());
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  printLocalTime();
  
//  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
//  Firebase.stream("Home_status", [](FirebaseStream stream) {
//    String eventType = stream.getEvent();   
//      Serial.println("eventType "+eventType);  
//      String path = stream.getPath();
//      String data1 = stream.getDataString();
//      Serial.println("path "+path);  
//      Serial.println("data1 "+data1);
//    if (eventType == "put" or eventType=="patch") {
////      String path = stream.getPath();
////      String data1 = stream.getDataString();
////      Serial.println("Changed1 "+path);  
////      Serial.println("Changed2 "+data1);  
//
//      if(path == "/device/light" or path =="/device"){
//        if(data1=="true" or data1=="{\"light\":true}"){
//            digitalWrite(pin1, HIGH);
//            Serial.println("Light ON");
////            counter==true
//        }else if(data1=="false" or data1=="{\"light\":false}"){
//            digitalWrite(pin1, LOW);
//            Serial.println("Light OFF");
//        }
//      }
//      if(path.substring(12) =="/device/fan" or path =="/device"){
//      if(data1=="true" or data1=="{\"fan\":true}"){
//        digitalWrite(pin2, HIGH);
//        Serial.println("Fan ON");
//       }else if(data1=="false" or data1=="{\"fan\":false}"){
//        digitalWrite(pin2, LOW);
//        Serial.println("Fan OFF");
//       }
//      }
//    }
//  });
//  Serial.print(" ");  
//  if (MDNS.begin("esp32")) {
//    Serial.println("MDNS responder started");
//  }
//  server.on("/device/0/", HTTP_GET,[](AsyncWebServerRequest *request) {
//    request->send(200, "text/plain", "OFF");
//    digitalWrite(pin1, LOW); 
//    digitalWrite(pin2, LOW);
//    led1=0;
//    led2=0;
//    int paramsNr = request->params();
//    Serial.println(paramsNr);
//    for(int i=0;i<paramsNr;i++){
//     AsyncWebParameter* p = request->getParam(i);
//     Serial.print("Param name: ");
//     Serial.println(p->name());
//     Serial.print("Param value: ");
//     Serial.println(p->value());
//     counterNo=(p->value().toInt());
//     Serial.println(counterNo);
//      counter=true;
//     }
////    led=0;
////    led1=0;
//    device0=true;
//    Serial.println("OFF");
//  });
//  server.on("/device/1/", HTTP_GET,[](AsyncWebServerRequest *request) {
//    request->send(200, "text/plain", "ON");
//    digitalWrite(pin1, HIGH);
//    digitalWrite(pin2, HIGH);
//    led1=1;
//    led2=1;
//    int paramsNr = request->params();
//    Serial.println(paramsNr);
//    for(int i=0;i<paramsNr;i++){
//     AsyncWebParameter* p = request->getParam(i);
//     Serial.print("Param name: ");
//     Serial.println(p->name());
//     Serial.print("Param value: ");
//     Serial.println(p->value());
//     counterNo=(p->value().toInt());
//     Serial.println(counterNo);
//      counter=true;
//     }
////    led=1;
////    led1=1;
//    device1=true;
//    Serial.println("ON");
//  });
//   server.on("/device1/1/", HTTP_GET,[](AsyncWebServerRequest *request) {
//    request->send(200, "text/plain", "Fan ON");
//    digitalWrite(pin1, HIGH);
//    led1=1;
//    int paramsNr = request->params();
//    Serial.println(paramsNr);
//    for(int i=0;i<paramsNr;i++){
//     AsyncWebParameter* p = request->getParam(i);
//     Serial.print("Param name: ");
//     Serial.println(p->name());
//     Serial.print("Param value: ");
//     Serial.println(p->value());
//     counterNo=(p->value().toInt());
//     Serial.println(counterNo);
//      counter=true;
//     }
////    led=1;
//    device11=true;
//    Serial.println("Light ON");
//  });
//  server.on("/device1/0/", HTTP_GET,[](AsyncWebServerRequest *request) {
//    request->send(200, "text/plain", "Fan OFF");
//    digitalWrite(pin1, LOW);
//    led1=0;
//    int paramsNr = request->params();
//    Serial.println(paramsNr);
//    for(int i=0;i<paramsNr;i++){
//     AsyncWebParameter* p = request->getParam(i);
//     Serial.print("Param name: ");
//     Serial.println(p->name());
//     Serial.print("Param value: ");
//     Serial.println(p->value());
//     counterNo=(p->value().toInt());
//     Serial.println(counterNo);
//      counter=true;
//     }
////    led=1;
//    device10=true;
//    Serial.println("Light OFF");
//  });
//
//   server.on("/device2/1/", HTTP_GET,[](AsyncWebServerRequest *request) {
//    request->send(200, "text/plain", "Light ON");
//    digitalWrite(pin2, HIGH);
//    led2=1;
//    int paramsNr = request->params();
//    Serial.println(paramsNr);
//    for(int i=0;i<paramsNr;i++){
//     AsyncWebParameter* p = request->getParam(i);
//     Serial.print("Param name: ");
//     Serial.println(p->name());
//     Serial.print("Param value: ");
//     Serial.println(p->value());
//     counterNo=(p->value().toInt());
//     Serial.println(counterNo);
//      counter=true;
//     }
////    led=1;
//    device21=true;
//    Serial.println("Fan ON");
//  });  
//  server.on("/device2/0/", HTTP_GET,[](AsyncWebServerRequest *request) {
//    request->send(200, "text/plain", "Fan OFF");
//    digitalWrite(pin2, LOW);
//    led2=0;
//    int paramsNr = request->params();
//    Serial.println(paramsNr);
//    for(int i=0;i<paramsNr;i++){
//     AsyncWebParameter* p = request->getParam(i);
//     Serial.print("Param name: ");
//     Serial.println(p->name());
//     Serial.print("Param value: ");
//     Serial.println(p->value());
//     counterNo=(p->value().toInt());
//     Serial.println(counterNo);
//      counter=true;
//     }
////    led=1;
//    device20=true;
//    Serial.println("Fan OFF");
//  });
//  server.on("/set/counter/", HTTP_GET,[](AsyncWebServerRequest *request) {
//    int paramsNr = request->params();
//    Serial.println(paramsNr);
//    for(int i=0;i<paramsNr;i++){
//     AsyncWebParameter* p = request->getParam(i);
//     Serial.print("Param name: ");
//     Serial.println(p->name());
//     Serial.print("Param value: ");
//     Serial.println(p->value());
//     counterNo=(p->value().toInt());
//     Serial.println(counterNo);
//     }
//    counter=true;
//    Serial.println("Light OFF");
//    request->send(200, "text/plain", "Set Value");
//  });
//  server.begin();
//  MDNS.addService("http", "tcp", 80);
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String message;
  for (int i = 0; i < length; i++) {
//    Serial.print((char)payload[i]);
    message = message + (char)payload[i];  //Conver *byte to String
  }
  Serial.println(message);
  if(message == "all/0"){
      Serial.println("ALL devices are OFF");
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      device0=true;
      led1=0;
      led2=0;

  }else if(message == "all/1"){
      Serial.println("ALL devices are ON");
      digitalWrite(pin1, HIGH);
      digitalWrite(pin2, HIGH);
      device1=true;
      led1=1;
      led2=1;

  }else if(message == "fan/0"){
      Serial.println("Fan is OFF");
      digitalWrite(pin2, LOW);
      device20=true;
      led2=0;

  }else if(message == "light/0"){
      Serial.println("Light is ON");
      digitalWrite(pin1, LOW);
      device10=true;
      led1=0;

  }else if(message == "fan/1"){
      Serial.println("Fan is ON");
      digitalWrite(pin2, HIGH);
      device21=true;
      led2=1;


  }else if(message == "light/1"){
      Serial.println("Light is ON");
      digitalWrite(pin1, HIGH);
      device11=true;
      led1=1;

  }
  Serial.println();
  Serial.println("-----------------------");
}
void loop() {
    client.loop();
//    client.subscribe("fan");
//    client.subscribe("light");

//  server.handleClient();
//  switch_manual();
//
  int out_ir_1 = digitalRead(read1);
//  Serial.println(out_ir_3);
  if(out_ir_1==0 and verify1==true){
    Serial.println("");
    Serial.println("Switch ON");
    if (led1==0)
    {
          led1=1;
          printf("ON switch2\n");
//          digitalWrite(pin2, LOW); //pull SS slow to prep other end for transfer
          digitalWrite(pin1, HIGH); //pull SS slow to prep other end for transfer

      }else if (led1==1){
          led1=0;
          printf("OFF switch2\n");
//          digitalWrite(pin2, HIGH); //pull SS slow to prep other end for transfer
          digitalWrite(pin1, LOW); //pull SS slow to prep other end for transfer
      }
      verify1=false;
      delay(100);
  }else if(out_ir_1==1 and verify1==false){
    Serial.println("");
    Serial.println("Switch OFF");
    if (led1==0)
      {
          led1=1;
          printf("ON switch2\n");
//          digitalWrite(pin2, LOW); //pull SS slow to prep other end for transfer
          digitalWrite(pin1, HIGH); //pull SS slow to prep other end for transfer

      }else if (led1==1){
          led1=0;
          printf("OFF switch2\n");
//          digitalWrite(pin2, HIGH); //pull SS slow to prep other end for transfer
          digitalWrite(pin1, LOW); //pull SS slow to prep other end for transfer
      }
      verify1=true;
    delay(100);
  }


  int out_ir_2 = digitalRead(read2);
//  Serial.println(out_ir_3);
  if(out_ir_2==0 and verify2==true){
    Serial.println("");
    Serial.println("Switch ON");
    if (led2==0)
    {
          led2=1;
          printf("ON switch2\n");
          digitalWrite(pin2, HIGH); //pull SS slow to prep other end for transfer
//          digitalWrite(pin1, LOW); //pull SS slow to prep other end for transfer

      }else if (led2==1){
          led2=0;
          printf("OFF switch2\n");
          digitalWrite(pin2, LOW); //pull SS slow to prep other end for transfer
//          digitalWrite(pin1, HIGH); //pull SS slow to prep other end for transfer
      }
      verify2=false;
      delay(100);
  }else if(out_ir_2==1 and verify2==false){
    Serial.println("");
    Serial.println("Switch OFF");
    if (led2==0)
      {
          led2=1;
          printf("ON switch2\n");
          digitalWrite(pin2, HIGH); //pull SS slow to prep other end for transfer
//          digitalWrite(pin1, LOW); //pull SS slow to prep other end for transfer

      }else if (led2==1){
          led2=0;
          printf("OFF switch2\n");
          digitalWrite(pin2, LOW); //pull SS slow to prep other end for transfer
//          digitalWrite(pin1, HIGH); //pull SS slow to prep other end for transfer
      }
      verify2=true;
    delay(100);
  }

//  if(device0==true){
//        printLocalTime();
//        Firebase.set("Home_status/device/",parsed_false);
//        Firebase.set("Home_logs/"+String(timeStringBuff2)+"/"+timeStringBuff1,parsed_false);
//        device0=false;
//        Serial.println("Firebase device false");
//        if(counter==true){
//          Firebase.set("counter",counterNo);
//          counter=false;
//          Serial.println("Set Counter");
//        }
//
//  }
//  if(device1==true){
//        printLocalTime();
//        Firebase.set("Home_status/device/",parsed_true);
//        Firebase.set("Home_logs/"+String(timeStringBuff2)+"/"+timeStringBuff1,parsed_true);
//        device1=false;
//        Serial.println("Firebase device True");
//        if(counter==true){
//          Firebase.set("counter",counterNo);
//          counter=false;
//          Serial.println("Set Counter");
//        }
//
//  }
//  if(device10==true){
//        printLocalTime();
//        Firebase.setBool("Home_status/device/light",false);
//        Firebase.setBool("Home_logs/"+String(timeStringBuff2)+"/"+timeStringBuff1+"/light/",false);
//        device10=false;
//        Serial.println("Firebase fan false");
//        if(counter==true){
//          Firebase.set("counter",counterNo);
//          counter=false;
//          Serial.println("Set Counter");
//        }
//
//  }
//  if(device11==true){
//        printLocalTime();
//        Firebase.setBool("Home_status/device/light",true);
//        Firebase.setBool("Home_logs/"+String(timeStringBuff2)+"/"+timeStringBuff1+"/light/",true);
//        device11=false;
//        Serial.println("Firebase fan True");
//        if(counter==true){
//          Firebase.set("counter",counterNo);
//          counter=false;
//          Serial.println("Set Counter");
//        }
//
//  }
//  if(device20==true){
//        printLocalTime();
//        Firebase.setBool("Home_status/device/fan",false);
//        Firebase.setBool("Home_logs/"+String(timeStringBuff2)+"/"+timeStringBuff1+"/fan/",false);
//        device20=false;
//        Serial.println("Firebase Light false");
//        if(counter==true){
//          Firebase.set("counter",counterNo);
//          counter=false;
//          Serial.println("Set Counter");
//        }
//
//  }
//  if(device21==true){
//        printLocalTime();
//        Firebase.setBool("Home_status/device/fan",true);
//        Firebase.setBool("Home_logs/"+String(timeStringBuff2)+"/"+timeStringBuff1+"/fan/",true);
//        device21=false;
//        Firebase.set("counter",counterNo);
//        Serial.println("Firebase Light True");
//        if(counter==true){
//          Firebase.set("counter",counterNo);
//          counter=false;
//          Serial.println("Set Counter");
//        }
//  }
//  if(counter==true){
//        Firebase.set("counter",counterNo);
//        counter=false;
//        Serial.println("Set Counter");
//  }
}
