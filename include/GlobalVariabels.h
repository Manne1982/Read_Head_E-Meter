#ifndef GlobalVariabels
#define GlobalVariabels

#include <Arduino.h>
#include <EthernetENC.h>
#include <PubSubClient.h>


//Projektvariablen

const uint8_t MSGStartPowerConsumption[] = {0x01, 0x00, 0x10, 0x07, 0x00, 0xFF}; //Aktuelle Leistung
const uint8_t MSGStartMeterReadingConsumption[] = {0x01, 0x00, 0x01, 0x08, 0x00, 0xFF}; //Akueller Zählerstand Verbrauch
const uint8_t MSGStartMeterReadingSupply[] = {0x01, 0x00, 0x02, 0x08, 0x00, 0xFF};  //Aktueller Zählerstand Einspeisung
const uint8_t MSGEnd[] = {0x01, 0x77, 0x07};
uint64_t Counter = 0;
unsigned long lastReading = 0;
uint8_t ReadNumber[2] = {1, 1};
uint8_t Mode = 3; // 0 = no counter active | 1 = Z1 is active | 2 = Z2 is active | 3 = Z1 and Z2 is active
uint32_t CountVar = 0; //Variable for count the successful data reception
int16_t currentUsage[2] = {0, 0};
uint32_t counterValue[2][2] = {0, 0, 0, 0};
uint32_t counterValueOld[2][2] = {0, 0, 0, 0};
unsigned long TimeOut = 0;



//-----------------------------------
//Variablen und Funktion zur Fehlersuche (Absturz Nano nach Empfang von Nachrichten)
//MQTT Variablen
EthernetClient * e_client = 0;
PubSubClient * MQTTclient = 0;


//-----------------------------------
unsigned long Break_200ms = 0;                             //Variable fuer Dinge die alle 200ms ausgefuehrt werden
unsigned long Break_1s = 0;                                //Variable fuer Dinge die alle 1s ausgefuehrt werden
unsigned long Break_60s = 0;                               //Variable fuer Dinge die alle 60s ausgefuehrt werden
unsigned long Break_10m = 0;                               //Variable fuer Dinge die alle 10m ausgefuehrt werden
unsigned long Break_h = 0;                                 //Variable fuer Dinge die alle 1h ausgefuehrt werden




#endif