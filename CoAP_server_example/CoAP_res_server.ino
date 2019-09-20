/*
ESP-COAP Server
*/

#include <ESP8266WiFi.h>
#include <coap_server.h>


coapServer coap;

//WiFi connection info
const char* ssid = "panoulu";
const char* password = "";
IPAddress ip(100,68,147,74);
int port =5683;

// coap server response callback
void callback_response_ack(coapPacket *packet, IPAddress ip, int port, int obs);

// coap server response callback
void callback_response_ack(coapPacket *packet, IPAddress ip, int port, int obs) {
    char p[packet->payloadlen + 1];
    memcpy(p, packet->payload, packet->payloadlen);
    p[packet->payloadlen] = NULL;
    Serial.println(p);
}

void setup() {
  yield();
  //serial begin
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println(" ");

  // Connect to WiFi network
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
  // Print the IP address
  Serial.println(WiFi.localIP());

  // this endpoint is single callback.
    coap.responseACK(callback_response_ack);

  // start coap server/client
  coap.start();
  // coap.start(5683);
}

void loop() {
//  // GET request (RD URI Discovery)
//  int msgid = coap.get(ip,port,"/.well-known/core?rt=core.rd");
//  int msgid = coap.get(ip,port,"/.well-known/core");

     // POST request (Registeration)
     //arguments server ip address,default port,url, payload,payloadlength
    char *payload = "</sensors/light1>;ct=40;rt=\"light\";if=\"sensor\"";
    int msgid =coap.post(ip,port,"/rd?ep=node1", payload,strlen(payload));
  
  coap.loop();
  
  delay(1000);
}
