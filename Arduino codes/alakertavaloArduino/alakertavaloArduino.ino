#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Ethernet.h>

//version 1.0.1
//active HIGH, preset 9 will light up all backlights.

#define RX 0
#define TX 1
#define back1 2
#define back2 3
#define eth1 4//eth?  
#define back3 5
#define back4 6
#define back5 7
#define back6 8
#define back7 9
#define eth2 10//eth?
#define eth3 11//eth?
#define eth4 12//eth?
#define eth5 13//eth?
#define back8 14
#define group1 15
#define group2 16
#define group3 17
#define group4 18
#define group5 19


uint8_t server[6] = {192, 168, 1, 1};
int port = 1883;


int pins[] = {RX, TX, back1, back2, eth1, back3, back4, back5, back6, back7, eth2, eth3, eth4, eth5, back8, group1, group2, group3, group4, group5};

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
void callback(char* intopic, byte* payload, unsigned int length) {
  Serial.println("callback");
  //we go through the received payload
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& payloadObj = jsonBuffer.parseObject(payload);
  changeBacklight(payloadObj["preset"]);
  changeLights(group1, payloadObj["group1"]);
  changeLights(group2, payloadObj["group2"]);
  changeLights(group3, payloadObj["group3"]);
  changeLights(group4, payloadObj["group4"]);
  changeLights(group5, payloadObj["group5"]);
  Serial.println("----------------end of print-------------------------");
}


void formatBacklight() {
  digitalWrite(back1, LOW);
  digitalWrite(back2, LOW);
  digitalWrite(back3, LOW);
  digitalWrite(back4, LOW);
  digitalWrite(back5, LOW);
  digitalWrite(back6, LOW);
  digitalWrite(back7, LOW);
  digitalWrite(back8, LOW);
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


//change backlights according to input
void changeBacklight(int i) {
  //clear current backight setting

  formatBacklight();

  //change backlight according to selected preset
  switch (i) {
    case 1:
      Serial.print("backlight ");
      Serial.println(i);
      digitalWrite(back1, HIGH);
      break;

    case 2:
      Serial.print("backlight ");
      Serial.println(i);
      digitalWrite(back2, HIGH);
      break;

    case 3:
      Serial.print("backlight ");
      Serial.println(i);
      digitalWrite(back3, HIGH);
      break;

    case 4:
      Serial.print("backlight ");
      Serial.println(i);
      digitalWrite(back4, HIGH);
      break;

    case 5:
      Serial.print("backlight ");
      Serial.println(i);
      digitalWrite(back5, HIGH);
      break;

    case 6:
      Serial.print("backlight ");
      Serial.println(i);
      digitalWrite(back6, HIGH);
      break;

    case 7:
      Serial.print("backlight ");
      Serial.println(i);
      digitalWrite(back7, HIGH);
      break;

    case 8:
      Serial.print("backlight ");
      Serial.println(i);
      digitalWrite(back8, HIGH);
      break;

    case 9:
      digitalWrite(back1, HIGH);
      digitalWrite(back2, HIGH);
      digitalWrite(back3, HIGH);
      digitalWrite(back4, HIGH);
      digitalWrite(back5, HIGH);
      digitalWrite(back6, HIGH);
      digitalWrite(back7, HIGH);
      digitalWrite(back8, HIGH);
      break;

    default:
      break;

  }
}


//check if connections are allright and maintain the MQTT with loop
void maintainEth() {
  if (!ethClient.connected()) {
    Ethernet.begin(mac, ip);
    Serial.println("reconnected ethernet");
  }
  if (!client.connected()) {
    client.connect(clientID);
    //client.connect(clientID, username, password);
    Serial.println("reconnected mqtt");
  }
  if (client.connected()) {
    if (sub) {}
    else {
      sub = client.subscribe("x/x/x");
      if (sub) {
        Serial.println("sub success");
      }
      else {
        Serial.println("sub fail");
      }
    }
  }
  if (client.state() != 0) {
    Serial.println(client.state());
  }
  client.loop();
}


void setup() {
  client.setCallback(callback);
  for (int i = 0; i <= 19; i++) {
    if (i != 10 && i != 11 && i != 12 && i != 13 && i != 4 && i != 0 && i != 1) {
      pinMode(pins[i], OUTPUT);
      digitalWrite(i, LOW);
    }
  }
  Serial.begin(9600);
  while (true) {
    if (!ethClient.connected()) {
      Ethernet.begin(mac, ip);
      Serial.println("connected ethernet");
      break;
    }
  }
  while (true) {
    if (!client.connected()) {
      client.connect(clientID);
      //client.connect(clientID, username, password);
      Serial.println("connected mqtt inside setup");
      break;
    }
  }
}

void loop() {
  maintainEth();
  //Serial.println("test");
}
