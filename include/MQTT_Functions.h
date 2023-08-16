#ifndef MQTT_Functions
#define MQTT_Functions

#include <Arduino.h>
#include <PubSubClient.h>

enum{
  MQTT_MSG_PowerConsumption = 0,
  MQTT_MSG_MeterReadingConsumption = 2,
  MQTT_MSG_MeterReadingSupply = 4,
  MQTT_MSG_Error = 6,
  MQTT_MSG_Mode = 7
};
//const char MQTTSubscribeRoot[3][25] = {"/Settings/Mode", "/Settings/RelationZ1", "/Settings/RelationZ2"};


bool MQTTinit();  //Wenn verbunden Rückgabewert true
//void MQTT_callback(char* topic, byte* payload, unsigned int length);
bool MQTT_sendMessage(int MQTT_MSGType, const uint8_t* MSG, uint8_t len);
bool MQTT_sendMessage(int MQTT_MSGType, int MSG);
bool MQTT_sendMessage(int MQTT_MSGType, double MSG);
bool MQTT_sendMessage(int MQTT_MSGType, uint8_t MSG);
bool MQTT_sendMessage(int MQTT_MSGType, uint32_t MSG);
bool MQTT_sendText(int _MSGType, const char * Text);
char * IntToStr(int _var);
char * IntToStr(float _var);
char * IntToStrHex(int _var);
char * IntToStr(uint32_t _var);


#include "MQTT_Functions.cpp"
#endif