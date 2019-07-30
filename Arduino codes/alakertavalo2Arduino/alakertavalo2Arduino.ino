#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Ethernet.h>

//Version 1.0
//active HIGH, pulse signal to curtains, pinouts in order

#define RX 0
#define TX 1
#define group6 2
#define group7 3
#define eth1 4//eth?   
#define group8 5    
#define group9 6   
#define group10 7   
#define group11 8    
#define group12 9    
#define eth2 10//eth?
#define eth3 11//eth?
#define eth4 12//eth?
#define eth5 13//eth?
#define group13 14  
#define onVerho 15  
#define offVerho 16
#define nan1 17
#define nan2 18
#define nan3 19


uint8_t server[6] = {192, 168, 1, 1};
int port = 1883;

//curtain control pulse length
int verhoDelay = 50;

int pins[] = {RX, TX, group6, group7, eth1, group8, group9, group10, group11, group12, eth2, eth3, eth4, eth5, group13, onVerho, offVerho, nan1, nan2, nan3};

bool sub = false;

byte mac[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
byte ip[] = { 192, 168, 1, 202 };

const char username[] = "x";
const char password[] = "x";
const char clientID[] = "x";

const char topic[] = "x";

EthernetClient ethClient;
PubSubClient client(server, port, ethClient);


//this is triggered, when message arrives to subscribed topic
void callback(char* intopic, byte* payload, unsigned int length){  
  //we go through the received payload
  StaticJsonBuffer<500> jsonBuffer;
  //change lights according to request
  JsonObject& payloadObj = jsonBuffer.parseObject(payload);
  changeLights(group6, payloadObj["group6"]);
  changeLights(group7, payloadObj["group7"]);
  changeLights(group8, payloadObj["group8"]);
  changeLights(group9, payloadObj["group9"]);
  changeLights(group10, payloadObj["group10"]);
  changeLights(group11, payloadObj["group11"]);
  changeLights(group12, payloadObj["group12"]);
  changeLights(group13, payloadObj["group13"]);
  changeVerho(payloadObj["verho"]);
  Serial.println("----------------end of print-------------------------");
}

//sen pulse data to curtains
void changeVerho(bool verho){
  if(verho){
    digitalWrite(onVerho, HIGH);
    delay(verhoDelay);
    digitalWrite(onVerho, LOW);
  }
  else
  {
    digitalWrite(offVerho, HIGH);
    delay(verhoDelay);
    digitalWrite(offVerho, LOW);
  }
}

//change lights according to input
void changeLights(int pin, bool a) {
  //if pin's state isn't same as requested, change it.
  digitalWrite(pin, a);
  Serial.print("pin: ");
  Serial.println(pin);
  Serial.print("value: ");
  Serial.println(a);
  Serial.println("");
  Serial.print("testing: ");
  Serial.println(digitalRead(pin));
}


//check if connections are allright and maintain the MQTT with loop
void maintainEth(){
  if (!ethClient.connected()) {
    Ethernet.begin(mac, ip);
    Serial.println("reconnected ethernet");
  }  
  if (!client.connected()) {
    client.connect(clientID);
    //client.connect(clientID, username, password);
    Serial.println("reconnected mqtt");
  }
  if(client.connected()){
    if(sub){}
    else{
      sub = client.subscribe("x/x/valot");
      if(sub){
        Serial.println("sub success");
      }
      else{
        Serial.println("sub fail");
      }
    }
  }
  if(client.state() != 0){
    Serial.println(client.state());
  }
  client.loop();
}


void setup() {
  client.setCallback(callback);
  for(int i = 0; i <= 19; i++){
    if (i != 10 && i != 11 && i != 12 && i != 13 && i != 4 && i != 0 && i != 1) {
      pinMode(pins[i], OUTPUT);
      digitalWrite(i, LOW);
    }
  }
  Serial.begin(9600);  
  while(true){
    if (!ethClient.connected()) {
      Ethernet.begin(mac, ip);
      Serial.println("connected ethernet");
      break;
    }
  }
  while(true){
    if (!client.connected()) {
      client.connect(clientID);
      //client.connect(clientID, username, password);
      Serial.println("connected mqtt inside setup");
      break;
    }
  }
}

void loop(){
  maintainEth();
}
