#include <ESP8266WiFi.h>
#include <Time.h>
#include "random.h"
#include "global.h"

/*+++++++++++++++++++Fn declaration++++++++++++++++++*/
void set_wifi(void); 
void initTime(void);
void blinkLED(void);
/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*+++++++++++For initiating wifi connect+++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/
void set_wifi(void)
{
  Serial.println("WiFi Initiating |^^^-^^^| ");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WLAN_SSID,WLAN_PASS);
  digitalWrite(LED_PIN, HIGH);
  while (WiFi.status() != WL_CONNECTED)
  {
      Serial.print(".");
      digitalWrite(LED_PIN, millis()>>8&1);  
      delay(50);
  }
  digitalWrite(LED_PIN, LOW);
  Serial.println("WiFi connected");
}
/*+++++++++++For getting NTP time stamps+++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/
void initTime(void)
{
  time_t epochTime;
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  while (true)
  {
      epochTime = time(NULL);
      if (epochTime == 0)
      {
          Serial.println("Fetching NTP epoch time failed! Waiting 2 seconds to retry.");
          delay(2000);
      }
      else
      {
          Serial.printf("Fetched NTP epoch time is: %lu.\r\n", epochTime);
          break;
      }
  }
}
/*+++++++++++++++For led indication+++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/
void blinkLED(void)
{
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
}
