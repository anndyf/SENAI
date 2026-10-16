// =====================================================
//  Desafio - contadora de pecas (ESP32 + MQTT + Node-RED)
//  CODIGO BASE: complete os 4 trechos marcados com TODO.
//  O codigo compila e roda mesmo sem completar: o que faltar
//  aparece como um comportamento errado (veja o "SINTOMA" de cada TODO).
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

// TROQUE "grupo01" pelo numero do SEU grupo nas 8 linhas abaixo!
const char* client_id          = "senai_alagoinhas_2026_grupo01";
const char* topic_contagem     = "senai_alagoinhas_2026/desafio/grupo01/contagem";
const char* topic_esteira      = "senai_alagoinhas_2026/desafio/grupo01/esteira";
const char* topic_led_verde    = "senai_alagoinhas_2026/desafio/grupo01/led_verde";
const char* topic_led_vermelho = "senai_alagoinhas_2026/desafio/grupo01/led_vermelho";
const char* topic_comando      = "senai_alagoinhas_2026/desafio/grupo01/comando";
// monitoramento (o ESP32 PUBLICA estes dois; o Node-RED so le)
const char* topic_status       = "senai_alagoinhas_2026/desafio/grupo01/status";   // JSON com todos os estados
const char* topic_online       = "senai_alagoinhas_2026/desafio/grupo01/online";   // "online" / "offline"
// -----------------------------------

#define SENSOR       27
#define LED_VERDE    23
#define LED_VERMELHO 22
#define LED_AMARELO  21   // acende enquanto uma peca esta sendo contada
#define MOTOR_IN1    26   // bancada: sem componente (o LED verde mostra a esteira) | planta: ponte H IN3
#define MOTOR_IN2    25   // planta: ponte H IN4 (sempre LOW)
#define DEBOUNCE     50   // ms

// ---------- VELOCIDADE DA ESTEIRA ----------
// 0 a 100 (%). Diminua para a esteira ficar mais lenta.
// Se o motor "engasgar" ou nao arrancar, aumente de 5 em 5.
// Se mesmo em 100 estiver fraco, troque a fonte do motor.
const int VELOCIDADE_PCT = 50;
const int PWM_FREQ = 1000;    // Hz
const int PWM_BITS = 8;       // 0 a 255
// -------------------------------------------

// ---------- MONITORAMENTO ----------
const bool MONITOR_SERIAL = true;           // mostra uma linha de status no Serial
const unsigned long MONITOR_MS   = 1000;    // intervalo da linha no Serial
const unsigned long HEARTBEAT_MS = 5000;    // o ESP32 publica o status a cada 5 s, mesmo sem mudanca
// -----------------------------------

WiFiClient espClient;
PubSubClient client(espClient);

bool esteiraLigada = false;   // agora quem liga a esteira e o Node-RED
bool ledVerdeLigado = false;
bool ledVermelhoLigado = false;
bool ledAmareloLigado = false;
unsigned long ultimoStatus = 0;
unsigned long ultimoMonitor = 0;
int  contagem = 0;
int  estadoAnterior = HIGH;
unsigned long ultimaMudanca = 0;

void publicaContagem();   // declaracoes antecipadas (usadas antes de serem definidas)
void publicaStatus();

// PWM no IN3 da ponte H (GPIO26): controla a velocidade sem mexer na fiacao.
// (Funciona nas versoes 2.x e 3.x do pacote ESP32 da Arduino IDE.)
void iniciaPWM() {
#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcAttach(MOTOR_IN1, PWM_FREQ, PWM_BITS);
#else
  ledcSetup(0, PWM_FREQ, PWM_BITS);
  ledcAttachPin(MOTOR_IN1, 0);
#endif
}

void escrevePWM(int duty) {
#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcWrite(MOTOR_IN1, duty);
#else
  ledcWrite(0, duty);
#endif
}

// Liga/desliga a esteira (ja pronta: use no callback)
void controlaEsteira(bool ligar) {
  esteiraLigada = ligar;
  int duty = (255 * VELOCIDADE_PCT) / 100;
  escrevePWM(ligar ? duty : 0);
  digitalWrite(MOTOR_IN2, LOW);   // um sentido so
  Serial.println(ligar ? "Esteira: LIGADA" : "Esteira: PARADA");
  publicaStatus();
}

// Deteccao de borda + debounce + contagem
// So conta com a esteira ligada; publica NA HORA em que a contagem muda
void verificaSensor() {
  int leitura = digitalRead(SENSOR);

  // TODO 2a (referencia: Aula 0, codigo3_borda_debounce)
  //          "mudou" deve ser verdadeiro quando
  //          (1) a leitura e diferente de estadoAnterior E
  //          (2) ja passou o tempo DEBOUNCE desde ultimaMudanca (use millis()).
  // SINTOMA se ficar assim: o sensor nunca e percebido.
  bool mudou = false;   // <-- troque o false pela condicao

  if (mudou) {
    ultimaMudanca = millis();
    estadoAnterior = leitura;

    // TODO 2b (referencia: Aula 0, anexoA_sala, "leitura == LOW && salaAberta")
    //          "descida" deve ser verdadeira so quando
    //          (1) a leitura e LOW (peca detectada) E (2) a esteira esta ligada.
    // SINTOMA se ficar assim: a contagem nunca sobe.
    bool descida = false;   // <-- troque o false pela condicao

    if (descida) {
      contagem++;
      Serial.print("Contagem: ");
      Serial.println(contagem);
      publicaContagem();
    }
    publicaStatus();   // o sensor mudou (detectando <-> livre): avisa o Node-RED
  }
}

// LED amarelo: acende ENQUANTO uma peca esta sendo contada
// (esteira ligada E sensor detectando). Apaga quando a peca passa ou a esteira para.
void atualizaLedAmarelo() {
  bool deveAcender = esteiraLigada && (digitalRead(SENSOR) == LOW);
  if (deveAcender != ledAmareloLigado) {
    ledAmareloLigado = deveAcender;
    digitalWrite(LED_AMARELO, deveAcender ? HIGH : LOW);
    publicaStatus();
  }
}

// Publica TODOS os estados reais do ESP32, em JSON, no topico "status".
// E chamada quando algo muda (esteira, LEDs, sensor, contagem) e a cada HEARTBEAT_MS.
void publicaStatus() {
  JsonDocument doc;
  doc["esteira"]      = esteiraLigada ? "LIGADA" : "PARADA";
  doc["velocidade"]   = VELOCIDADE_PCT;
  doc["led_verde"]    = ledVerdeLigado ? "ON" : "OFF";
  doc["led_vermelho"] = ledVermelhoLigado ? "ON" : "OFF";
  doc["led_amarelo"]  = ledAmareloLigado ? "ON" : "OFF";
  doc["sensor"]       = (digitalRead(SENSOR) == LOW) ? "DETECTANDO" : "LIVRE";
  doc["contagem"]     = contagem;
  doc["rssi"]         = WiFi.RSSI();
  doc["uptime_s"]     = millis() / 1000;
  char buffer[256];
  serializeJson(doc, buffer);
  client.publish(topic_status, buffer);
  ultimoStatus = millis();
}

// Linha de status no Serial Monitor (monitoramento pelo proprio ESP32)
void mostraStatusSerial() {
  Serial.print("[STATUS] esteira=");
  Serial.print(esteiraLigada ? "LIGADA" : "PARADA");
  Serial.print(" | verde=");
  Serial.print(ledVerdeLigado ? "ON" : "OFF");
  Serial.print(" | vermelho=");
  Serial.print(ledVermelhoLigado ? "ON" : "OFF");
  Serial.print(" | amarelo=");
  Serial.print(ledAmareloLigado ? "ON" : "OFF");
  Serial.print(" | sensor=");
  Serial.print(digitalRead(SENSOR) == LOW ? "DETECTANDO" : "livre");
  Serial.print(" | contagem=");
  Serial.print(contagem);
  Serial.print(" | wifi=");
  Serial.print(WiFi.RSSI());
  Serial.print(" dBm | mqtt=");
  Serial.println(client.connected() ? "OK" : "DESCONECTADO");
}

// Publica o JSON {"contagem": N}
void publicaContagem() {
  JsonDocument doc;
  // TODO 4 (referencia: Aula 0, codigo4_esteira_e_alarme)
  //         coloque a contagem dentro do JSON, com a chave "contagem".
  //         O resultado publicado deve ser: {"contagem":3}
  // SINTOMA se ficar vazio: o Node-RED recebe {} e a contagem nunca aparece.

  char buffer[64];
  serializeJson(doc, buffer);
  client.publish(topic_contagem, buffer);
  Serial.print("Publicado: ");
  Serial.println(buffer);
}

// Chamada automaticamente quando chega mensagem nos topicos assinados
void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  String t = String(topic);

  // 1) Topico de comando: aceita RESET
  if (t == topic_comando) {
    // TODO 3a (referencia: Aula 0, anexoA_sala, tratamento do "ZERAR")
    //          se a mensagem for "RESET", zere a contagem e avise o Node-RED
    //          (use publicaContagem() e publicaStatus()).
    // SINTOMA se ficar vazio: o botao Reiniciar nao zera a contagem do ESP32.

    return;
  }

  // 2) Demais topicos: so aceitam ON ou OFF (maiusculas)
  int estado;
  if (msg == "ON")       estado = HIGH;
  else if (msg == "OFF") estado = LOW;
  else return;

  if (t == topic_led_verde) {
    digitalWrite(LED_VERDE, estado);
    ledVerdeLigado = (estado == HIGH);
    Serial.print("LED verde: ");
    Serial.println(msg);
    publicaStatus();
  }
  else if (t == topic_led_vermelho) {
    digitalWrite(LED_VERMELHO, estado);
    ledVermelhoLigado = (estado == HIGH);
    Serial.print("LED vermelho: ");
    Serial.println(msg);
    publicaStatus();
  }

  // TODO 3b (referencia: Aula 0, codigo4_esteira_e_alarme)
  //          se o topico for topic_esteira, ligue ou desligue a esteira.
  //          Dica: "estado" ja vale HIGH (ON) ou LOW (OFF); use controlaEsteira(...).
  // SINTOMA se ficar vazio: o botao Iniciar nao liga a esteira.

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
    // 3o a 6o parametros: "testamento" (LWT). Se o ESP32 cair sem avisar,
    // o broker publica "offline" no topico online (retido).
    if (client.connect(client_id, NULL, NULL, topic_online, 0, true, "offline")) {
      Serial.println("OK");
      client.publish(topic_online, "online", true);   // retido: quem entrar depois ja ve o estado
      client.subscribe(topic_esteira);
      client.subscribe(topic_led_verde);
      client.subscribe(topic_led_vermelho);
      client.subscribe(topic_comando);
      publicaContagem();   // avisa o Node-RED da contagem atual
      publicaStatus();     // e do estado de tudo
    } else {
      Serial.print("falhou, codigo = ");
      Serial.println(client.state());   // -2 = broker inacessivel (rede ou porta bloqueada)
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  digitalWrite(LED_VERDE, LOW);      // tudo comeca desligado:
  digitalWrite(LED_VERMELHO, LOW);   // quem manda e o Node-RED
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  iniciaPWM();                       // IN1 passa a ser controlado por PWM
  escrevePWM(0);                     // esteira parada

  // TODO 1 (referencia: Aula 0, qualquer codigo, ex.: codigo1_led_espelha_botao)
  //        configure o pino SENSOR como entrada com pull-up interno.
  // SINTOMA se faltar: o Serial mostra o sensor "detectando" e "livre" sozinho, sem ninguem encostar.


  conectaWiFi();
  client.setBufferSize(512);   // o JSON de status e maior que o padrao (256)
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) conectaWiFi();   // reconecta se o Wi-Fi cair
  if (!client.connected()) conectaMQTT();
  client.loop();

  verificaSensor();   // roda a toda hora: a peca passa em menos de 1 segundo!
  atualizaLedAmarelo();

  // heartbeat: mesmo sem mudanca, o Node-RED recebe o status a cada HEARTBEAT_MS
  if (client.connected() && millis() - ultimoStatus >= HEARTBEAT_MS) publicaStatus();

  // monitoramento no Serial
  if (MONITOR_SERIAL && millis() - ultimoMonitor >= MONITOR_MS) {
    ultimoMonitor = millis();
    mostraStatusSerial();
  }
}
