#include <SPI.h>
#include <mcp2515.h>
#define VENT 3 //pin digital 3

struct can_frame canMsg;
MCP2515 mcp2515(13);  // ss, pin 13 en Ard NANO


void setup() {
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();

  pinMode(VENT, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    if (canMsg.can_id == 0xC1) {  //lectura del id DEL modulo DHT11
      if (canMsg.data[1] >= 28) {  // valor limite de temperatura
        digitalWrite(VENT, HIGH);
      } else {  // si el  valor es menor lo apaga
        digitalWrite(VENT, LOW);
      }
    }
  }
}