#include "MQTT_Functions.h"
#include "GlobalVariabels.h"
#include <EthernetENC.h>
#include <PubSubClient.h>
#include "Project_Settings_1.h"

//MQTT-Funktionen
/*void MQTT_callback(char* topic, byte* payload, unsigned int length)
{

//  const String TempTopic = topic;
//  String Value = (char*) payload;
//  Value = Value.substring(0, length);
  payload[length] = 0;
  //if(TempTopic.substring(strlen(MQTT_rootpath), (strlen(MQTT_rootpath) + MQTTSubscribeRoot[0].length()))==MQTTSubscribeRoot[0])
  if(strncmp(&topic[strlen(MQTT_rootpath)], &MQTTSubscribeRoot[0][0], strlen(&MQTTSubscribeRoot[0][0]))==0)
  {
    int Value = 0;
    sprintf((char *) payload, "%i", Value);
    if((Value < 256)&&(Value > 0))
      Mode = Value;
  }
//  else if(TempTopic.substring(strlen(MQTT_rootpath), (strlen(MQTT_rootpath) + MQTTSubscribeRoot[1].length()))==MQTTSubscribeRoot[1])
  else if(strncmp(&topic[strlen(MQTT_rootpath)], &MQTTSubscribeRoot[1][0], strlen(&MQTTSubscribeRoot[1][0]))==0)
  {
    int Value = 0;
    sprintf((char *) payload, "%i", Value);
    if((Value < 100)&&(Value > 0))
      ReadRelations[0] = Value;
  }  
//  else if(TempTopic.substring(strlen(MQTT_rootpath), (strlen(MQTT_rootpath) + MQTTSubscribeRoot[2].length()))==MQTTSubscribeRoot[2])
  else if(strncmp(&topic[strlen(MQTT_rootpath)], &MQTTSubscribeRoot[2][0], strlen(&MQTTSubscribeRoot[2][0]))==0)
  {
    int Value = 0;
    sprintf((char *) payload, "%i", Value);
    if((Value < 100)&&(Value > 0))
      ReadRelations[1] = Value;
  }
}*/
bool MQTTinit()
{
  if(MQTTclient.connected())
    MQTTclient.disconnect();
  IPAddress IPTemp;
  IPTemp.fromString(MQTT_Server);
  MQTTclient.setServer(IPTemp, MQTT_Port);          
  if(!MQTTclient.setBufferSize(64))
  {
    Serial.println("MQTT_RS_F");
  }
//  Serial.print("Buffer: ");
//  Serial.println(MQTTclient.getBufferSize());
//  MQTTclient.setCallback(MQTT_callback);
  unsigned long int StartTime = millis();
  while ((millis() < (StartTime + 5000))&&(!MQTTclient.connect(EthernetMAC, MQTT_Username, MQTT_Password))){
    delay(200);
  }
  if(MQTTclient.connected()){
    #ifdef BGTDEBUG2
      Serial.println("MQTTclient connected");
    #endif
/*    String SubscribeTemp = "";
    String Root = MQTT_rootpath;
    char SubscribeTemp[strlen(MQTT_rootpath)+25] = "";
    for(int i = 0; i < 3; i++)
    {
      SubscribeTemp = MQTT_rootpath + MQTTSubscribeRoot[i];
      sprintf(SubscribeTemp, "%s%s", MQTT_rootpath, MQTTSubscribeRoot[i]);
      MQTTclient.subscribe(SubscribeTemp);
    }*/
    return true;
  }
  else
  {
    #ifdef BGTDEBUG2
      Serial.println("MQTTclient connection failure");
/*      Serial.print("MAC-Adresse: ");
      Serial.println((varConfig.NW_Flags & NW_EthernetActive)?EthernetMAC:WiFi.macAddress().c_str());
      Serial.print("Benutzername: ");
      Serial.println(varConfig.MQTT_Username);
      Serial.print("Passwort: ");
      Serial.println(varConfig.MQTT_Password);
      Serial.print("Port: ");
      Serial.println(varConfig.MQTT_Port);
      Serial.print("Server: ");
      Serial.println(varConfig.MQTT_Server);*/
    #endif
    
    return false;
  }
}
bool MQTT_sendText(int _MSGType, const char * Text)
{
  return MQTT_sendMessage(_MSGType, (const uint8_t *)Text, strlen(Text));
}

bool MQTT_sendMessage(int MQTT_MSGType, const uint8_t* MSG, uint8_t len)
{
  int lenPath = strlen(MQTT_rootpath);
  char strPathVar[lenPath+20];
  switch (MQTT_MSGType)
  {
  case MQTT_MSG_Error:
    sprintf(strPathVar, "%s/Error", MQTT_rootpath);
    return MQTTclient.publish(strPathVar, MSG, len, true);
  case MQTT_MSG_Mode:
    sprintf(strPathVar, "%s/Mode", MQTT_rootpath);
    return MQTTclient.publish(strPathVar, MSG, len, false);
  case MQTT_MSG_PowerConsumption:
    sprintf(strPathVar, "%s/PwrConsZ%u", MQTT_rootpath, 1);
    return MQTTclient.publish(strPathVar, MSG, len, false);
  case MQTT_MSG_MeterReadingConsumption:
    sprintf(strPathVar, "%s/MtrRdConsZ%u", MQTT_rootpath, 1);
    return MQTTclient.publish(strPathVar, MSG, len, true);
  case MQTT_MSG_MeterReadingSupply:
    sprintf(strPathVar, "%s/MtrRdSplyZ%u", MQTT_rootpath, 1);
    return MQTTclient.publish(strPathVar, MSG, len, true);
  case (MQTT_MSG_PowerConsumption + 1):
    sprintf(strPathVar, "%s/PwrConsZ%u", MQTT_rootpath, 2);
    return MQTTclient.publish(strPathVar, MSG, len, false);
  case (MQTT_MSG_MeterReadingConsumption + 1):
    sprintf(strPathVar, "%s/MtrRdConsZ%u", MQTT_rootpath, 2);
    return MQTTclient.publish(strPathVar, MSG, len, true);
  case (MQTT_MSG_MeterReadingSupply + 1):
    sprintf(strPathVar, "%s/MtrRdSplyZ%u", MQTT_rootpath, 2);
    return MQTTclient.publish(strPathVar, MSG, len, true);
  
  default:
    return false;
  }
}
bool MQTT_sendMessage(int MQTT_MSGType, int MSG)
{
  char * StrMSG = IntToStr(MSG);
  bool Temp = MQTT_sendMessage(MQTT_MSGType, (const uint8_t*) StrMSG, strlen(StrMSG));
  delete[] StrMSG;
  return Temp;
}
bool MQTT_sendMessage(int MQTT_MSGType, uint8_t MSG)
{
  char * StrMSG = IntToStr(MSG);
  bool Temp = MQTT_sendMessage(MQTT_MSGType, (const uint8_t*) StrMSG, strlen(StrMSG));
  delete[] StrMSG;
  return Temp;
}
bool MQTT_sendMessage(int MQTT_MSGType, uint32_t MSG)
{
  char * StrMSG = IntToStr(MSG);
  bool Temp = MQTT_sendMessage(MQTT_MSGType, (const uint8_t*) StrMSG, strlen(StrMSG));
  delete[] StrMSG;
  return Temp;
}
bool MQTT_sendMessage(int MQTT_MSGType, float MSG)
{
  char * StrMSG = IntToStr(MSG);
  bool Temp = MQTT_sendMessage(MQTT_MSGType, (const uint8_t*) StrMSG, strlen(StrMSG));
  delete[] StrMSG;
  return Temp;
}

char * IntToStr(int _var)
{
    char * Temp = new char[20];
    sprintf(Temp,"%d",_var);
    return Temp;
}
char * IntToStr(char _var)
{
    char * Temp = new char[20];
    sprintf(Temp,"%d",_var);
    return Temp;
}
char * IntToStr(long int _var)
{
    char * Temp = new char[20];
    sprintf(Temp,"%ld",_var);
    return Temp;
}
char * IntToStr(uint32_t _var)
{
    char * Temp = new char[20];
    sprintf(Temp,"%lu",_var);
    return Temp;
}
char * IntToStr(double _var)
{
    char * Temp = new char[20];
    sprintf(Temp,"%f",_var);
    return Temp;
}
char * IntToStrHex(int _var)
{
    char * Temp = new char[20];
    sprintf(Temp,"%x",_var);
    return Temp;
}
