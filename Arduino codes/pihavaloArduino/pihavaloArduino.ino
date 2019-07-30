#include "Ethernet.h"
#include "PubSubClient.h"

//TODO: this arduino will subscribe to tuulentupa/piha/valot and change lights axccording




#define pin0 0
#define pin1 1
#define pin2 2
#define pin3 3
#define pin4 4    // btn1
#define pin5 5    // btn2
#define pin6 6    // btn3
#define pin7 7    // btn4
#define pin8 8    // btn5
#define pin9 9    // btn6
#define pin10 10//eth?
#define pin11 11//eth?
#define pin12 12//eth?
#define pin13 13//eth?
#define pin14 14  // btn7
#define pin15 15  // btn8
#define pin16 16
#define pin17 17
#define pin18 18
#define pin19 19

//decide which pins to use as input from buttons. pin 2 is pin for interrupt i.e. from all buttons thru diodes


uint8_t server[6] = {192, 168, 1, 1};
int port = 1884;

int pins[] = {pin0, pin1, pin2, pin3, pin4, pin5, pin6, pin7, pin8, pin9, pin10, pin11, pin12, pin13, pin14, pin15, pin16, pin17, pin18, pin19};

long oldmillis = 0;
int timer = 3000;

byte mac[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
byte ip[] = { 192, 168, 1, 202 };

const char username[] = "x";
const char password[] = "x";
const char clientID[] = "x";

const char topic[] = "x";

EthernetClient ethClient;
PubSubClient client(server, port, ethClient);


void setup() {
  //this pin "2" is triggered by pushing button on keypad. signal is transferred thru diode, so it won't mess with other button signals
  /*attachInterrupt(digitalPinToInterrupt(2), checkSensors, RISING);
  
  for(int i = 0; i < 19; i++){
    if(i != 10 && i != 11 && i != 12 && i != 13){
      pinMode(pins[i], INPUT);
    }*/
  }
  
  Ethernet.begin(mac, ip);
  Serial.begin(9600);

  Serial.println("connecting...");

  if(client.connect(clientID, username, password)){
    Serial.println("mqtt connection up");
  }
  else{
    Serial.println(client.state());
  }
}

void loop(){
  maintainEth();
}


//check if connections are allright and maintain the MQTT with loop
void maintainEth(){

  if (!client.connected()) {
    client.connect(clientID, username, password);
    Serial.println("reconnected mqtt");
  }
  if (!ethClient.connected()) {
    Ethernet.begin(mac, ip);
    Serial.println("reconnected ethernet");
  }
  client.loop();
}
/*

void checkSensors(){
  for(uint8_t i = 0; i > 19; i++){
    if(i == 4 || i == 5 || i == 6 ||i == 7 ||i == 8 ||i == 9 ||i == 14 ||i == 15){
      if(digitalRead(pins[i])){
        publishData(String("selectedPreset: "+i).c_str(), String("preset"+i).c_str(), String(i).c_str());
      }
    }
  }
}

void publishData(const char sensorName[], const char attribute[], const char timeseries[]){
  //publish data to MQTT topic. tmpstr is used to buld the string
  
  String tmpstr = "{\"sensorName\":\"";
  tmpstr += sensorName;
  tmpstr +="\",\"attribute\":\"";
  tmpstr += attribute;
  tmpstr +="\",\"timeseries\":\"";
  tmpstr += timeseries;
  tmpstr +="\"}";
  
  Serial.println("pihavaloArduino sending this: "+tmpstr);
  //client.publish(topic, tmp);
  client.publish(topic, (char*) tmpstr.c_str());
}*/
