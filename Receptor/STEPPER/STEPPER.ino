#include <SPI.h>
#include <mcp2515.h>
#include <Stepper.h>

/*
 BOTONES DE STOP EN LIMITES DE VENTANA
 LL - EN PIN DIGITAL 2
 LR - EN PIN DIGITAL 3
*/
int LL = 0; 
int LR = 0;

struct can_frame canMsg;
MCP2515 mcp2515(13);  // ss, pin 13 Ard NANO

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
Stepper myStepper(stepsPerRevolution, 4, 6, 5, 7); // Pin's de controlador en 4, 5, 6, 7

void setup() {
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();

  myStepper.setSpeed(100);  // set the speed at X rpm
  Serial.begin(9600);
}

void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    if (canMsg.can_id == 0xA1) {
      LL = digitalRead(2); //ubicacion de los botones límite
      LR = digitalRead(3); //pin digital 2 y 3
      if (LL) {  // botones de panico
        canMsg.data[0] = 0x00;
      } else {
        if (canMsg.data[0]) {
          myStepper.step(-stepsPerRevolution);
        }
      }
      if (LR) {  // botones de panico
        canMsg.data[1] = 0x00;
      } else {
        if (canMsg.data[1]) {
          myStepper.step(stepsPerRevolution);
        }
      }
    }
  }
}