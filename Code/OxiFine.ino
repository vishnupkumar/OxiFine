#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
 
char auth[] = "your blynk auth token";       
char ssid[] = "your wifi ssid";                              
char pass[] = "your wifi password";

PulseOximeter pox;
int BPM, SpO2;
uint32_t tsLastReport = 0;
#define REPORTING_PERIOD_MS 1000
 
void onBeatDetected()
{
    Serial.println("Beat Detected!");
}
 
void setup()
{
    Serial.begin(115200);
    Blynk.begin(auth, ssid, pass);
 
    if (!pox.begin())
    {
         Serial.println("FAILED");
         for(;;);
    }
    else
    {
         Serial.println("SUCCESS");
         pox.setOnBeatDetectedCallback(onBeatDetected);
    }
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
}
 
void loop()
{
    pox.update();
    Blynk.run();
 
    BPM = pox.getHeartRate();
    SpO2 = pox.getSpO2();
    
    if (millis() - tsLastReport > REPORTING_PERIOD_MS)
    {
        Blynk.virtualWrite(V0, BPM);
        Blynk.virtualWrite(V1, SpO2);
        tsLastReport = millis();
    }
}
