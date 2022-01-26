/*
izera iot based irrigation system using ir remote as recover communication protocal 
 */

#include <ESP8266WiFi.h>
#include <Wire.h> 

#include <IRremote.h>

//#include "SSD1306.h" 
//SSD1306  display(0x3c, D1, D2);
// replace with your channel's thingspeak API key, 
String apiKey = "8ABJUBGPRZHX1CEE";
const char* ssid = "izera@";
const char* password = "izera1234@";
 
const char* server = "api.thingspeak.com";
const int RECV_PIN = 12; // D6

int pump =0; // D3
int led= 16; // D0
int buzzer = 4; //D2
IRrecv irrecv(RECV_PIN);
decode_results results;
 
WiFiClient client;

void setup() { 
  digitalWrite(pump,LOW);    
  Serial.begin(115200);
  irrecv.enableIRIn();
  pinMode(A0,INPUT);
pinMode(buzzer,OUTPUT);
  pinMode(pump,OUTPUT);
  //digitalWrite(pump,1);
  pinMode(led,OUTPUT);
  pinMode(RECV_PIN,INPUT);
  delay(10);
  int moisture = analogRead(A0);
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
}
 void lighting()
 { digitalWrite(led,OUTPUT);
 delay(200);
 digitalWrite(led,LOW);
 delay(200);

 }
 void buzzerhorn (){
  digitalWrite(buzzer,HIGH);
  delay(200);
  digitalWrite(buzzer,LOW);
  delay(200);
}
void irrigate()
{
  digitalWrite(pump,HIGH);
}
void irremote()
{       // ---------passing ir remote code decode the remote you're using assign hexadecimal code to any action listeners -------------------------------------------- 
  // Activating manually operation with IR remote .  
digitalWrite(pump,LOW);
 if (irrecv.decode(&results)){
      Serial.println(results.value,HEX);
 
        switch(results.value){
          case 0xFF02FD: //Red Keypad Button
       irrigate();  
        buzzerhorn();
        lighting();
        /*  case 0xFFA857: 
        digitalWrite(pump,LOW);
          lighting();*/
        break;
    }
    irrecv.resume(); 
      return; 
  }     
   }

void loop() {
  /*if(digitalRead(pump)==LOW)
  {irremote();}
  else {*/
  //irremote();
int moisture = analogRead(A0);
 if(moisture>=1000)
  {
   // irremote();
    irrigate();
    buzzerhorn();
    Serial.println("manual mode"); 
    }
  
  else if(moisture>=800){
  /*return to your local website i.e izeraifituburyo @hiroku appp .com  there fore compile*/
    irrigate();
    lighting();
    digitalWrite(led,HIGH);
    Serial.println("pump is off/n");
    Serial.println(moisture);
   }
 else { 
   //digitalWrite(pump,LOW);
   lighting();
   //irremote();
  }
  if (client.connect(server,80)) {  //   "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
           postStr +="&field1=";
           postStr += String(moisture);
           postStr += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n"); 
     client.print("Host: api.thingspeak.com\n"); 
     client.print("Connection: close\n"); 
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n"); 
     client.print("Content-Type: application/x-www-form-urlencoded\n"); 
     client.print("Content-Length: "); 
     client.print(postStr.length()); 
     client.print("\n\n"); 
     client.print(postStr);
           
 
     Serial.print("Moisture Sensor Value: ");
     Serial.print(moisture);
     //display.drawString(0, 20,"Moisture: ");
     //display.drawString(0, 30,String(temp));
     Serial.println(" send to Thingspeak");
     //display.drawString(0, 10, "Send to Thingspeak");   
     //display.display(); 
  }
  client.stop();
   

   Serial.println("Waiting...");    
  // thingspeak needs minimum 15 sec delay between updates
  // Here I set upload every 60 seconds
  for(unsigned int i = 0; i < 10; i++)
  {
    delay(200);                         
  }  }
  
//}
/*
void irremote()
{       // ---------passing ir remote code decode the remote you're using assign hexadecimal code to any action listeners -------------------------------------------- 
*/
