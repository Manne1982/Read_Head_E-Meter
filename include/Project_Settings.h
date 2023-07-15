#ifndef Project_Settings
#define Project_Settings

#include <Arduino.h>

enum {flagRH1 = 0x01, flagRH2 = 0x02};

//MQTT Login
const char MQTT_Username[]= "User";
const char MQTT_Password[]= "1234";
const char MQTT_Server[]= "192.168.178.2";
const char MQTT_rootpath[]= "/PowerBox/ElectricMeter";
const uint16_t MQTT_Port = 1883;

//Ethernet
uint8_t mac[6] = {0xA0,0xA1,0xA2,0xA3,0xA4,0xA6}; //For Ethernet connection
char const EthernetMAC[] = "A0:A1:A2:A3:A4:A6";         //For Ethernet connection (MQTT)

//ReadHeadSettings
uint8_t ReadRelations[2] = {1, 1};

uint8_t readTimeout_Seconds = 10;
uint8_t Mode = 3; // 0 = no counter active | 1 = Z1 is active | 2 = Z2 is active | 3 = Z1 and Z2 is active



#endif