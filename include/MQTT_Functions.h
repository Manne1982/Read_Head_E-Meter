#ifndef MQTT_Functions
#define MQTT_Functions

#include <Arduino.h>
#include <PubSubClient.h>

enum{
  MQTT_MSG_PowerConsumption = 0,
  MQTT_MSG_MeterReadingConsumption = 2,
  MQTT_MSG_MeterReadingSupply = 4,
  MQTT_MSG_Error = 6,
};


bool MQTTinit();  //Wenn verbunden Rückgabewert true
void MQTT_callback(char* topic, byte* payload, unsigned int length);
bool MQTT_sendMessage(int MQTT_MSGType, const uint8_t* MSG, uint8_t len);
bool MQTT_sendMessage(int MQTT_MSGType, int MSG);
bool MQTT_sendMessage(int MQTT_MSGType, double MSG);
bool MQTT_sendMessage(int MQTT_MSGType, uint8_t MSG);
bool MQTT_sendMessage(int MQTT_MSGType, uint32_t MSG);
bool MQTT_sendText(int _MSGType, String Text);
String IntToStr(int _var);
String IntToStr(float _var);
String IntToStrHex(int _var);
String IntToStr(uint32_t _var);


#include "MQTT_Functions.cpp"
#endif