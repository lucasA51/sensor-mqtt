#include <WiFi.h>
#include <PubSubClient.h>

// Definição dos pinos para o primeiro HC-SR04
#define TRIG_PIN_1 5
#define ECHO_PIN_1 18

// Velocidade do som em cm/µs
#define SOUND_SPEED 0.034

// Simulação da distância
float dist_in_cm;

// Wi-Fi
const char* ssid = "Wokwi-GUEST";  // Padrão do Wokwi
const char* password = "";         // Wokwi não exige senha

// MQTT Broker público (ou substitua por outro público seu)
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado. IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("ESP32Client-Wokwi")) {
      Serial.println("Conectado ao broker MQTT.");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  // Configura os pinos do primeiro HC-SR04
  pinMode(TRIG_PIN_1, OUTPUT);
  pinMode(ECHO_PIN_1, INPUT);

}

// Função para medir a distância usando o HC-SR04
float medirDistancia(int trigPin, int echoPin) {
  // Gera um pulso de 10 microssegundos no pino Trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Mede o tempo de ida e volta do sinal ultrassônico
  long duracao = pulseIn(echoPin, HIGH);

  // Calcula a distância em centímetros
  float distancia = (duracao * SOUND_SPEED) / 2;

  return distancia;
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Simula uma leitura de distância entre 0 e 400 cm
  dist_in_cm = medirDistancia(TRIG_PIN_1, ECHO_PIN_1);

  // Envio por MQTT
  char msg[50];
  dtostrf(dist_in_cm, 6, 2, msg);
  client.publish("sensor/distancia", msg);

  Serial.print("Distância enviada (simulada): ");
  Serial.println(msg);

  delay(1000);
}