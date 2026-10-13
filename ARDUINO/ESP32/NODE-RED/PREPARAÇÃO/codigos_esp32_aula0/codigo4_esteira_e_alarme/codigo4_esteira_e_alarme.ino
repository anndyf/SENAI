// =====================================================
//  Aula 0 - Botao, borda e MQTT por evento
//  SENAI Alagoinhas - Sistemas Eletronicos e Microcontrolados
// =====================================================
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define MODO 1   // 1 = esteira com botao e dashboard (Bloco 3) | 2 = alarme de porta (Bloco 4)

// ---------- CONFIGURACAO ----------
const char* ssid        = "NOME_DA_REDE";
const char* password    = "SENHA_DA_REDE";
const char* mqtt_server = "broker.hivemq.com";   // broker publico
const int   mqtt_port   = 1883;

// TROQUE "grupo01" pelo numero do SEU grupo nas 6 linhas abaixo!
const char* client_id          = "senai_alagoinhas_2026_aula0_grupo01";
const char* topic_estado       = "senai_alagoinhas_2026/aula0/grupo01/estado";
const char* topic_esteira      = "senai_alagoinhas_2026/aula0/grupo01/esteira";
const char* topic_porta        = "senai_alagoinhas_2026/aula0/grupo01/porta";
const char* topic_led_verde    = "senai_alagoinhas_2026/aula0/grupo01/led_verde";
const char* topic_led_vermelho = "senai_alagoinhas_2026/aula0/grupo01/led_vermelho";
// -----------------------------------

#define BOTAO        27
#define LED_VERDE    23
#define LED_VERMELHO 22
#define ESTEIRA_IN1  26   // bancada: LED amarelo | esteira real: ponte H IN1
#define ESTEIRA_IN2  25   // esteira real: ponte H IN2 (sempre LOW) | sem uso na bancada
#define DEBOUNCE     50   // ms

WiFiClient espClient;
PubSubClient client(espClient);

bool esteiraLigada = false;
int  acionamentos = 0;            // quantas vezes a esteira foi LIGADA
int  estadoAnterior = HIGH;
unsigned long ultimaMudanca = 0;

// ---- Liga/desliga a esteira (um sentido so) ----
void controlaEsteira(bool ligar) {
  if (ligar && !esteiraLigada) acionamentos++;   // soma so quando passa de PARADA para LIGADA
  esteiraLigada = ligar;
  digitalWrite(ESTEIRA_IN1, ligar ? HIGH : LOW);
  digitalWrite(ESTEIRA_IN2, LOW);
  Serial.println(ligar ? "Esteira: LIGADA" : "Esteira: PARADA");
}

// ---- Publica o estado da esteira: {"esteira":"ON","acionamentos":3} ----
void publicaEstado() {
  JsonDocument doc;
  doc["esteira"]      = esteiraLigada ? "ON" : "OFF";
  doc["acionamentos"] = acionamentos;
  char buffer[64];
  serializeJson(doc, buffer);
  client.publish(topic_estado, buffer);
  Serial.print("Publicado: ");
  Serial.println(buffer);
}

// ---- Publica a porta: {"porta":"ABERTA"} ou {"porta":"FECHADA"} ----
void publicaPorta(bool aberta) {
  JsonDocument doc;
  doc["porta"] = aberta ? "ABERTA" : "FECHADA";
  char buffer[64];
  serializeJson(doc, buffer);
  client.publish(topic_porta, buffer);
  Serial.print("Publicado: ");
  Serial.println(buffer);
}

// ---- MODO 1: cada APERTO alterna a esteira ----
void verificaInterruptor() {
  int leitura = digitalRead(BOTAO);
  if (leitura != estadoAnterior && millis() - ultimaMudanca > DEBOUNCE) {
    ultimaMudanca = millis();
    estadoAnterior = leitura;
    if (leitura == LOW) {                       // so na descida
      controlaEsteira(!esteiraLigada);
      publicaEstado();                          // avisa na hora que mudou
    }
  }
}

// ---- MODO 2: aperto = porta ABERTA, soltar = porta FECHADA ----
void verificaPorta() {
  int leitura = digitalRead(BOTAO);
  if (leitura != estadoAnterior && millis() - ultimaMudanca > DEBOUNCE) {
    ultimaMudanca = millis();
    estadoAnterior = leitura;
    publicaPorta(leitura == LOW);               // LOW = apertado = ABERTA
  }
}

// Chamada automaticamente quando chega mensagem nos topicos assinados
void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  String t = String(topic);

  // So aceita ON ou OFF (maiusculas). Qualquer outra coisa e ignorada.
  int estado;
  if (msg == "ON")       estado = HIGH;
  else if (msg == "OFF") estado = LOW;
  else return;

  if (t == topic_esteira) {
    controlaEsteira(estado == HIGH);
#if MODO == 1
    publicaEstado();    // o dashboard comandou: avisa o novo estado de volta
#endif
  } else if (t == topic_led_verde) {
    digitalWrite(LED_VERDE, estado);
    Serial.print("LED verde: ");
    Serial.println(msg);
  } else if (t == topic_led_vermelho) {
    digitalWrite(LED_VERMELHO, estado);
    Serial.print("LED vermelho: ");
    Serial.println(msg);
  }
}

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
      client.subscribe(topic_esteira);
      client.subscribe(topic_led_verde);
      client.subscribe(topic_led_vermelho);
#if MODO == 1
      publicaEstado();   // avisa o estado atual ao conectar
#endif
    } else {
      Serial.print("falhou, codigo = ");
      Serial.println(client.state());   // -2 = broker inacessivel (rede ou porta bloqueada)
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BOTAO, INPUT_PULLUP);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(ESTEIRA_IN1, OUTPUT);
  pinMode(ESTEIRA_IN2, OUTPUT);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(ESTEIRA_IN1, LOW);   // tudo comeca desligado
  digitalWrite(ESTEIRA_IN2, LOW);
  conectaWiFi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) conectaWiFi();   // reconecta se o Wi-Fi cair
  if (!client.connected()) conectaMQTT();
  client.loop();

#if MODO == 1
  verificaInterruptor();
#else
  verificaPorta();
#endif
}
