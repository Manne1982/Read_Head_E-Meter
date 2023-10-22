#include <Arduino.h>
#include <EthernetENC.h>
#include <PubSubClient.h>
#include "MQTT_Functions.h"
#include "Project_Settings_1.h"
#include "GlobalVariabels.h"
#include <avr/wdt.h>
#define IOZ2 2
#define IOZ1 3



//int convertStrtoHEXStr(const uint8_t * _Input, char * _Output, uint16_t _CharCount);
int16_t evaluateData(uint8_t * Data, uint16_t length, uint32_t * CounterStates);
void switchSerialInput(uint8_t Output, uint8_t Mode);


void setup() {
  pinMode(IOZ1, OUTPUT);
  pinMode(IOZ2, OUTPUT);
  switchSerialInput(1, Mode);

  Serial.begin(9600);
  Ethernet.init(10);
  #ifdef BGTDEBUG2
    Serial.println("EI B");
  #endif
  if(Ethernet.begin(mac)) //Configure IP address via DHCP
  {
    #ifdef BGTDEBUG
      Serial.println(Ethernet.localIP());
      Serial.println(Ethernet.gatewayIP());
      Serial.println(Ethernet.subnetMask());
      Serial.println("Eth_ok");
    #endif
    // e_client = new EthernetClient;
    // delay(300);
    // MQTTclient = new PubSubClient(e_client);
    // delay(300);
    // #ifdef BGTDEBUG
    //   Serial.println("Var erstellt");
    // #endif
  }
  else
  {
    #ifdef BGTDEBUG2
      Serial.println("EFehler");
    #endif
  }
  while(!MQTTinit())
  {
    #ifdef BGTDEBUG
      delay(500);
      Serial.print("MQTT: ");
      Serial.println(MQTTclient.state());
      Serial.print("ETH: ");
      Serial.println(Ethernet.linkStatus());
    #endif
  }
  #ifdef BGTDEBUG2
    Serial.println("MQTT_OK");
    //Serial.println("MQTT_D");
    Serial.println(MQTTclient.state());
  #endif

  wdt_enable(WDTO_4S);
  MQTT_sendMessage(MQTT_MSG_Mode, (int) Mode);
  TimeOut = millis() + (unsigned long) readTimeout_Seconds * 1000;
}

void loop() {
  if(!MQTTclient.loop()){
    if(TimeOut < millis())
    {
      if(Ethernet.linkStatus() == LinkON)
      {
        if(!MQTTinit())
        {
          TimeOut = millis() + 300000;
          goto End;
        }
        else
        TimeOut = millis() + (unsigned long) readTimeout_Seconds * 1000;
      }
      else
      {
        Ethernet.begin(mac);
        goto End;
      }
    }
    else
    {
      goto End;
    }
  }
  if((TimeOut < millis()) && (Mode & 0x03))
  {
    #ifdef BGTDEBUG2
      Serial.print("Timeout Z");
      Serial.println((CountVar%2) +1);
    #endif

    Mode &= ~(1<<((CountVar)%2));
    char Temp[10] = "";
    MQTT_sendMessage(MQTT_MSG_Mode, Mode);
    sprintf(Temp, "Timeout %u", (unsigned int) ((CountVar)%2));
    MQTT_sendText(MQTT_MSG_Error, Temp);
    CountVar++;
    switchSerialInput(((CountVar)%2) + 1, Mode);
    TimeOut = millis() + (unsigned long) readTimeout_Seconds * 1000;

    #ifdef BGTDEBUG2
      Serial.print("Mode: ");
      Serial.println(Mode);
    #endif

  }
  if (Serial.available()) //Daten auf Serieller Schnittstelle angekommen
  {
    int16_t currentUsage = 0;
    uint32_t counterValue[2] = {0, 0};
    Buffer = (char*)malloc(380);
    int length = Serial.readBytes(Buffer, 380);
    Serial.println(length);
    Buffer[length] = 0;
    #ifdef BGTDEBUG2
      Serial.print("Länge: ");
      Serial.println(length);
    #endif
    if(length < 300)
    {
      free(Buffer);
      goto End;
    }
    else
    {
      ReadRelationsCount++;
      if(((Mode & 0x03)==0x03)&&(ReadRelationsCount >= ReadRelations[CountVar%2]))
      {
        switchSerialInput(((CountVar+1)%2) + 1, Mode);
      }
    }

    TimeOut = millis() + (unsigned long) readTimeout_Seconds * 1000;
    currentUsage = evaluateData((uint8_t *)Buffer, length, counterValue);
    free(Buffer);
    MQTT_sendMessage(MQTT_MSG_PowerConsumption + (CountVar%2), currentUsage);
    MQTT_sendMessage(MQTT_MSG_MeterReadingConsumption + (CountVar%2), counterValue[0]);
    MQTT_sendMessage(MQTT_MSG_MeterReadingSupply + (CountVar%2), counterValue[1]);
/*    if(((counterValue[CountVar%2][0]-counterValueOld[CountVar%2][0]) < 1000)&&((counterValue[CountVar%2][1]-counterValueOld[CountVar%2][1]) < 1000))
    {
      MQTT_sendMessage(MQTT_MSG_PowerConsumption + (CountVar%2), currentUsage[CountVar%2]);
      MQTT_sendMessage(MQTT_MSG_MeterReadingConsumption + (CountVar%2), counterValue[CountVar%2][0]);
      MQTT_sendMessage(MQTT_MSG_MeterReadingSupply + (CountVar%2), counterValue[CountVar%2][1]);
       #ifdef BGTDEBUG2    
        Serial.print("currentUsageZ");
        Serial.print(CountVar%2 + 1);
        Serial.print(": ");
        Serial.println(currentUsage[CountVar%2]);
        Serial.print("counterValue1Z");
        Serial.print(CountVar%2 + 1);
        Serial.print(": ");
        Serial.println(counterValue[CountVar%2][0]);
        Serial.print("counterValue2Z");
        Serial.print(CountVar%2 + 1);
        Serial.print(": ");
        Serial.println(counterValue[CountVar%2][1]);
      #endif
      counterValueOld[CountVar%2][0] = counterValue[CountVar%2][0];
      counterValueOld[CountVar%2][1] = counterValue[CountVar%2][1];
    }
    else
    {
      if((counterValueOld[CountVar%2][0] == 0)&&(counterValue[CountVar%2][0]))
        counterValueOld[CountVar%2][0] = counterValue[CountVar%2][0];
      if((counterValueOld[CountVar%2][1] == 0)&&(counterValue[CountVar%2][1]))
        counterValueOld[CountVar%2][1] = counterValue[CountVar%2][1];
    }*/
    #ifdef BGTDEBUG2    
      Serial.print("VerhältnissZähler: ");
      Serial.print(ReadRelationsCount);
      Serial.print(" | Soll: ");
      Serial.print(ReadRelations[CountVar%2]);
      Serial.print(" | Mode: ");
      Serial.println(Mode);
    #endif
    if(((Mode & 0x03)==0x03)&&(ReadRelationsCount >= ReadRelations[CountVar%2]))
    {
      CountVar++;
      ReadRelationsCount = 0;
    }
  }
  End:
  wdt_reset();
}

int16_t evaluateData(uint8_t * Data, uint16_t length, uint32_t * CounterStates)
{
  int Positions[4] = {0, 0, 0, 0};
  int ValueStart[3] = {0, 0, 0};
  int ValueStop[3] = {0, 0, 0};
  int16_t currentConsume = 0;


  for(uint16_t i = 0; i < length; i++)
  {
    if((Data[i] == MSGStartMeterReadingConsumption[Positions[0]])&&(Positions[0] < 6))
    {
      if(Positions[0]<4)
      {
        Positions[0]++;
      }
      else
      {
        ValueStart[0] = i+1;
        Positions[0]++;
        ValueStop[0] = 1;
      }
    }
    else
    {
      if((Positions[0]>0)&&(Positions[0]<5))
        Positions[0] = 0;
    }
    if((Data[i] == MSGStartMeterReadingSupply[Positions[1]])&&(Positions[1] < 6))
    {
      if(Positions[1]<4)
      {
        Positions[1]++;
      }
      else
      {
        ValueStart[1] = i+1;
        Positions[1]++;
        ValueStop[1] = 1;
      }
    }
    else
    {
      if((Positions[1]>0)&&(Positions[1]<5))
        Positions[1] = 0;
    }
    if((Data[i] == MSGStartPowerConsumption[Positions[2]])&&(Positions[2] < 6))
    {
      if(Positions[2]<4)
      {
        Positions[2]++;
      }
      else
      {
        ValueStart[2] = i+1;
        Positions[2]++;
        ValueStop[2] = 1;
      }
    }
    else
    {
      if((Positions[2]>0)&&(Positions[2]<5))
        Positions[2] = 0;
    }
    if((Data[i] == MSGEnd[Positions[3]])&&(Positions[3] < 3))
    {
      if(Positions[3]<1)
      {
        Positions[3]++;
      }
      else
      {
        Positions[3] = 0;
        
        for(int j = 0; j < 3; j++)
        {
          if(ValueStop[j] == 1)
          {
            ValueStop[j] = i - 3;
            break;
          }
        }
      }
    }
    else
    {
      if((Positions[3]>0)&&(Positions[3]<2))
        Positions[3] = 0;
    }
    //Start (erstes Zeichen der Variable) und Ende (letztes Zeichen der Variable) der 3 Variablen ermittelt als nächstes Übertragung der Variablen als HEX-Werte über Serielle Schnittstelle
  }
  for(int i = 0; i < 3; i++)
  {
    #ifdef BGTDEBUG2
      Serial.print("ValueStart: ");
      Serial.println(ValueStart[i]);
    char Temp[70]="";
    convertStrtoHEXStr((uint8_t*)&Data[ValueStart[i]], Temp, 15);
    Serial.println(Temp);
    #endif
    if(i == 2)
    {
      uint8_t * pInteger = (uint8_t *) &currentConsume;
      pInteger[1] = Data[ValueStart[i]+7];
      pInteger[0] = Data[ValueStart[i]+8];
    }
    else
    {
      CounterStates[i] = 0;
      if(Positions[i] == 0)
        break;
      uint8_t * pInteger = (uint8_t *) &CounterStates[i];
      for(int j = 0; j < 4; j++)
        pInteger[j] = Data[ValueStart[i]+11 - j];
//      pInteger[3] = Data[ValueStart[i]+9];
//      pInteger[2] = Data[ValueStart[i]+10];
//      pInteger[1] = Data[ValueStart[i]+11];
//      pInteger[0] = Data[ValueStart[i]+12];
    }
  }
  return currentConsume;
}

int convertStrtoHEXStr(const uint8_t * _Input, char * _Output, uint16_t _CharCount)
{
  if(_CharCount<2)
    return 0;
  sprintf(_Output, "%02hhX", _Input[0]);
  for(uint16_t i = 1; i < _CharCount; i++)
  {
    sprintf(&_Output[i*3-1], " %02hhX", _Input[i]);
  
  }
  _Output[_CharCount * 3] = 0;
  return _CharCount * 3;
}
//---------------------------------------------------------------------
void switchSerialInput(uint8_t Output, uint8_t Mode)
{
  switch (Output)
  {
  case 1:
      if((Mode & 0x01)==0)
        break;
//      Serial.println("Ausgang Z1");
      digitalWrite(IOZ1, 1);
      digitalWrite(IOZ2, 0);
      break;
  case 2:
      if((Mode & 0x02)==0)
        break;
//      Serial.println("Ausgang Z2");
      digitalWrite(IOZ1, 0);
      digitalWrite(IOZ2, 1);
      break;
  
  default:
//      Serial.println("Ausgang Aus");
      digitalWrite(IOZ1, 0);
      digitalWrite(IOZ2, 0);
      break;
  }
}
