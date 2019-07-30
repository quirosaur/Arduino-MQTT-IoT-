#include "Ethernet.h"
#include "PubSubClient.h"


//defining pins
#define sensor1 0
#define sensor2 1
#define sensor3 2
#define sensor4 3
#define eth1 4//eth?   
#define sensor5 5    
#define sensor6 6   
#define sensor7 7   
#define sensor8 8    
#define sensor9 9    
#define eth2 10//eth?
#define eth3 11//eth?
#define eth4 12//eth?
#define eth5 13//eth?
#define sensor10 14 //should this be used as analog?
#define sensor11 15 //should this be used as analog?
#define sensor12 16 //should this be used as analog?
#define sensor13 17 //should this be used as analog?
#define sensor14 18 //should this be used as analog?
#define sensor15 19 //should this be used as analog?

//defining sensor names
#define sensor1Name sensor1
#define sensor2Name sensor2
#define sensor3Name sensor3
#define sensor4Name sensor4
#define sensor5Name sensor5
#define sensor6Name sensor6
#define sensor7Name sensor7
#define sensor8Name sensor8
#define sensor9Name sensor9
#define sensor10Name sensor10
#define sensor11Name sensor11
#define sensor12Name sensor12
#define sensor13Name sensor13
#define sensor14Name sensor14
#define sensor15Name sensor15

uint8_t server[6] = {192, 168, 1, 1};
int port = 1884;

int pins[] = {sensor1, sensor2, sensor3, sensor4, eth1, sensor5, sensor6, sensor7, sensor8, sensor9, eth2, eth3, eth4, eth5, sensor10, sensor11, sensor12, sensor13, sensor14, sensor15};

unsigned long interval = 300000;
unsigned long previousMillis = 0;

byte mac[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
byte ip[] = { 192, 168, 1, 202 };

const char username[] = "x";
const char password[] = "x";
const char clientID[] = "x";

const char topic[] = "x";


EthernetClient ethClient;
PubSubClient client(server, port, ethClient);


//check if connections are allright and maintain the MQTT with loop
void maintainEth() {
  if (!ethClient.connected()) {
    Ethernet.begin(mac, ip);
    //Serial.println("reconnected ethernet");
  }
  if (!client.connected()) {
    client.connect(clientID, username, password);
    //Serial.println("reconnected mqtt");
  }
  else {
    //Serial.println(client.state());
  }
  client.loop();
}


int debounce(long currentMillis) {
  if ((unsigned long)(currentMillis - previousMillis) >= interval) {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}


void checkSensors() {
  publishData(String(sensor1Name).c_str(),String(digitalRead(sensor1)).c_str());
  publishData(String(sensor2Name).c_str(),String(digitalRead(sensor2)).c_str());
  publishData(String(sensor3Name).c_str(),String(digitalRead(sensor3)).c_str());
  publishData(String(sensor4Name).c_str(),String(digitalRead(sensor4)).c_str());
  publishData(String(sensor5Name).c_str(),String(digitalRead(sensor5)).c_str());
  publishData(String(sensor6Name).c_str(),String(digitalRead(sensor6)).c_str());
  publishData(String(sensor7Name).c_str(),String(digitalRead(sensor7)).c_str());
  publishData(String(sensor8Name).c_str(),String(digitalRead(sensor8)).c_str());
  publishData(String(sensor9Name).c_str(),String(digitalRead(sensor9)).c_str());
  publishData(String(sensor10Name).c_str(),String(digitalRead(sensor10)).c_str());
  publishData(String(sensor11Name).c_str(),String(digitalRead(sensor11)).c_str());
  publishData(String(sensor12Name).c_str(),String(digitalRead(sensor12)).c_str());
  publishData(String(sensor13Name).c_str(),String(digitalRead(sensor13)).c_str());
  publishData(String(sensor14Name).c_str(),String(digitalRead(sensor14)).c_str());
  publishData(String(sensor15Name).c_str(),String(digitalRead(sensor15)).c_str());
}

void publishData(const char sensorName[],const char timeseries[]) {
  //publish data to MQTT topic. tmpstr is used to buld the string
  
  String tmpstr = "{\"sensorName\":\"";  
  tmpstr += sensorName;
  tmpstr += "\", \"preset\": ";
  tmpstr += timeseries;
  tmpstr += " }";

  client.publish(topic, (char*) tmpstr.c_str());
}



void setup() {
  //this pin "2" is triggered by pushing button on keypad. signal is transferred thru diode,
  //so it won't mess with other button signals
  //attachInterrupt(digitalPinToInterrupt(2), checkSensors, RISING);

  for (int i = 0; i > 19; i++) {
    if (i != 10 && i != 11 && i != 12 && i != 13 && i != 4) {
      pinMode(pins[i], INPUT);
    }
  }

  //Ethernet.begin(mac, ip);
  //Serial.begin(9600);
  //Serial.println("setup done");


  if (client.connect(clientID, username, password)) {
    //Serial.println("mqtt connection up");
  }
  else {
    //Serial.println(client.state());
  }
}

void loop() {
  if (debounce(millis())) {    
    checkSensors();
  }
  maintainEth();
}
