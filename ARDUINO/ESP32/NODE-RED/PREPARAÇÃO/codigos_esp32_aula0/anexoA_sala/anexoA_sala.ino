// =====================================================
//  Aula 0 - Anexo A: controle de lotacao de uma sala
//  Botao = catraca (cada aperto = 1 pessoa)
//  SENAI Alagoinhas - Sistemas Eletronicos e Microcontrolados
// =====================================================
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ---------- CONFIGURACAO ----------
const char* ssid        = "NOME_DA_REDE";
const char* password    = "SENHA_DA_REDE";
const char* mqtt_server = "broker.hivemq.com";   // broker publico
const int   mqtt_port   = 1883;

// TROQUE "grupo01" pelo numero do SEU grupo nas 6 linhas abaixo!
const char* client_id          = "senai_alagoinhas_2026_aula0_grupo01";
const char* topic_visitas      = "senai_alagoinhas_2026/aula0/grupo01/visitas";
const char* topic_sala         = "senai_alagoinhas_2026/aula0/grupo01/sala";
const char* topic_comando      = "senai_alagoinhas_2026/aula0/grupo01/comando";
const char* topic_led_vermelho = "senai_alagoinhas_2026/aula0/grupo01/led_vermelho";
// O quinto topico (led_verde) nao e usado: o LED verde acompanha o comando "sala".
// -----------------------------------

#define BOTAO        27
#define LED_VERDE    23
#define LED_VERMELHO 22
#define DEBOUNCE     50   // ms

WiFiClient espClient;
PubSubClient client(espClient);

int  visitas = 0;           // pessoas contadas
bool salaAberta = false;    // so conta com a sala aberta
int  estadoAnterior = HIGH;
unsigned long ultimaMudanca = 0;

// ---- Publica a contagem: {"visitas":2} ----
void publicaVisitas() {
  JsonDocument doc;
  doc["visitas"] = visitas;
  char buffer[64];
  serializeJson(doc, buffer);
  client.publish(topic_visitas, buffer);
  Serial.print("Publicado: ");
  Serial.println(buffer);
}

// ---- Cada APERTO conta 1 pessoa, mas so com a sala aberta ----
void verificaVisita() {
  int leitura = digitalRead(BOTAO);
  if (leitura != estadoAnterior && millis() - ultimaMudanca > DEBOUNCE) {
    ultimaMudanca = millis();
    estadoAnterior = leitura;       // atualiza SEMPRE, mesmo com a sala fechada

    if (leitura == LOW && salaAberta) {
      visitas++;
      Serial.print("Visitas: ");
      Serial.println(visitas);
      publicaVisitas();             // publica quando MUDA
    }
  }
}

// Chamada automaticamente quando chega mensagem nos topicos assinados
void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  String t = String(topic);

  // 1) Comando de texto: ZERAR
  if (t == topic_comando) {
    if (msg == "ZERAR") {
      visitas = 0;
      Serial.println("Contagem zerada");
      publicaVisitas();             // avisa o Node-RED
    }
    return;
  }

  // 2) Comandos ON/OFF
  int estado;
  if (msg == "ON")       estado = HIGH;
  else if (msg == "OFF") estado = LOW;
  else return;

  if (t == topic_sala) {
    salaAberta = (estado == HIGH);
    digitalWrite(LED_VERDE, estado);   // verde = sala aberta
    Serial.println(salaAberta ? "Sala: ABERTA" : "Sala: FECHADA");
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
      client.subscribe(topic_sala);
      client.subscribe(topic_comando);
      client.subscribe(topic_led_vermelho);
      publicaVisitas();   // avisa a contagem atual ao conectar
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
  digitalWrite(LED_VERDE, LOW);      // tudo comeca desligado
  digitalWrite(LED_VERMELHO, LOW);
  conectaWiFi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) conectaWiFi();   // reconecta se o Wi-Fi cair
  if (!client.connected()) conectaMQTT();
  client.loop();
  verificaVisita();
}
