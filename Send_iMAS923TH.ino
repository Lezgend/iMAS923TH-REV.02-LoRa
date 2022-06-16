#include <Arduino.h>
#include <WiMODLoRaWAN.h>
#include <SoftwareSerial.h>

SoftwareSerial mSerial(11, 12); // rx tx ||| 12 ==> RXD, 11 ==> TXD
WiMODLoRaWAN wimod(mSerial); 

TWiMODLORAWAN_TX_Data txData;

#define DEV_ADR 0x01020304 //  Device address (DevEUI)
#define STR_APPSKEY "1698069B9F2F95FC6E370B5F383D2659" // AppKey
#define STR_NWKSKEY "1698069B9F2F95FC6E370B5F383D2659" // NwkKey
unsigned char APPSKEY[16];
unsigned char NWKSKEY[16];

void setHEXStrToByte (const char* strID, uint8_t *byteID) {
  if (strlen (strID) % 2 != 0) return;
  for (size_t i = 0,j = 0; i < strlen (strID); i += 2, j++) {
    String byteHex = String (strID[i]) + String (strID[i+1]);
    byteID[j] = strtol(byteHex.c_str(), NULL, 16);
  }
}

void setup() {
  setHEXStrToByte(STR_APPSKEY, APPSKEY);
  setHEXStrToByte(STR_NWKSKEY, NWKSKEY);
  
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
  
TWiMODLORAWAN_ActivateDeviceData activationData;

  // set activate data
  activationData.DeviceAddress = DEV_ADR;
  memcpy(activationData.NwkSKey, NWKSKEY, 16);
  memcpy(activationData.AppSKey, APPSKEY, 16);
  
  bool isActivated = wimod.ActivateDevice(activationData);
  if (isActivated == false) {
    Serial.print("Fail Activate => code : ");
    Serial.println((int) wimod.GetLastResponseStatus());
    while(1);
  }
  // Activated section //
  Serial.println("Activated");
}

void loop() {
  Serial.println("Send data");
  const char msg[] = "Hello World!";
  txData.Port = 0x01;
  txData.Length = strlen(msg);
  memcpy(txData.Payload, msg, strlen(msg));
  wimod.SendUData(&txData);
  delay(30000);
}
