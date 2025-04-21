
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID           "TMPL2AbcmmxM2" 
#define BLYNK_TEMPLATE_NAME         "ESE3500"
#define BLYNK_AUTH_TOKEN            "GbFkys4_FPw8G8-EQ9veBrG9TAoxZ7Q4"

#define MY_SSID                     "AirPennNet-Device" 
#define MY_PASSWORD                 "penn1740wifi" 

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
#define CONTROL_PIN 17


void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, MY_SSID, MY_PASSWORD);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(CONTROL_PIN, OUTPUT);
}

void loop()
{
  Blynk.run();                
    
}

BLYNK_WRITE(V1)
{
  if(param.asInt() == 1)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(CONTROL_PIN, HIGH);
  }
  else
  {
   digitalWrite(LED_BUILTIN, LOW);
   digitalWrite(CONTROL_PIN, LOW); 
  }
}

BLYNK_WRITE(V0)
{
  String inputText = param.asStr();  // get input from app
  Serial.println("Received from Blynk: " + inputText);
  Serial1.println(inputText);  //send to atmega
}