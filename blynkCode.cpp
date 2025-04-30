/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID           "TMPL2AbcmmxM2"
#define BLYNK_TEMPLATE_NAME         "ESE3500"
#define BLYNK_AUTH_TOKEN            "GbFkys4_FPw8G8-EQ9veBrG9TAoxZ7Q4"

// Your WiFi credentials.
// Set password to "" for open networks.
#define MY_SSID                     "AirPennNet-Device"
#define MY_PASSWORD                 "penn1740wifi"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = BLYNK_AUTH_TOKEN;

// Define UART pins for communication with ATMEGA
#define UART_TX_PIN 17
#define UART_RX_PIN 18  

void setup()
{
  // Debug console to USB serial
  Serial.begin(115200);
  
  // UART communication with ATMEGA
  Serial1.begin(9600, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);
  
  // Connect to Blynk
  Blynk.begin(auth, MY_SSID, MY_PASSWORD);
  
  Serial.println("ESP32 initialized and connected to Blynk");
}

{
  Blynk.run();
  
  // Forward serial data from ATMEGA to debug console
  while (Serial1.available()) {
    char c = Serial1.read();
    Serial.write(c);  
  }
}

BLYNK_WRITE(V0)
{
  String inputText = param.asStr();  // Get text input
  Serial.println("Sending to ATMEGA: " + inputText);  
  Serial1.println(inputText);  // Send to ATmega via UART
}
