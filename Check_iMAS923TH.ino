#include <Arduino.h>
#include <WiMODLoRaWAN.h>
#include <SoftwareSerial.h>
SoftwareSerial mSerial(11, 12); // rx tx
WiMODLoRaWAN wimod(mSerial);

void setup() {
  Serial.begin(115200); // setup for debug
  mSerial.begin(115200);
  
  wimod.begin();
  delay(100); wimod.Reset(); delay(100);
  wimod.DeactivateDevice();
  
  Serial.print(F("Ping WiMOD: "));
  if (wimod.Ping() != true) {
    Serial.println("FAILED");
    while(1);
  }
  Serial.println("Ok");
}

void loop() {
  // put your main code here, to run repeatedly:

}
