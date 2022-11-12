// ******************************************************************
// COMP-10184
// External LED Wiring, PIR Sensor Test Program
//
// This program...
//
// @author  Damanpreet Singh
// @id   000741359
//
// I created this work and I have not shared it with anyone else.

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

const char *ssid = "$$$$$";
const char *password = "$$$$$$$";

WiFiClient client;

unsigned long myChannelNumber = 1932728;
const char *myWriteAPIKey = "RBFWDW1UBVORQKNR";

// pin assignments for external LEDs
#define PIN_LED_GREEN D1
#define PIN_LED_YELLOW D2
#define PIN_LED_RED D3

// pin assignment for PIR input
#define PIN_PIR D5

void setup()
{
  Serial.begin(115200);
  Serial.println("\nAsyncThingSpeak");

  Serial.println("Name: Damanpreet Singh");
  Serial.println("Student Number: 000741359");

  // setup LED outputs
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_YELLOW, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);

  // setup PIR input
  pinMode(PIN_PIR, INPUT);

  // built-in LED
  //pinMode(LED_BUILTIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client); // Initialize ThingSpeak
}
// ******************************************************************
// boolean variable to check if the PIR sensor is triggered
boolean triggered = false;
// ***********************************
// isr for PIR sensor/ MOtion Detetion sensor
void IRAM_ATTR isr()
{
  triggered = true;
}


void loop()
{
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED)
  {
    // YELLOW LED on
    digitalWrite(PIN_LED_YELLOW, HIGH);
    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_RED, LOW);

    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, password); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
    Serial.printf("\nDevice Connected Via IP: %s \n", WiFi.localIP().toString().c_str());
  }
  // YELLOW GREEN LED on since its connected
  digitalWrite(PIN_LED_GREEN, HIGH);
  digitalWrite(PIN_LED_YELLOW, LOW);
  digitalWrite(PIN_LED_RED, LOW);

  attachInterrupt(PIN_PIR, isr, RISING);

  if (triggered){
    // RED LED on is sensor is triggered
    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_YELLOW, LOW);
    digitalWrite(PIN_LED_RED, HIGH);
    delay(2000);
    digitalWrite(PIN_LED_GREEN, HIGH);
    digitalWrite(PIN_LED_YELLOW, LOW);
    digitalWrite(PIN_LED_RED, LOW);

    /// @brief This will writes the value of the field to the ThingSpeak channel.
    // will print a message is serial port if trigger was sent to the channel
    int x = ThingSpeak.writeField(myChannelNumber, 1, 1, myWriteAPIKey);
    if (x == 200)
    {
      Serial.println("Channel update successful.");
    }
    else
    {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    delay(20000);
    triggered = false;
  }
  delay(1);
  
}

