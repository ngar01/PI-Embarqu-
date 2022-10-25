//Connect with pin 18 and 19
#include <TinyGPS.h>
#include <SPI.h>
#include <LoRa.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
//long   lat,lon; // create variable for latitude and longitude object
float lat, lon;
float lat2;
float lat1;
float long2;
float long1;
String latlong;
TinyGPS gps; // create gps object

static const uint32_t GPSBaud = 9600;
//LoR32u4II 868MHz or 915MHz (black board)
#define SCK     15
#define MISO    14
#define MOSI    16
#define SS      8
#define RST     4
#define DI0     7
#define BAND    868E6  // 915E6
#define PABOOST true


//####### Defines the pins to which sensor and LED are connected.
const int pinLedRouge   = 6;
const int pinLedVert   = 2;
const int pinLedBlue   = 3;
const int pinLight = A1;
int speakerPin = 5;
int smokePin = A5;

//###loraVariables
String outgoing; // outgoing message
String redzone;
String type;
int flag = 0;
long lastSendTime = 0;
int interval = 2000;

void setup() {
  Serial.begin(115200); // connect serial
  Serial.println("The GPS Received Signal:");
  Serial1.begin(9600); // connect gps sensor

  Serial.begin(115200);

  Serial1.begin(9600); // connect gps sensor

  pinMode(pinLight, INPUT);
  pinMode(smokePin, INPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(pinLedVert, OUTPUT);
  pinMode(pinLedRouge, OUTPUT);
  pinMode(pinLedBlue, OUTPUT);
  digitalWrite(pinLedBlue, LOW);
  while (!Serial);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(BAND,PABOOST)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  outgoing = "ON";   // send a message
  sendMessage(outgoing, flag); //flag=0

  LoRa.onReceive(onReceive);
  LoRa.receive();
  delay(100);
}

void loop() {

  gpss();
  compare(redzone);
  light();
  smoke();
}

void compare(String payload) {

  DynamicJsonDocument redZone(1024);
  DeserializationError   error = deserializeJson(redZone, payload);
  //  if (error) {
  //    Serial.println(error.c_str());
  //    return;
  //  }
  lat2 = redZone["lat2"];
  lat1 = redZone["lat1"];
  long2 = redZone["long2"];
  long1 = redZone["long1"];

  if (lat >= lat2 && lat <= lat1 && lon >= long1 && lon <= long2)
  {
    type = "0";
    latlong = "{\"longitude\":" + String(10.189849) + ",\"type\":" + String(type) + ",\"latitude\":" + String(36.899368) + ",\"id_user\":1}";
    sendMessage(latlong, 2);
    Redzone();
  }
  //  else{
  //    Serial.println("Not equal");
  //  }
}
void Redzone() {
  digitalWrite(pinLedRouge, HIGH);
  //delay(500);
  digitalWrite(pinLedRouge, LOW);
  //delay(500);
  tone(speakerPin, 1000, 300);
}

void gpss() {
  while (Serial1.available()) { // check for gps data

    if (gps.encode(Serial1.read())) // encode gps data
    {
      gps.f_get_position(&lat, &lon); // get latitude and longitude
      Serial.print("Position: ");
      //Latitude
      Serial.print("Latitude: ");
      Serial.print(lat, 6);
      Serial.print(",");
      //Longitude
      Serial.print("Longitude: ");
      Serial.println(lon, 6);
    }
  }
}
void smoke() {

  int smokeValue = analogRead(smokePin);
  //  Serial.print("Smoke: ");
  //  Serial.println(smokeValue);
  if (smokeValue > 400) {

    //    Serial.print("Smoke: ");
    //    Serial.println(smokeValue);

    latlong = "{\"longitude\":" + String(lon) + ",\"type\":1" + ",\"latitude\":" + String(lat) + ",\"id_user\":1}";
    sendMessage(latlong, 2);
    digitalWrite(pinLedBlue, HIGH);
    //delay(500);
    digitalWrite(pinLedBlue, LOW);
    //delay(500);

    tone(speakerPin, 1000, 200);
  }
  else
  {
    digitalWrite(pinLedBlue, LOW);
    noTone(speakerPin);
  }
}
void light() {

  int lightValue = analogRead(pinLight);
  //  Serial.print("Light: ");
  //  Serial.println(lightValue);
  if ( lightValue < 100) {
    digitalWrite(pinLedVert, HIGH);

  } else {
    digitalWrite(pinLedVert, LOW);
  }


}

void onReceive(int packetSize) {
  if (packetSize == 0) return;

  flag = LoRa.read();
  String incoming = "";
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (flag == 1) {
    redzone = incoming;
  }
  Serial.println(incoming);
}
void sendMessage(String outgoing, int flag) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(flag);
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
}
