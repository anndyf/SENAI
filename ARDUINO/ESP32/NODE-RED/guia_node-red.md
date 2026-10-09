# Guia Prático — ESP32 → MQTT → Node-RED → InfluxDB Cloud, com Alarme de Temperatura (Windows)

**SENAI Alagoinhas · Sistemas Eletrônicos e Microcontrolados · Aulas 9 e 10**

Na aula passada o ESP32 publicou a temperatura simulada (potenciômetro) no broker público HiveMQ. Agora o dado vai **além**: o Node-RED recebe, **guarda no banco de dados InfluxDB Cloud**, mostra num **dashboard** e aplica uma **regra de alarme** que comanda **dois LEDs** no ESP32.

> **A regra desta aula**
>
> | Temperatura (potenciômetro) | LED verde | LED vermelho | Texto no dashboard |
> |---|---|---|---|
> | **até 35 °C** | ACESO | apagado | Temperatura OK |
> | **acima de 35 °C** | **apagado** | **ACESO** | ALARME: temperatura alta! |
>
> 35 °C corresponde a cerca de **70 % da rotação** do potenciômetro (35 ÷ 50).

```
  ESP32 ──publica──► HiveMQ ──► Node-RED ──► InfluxDB Cloud (histórico)
    ▲   (sensor)     (broker)      │  ├────► Dashboard (gauge, gráfico, estado)
    │                              │  └────► REGRA: temp > 35 ?
    └──── led_verde / led_vermelho ◄───────────────┘
              (ON / OFF)
```

**Materiais:** ESP32 DevKit V1, protoboard, potenciômetro, **2 LEDs (1 verde + 1 vermelho)**, **2 resistores de 220 Ω**, jumpers, cabo USB, notebook com Windows.

---

## Mapa das etapas

| Etapa | O que fazemos |
|---|---|
| 1 | Montar o circuito e gravar o código completo no ESP32 |
| 2 | Instalar o Node.js e o Node-RED |
| 3 | Receber os dados do ESP32 no Node-RED |
| 4 | Criar o banco InfluxDB Cloud e gravar os dados |
| 5 | Dashboard + regra do alarme (LED verde/vermelho) |
| 6 | Importar o fluxo pronto (atalho), problemas comuns e atividade |

---

## Etapa 1 — Circuito e código do ESP32

### 1.1 Ligações

| Componente | Ligação |
|---|---|
| Potenciômetro — pino 1 | 3V3 |
| Potenciômetro — pino do meio (cursor) | **GPIO 34** |
| Potenciômetro — pino 3 | GND |
| LED **verde** — perna longa (ânodo +) | **GPIO 23**, passando por resistor de 220 Ω |
| LED **vermelho** — perna longa (ânodo +) | **GPIO 22**, passando por resistor de 220 Ω |
| LEDs — perna curta (cátodo −) | GND |

> ⚠️ **Atenção:** o LED só funciona na polaridade certa (perna longa = positivo) e **nunca sem resistor** — sem ele o LED e o pino do ESP32 podem queimar.

### 1.2 Bibliotecas (Arduino IDE → Gerenciar Bibliotecas)

- **PubSubClient** (Nick O'Leary)
- **ArduinoJson** (Benoit Blanchon) — versão 7

### 1.3 Código completo do ESP32

Troque `NOME_DA_REDE`, `SENHA_DA_REDE` e **`aluno01` pelo seu número nas 4 linhas indicadas**.

```cpp
// =====================================================
//  Aula 9 - ESP32 + MQTT: potenciometro, 2 LEDs e Node-RED
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

// TROQUE "aluno01" pelo seu numero nas 4 linhas abaixo!
const char* client_id          = "senai_alagoinhas_2026_aluno01";
const char* topic_pub          = "senai_alagoinhas_2026/aula8/aluno01/sensor";
const char* topic_led_verde    = "senai_alagoinhas_2026/aula8/aluno01/led_verde";
const char* topic_led_vermelho = "senai_alagoinhas_2026/aula8/aluno01/led_vermelho";
// -----------------------------------

#define POTPIN       34     // cursor do potenciometro (ADC1)
#define LED_VERDE    23     // LED verde: temperatura OK
#define LED_VERMELHO 22     // LED vermelho: alarme de temperatura
#define AMOSTRAS     20     // leituras para a media (filtro)
#define TEMP_MAX     50.0   // escala simulada: 0 a 50 graus C

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long ultimoEnvio = 0;

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

  // Descobre qual LED comandar pelo topico que chegou
  if (t == topic_led_verde) {
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
      client.subscribe(topic_led_verde);
      client.subscribe(topic_led_vermelho);
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
  digitalWrite(LED_VERDE, LOW);      // os LEDs comecam apagados:
  digitalWrite(LED_VERMELHO, LOW);   // quem manda neles e o Node-RED
  analogReadResolution(12);          // ADC de 12 bits: 0 a 4095
  analogSetAttenuation(ADC_11db);    // faixa de leitura de 0 a ~3,3 V
  conectaWiFi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) conectaWiFi();   // reconecta se o Wi-Fi cair
  if (!client.connected()) conectaMQTT();
  client.loop();

  if (millis() - ultimoEnvio > 5000) {   // a cada 5 segundos
    ultimoEnvio = millis();

    // 1) LEITURA + 2) FILTRO: media de varias amostras
    long somaBruto = 0;
    long somaMv    = 0;
    for (int i = 0; i < AMOSTRAS; i++) {
      somaBruto += analogRead(POTPIN);
      somaMv    += analogReadMilliVolts(POTPIN);
      delay(2);
    }
    int   bruto  = somaBruto / AMOSTRAS;             // 0 a 4095
    float tensao = (somaMv / AMOSTRAS) / 1000.0;     // em volts

    // 3) CONVERSAO para a escala de engenharia (0 a 50 graus C)
    float temperatura = bruto * TEMP_MAX / 4095.0;

    JsonDocument doc;
    doc["bruto"]       = bruto;
    doc["tensao"]      = round(tensao * 100) / 100.0;
    doc["temperatura"] = round(temperatura * 10) / 10.0;
    char buffer[128];
    serializeJson(doc, buffer);

    client.publish(topic_pub, buffer);
    Serial.print("Publicado: ");
    Serial.println(buffer);
  }
}
```

### 1.4 O que mudou em relação à aula passada

| Antes (Aula 8) | Agora |
|---|---|
| 1 LED, tópico `.../led` | **2 LEDs**: `.../led_verde` (GPIO 23) e `.../led_vermelho` (GPIO 22) |
| LED comandado à mão | LEDs comandados **automaticamente pela regra do Node-RED** |
| — | `callback` descobre qual LED comandar pelo tópico recebido |

> ⚠️ **Quem já tinha o código antigo precisa gravar este de novo**, porque os tópicos mudaram.

### 1.5 Saída esperada no Serial Monitor (115200)

```
Conectando ao Wi-Fi....
Wi-Fi OK - IP: 192.168.x.x
Conectando ao broker... OK
Publicado: {"bruto":2048,"tensao":1.65,"temperatura":25}
```

### 1.6 Teste manual dos LEDs (antes do Node-RED)

No cliente web do HiveMQ (`hivemq.com/demos/websocket-client`), conecte e publique:

| Tópico | Mensagem | Resultado |
|---|---|---|
| `senai_alagoinhas_2026/aula8/aluno01/led_verde` | `ON` | verde acende |
| `senai_alagoinhas_2026/aula8/aluno01/led_vermelho` | `ON` | vermelho acende |
| (os mesmos) | `OFF` | apaga |

Só avance quando os **dois** LEDs responderem.

---

## Etapa 2 — Instalar Node.js e Node-RED

1. Baixe e instale o Node.js **LTS** em nodejs.org (próximo, próximo, concluir).
2. Abra o **Prompt de Comando (cmd)** — não o PowerShell.
3. Instale o Node-RED:
   ```
   npm install -g --unsafe-perm node-red
   ```
4. Inicie:
   ```
   node-red
   ```
5. Abra no navegador: **http://localhost:1880**

> Deixe a janela do cmd aberta enquanto usa o Node-RED. Fechou, parou.

---

## Etapa 3 — Receber os dados do ESP32

1. Arraste um nó **mqtt in** para a área de trabalho e dê duplo clique:
   - **Server:** lápis → Server `broker.hivemq.com`, Port `1883` → Add
   - **Topic:** `senai_alagoinhas_2026/aula8/aluno01/sensor` (com o **seu** número)
   - **QoS:** 0
   - **Output:** *a parsed JSON object*
2. Arraste um nó **debug** e ligue o mqtt in nele.
3. **Deploy** (botão vermelho). O nó mostra *connected*.
4. Abra a aba de debug (ícone de inseto): a cada 5 s chega `{bruto, tensao, temperatura}`.

Gire o potenciômetro e veja a temperatura mudar. 

---

## Etapa 4 — InfluxDB Cloud: guardar o histórico

### 4.1 Criar conta e bucket

1. Crie a conta gratuita em **influxdata.com/get-influxdb** (plano Free).
2. No menu, abra **Load Data → Buckets → Create Bucket**, nome **`aula_mqtt`**.
3. Em **API Tokens → Generate API Token → Custom API Token**, dê permissão **somente de escrita (Write)** no bucket `aula_mqtt`. Copie o token (aparece **uma vez só**).

Anote os 4 dados que o Node-RED vai pedir:

| Dado | Onde achar |
|---|---|
| URL | endereço do seu painel (ex.: `https://us-east-1-1.aws.cloud2.influxdata.com`) |
| Organização | nome da sua organização / ID |
| Bucket | `aula_mqtt` |
| Token | o token de escrita que você copiou |

> 🔒 O token é como uma senha. Não coloque em print nem envie em grupo.

### 4.2 Instalar o nó do InfluxDB

No Node-RED: menu ☰ → **Manage palette → Install** → procure **`node-red-contrib-influxdb`** → Install.

### 4.3 Função "prepara dados"

Arraste um nó **function**, nome **prepara dados**, **Outputs: 2**, e cole:

```javascript
// A mensagem do ESP32 ja chega como objeto:
// {"bruto":2048,"tensao":1.65,"temperatura":25}
var d = msg.payload;

// 1) Confere se e um objeto com os tres campos numericos
if (d === null || typeof d !== "object" ||
    typeof d.bruto !== "number" ||
    typeof d.tensao !== "number" ||
    typeof d.temperatura !== "number") {
    node.warn("Mensagem ignorada: " + JSON.stringify(msg.payload));
    return null;
}

// 2) O aluno vem do topico: senai_alagoinhas_2026/aula8/aluno01/sensor
var aluno = msg.topic.split("/")[2];

// Saida 1: para o InfluxDB -> [campos, tags]
var influx = { payload: [
    { bruto: d.bruto, tensao: d.tensao, temperatura: d.temperatura },
    { aluno: aluno }
]};

// Saida 2: so a temperatura, para o painel e para a regra do alarme
var temp = { payload: d.temperatura };

return [influx, temp];
```

Ligue: `mqtt in` → `prepara dados`.

### 4.4 Nó influxdb out

Arraste **influxdb out** (categoria *storage*) e ligue à **saída 1** da função. Configure:

- **Server:** lápis → **Version: 2.0**, **URL**, marque **Enable secure connection (TLS)**, **Token**
- **Organisation:** a sua · **Bucket:** `aula_mqtt`
- **Measurement:** `sensor`

Adicione também um **debug** (nome "vai para o Influx") na saída 1. **Deploy**.

### 4.5 Conferir no banco

No InfluxDB Cloud → **Data Explorer** (modo SQL):

```sql
SELECT * FROM sensor ORDER BY time DESC LIMIT 20
```

Devem aparecer as linhas com `aluno`, `bruto`, `tensao`, `temperatura`.

### Entendendo o fluxo

Um ponto gravado no InfluxDB tem:

| Parte | No nosso caso |
|---|---|
| **Measurement** (a "tabela") | `sensor` |
| **Tags** (identificam, indexadas) | `aluno` |
| **Fields** (os valores medidos) | `bruto`, `tensao`, `temperatura` |
| **Timestamp** | momento do recebimento (automático) |

Na forma de texto (*line protocol*): `sensor,aluno=aluno01 bruto=2048,tensao=1.65,temperatura=25 1760000000000000000`

**Por que a função valida os dados?** O broker HiveMQ é **público**: qualquer pessoa pode publicar no seu tópico. Se chegar texto no lugar de número, a função descarta (`return null`) em vez de sujar o banco.

**Por que 2 saídas?** Saída 1 vai para o banco; saída 2 leva só a temperatura para o dashboard e para a regra do alarme.

---

## Etapa 5 — Dashboard e regra do alarme

### 5.1 Instalar o dashboard

Manage palette → Install → **`node-red-dashboard`**.

> ℹ️ Este pacote está marcado como *descontinuado* (sem manutenção), mas ainda funciona bem para a aula.

### 5.2 Gauge, gráfico e estado

Ligue a **saída 2** de "prepara dados" em:

- **ui_gauge** — Label `Temperatura`, Units `°C`, Range **min 0 / max 50**, Sectors: `0`, **`30`**, **`35`**, `50` (verde até 30, amarelo 30–35, vermelho acima de 35)
- **ui_chart** — Label `Temperatura`, X-axis últimos 10 minutos

### 5.3 Função "regra do alarme"

Arraste outro **function**, nome **regra do alarme**, **Outputs: 3**, e cole:

```javascript
// ===== REGRA DO ALARME =====
var LIMITE = 35;   // graus C: acima disso, alarme

var temp = msg.payload;          // temperatura que chegou (numero)
var alarme = temp > LIMITE;      // true = acima do limite

// Saida 1: LED verde (temperatura OK)  -> apaga se estiver em alarme
var verde = { payload: alarme ? "OFF" : "ON" };

// Saida 2: LED vermelho (alarme)       -> acende se estiver em alarme
var vermelho = { payload: alarme ? "ON" : "OFF" };

// Saida 3: texto de estado para o painel
var estado = { payload: alarme ? "ALARME: temperatura alta!" : "Temperatura OK" };

return [verde, vermelho, estado];
```

Ligue a **saída 2** de "prepara dados" na entrada desta função.

### 5.4 Saídas da regra

| Saída | Liga em | Configuração |
|---|---|---|
| 1 (verde) | **mqtt out** | Topic `senai_alagoinhas_2026/aula8/aluno01/led_verde`, QoS 0, Retain false, Server `broker.hivemq.com` |
| 2 (vermelho) | **mqtt out** | Topic `senai_alagoinhas_2026/aula8/aluno01/led_vermelho`, igual ao anterior |
| 3 (estado) | **ui_text** | Label `Estado` |

**Deploy** e abra o dashboard em **http://localhost:1880/ui**.

### 5.5 Testando a regra

| Gire o potenciômetro para... | Esperado |
|---|---|
| Menos de ~70 % (≤ 35 °C) | Verde aceso, vermelho apagado, "Temperatura OK" |
| Mais de ~70 % (> 35 °C) | **Verde apaga**, vermelho acende, "ALARME: temperatura alta!" |
| Voltar para baixo | Verde volta, vermelho apaga |

O LED muda na próxima leitura (até 5 s), pois o ESP32 publica a cada 5 s.

### 5.6 Diagrama completo do fluxo

```
[mqtt in sensor] ─┬─► [debug]
                  └─► [prepara dados] ─┬─ saída 1 ─┬─► [influxdb out]
                                       │           └─► [debug "vai para o Influx"]
                                       └─ saída 2 ─┬─► [gauge]
                                                   ├─► [gráfico]
                                                   └─► [regra do alarme] ─┬─ 1 ─► [mqtt out led_verde]
                                                                          ├─ 2 ─► [mqtt out led_vermelho]
                                                                          └─ 3 ─► [ui_text Estado]
```

### 5.7 Explicando o código da regra

| Linha | O que faz |
|---|---|
| `var LIMITE = 35;` | Define a temperatura limite (mude aqui para outro valor) |
| `var temp = msg.payload;` | Pega a temperatura que chegou |
| `var alarme = temp > LIMITE;` | `true` se acima do limite, `false` caso contrário |
| `verde = alarme ? "OFF" : "ON"` | Em alarme apaga o verde; senão acende |
| `vermelho = alarme ? "ON" : "OFF"` | Em alarme acende o vermelho; senão apaga |
| `return [verde, vermelho, estado];` | Envia cada mensagem para a sua saída |

**Por que a regra fica no Node-RED e não no ESP32?** Para mudar o limite basta editar um número no Node-RED, sem regravar a placa; e uma só regra pode comandar vários dispositivos.

**Por que enviamos ON/OFF a cada leitura?** Assim, se o ESP32 reiniciar, em até 5 s os LEDs voltam ao estado correto.

> ⚠️ **Limite:** se o Node-RED parar, os LEDs **ficam congelados** no último estado. Em sistemas reais críticos, a proteção também deve existir no próprio equipamento.

### Caminho completo do dado

1. Potenciômetro gera tensão → 2. ADC lê 0–4095 → 3. ESP32 faz média de 20 amostras → 4. converte para °C → 5. monta o JSON → 6. publica no HiveMQ → 7. Node-RED recebe → 8. valida e separa → 9. grava no InfluxDB → 10. aplica a regra → 11. publica ON/OFF e o ESP32 acende/apaga os LEDs.

---

## Etapa 6 — Atalho, problemas comuns e atividade

### 6.1 Importar o fluxo pronto

Menu ☰ → **Import** → cole o JSON abaixo → **Import**.

Depois **ajuste**:

1. O número `aluno01` nos **3 nós MQTT** (mqtt in, e os dois mqtt out)
2. No nó do Influx: **URL**, **Token**, **organização** e **bucket**
3. **Deploy**

```json
[
  {
    "id": "tab1",
    "type": "tab",
    "label": "MQTT para InfluxDB",
    "disabled": false,
    "info": ""
  },
  {
    "id": "brk1",
    "type": "mqtt-broker",
    "name": "HiveMQ publico",
    "broker": "broker.hivemq.com",
    "port": "1883",
    "clientid": "",
    "autoConnect": true,
    "usetls": false,
    "protocolVersion": "4",
    "keepalive": "60",
    "cleansession": true,
    "autoUnsubscribe": true,
    "birthTopic": "",
    "birthQos": "0",
    "birthRetain": "false",
    "birthPayload": "",
    "birthMsg": {},
    "closeTopic": "",
    "closeQos": "0",
    "closeRetain": "false",
    "closePayload": "",
    "closeMsg": {},
    "willTopic": "",
    "willQos": "0",
    "willRetain": "false",
    "willPayload": "",
    "willMsg": {},
    "userProps": "",
    "sessionExpiry": ""
  },
  {
    "id": "inf1",
    "type": "influxdb",
    "hostname": "127.0.0.1",
    "port": "8086",
    "protocol": "http",
    "database": "database",
    "name": "InfluxDB Cloud",
    "usetls": true,
    "tls": "",
    "influxdbVersion": "2.0",
    "url": "https://COLE-A-URL-DO-PROFESSOR",
    "timeout": "10",
    "rejectUnauthorized": true
  },
  {
    "id": "grp1",
    "type": "ui_group",
    "name": "ESP32",
    "tab": "tabui1",
    "order": 1,
    "disp": true,
    "width": "6",
    "collapse": false,
    "className": ""
  },
  {
    "id": "tabui1",
    "type": "ui_tab",
    "name": "Aula MQTT",
    "icon": "dashboard",
    "disabled": false,
    "hidden": false
  },
  {
    "id": "min1",
    "type": "mqtt in",
    "z": "tab1",
    "name": "sensor do aluno01",
    "topic": "senai_alagoinhas_2026/aula8/aluno01/sensor",
    "qos": "0",
    "datatype": "json",
    "broker": "brk1",
    "nl": false,
    "rap": true,
    "rh": 0,
    "inputs": 0,
    "x": 160,
    "y": 160,
    "wires": [
      [
        "dbg0",
        "fn1"
      ]
    ]
  },
  {
    "id": "dbg0",
    "type": "debug",
    "z": "tab1",
    "name": "chegou do broker",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": false,
    "complete": "payload",
    "targetType": "msg",
    "statusVal": "",
    "statusType": "auto",
    "x": 420,
    "y": 80,
    "wires": []
  },
  {
    "id": "fn1",
    "type": "function",
    "z": "tab1",
    "name": "prepara dados",
    "func": "// A mensagem do ESP32 ja chega como objeto:\n// {\"bruto\":2048,\"tensao\":1.65,\"temperatura\":25}\nvar d = msg.payload;\n\n// 1) Confere se e um objeto com os tres campos numericos\nif (d === null || typeof d !== \"object\" ||\n    typeof d.bruto !== \"number\" ||\n    typeof d.tensao !== \"number\" ||\n    typeof d.temperatura !== \"number\") {\n    node.warn(\"Mensagem ignorada: \" + JSON.stringify(msg.payload));\n    return null;\n}\n\n// 2) O aluno vem do topico: senai_alagoinhas_2026/aula8/aluno01/sensor\nvar aluno = msg.topic.split(\"/\")[2];\n\n// Saida 1: para o InfluxDB -> [campos, tags]\nvar influx = { payload: [\n    { bruto: d.bruto, tensao: d.tensao, temperatura: d.temperatura },\n    { aluno: aluno }\n]};\n\n// Saida 2: so a temperatura, para o painel e para a regra do alarme\nvar temp = { payload: d.temperatura };\n\nreturn [influx, temp];",
    "outputs": 2,
    "timeout": 0,
    "noerr": 0,
    "initialize": "",
    "finalize": "",
    "libs": [],
    "x": 420,
    "y": 160,
    "wires": [
      [
        "iout1",
        "dbg1"
      ],
      [
        "gauge1",
        "chart1",
        "fn2"
      ]
    ]
  },
  {
    "id": "iout1",
    "type": "influxdb out",
    "z": "tab1",
    "influxdb": "inf1",
    "name": "gravar no Influx",
    "measurement": "sensor",
    "precision": "",
    "retentionPolicy": "",
    "database": "database",
    "precisionV18FluxV20": "ms",
    "retentionPolicyV18Flux": "",
    "org": "COLE-A-ORGANIZACAO",
    "bucket": "COLE-O-BUCKET",
    "x": 680,
    "y": 120,
    "wires": []
  },
  {
    "id": "dbg1",
    "type": "debug",
    "z": "tab1",
    "name": "vai para o Influx",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": false,
    "complete": "payload",
    "targetType": "msg",
    "statusVal": "",
    "statusType": "auto",
    "x": 690,
    "y": 60,
    "wires": []
  },
  {
    "id": "gauge1",
    "type": "ui_gauge",
    "z": "tab1",
    "name": "",
    "group": "grp1",
    "order": 1,
    "width": 0,
    "height": 0,
    "gtype": "gage",
    "title": "Temperatura",
    "label": "°C",
    "format": "{{value}}",
    "min": 0,
    "max": 50,
    "colors": [
      "#00b500",
      "#e6e600",
      "#ca3838"
    ],
    "seg1": "30",
    "seg2": "35",
    "diff": false,
    "className": "",
    "x": 680,
    "y": 200,
    "wires": []
  },
  {
    "id": "chart1",
    "type": "ui_chart",
    "z": "tab1",
    "name": "",
    "group": "grp1",
    "order": 2,
    "width": 0,
    "height": 0,
    "label": "Histórico",
    "chartType": "line",
    "legend": "false",
    "xformat": "HH:mm:ss",
    "interpolate": "linear",
    "nodata": "",
    "dot": false,
    "ymin": "0",
    "ymax": "50",
    "removeOlder": 1,
    "removeOlderPoints": "",
    "removeOlderUnit": "3600",
    "cutout": 0,
    "useOneColor": false,
    "useUTC": false,
    "colors": [
      "#1f77b4",
      "#aec7e8",
      "#ff7f0e",
      "#2ca02c",
      "#98df8a",
      "#d62728",
      "#ff9896",
      "#9467bd",
      "#c5b0d5"
    ],
    "outputs": 1,
    "useDifferentColor": false,
    "className": "",
    "x": 680,
    "y": 260,
    "wires": [
      []
    ]
  },
  {
    "id": "fn2",
    "type": "function",
    "z": "tab1",
    "name": "regra do alarme",
    "func": "// ===== REGRA DO ALARME =====\nvar LIMITE = 35;   // graus C: acima disso, alarme\n\nvar temp = msg.payload;          // temperatura que chegou (numero)\nvar alarme = temp > LIMITE;      // true = acima do limite\n\n// Saida 1: LED verde (temperatura OK)  -> apaga se estiver em alarme\nvar verde = { payload: alarme ? \"OFF\" : \"ON\" };\n\n// Saida 2: LED vermelho (alarme)       -> acende se estiver em alarme\nvar vermelho = { payload: alarme ? \"ON\" : \"OFF\" };\n\n// Saida 3: texto de estado para o painel\nvar estado = { payload: alarme ? \"ALARME: temperatura alta!\" : \"Temperatura OK\" };\n\nreturn [verde, vermelho, estado];",
    "outputs": 3,
    "timeout": 0,
    "noerr": 0,
    "initialize": "",
    "finalize": "",
    "libs": [],
    "x": 680,
    "y": 340,
    "wires": [
      [
        "mout_v"
      ],
      [
        "mout_r"
      ],
      [
        "txt1"
      ]
    ]
  },
  {
    "id": "mout_v",
    "type": "mqtt out",
    "z": "tab1",
    "name": "LED verde",
    "topic": "senai_alagoinhas_2026/aula8/aluno01/led_verde",
    "qos": "0",
    "retain": "false",
    "respTopic": "",
    "contentType": "",
    "userProps": "",
    "correl": "",
    "expiry": "",
    "broker": "brk1",
    "x": 930,
    "y": 320,
    "wires": []
  },
  {
    "id": "mout_r",
    "type": "mqtt out",
    "z": "tab1",
    "name": "LED vermelho",
    "topic": "senai_alagoinhas_2026/aula8/aluno01/led_vermelho",
    "qos": "0",
    "retain": "false",
    "respTopic": "",
    "contentType": "",
    "userProps": "",
    "correl": "",
    "expiry": "",
    "broker": "brk1",
    "x": 930,
    "y": 360,
    "wires": []
  },
  {
    "id": "txt1",
    "type": "ui_text",
    "z": "tab1",
    "group": "grp1",
    "order": 3,
    "width": 0,
    "height": 0,
    "name": "",
    "label": "Estado",
    "format": "{{msg.payload}}",
    "layout": "row-spread",
    "className": "",
    "style": false,
    "font": "",
    "fontSize": 16,
    "color": "#000000",
    "x": 920,
    "y": 400,
    "wires": []
  }
]
```

> O token não vai dentro do JSON; precisa ser digitado de novo.

### 6.2 Problemas comuns

| Sintoma | Causa provável | Solução |
|---|---|---|
| Nó mqtt in sem "connected" | Rede bloqueando a porta 1883 | Teste em outra rede / roteador do celular |
| Nada chega no debug | `aluno` diferente no ESP32 e no Node-RED | Conferir o número nos tópicos |
| Nada grava no Influx | Token/URL/org/bucket errados | Ver o erro no painel de debug; refazer os 4 dados |
| Erro 401 no Influx | Token inválido ou sem permissão de escrita | Gerar token novo com Write no bucket |
| LED não responde | Tópico errado ou código antigo no ESP32 | Regravar o código desta aula |
| LED verde e vermelho acesos juntos | Teste manual deixou um aceso | Gire o potenciômetro e aguarde a próxima leitura |
| LED não acende nunca | Polaridade invertida ou sem resistor/GND | Conferir perna longa e GND |
| Dashboard vazio | Falta Deploy ou dashboard não instalado | Instalar `node-red-dashboard` e dar Deploy |
| LEDs param de mudar | Node-RED parado | Verificar a janela do cmd |

### 6.3 Atividade

**Entrega:** foto do circuito com os dois LEDs, prints do dashboard (normal e em alarme) e do Data Explorer (**escondendo o token**).

**Melhoria 1 — Histerese.** Hoje, perto de 35 °C o LED pode ficar "piscando" entre verde e vermelho. Use dois limites: liga o alarme acima de 35, só desliga abaixo de 33. Substitua **todo o código** da "regra do alarme" por:

```javascript
var LIGA = 35;     // acima disso, liga o alarme
var DESLIGA = 33;  // so desliga abaixo disso

var temp = msg.payload;
var alarme = context.get("alarme") || false;   // lembra o estado anterior

if (temp > LIGA) {
    alarme = true;
} else if (temp < DESLIGA) {
    alarme = false;
}
context.set("alarme", alarme);

var verde    = { payload: alarme ? "OFF" : "ON" };
var vermelho = { payload: alarme ? "ON" : "OFF" };
var estado   = { payload: alarme ? "ALARME: temperatura alta!" : "Temperatura OK" };

return [verde, vermelho, estado];
```

**Melhoria 2 — Limite ajustável pelo dashboard.** Adicione um **ui_slider** (0–50) ligado a uma função que guarda o limite:

```javascript
flow.set("limite", Number(msg.payload));
return null;
```

E substitua **todo o código** da "regra do alarme" por este:

```javascript
// Le o limite guardado pelo painel; se ainda nao existe, usa 35
var LIMITE = flow.get("limite");
if (LIMITE === undefined) { LIMITE = 35; }

var temp = msg.payload;
var alarme = temp > LIMITE;

var verde    = { payload: alarme ? "OFF" : "ON" };
var vermelho = { payload: alarme ? "ON" : "OFF" };
var estado   = { payload: alarme ? "ALARME: temperatura alta!" : "Temperatura OK" };

return [verde, vermelho, estado];
```

**Melhoria 3 — Consulta.** No Data Explorer, mostre só a última hora:

```sql
SELECT time, temperatura FROM sensor WHERE time >= now() - interval '1 hour' ORDER BY time
```

### 6.4 Cola rápida

| Item | Valor |
|---|---|
| Broker | `broker.hivemq.com` : `1883` |
| Sensor | `senai_alagoinhas_2026/aula8/alunoXX/sensor` |
| LED verde | `.../alunoXX/led_verde` (GPIO 23) |
| LED vermelho | `.../alunoXX/led_vermelho` (GPIO 22) |
| Limite | 35 °C |
| Node-RED | http://localhost:1880 · dashboard `/ui` |
| Influx | measurement `sensor`, tag `aluno`, bucket `aula_mqtt` |

**Próxima aula:** consultas e gráficos mais avançados no InfluxDB.
