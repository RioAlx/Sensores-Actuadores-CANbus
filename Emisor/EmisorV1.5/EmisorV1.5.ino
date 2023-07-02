/*
  //  EMISOR EN ARDUINO UNO R3 //
DHT11 id = 0xC1
LDR id = 0xB1
STEPPER = 0xA1
*/

#include <SPI.h>
#include <mcp2515.h>
#include <DHT.h>
#define LDR A0
#define BL 6
#define BR 7
#define DHTPIN 5  // pin digital 41
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int lectura = 0;

struct can_frame canMsg1;
struct can_frame canMsg2;
struct can_frame canMsg3;
MCP2515 mcp2515(10);  //cs pin 10 por defecto en Ard UNO 

void setup() {
  SPI.begin();
  dht.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();

  Serial.begin(9600);
  pinMode(BL, INPUT);
  pinMode(BR, INPUT);
  pinMode(LDR, INPUT);
}

void loop() {
  int h = dht.readHumidity();
  int t = dht.readTemperature();

  // SENSOR DHT11 //
  canMsg1.can_id = 0xC1;
  canMsg1.can_dlc = 2;
  canMsg1.data[0] = h;  //actualiza el valor de humedad
  canMsg1.data[1] = t;  //actualiza el valor de temperatura

  // SENSOR LDR //
  lectura = analogRead(LDR);
  canMsg2.can_id = 0xB1;
  canMsg2.can_dlc = 1;
  if (lectura < 500) {
    canMsg2.data[0] = 0x01;
  } else {
    canMsg2.data[0] = 0x00;
  }

  // ELEVADOR VENTANA //
  canMsg3.can_id = 0xA1;
  canMsg3.can_dlc = 2;
  if (digitalRead(BL)) {
    canMsg3.data[0] = 0x01;
  } else {
    canMsg3.data[0] = 0x00;
  }
  if (digitalRead(BR)) {
    canMsg3.data[1] = 0x01;
  } else {
    canMsg3.data[1] = 0x00;
  }

  // ENVIO DE LOS MENSAJES //
  mcp2515.sendMessage(&canMsg1);
  mcp2515.sendMessage(&canMsg2);
  mcp2515.sendMessage(&canMsg3);

  delay(100);
}
