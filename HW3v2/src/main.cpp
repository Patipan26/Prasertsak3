#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <DHT.h>

// DHT sensor setup 
#define DHTTYPE DHT22
#define DHTPIN 22
DHT dht(DHTPIN, DHTTYPE);
// WIFI config
const char *ssid = "Wokwi-GUEST"; // setup SSID of WiFi
const char *password = "";  // setup WiFi password
WiFiClient wifiClient;
// MQTT client config
const char *mqttServer = "broker.hivemq.com"; // setup MQTT broker URL or IP Address
const int mqttPort = 1883;  // setup MQTT port
const char *mqttUser = "";  // setup MQTT user
const char *mqttPassword = "";  // setup MQTT password
const char *topic_pub = "ITKPS/Labmqtt/counter";  // setup topic name for publishing


PubSubClient mqttClient(wifiClient);
// some variables buffer
String publishMessage;



void setup_wifi()
{
  // Start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi connected on ");
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());
  Serial.print(" MAC address: ");
  Serial.println(WiFi.macAddress());
}

void reconnectMQTT()
{
  
  // Loop until reconnected
  char mqttinfo[80];
  char clientId[100] = "\0";
  snprintf(mqttinfo, 75, "Attempting MQTT connection at %s:%d (%s/%s)...", 
                          mqttServer, mqttPort, mqttUser, mqttPassword);
  while (!mqttClient.connected())
  {
    Serial.println(mqttinfo);
    sprintf(clientId, "ESP32Client-%04X", random(0xffff));
    if (mqttClient.connect(clientId))
    {
      Serial.println("---> MQTT Broker connected...");
      // subscribe here after connected
      // ในตัวอย่างของฝั่งส่งไม่มีการ subscribe topic ใด
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");      
      delay(5000);  // Recommended: Wait 5 seconds before retrying
    }
  }
}

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  mqttClient.setServer(mqttServer, mqttPort);
  // 
  // setup pin for demo publish button
  //
  // pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{
  if (!mqttClient.connected())
  {
    reconnectMQTT();
  }
  mqttClient.loop();

  float temperature = dht.readTemperature();
  float humidity  = dht.readHumidity();

  publishMessage = String(temperature) + " " + String(humidity);
  mqttClient.publish(topic_pub,publishMessage.c_str());
  Serial.println(publishMessage);
  delay(500); // this speeds up the simulation
}