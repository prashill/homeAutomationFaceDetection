#include <ESP8266WiFi.h>
//#include <WiFiClient.h> 
//#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <mDNSResolver.h>
#include <ESP8266mDNS.h>


const char* ssid = "ARHAM7A2";
const char* password = "arham@1207";
const char* ntpServer = "1.asia.pool.ntp.org";
const long  gmtOffset_sec = 60*60*4.5;
const int   daylightOffset_sec = 3600;
char timeStringBuff1[25]; //50 chars should be enough
char timeStringBuff2[25]; //50 chars should be enough

//const char* mqttServer = "m16.cloudmqtt.com";
//const int mqttPort = 15034;
//const char* mqttUser = "dtvunaab";
//const char* mqttPassword = "JVvPM5IvTY5B";

//#include <PubSubClient.h>
//WiFiClient espClient;
//PubSubClient client(espClient);

#define NAME_TO_RESOLVE "esp32.local"
using namespace mDNSResolver;
WiFiUDP udp;
Resolver resolver(udp);
int pin1 = 12;
int pin2 = 13;
//int pin3 = 14;
bool check=false;
int counter=0;
int temp=1;
int k1=0;
int k2=0;
int t1=0;
int t2=0;
int p=1;
int m=0;
int wait=0;
int reset=120;
int out_ir_1 = 0;
int out_ir_2 = 0;
int out_ir_3 = 0;
HTTPClient http;
int httpCode;
String ip_address="192.168.29.120";
//IPAddress ip;
char buffer[80];
int buffer_int=50;

void printLocalTime()
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer,80,"%H",timeinfo);
  //struct tm timeinfo;
  //time_t now = time(nullptr);
//  Serial.println(buffer);
  //Serial.print(ctime(&now));
  //Serial.print(&timeinfo, " %d %B %Y %H:%M:%S ");
}
void setup()
{
  pinMode(pin1,INPUT);
  pinMode(pin2,INPUT);
//  pinMode(pin3,OUTPUT);
  Serial.begin(115200);
  delay(2000);
  Serial.println("Connecting to ");
  
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
//  client.setServer(mqttServer, mqttPort);
////  client.setCallback(callback);
//  while (!client.connected()) {
//    Serial.println("Connecting to MQTT...");
//    if (client.connect("Publisher", mqttUser, mqttPassword )) {
//      Serial.println("connected");  
//    } else {
//      Serial.print("failed with state ");
//      Serial.print(client.state());
//      delay(2000);
//    }
//  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  Serial.println("");
  Serial.print("Resolving ");
  Serial.println(NAME_TO_RESOLVE);
  resolver.setLocalIP(WiFi.localIP());
  IPAddress ip = resolver.search(NAME_TO_RESOLVE);
  if(ip != INADDR_NONE) {
    Serial.print("Resolved: ");
    Serial.println(ip);
    ip_address=ip.toString();
  } else {
    Serial.println("Not resolved");
  }
  
}
//void callback(char* topic, byte* payload, unsigned int length) {
// 
//  Serial.print("Message arrived in topic: ");
//  Serial.println(topic);
// 
//  Serial.print("Message:");
//  for (int i = 0; i < length; i++) {
//    Serial.print((char)payload[i]);
//  }
// 
//  Serial.println();
//  Serial.println("-----------------------");
//}
void loop()
{
//  client.loop();
  resolver.loop();
  if ((WiFi.status() == WL_CONNECTED)) {
    out_ir_1 = digitalRead(pin1);
    out_ir_2 = digitalRead(pin2);
//    out_ir_3 = digitalRead(pin3);
//    if(out_ir_3==1){
//      Serial.println("Switch");
//      http.begin("http://"+ip_address+"/device/0/?param="+0); //Specify the URL
//      httpCode = http.GET();
//      http.end();
//      counter=0;
//     }
    Serial.println("");
    Serial.print(out_ir_1);Serial.print(out_ir_2);Serial.print(" counter:");Serial.print(counter);
    Serial.print(" k1:");Serial.print(k1);Serial.print(" k2:");Serial.print(k2);Serial.print(" t1:");Serial.print(t1);Serial.print(" t2:");Serial.println(t2);    
    if (out_ir_1 > out_ir_2){
      k1=k1+1;
      Serial.println(k1);
      }
      if (out_ir_1 < out_ir_2){
        k2=k2+1;
        if (k2>p){
          if (k1>p){
            k1=0;
            k2=0;
            t1=0;
            t2=0;
            temp=counter;
            counter=counter+1;
            check=true;
             Serial.println("\n=========================== ENTRY ===========================");
            if (counter > 1){
              Serial.println("\n##########=========================== ENTRY ===========================#######");
              http.begin("http://"+ip_address+"/set/counter/?param="+counter); //Specify the URL
              httpCode = http.GET();
              http.end();
          if((buffer_int >= 0 and buffer_int <= 6) or (buffer_int >= 17 and buffer_int <= 24)) {
            Serial.println(buffer_int);
            Serial.println("Light On");
//            client.publish("mesg", "light/1");
            Serial.println("=====================ON====================");
          }
            }
          }
        }
      }
      if (out_ir_1 < out_ir_2){
        t1=t1+1;
//        Serial.println("t1"+t1);
      }
      if (out_ir_1 > out_ir_2){
        t2=t2+1;
        if (t2>p){
          if (t1>p){
            t1=0;
            t2=0;
            k1=0;
            k2=0;
            Serial.println("\n=========================== EXIT ===========================");
            if (counter!=0){
              counter=counter-1;
              check=true;
              if (counter > 0){
                Serial.println("\n##########=========================== Exit ===========================#######");
                http.begin("http://"+ip_address+"/set/counter/?param="+counter); //Specify the URL
                httpCode = http.GET();
                http.end();
              }
            }else{
              http.begin("http://"+ip_address+"/device/0/?param="+counter); //Specify the URL
              httpCode = http.GET();
              http.end();
//              client.publish("mesg", "all/0");
            }
          }
        }
      }
      if (check==true){
        if (counter == 0){
            http.begin("http://"+ip_address+"/device/0/?param="+counter); //Specify the URL
            httpCode = http.GET();
            http.end();
//              client.publish("mesg", "all/0");
            Serial.println("=====================OFF====================");
        }else if (counter==1 and temp==0){
          printLocalTime();
          buffer_int=(String(buffer)).toInt();
          Serial.println(buffer_int);
          if((buffer_int >= 0 and buffer_int <= 6) or (buffer_int >= 17 and buffer_int <= 24)) {
            Serial.println(buffer_int);
            Serial.println("Light On");
            http.begin("http://"+ip_address+"/device/1/?param="+counter); //Specify the URL
            httpCode = http.GET(); 
            http.end();
//             client.publish("mesg", "all/1");
            Serial.println("=====================ON====================");
          }else{
            http.begin("http://"+ip_address+"/device2/1/?param="+counter); //Specify the URL
            httpCode = http.GET(); 
            http.end();
//             client.publish("mesg", "fan/1");
            Serial.println("=====================Fan ON====================");
          }
        }
      check=false;
      }
      wait=wait+1;
      if (out_ir_1==0 and out_ir_2==0){
        if (k1 >0 or k2>0 or t1>0 or t2>0){
          m=m+1;
          if (m > reset){
            Serial.println("Set all Zerow");
            m=0;
            k1=0;
            k2=0;
            t2=0;
            t1=0;
          }
        }
      }
      if(k1 >reset or k2>reset or t1>reset or t2>reset){
        k1=0;
        k2=0;
        t2=0;
        t1=0;
        Serial.println("Reset All");
      }
    delay(8);
  }
}
