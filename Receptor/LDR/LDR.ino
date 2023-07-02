#include <SPI.h>
#include <mcp2515.h>
#define LED 3 // pin digital 3 LDR

struct can_frame canMsg;
MCP2515 mcp2515(13);  // ss, pin 13 en Ard NANO


void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
}

void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    //Serial.println(canMsg3.can_id, 0xA4);
    if (canMsg.can_id == 0xB1) { //lectura del id DEL modulo LDR
      Serial.println(canMsg.data[0]);
      if ( canMsg.data[0] == 1){ // si el valor es 1 enciende
        digitalWrite(LED, 1);
      }else { // si el  valor es 0 apaga
        digitalWrite(LED, 0);
      }
    }
  }
}