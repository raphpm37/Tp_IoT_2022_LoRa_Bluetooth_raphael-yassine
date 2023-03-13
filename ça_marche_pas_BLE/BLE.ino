#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "SHT2x.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DI0 26
#define freq 8695E5
#define sf 9
#define sb 125E3

#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/

//float temperature = 0;
//float humidity = 0;

const char *ssid = "eduharam";
const char *password = "ici123456";


const int ledPin = 4;

const char *mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
float temperature = 0;
float humidity = 0;
union pack
{
  uint8_t frame[16]; // trames avec octets
  float data[4]; // 4 valeurs en virgule flottante
} sdp ;
SHT2x SHT2x;
//----------------------------------------------------
void setup(){
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    Wire.begin(21, 22);
    SHT2x.begin();
    Serial.begin (115200);
    pinMode(DI0, INPUT); // signal interrupt
    SPI.begin(SCK,MISO,MOSI,SS);
    LoRa.setPins(SS,RST,DI0);

    if (!LoRa.begin(freq)) {
      Serial.println("Starting LoRa failed!");
      while (1);
    }
    LoRa.setSpreadingFactor(sf) ;
    LoRa.setSignalBandwidth(sb) ;
}
//----------------------------------------------------
void setup_wifi()
{
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}
//----------------------------------------------------
void setup_bt() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("Long name works now");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setValue("Hello World says Neil");
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}
//----------------------------------------------------
void callback(char *topic, byte *message, unsigned int length)
{
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }
    Serial.println();

    // Feel free to add more if statements to control more GPIOs with MQTT

    // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
    // Changes the output state according to the message
    if (String(topic) == "esp32/output")
    {
        Serial.print("Changing output to ");
        if (messageTemp == "on")
        {
            Serial.println("on");
            digitalWrite(ledPin, HIGH);
        }
        else if (messageTemp == "off")
        {
            Serial.println("off");
            digitalWrite(ledPin, LOW);
        }
    }
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("espClient"))
        {
            Serial.println("connected");
            // Subscribe
            client.subscribe("esp32/output");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}




float d1;
float d2;
int i =0;

void loop() // la boucle de 1’emetteur
{
  int rssi;
  char rssitring[8];
  i=0;
    while (LoRa.available()) {
      sdp.frame[i]=LoRa.read();i++; 
  }
    d1=sdp.data[0] ;d2=sdp.data[1]; 
    rssi=LoRa.packetRssi(); // force du signal en réception en dB
    //Serial.println(d1); Serial.println(d2);
    Serial.println(rssi) ;
    dtostrf(rssi, 1, 2, rssitring);
    Serial.println(rssitring) ;
    client.publish("esp32/signal", rssitring);
    delay(5000);
 
 //if (!client.connected())
    //{
      //  reconnect();
   // }
   /// client.loop();

    //long now = millis();
    //if (now - lastMsg > 5000)
    //{
        //lastMsg = now;
        //temperature = SHT2x.GetTemperature();
        //char tempString[8];
       // dtostrf(temperature, 1, 2, tempString);
        //Serial.print("Temperature: ");
        //Serial.println(tempString);
        //Serial.println(rssi) ;
        //client.publish("esp32/signal", rssi);

        //humidity = SHT2x.GetHumidity();

        // Convert the value to a char array
        //char humString[8];
        //dtostrf(humidity, 1, 2, humString);
        //Serial.print("Humidity: ");
        //Serial.println(humString);
        //client.publish("esp32/humidify", humString);
    //}
    
}
