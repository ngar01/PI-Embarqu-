#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>

//LoR32u4II 868MHz or 915MHz (black board)
#define SCK     15
#define MISO    14
#define MOSI    16
#define SS      8
#define RST     4
#define DI0     7
#define BAND    868E6  // 915E6
#define PABOOST true

//#Loravariables
String outgoing;              // outgoing message
int flag = -1;
long lastSendTime = 0;
int interval = 2000;

String data;
String redZonePos;

int  size_ = 0;
String  payload = "";

void setup() {
  Serial.begin(115200);
  LoRa.setPins(SS, RST, DI0); // set CS, reset, IRQ pin

  if (!LoRa.begin(BAND,PABOOST)) {             // initialize ratio at 915 MHz
    while (true);                       // if failed, do nothing
  }

}

void loop() {
  //LoRa.receive();
  if (payload == "") {
    Serial.println("ON");
  }

  if (Serial.available() > 0) {
    payload = Serial.readStringUntil('\n');
    sendMessage(payload, 1);
    flag = -1;
  }

  if (millis() - lastSendTime > interval) {
    if (flag == 0)
    {
      sendMessage(payload, 1);
      flag = -1;
    }
    if (flag == 2) {
      Serial.println(redZonePos);
      flag = -1;
    }
    lastSendTime = millis();            // timestamp the message
    interval = random(2000) + 1000;    // 2-3 seconds
  }
  onReceive(LoRa.parsePacket());
}

void sendMessage(String outgoing, int flag) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(flag);
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  //int recipient = LoRa.read();
  flag = LoRa.read();
  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }
  if (flag == 2) {
    redZonePos = incoming;
  }
}
