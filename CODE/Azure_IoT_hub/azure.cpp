#include <Arduino.h>
#include <AzureIoTHub.h>
#include <AzureIoTProtocol_MQTT.h>
#include <AzureIoTUtility.h>
#include "azure.h"
#include "random.h"
#include "global.h"

/*++++++++++++++++++obj declaration++++++++++++++++++*/
static IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;
/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*+++++++++++++Primary connectionString +++++++++++++*/
const char *connectionString = "PASTE YOUR PRIMARY CONNECTION STRING HERE";
/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*+++++++++++++++++++Fn declaration++++++++++++++++++*/
void DoWork(void);
void set_client(void);
void Publish_Message(char *msg);
static void SendMessage(IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle, char *buffer);
static void sendCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void *userContextCallback);
IOTHUBMESSAGE_DISPOSITION_RESULT receiveMessageCallback(IOTHUB_MESSAGE_HANDLE message, void *userContextCallback);
/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*+++++++++++++For running LL_DoWork ++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/
void DoWork(void)
{
  IoTHubClient_LL_DoWork(iotHubClientHandle);
}
/*++++++++++++For message publishing ++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/
void Publish_Message(char *msg)
{
  SendMessage(iotHubClientHandle, msg);
}
/*++++++++++++For setting client handle++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/
void set_client(void)
{
   iotHubClientHandle     = IoTHubClient_LL_CreateFromConnectionString(connectionString, MQTT_Protocol);
   if (iotHubClientHandle == NULL)
   {
     Serial.println("IoTHubClient_CreateFromConnectionString : FAILED  !!");
     while (1);
   }
   else
     Serial.println("IoTHubClient_CreateFromConnectionString : SUCCESS !!");

   IoTHubClient_LL_SetOption(iotHubClientHandle, "product_info", "NodeMCU");
   IoTHubClient_LL_SetMessageCallback(iotHubClientHandle, receiveMessageCallback, NULL);
}
/*+++++++++++++For sendCallback(publish)+++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/
static void sendCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void *userContextCallback)
{
  if (IOTHUB_CLIENT_CONFIRMATION_OK == result)
  {
      Serial.println("Message sent to Azure IoT Hub : success");
      blinkLED();
  }
  else
      Serial.println("Message sent to Azure IoT Hub : failure");
}
/*++++++++++++++For publishing message+++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/
static void SendMessage(IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle, char *buffer)
{
    IOTHUB_MESSAGE_HANDLE messageHandle = IoTHubMessage_CreateFromByteArray((const unsigned char *)buffer, strlen(buffer));
    if (messageHandle == NULL)
        Serial.println("Unable to create a new IoTHubMessage.");
    else
    {
        Serial.printf("Created message: %s\n", buffer);
        
        if (IoTHubClient_LL_SendEventAsync(iotHubClientHandle, messageHandle, sendCallback, NULL) != IOTHUB_CLIENT_OK)
            Serial.println("Failed to hand over the message to IoTHubClient.");
        else
            Serial.println("IoTHubClient accepted the message for delivery.");
            
        IoTHubMessage_Destroy(messageHandle);
    }
}
/*+++++++++++For receiveMessageCallback +++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/
IOTHUBMESSAGE_DISPOSITION_RESULT receiveMessageCallback(IOTHUB_MESSAGE_HANDLE message, void *userContextCallback)
{
    size_t size;
    const unsigned char *buffer;
    IOTHUBMESSAGE_DISPOSITION_RESULT result;
   
    if (IoTHubMessage_GetByteArray(message, &buffer, &size) != IOTHUB_MESSAGE_OK)
    {
        Serial.println("Unable to IoTHubMessage_GetByteArray.");
        result = IOTHUBMESSAGE_REJECTED;
    }
    else
    {
        char *temp = (char *)malloc(size + 1);
        
        if(temp    == NULL)
          return IOTHUBMESSAGE_ABANDONED;

        strncpy(temp, (const char *)buffer, size);
        temp[size] = '\0';
        Serial.printf("Received C2D message: %s\n", temp);        
        free(temp);
        blinkLED();
    }
    return IOTHUBMESSAGE_ACCEPTED;
}   
