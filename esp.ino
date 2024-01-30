#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Replace these with your Wi-Fi and MQTT broker details
const char* ssid = "bijeli45";
const char* password = "12345678";
const char* mqttServer = "192.168.23.69";
const int mqttPort = 1883;

const int sensorPin = A0;

WiFiClient espClient;
PubSubClient client(espClient);

const int pirPin = 2;

void setup() {
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  setupWifi();
  client.setServer(mqttServer, mqttPort);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  int pirValue = digitalRead(pirPin);
  int mqValue = analogRead(A0);

  if (mqValue > 345) {
      Serial.println("G1");
      client.publish("sensors/gas", "Zrak nije dobar. Plin je detektiran.");
    }
  else{
    client.publish("sensors/gas","Zrak je dobar.");
  }

  if (pirValue == HIGH) {
    Serial.println("M1");
    client.publish("sensors/motion", "Pokret detektiran. Svijetla su upaljena.");
  } else {
    Serial.println("M0");
    client.publish("sensors/motion", "Svijetla ugasena.");
  }

  if (Serial.available() > 0) {
    String receivedData = Serial.readStringUntil('\n');
    receivedData.trim();

    if (receivedData.equals("C1")) {
      client.publish("sensors/code", "Tocna sifra.");
    } else if (receivedData.equals("C0")) {
      client.publish("sensors/code", "Netocna sifra");
    }
  }

  delay(1000);
}

void setupWifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}
