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
unsigned long TimeOut = 0;
uint32_t CountVar = 0; //Variable for count the successful data reception
//uint32_t counterValueOld[2][2] = {0, 0, 0, 0};
uint8_t ReadRelationsCount = 0;
char * Buffer = 0;
//char Buffer[380];


//-----------------------------------
//Variablen und Funktion zur Fehlersuche (Absturz Nano nach Empfang von Nachrichten)
//MQTT Variablen
// EthernetClient * e_client = 0;
// PubSubClient * MQTTclient = 0;
EthernetClient e_client;
PubSubClient MQTTclient(e_client);


//-----------------------------------




#endif