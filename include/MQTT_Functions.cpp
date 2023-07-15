#include "MQTT_Functions.h"
#include "GlobalVariabels.h"
#include <EthernetENC.h>
#include <PubSubClient.h>
#include "Project_Settings.h"

String MQTTSubscribeRoot[] = {"/Settings/Mode", "/Settings/ReadNumberZ1", "/Settings/ReadNumberZ2"};

//MQTT-Funktionen
void MQTT_callback(char* topic, byte* payload, unsigned int length)
{
  const String TempTopic = topic;
  String Value = (char*) payload;
  Value = Value.substring(0, length);
  
  if(TempTopic.substring(strlen(MQTT_rootpath), (strlen(MQTT_rootpath) + MQTTSubscribeRoot[0].length()))==MQTTSubscribeRoot[0])
  {
    if((Value.toInt() < 256)&&(Value.toInt() > 0))
      Mode = Value.toInt();
  }
  else if(TempTopic.substring(strlen(MQTT_rootpath), (strlen(MQTT_rootpath) + MQTTSubscribeRoot[1].length()))==MQTTSubscribeRoot[1])
  {
    if((Value.toInt() < 100)&&(Value.toInt() > 0))
      ReadRelations[0] = Value.toInt();
  }  
  else if(TempTopic.substring(strlen(MQTT_rootpath), (strlen(MQTT_rootpath) + MQTTSubscribeRoot[2].length()))==MQTTSubscribeRoot[2])
  {
    if((Value.toInt() < 100)&&(Value.toInt() > 0))
      ReadRelations[1] = Value.toInt();
  }
}
bool MQTTinit()
{
  if(MQTTclient->connected())
    MQTTclient->disconnect();
  IPAddress IPTemp;
  IPTemp.fromString(MQTT_Server);
  MQTTclient->setServer(IPTemp, MQTT_Port);  
  MQTTclient->setCallback(MQTT_callback);
  unsigned long int StartTime = millis();
  while ((millis() < (StartTime + 5000))&&(!MQTTclient->connect(EthernetMAC, MQTT_Username, MQTT_Password))){
    delay(200);
  }
  if(MQTTclient->connected()){
    #ifdef BGTDEBUG
      Serial.println("MQTTclient connected");
    #endif
    String SubscribeTemp = "";
    String Root = MQTT_rootpath;
    for(int i = 0; i < 3; i++)
    {
      SubscribeTemp = MQTT_rootpath + MQTTSubscribeRoot[i];
      MQTTclient->subscribe(SubscribeTemp.c_str());
    }
    return true;
  }
  else
  {
    #ifdef BGTDEBUG
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
bool MQTT_sendText(int _MSGType, String Text)
{
  return MQTT_sendMessage(_MSGType, (const uint8_t *)Text.c_str(), Text.length());
}

bool MQTT_sendMessage(int MQTT_MSGType, const uint8_t* MSG, uint8_t len)
{
  int lenPath = strlen(MQTT_rootpath);
  char strPathVar[lenPath+20];

  switch (MQTT_MSGType)
  {
  case MQTT_MSG_Error:
    sprintf(strPathVar, "%s/Error", MQTT_rootpath);
    return MQTTclient->publish(strPathVar, MSG, len, false);
  case MQTT_MSG_PowerConsumption:
    sprintf(strPathVar, "%s/PowerConsumptionZ1", MQTT_rootpath);
    return MQTTclient->publish(strPathVar, MSG, len, true);
  case MQTT_MSG_MeterReadingConsumption:
    sprintf(strPathVar, "%s/MeterReadingConsumptionZ1", MQTT_rootpath);
    return MQTTclient->publish(strPathVar, MSG, len, true);
  case MQTT_MSG_MeterReadingSupply:
    sprintf(strPathVar, "%s/MeterReadingSupplyZ1", MQTT_rootpath);
    return MQTTclient->publish(strPathVar, MSG, len, true);
  case (MQTT_MSG_PowerConsumption + 1):
    sprintf(strPathVar, "%s/PowerConsumptionZ2", MQTT_rootpath);
    return MQTTclient->publish(strPathVar, MSG, len, true);
  case (MQTT_MSG_MeterReadingConsumption + 1):
    sprintf(strPathVar, "%s/MeterReadingConsumptionZ2", MQTT_rootpath);
    return MQTTclient->publish(strPathVar, MSG, len, true);
  case (MQTT_MSG_MeterReadingSupply + 1):
    sprintf(strPathVar, "%s/MeterReadingSupplyZ2", MQTT_rootpath);
    return MQTTclient->publish(strPathVar, MSG, len, true);
  
  default:
    return false;
  }
}
bool MQTT_sendMessage(int MQTT_MSGType, int MSG)
{
  return MQTT_sendMessage(MQTT_MSGType, (const uint8_t*) IntToStr(MSG).c_str(), IntToStr(MSG).length());
}
bool MQTT_sendMessage(int MQTT_MSGType, uint8_t MSG)
{
  return MQTT_sendMessage(MQTT_MSGType, (const uint8_t*) IntToStr(MSG).c_str(), IntToStr(MSG).length());
}
bool MQTT_sendMessage(int MQTT_MSGType, uint32_t MSG)
{
  return MQTT_sendMessage(MQTT_MSGType, (const uint8_t*) IntToStr(MSG).c_str(), IntToStr(MSG).length());
}
bool MQTT_sendMessage(int MQTT_MSGType, float MSG)
{
  return MQTT_sendMessage(MQTT_MSGType, (const uint8_t*) IntToStr(MSG).c_str(), IntToStr(MSG).length());
}

String IntToStr(int _var)
{
    char Temp[20];
    sprintf(Temp,"%d",_var);
    return Temp;
}
String IntToStr(char _var)
{
    char Temp[20];
    sprintf(Temp,"%d",_var);
    return Temp;
}
String IntToStr(long int _var)
{
    char Temp[20];
    sprintf(Temp,"%ld",_var);
    return Temp;
}
String IntToStr(uint32_t _var)
{
    char Temp[20];
    sprintf(Temp,"%lu",_var);
    return Temp;
}
String IntToStr(double _var)
{
    char Temp[20];
    sprintf(Temp,"%f",_var);
    return Temp;
}
String IntToStrHex(int _var)
{
    char Temp[20];
    sprintf(Temp,"%x",_var);
    return Temp;
}
