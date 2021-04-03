#include <ESP8266WiFi.h>
#include "random.h"
#include "global.h"
#include "azure.h"

/*++++++++++++++++Variable declaration+++++++++++++++*/
int messageCount = 0;
/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/

void setup() 
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  Serial.begin(9600);
  Serial.setDebugOutput(true);
  
  set_wifi();
  initTime();
  set_client();     
}

void loop() 
{
  while (Serial.available()>0)
  {
       if(Serial.read()=='S')
       { 
            char messagePayload[128];
            memset(messagePayload,0,sizeof(messagePayload));
            sprintf(messagePayload,"{deviceId : IOTHUBNAME , messageId :%d , temperature : 20 ,humidity : 31 , tempAlert:false}",messageCount);
            Publish_Message(messagePayload);
            messageCount++;
            delay(INTERVAL);
       }
  }
  DoWork();
}
