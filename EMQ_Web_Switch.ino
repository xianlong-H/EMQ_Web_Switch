#include <WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = "602iot"; // Enter your WiFi name
const char *password = "18wulian";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "red263b0.cn.emqx.cloud";
const char *topic = "2018A14133/switch";
const char *mqtt_username = "602iot";
const char *mqtt_password = "602iot";
const int mqtt_port = 12498;

#define Green_LED 19
#define Red_LED 21
#define Blue_LED 18

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(Green_LED, OUTPUT);
  pinMode(Red_LED, OUTPUT);
  pinMode(Blue_LED, OUTPUT);
  // Set software serial baud to 115200;
  Serial.begin(115200);
  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to the WiFi network!");
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Public emqx mqtt broker connected!");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");

  String Payload = "";
  for (int i = 0; i < length; i++) {
    Payload += (char)payload[i];
    Serial.print((char) payload[i]);
  }
  Serial.println();
//通过判断Web发过来的字符串来进行设备的开关
  if (Payload == "Green led on") {
    digitalWrite(Green_LED, HIGH);
  }
  else if (Payload == "Green led off") {
    digitalWrite(Green_LED, LOW);
  }
  else if (Payload == "Red led on") {
    digitalWrite(Red_LED, HIGH);
  }
  else if (Payload == "Red led off") {
    digitalWrite(Red_LED, LOW);
  }
  else if (Payload == "Blue led on") {
    digitalWrite(Blue_LED, HIGH);
  }
  else if (Payload == "Blue led off") {
    digitalWrite(Blue_LED, LOW);
  }
  else if (Payload == "All on") {
    digitalWrite(Green_LED, HIGH);
    digitalWrite(Red_LED, HIGH);
    digitalWrite(Blue_LED, HIGH);
  }
  else if (Payload == "All off") {
    digitalWrite(Green_LED, LOW);
    digitalWrite(Red_LED, LOW);
    digitalWrite(Blue_LED, LOW);
  }
}

void loop() {
  client.loop();
}
