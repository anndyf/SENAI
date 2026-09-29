// =====================================================
//  Aula 8 - ESP32 + MQTT (versao SEM sensor: dados simulados)
//  Quando tiver o DHT22, veja o comentario "COM SENSOR" no loop
// =====================================================
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ---------- CONFIGURACAO ----------
const char* ssid        = "SKYWALKER";
const char* password    = "08Anndy!";
const char* mqtt_server = "192.168.0.49";   // IP do computador com o Mosquitto
const int   mqtt_port   = 1883;

// TROQUE "aluno01" pelo seu numero nas 3 linhas abaixo!
const char* client_id = "ESP32_aluno01";
const char* topic_pub = "senai/aula8/aluno01/sensor";
const char* topic_sub = "senai/aula8/aluno01/led";

#define LEDPIN 2   // LED azul da placa

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long ultimoEnvio = 0;
float temperatura = 25.0;   // valor inicial da simulacao

// ---------- RECEBENDO COMANDOS ----------
void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  Serial.print("Comando recebido: ");
  Serial.println(msg);
  if (msg == "ON")  digitalWrite(LEDPIN, HIGH);
  if (msg == "OFF") digitalWrite(LEDPIN, LOW);
}

// ---------- CONEXOES ----------
void conectaWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nWi-Fi OK - IP: ");
  Serial.println(WiFi.localIP());
}

void conectaMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao broker... ");
    if (client.connect(client_id)) {
      Serial.println("OK");
      client.subscribe(topic_sub);
    } else {
      Serial.print("falhou, codigo = ");
      Serial.println(client.state());   // -2 = broker inacessivel (IP ou firewall)
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LEDPIN, OUTPUT);
  randomSeed(esp_random());
  conectaWiFi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

// ---------- PUBLICANDO A CADA 5 SEGUNDOS ----------
void loop() {
  if (!client.connected()) conectaMQTT();
  client.loop();

  if (millis() - ultimoEnvio > 5000) {
    ultimoEnvio = millis();

    // SIMULACAO: a temperatura varia um pouco a cada leitura
    temperatura += random(-5, 6) / 10.0;
    temperatura = constrain(temperatura, 20.0, 35.0);
    float umidade = 55.0 + random(0, 100) / 10.0;

    // COM SENSOR: troque as 3 linhas acima por
    //   float temperatura = dht.readTemperature();
    //   float umidade = dht.readHumidity();
    //   if (isnan(temperatura) || isnan(umidade)) return;
    // (e adicione #include <DHT.h> e DHT dht(4, DHT22); no topo, e dht.begin(); no setup)

    JsonDocument doc;
    doc["temperatura"] = round(temperatura * 10) / 10.0;
    doc["umidade"]     = round(umidade * 10) / 10.0;
    char buffer[128];
    serializeJson(doc, buffer);

    client.publish(topic_pub, buffer);
    Serial.print("Publicado: ");
    Serial.println(buffer);
  }
}
