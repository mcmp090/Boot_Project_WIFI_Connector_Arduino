/*
 * Copyright (c) 2018, circuits4you.com
 * All rights reserved.
 * Create a TCP Server on ESP8266 NodeMCU. 
 * TCP Socket Server Send Receive Demo
*/

#include <ESP8266WiFi.h>

#define SendKey 0  //Button to send data Flash BTN on NodeMCU

int port = 23;  //Port number
WiFiServer server(port);

//Server connect to WiFi Network
const char *ssid = "";  //Enter your wifi SSID
const char *password = "";  //Enter your wifi Password
 String answer = "";
int l;
int count=0;
bool trans = false;
long timeout;
bool tiefenmesser;
bool positionslichter;
bool lichtvorne;
bool lichthinten;
bool v12;
bool usb;
int x;
int y;
int z;

//=======================================================================
//                    Power on setup
//=======================================================================
void setup() 
{
  Serial.begin(115200);
  pinMode(SendKey,INPUT_PULLUP);  //Btn to send data
  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //Connect to wifi
 
  // Wait for connection  
  Serial.println("Connecting to Wifi");
  while (WiFi.status() != WL_CONNECTED) {   
    delay(500);
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
  server.begin();
  Serial.print("Open Telnet and connect to IP:");
  Serial.print(WiFi.localIP());
  Serial.print(" on port ");
  Serial.println(port);
}
//=======================================================================
//                    Loop
//=======================================================================

void loop() 
{
  WiFiClient client = server.available();
  
  if (client) {
    if(client.connected())
    {
      Serial.println("Client Connected");
    }
    //Moin
    while(client.connected()){  
         l=0;
         if(trans == true){
           if(millis()-timeout > 5000){
             trans = false;
             answer = "";
             break;
           }
         }
      while(client.available()>0){
        l++;
        // read data from the connected client
        //answer = answer + String(client.read());
        char msg = client.read();
        Serial.println(msg);
        if(trans == true){
          if(msg == 't'){
            trans = false;
            Serial.println("Message: " + answer);
            x = process(answer);
            y = x / 255;
            z = x % 255;
            if(y == 0 || (y == 1 && z == 0)){
              client.write(1);
              client.write(x);
            }else{
              client.write(y+1);
              for(int i=0; i<y; i++){
                client.write(255);
              }
              client.write(x - (y * 255));
            }
            answer = "";
            
          }else{
            answer += msg;
          }
        }else{
          if(msg == 't'){
            trans = true;
            timeout = millis();
          }
          if(l != 0){
      Serial.println(l);
      }
      l=0;
      //answer = "";
      //Send Data to connected client
      while(Serial.available()>0)
      {
        client.write(Serial.read());
      }
        }
      }
      
    }
    client.stop();
    Serial.println("Client disconnected");    
  }
}
//=======================================================================

int process(String msg){
  String inre = split(msg, '.', 0);
  String con = split(msg, '.', 1);
  String val = split(msg, '.', 2);
  int ans;

  if(inre.equals("1")){
    int content = con.toInt();
    Serial.println("Content: " + String(content));
    switch (content){
      case 1:
        if(tiefenmesser == false){
          tiefenmesser = true;
          ans = 1;
        }else{
          tiefenmesser = false;
          ans = 0;
        }
      break;
      case 2:
      if(positionslichter == false){
          positionslichter = true;
          ans = 1;
        }else{
          positionslichter = false;
          ans = 0;
        }
      break;
      case 3:
      if(lichtvorne == false){
          lichtvorne = true;
          ans = 1;
        }else{
          lichtvorne = false;
          ans = 0;
        }
      break;
      case 4:
      if(lichthinten == false){
          lichthinten = true;
          ans = 1;
        }else{
          lichthinten = false;
          ans = 0;
        }
      break;
      case 5:
      if(v12 == false){
          v12 = true;
          ans = 1;
        }else{
          v12 = false;
          ans = 0;
        }
      break;
      case 6:
      if(usb == false){
          usb = true;
          ans = 1;
        }else{
          usb = false;
          ans = 0;
        }
      break;
      default:
      break;
    }
  }else{
    int content = con.toInt();
    Serial.println("Content: " + String(content));
    switch (content){
      case 1:
        if(tiefenmesser == true){
          
          ans = 1;
        }else{
          
          ans = 0;
        }
      break;
      case 2:
      if(positionslichter == true){
          
          ans = 1;
        }else{
          
          ans = 0;
        }
      break;
      case 3:
      if(lichtvorne == true){
         
          ans = 1;
        }else{
          
          ans = 0;
        }
      break;
      case 4:
      if(lichthinten == true){
          
          ans = 1;
        }else{
          
          ans = 0;
        }
      break;
      case 5:
      if(v12 == true){
          
          ans = 1;
        }else{
         
          ans = 0;
        }
      break;
      case 6:
      if(usb == true){
          
          ans = 1;
        }else{
          
          ans = 0;
        }
      break;
      default:
      break;
    }
  }
  Serial.println("Answer:" + String(ans));
  return ans;
}

String split(String s, char parser, int index) {
  String rs="";
  int parserIndex = index;
  int parserCnt=0;
  int rFromIndex=0, rToIndex=-1;
  while (index >= parserCnt) {
    rFromIndex = rToIndex+1;
    rToIndex = s.indexOf(parser,rFromIndex);
    if (index == parserCnt) {
      if (rToIndex == 0 || rToIndex == -1) return "";
      return s.substring(rFromIndex,rToIndex);
    } else parserCnt++;
  }
  return rs;
}
