/*
ESP-COAP Client
*/
#include <ESP8266WiFi.h>
#include "coap_client.h"


#define LED_0 0

//instance for coapclient
coapClient coap;

//WiFi connection info
const char* ssid = "Gilas";
const char* password = "";

//ip address and default port of coap server in which your interested in
IPAddress ip(192,168,0,115);
int port =5683;

// coap client response callback
void callback_response(coapPacket &packet, IPAddress ip, int port);


// coap client response callback
void callback_response(coapPacket &packet, IPAddress ip, int port) {
    char p[packet.payloadlen + 1];
    memcpy(p, packet.payload, packet.payloadlen);
    p[packet.payloadlen] = NULL;
    Serial.println(p);
    
    //response from coap server
    if(packet.type==3 && packet.code==0){
        Serial.println("ping ok");
    }
}

void setup() {

      pinMode(LED_0, OUTPUT);
      digitalWrite(LED_0, HIGH);
   
    Serial.begin(115200);

    WiFi.begin(ssid, password);
    Serial.println(" ");

    // Connection info to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    //delay(500);
    yield();
    Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    // Print the IP address of client
    Serial.println(WiFi.localIP());
    delay(1000);

    // client response callback.
    // this endpoint is single callback.
    coap.response(callback_response);

    // start coap client
    coap.start();
    delay(5000);
}

void loop() {
   
  //get request (data request)
  int msgid = coap.get(ip,port,"/sensors/light_lux");
  bool state= coap.loop();

  delay(2000);
}
