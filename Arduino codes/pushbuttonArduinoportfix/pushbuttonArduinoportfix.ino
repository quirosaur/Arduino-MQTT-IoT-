#include "Ethernet.h"
#include "PubSubClient.h"



//version 1.0.1

//fixed pir data publishing. using digitalread instead of the bool variable.

//this has separate topics for each pir and button, static memory has been used with strings
//HIGH level triggers, no debounce, no interrupt. doublepress not on this end.
//previous state remembered to disable spamming to MQTT. only send message once per press.

#define RX 0
#define TX 1
#define pir1 2
#define pir2 3
#define eth1 4//eth?
#define pir3 5
#define pir4 6
#define pir5 7
#define btn1 8
#define btn2 9
#define eth2 10//eth?
#define eth3 11//eth?
#define eth4 12//eth?
#define eth5 13//eth?
#define btn3 14
#define btn4 15
#define btn5 16
#define btn6 17
#define btn7 18
#define btn8 19

#define topic0 "x/x/nappi0"
#define topic1 "x/x/nappi1"
#define topic2 "x/x/nappi2"
#define topic3 "x/x/nappi3"
#define topic4 "x/x/nappi4"
#define topic5 "x/x/nappi5"
#define topic6 "x/x/nappi6"
#define topic7 "x/x/nappi7"
#define topic8 "x/x/nappi8"
#define topic9 "x/x/nappi9"
#define topicPir1 "x/x/pir1"
#define topicPir2 "x/x/pir2"
#define topicPir3 "x/x/pir3"
#define topicPir4 "x/x/pir4"
#define topicPir5 "x/x/pir5"



//mosquitto
uint8_t server[6] = {192, 168, 1, 1};
int port = 1883;

//all pins listed for cycling them through
int pins[] = {RX, TX, pir1, pir2, eth1, pir3, pir4, pir5, btn1, btn2, eth2, eth3, eth4, eth5, btn3, btn4, btn5, btn6, btn7, btn8};


byte mac[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
byte ip[] = { 192, 168, 1, 202 };

const char username[] = "x";
const char password[] = "x";
const char clientID[] = "x";

const char topic[] = "x";

//state variables
bool btn1b = false;
bool btn2b = false;
bool btn3b = false;
bool btn4b = false;
bool btn5b = false;
bool btn6b = false;
bool btn7b = false;
bool btn8b = false;
bool pir1b = false;
bool pir2b = false;
bool pir3b = false;
bool pir4b = false;
bool pir5b = false;

EthernetClient ethClient;
PubSubClient client(server, port, ethClient);


//check if connections are allright and maintain the MQTT connection.
void maintainEth() {
  if (!ethClient.connected()) {
    Ethernet.begin(mac, ip);
    Serial.println(F("connecting ethernet"));
  }
  if (!client.connected()) {
    client.connect(clientID);
    //client.connect(clientID, username, password);
    Serial.println(F("connecting mqtt"));
  }
  else {
    //Serial.println(client.state());
  }
  client.loop();
}

void checkPIR() {
  if (digitalRead(pir1) != pir1b) {
    pir1b = digitalRead(pir1);
    if (digitalRead(pir1)) {
      Serial.println(F("pir1"));
      publishData(String(F("pir")).c_str(), String(topicPir1).c_str(), 2);
    }
  }
  if (digitalRead(pir2) != pir2b) {
    pir2b = digitalRead(pir2);
    if (digitalRead(pir2)) {
      Serial.println(F("pir2"));
      publishData(String(F("pir")).c_str(), String(topicPir2).c_str(), 2);
    }
  }
  if (digitalRead(pir3) != pir3b) {
    pir3b = digitalRead(pir3);
    if (digitalRead(pir3)) {
      Serial.println(F("pir3"));
      publishData(String(F("pir")).c_str(), String(topicPir3).c_str(), 2);
    }
  }
  if (digitalRead(pir4) != pir4b) {
    pir4b = digitalRead(pir4);
    if (digitalRead(pir4)) {
      Serial.println(F("pir4"));
      publishData(String(F("pir")).c_str(), String(topicPir4).c_str(), 2);
    }
  }
  if (digitalRead(pir5) != pir5b) {
    pir5b = digitalRead(digitalRead(pir5));
    if (btn5b) {
      publishData(String(F("pir")).c_str(), String(topicPir5).c_str(), 2);
    }
  }
}

//go through buttons to check if any are pressed
void checkButtons() {

  if (digitalRead(btn1) != btn1b) {
    btn1b = digitalRead(btn1);
    if (btn1b) {
      publishData(String(F("button")).c_str(), String(topic1).c_str(), 1);
    }
  }
  if (digitalRead(btn2) != btn2b) {
    btn2b = digitalRead(btn2);
    if (btn2b) {
      publishData(String(F("button")).c_str(), String(topic2).c_str(), 1);
    }
  }
  if (digitalRead(btn3) != btn3b) {
    btn3b = digitalRead(btn3);
    if (btn3b) {
      publishData(String(F("button")).c_str(), String(topic3).c_str(), 1);
    }
  }
  if (digitalRead(btn4) != btn4b) {
    btn4b = digitalRead(btn4);
    if (btn4b) {
      publishData(String(F("button")).c_str(), String(topic4).c_str(), 1);
    }
  }
  if (digitalRead(btn5) != btn5b) {
    btn5b = digitalRead(btn5);
    if (btn5b) {
      publishData(String(F("button")).c_str(), String(topic5).c_str(), 1);
    }
  }
  if (digitalRead(btn6) != btn6b) {
    btn6b = digitalRead(btn6);
    if (btn6b) {
      publishData(String(F("button")).c_str(), String(topic6).c_str(), 1);
    }
  }
  if (digitalRead(btn7) != btn7b) {
    btn7b = digitalRead(btn7);
    if (btn7b) {
      publishData(String(F("button")).c_str(), String(topic7).c_str(), 1);
    }
  }
  if (digitalRead(btn8) != btn8b) {
    btn8b = digitalRead(btn8);
    if (btn8b) {
      publishData(String(F("button")).c_str(), String(topic8).c_str(), 1);
    }
  }
}


void publishData(const char timeseries[], const char topic[], int i) {
  //publish data to MQTT topic. tmpstr is used to buld the string
  String tmpstr = F("");
  if (i == 1) {
    tmpstr = F("{\"sensorName\":\"x\", \"value\": ");
    tmpstr += timeseries;
    tmpstr += F(" }");
    Serial.println(F("publishdata, btn"));
  }
  if (i == 2) {
    tmpstr = F("{\"sensorName\":\"x\", \"value\": ");
    tmpstr += timeseries;
    tmpstr += F(" }");
    Serial.println(F("publishdata, pir"));
  }

  //Serial.println(F("x sending this: ") + tmpstr);
  //client.publish(topic, tmp);
  client.publish(topic, (char*) tmpstr.c_str());
}

void setup() {
  for (int i = 0; i <= 19; i++) {
    if (i != 10 && i != 11 && i != 12 && i != 13 && i != 4) {
      pinMode(pins[i], INPUT);
    }
  }

  //Ethernet.begin(mac, ip);
  Serial.begin(9600);
  //Serial.println("setup done");

  //client.connect(clientID, username, password);
  if (client.connect(clientID)) {
    //Serial.println("mqtt connection up");
  }
  else {
    //Serial.println(client.state());
  }
}

void loop() {
  checkButtons();
  checkPIR();
  maintainEth();
}
