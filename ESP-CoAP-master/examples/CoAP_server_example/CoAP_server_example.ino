/*
ESP-COAP Server
*/

#include <ESP8266WiFi.h>
#include <coap_server.h>

#define LED_0 0

coapServer coap;

//WiFi connection info
const char* ssid = "panoulu";
const char* password = "";
IPAddress RDip(192,168,0,108); //Resource Diredtory (RD)
int RDport =5683;

// CoAP server endpoint url callback
void callback_light(coapPacket &packet, IPAddress ip, int port, int obs);

// response from an external server (RD) callback
void callback_response_ack(coapPacket *packet, IPAddress ip, int port, int obs);


void setup() {

  pinMode(LED_0, OUTPUT);
  digitalWrite(LED_0, LOW);
  
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

  // handling received request
  // can add multiple function call for differnt urls.
  coap.server(callback_light, "sensors/light_lux");

  // handling response from an external server (RD)
  coap.responseACK(callback_response_ack);

  // start coap server/client
  coap.start();
  // coap.start(5683);
  delay(2000);
}

void loop() {
  
//  // GET request (RD URI Discovery)
//  int msgid = coap.get(RDip, RDport, "/.well-known/core?rt=core.rd");

//  // POST request (Resource registeration)
//  //arguments server ip address,default port,url, payload,payloadlength
//  char *payload = "</sensors/light_lux_18>;ct=40;rt=\"light-lux\";if=\"sensor\"";
//  int msgid =coap.post(RDip, RDport, "/rd?ep=n18", payload, strlen(payload));
  
  coap.loop();
  delay(5000);
}


// CoAP server endpoint URL callback
void callback_light(coapPacket *packet, IPAddress ip, int port,int obs) {
  // send response
  char *payload="500_lux"; //dummy data
  coap.sendResponse(ip, port, payload);
}

// response from an external server (RD) callback
void callback_response_ack(coapPacket *packet, IPAddress ip, int port, int obs) {
  
  if (packet->code == COAP_CONTENT){         // response of RD URIs descovery 
    char p[packet->payloadlen + 1];
    memcpy(p, packet->payload, packet->payloadlen);
    p[packet->payloadlen] = NULL;
    Serial.println(p);
    
  }else if(packet->code == COAP_CREATED){    // response of resource registeration
    String Location_Path = "";
    for (int i = 0; i < packet->optionnum; i++) {
      char path[packet->options[i].length + 1];
      memcpy(path,packet->options[i].buffer,packet->options[i].length);
      path[packet->options[i].length] = NULL;
      if(Location_Path.length() > 0)
        Location_Path += "/";
      Location_Path += path;
    }
    Serial.println(Location_Path);
    
  }else{
    Serial.print("COAP_RESPONSE_CODE "); Serial.println(packet->code);
  }  
}
