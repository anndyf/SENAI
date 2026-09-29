# Guia Prático — ESP32 na Rede com MQTT (Windows)

**SENAI · Sistemas Eletrônicos e Microcontrolados · Aulas 7 e 8**

Este guia mostra, passo a passo, como programar o ESP32, conectá-lo ao Wi-Fi e fazê-lo enviar dados para um broker MQTT. Siga as etapas **na ordem** e só avance quando a etapa atual funcionar.

---

## O que você vai precisar

- Placa **ESP32 DevKit** (DOIT ESP32 DEVKIT V1 ou similar)
- **Cabo USB de dados** (atenção: muitos cabos só carregam e não transmitem dados)
- Computador com **Windows 10 ou 11**
- Rede **Wi-Fi 2,4 GHz** (o ESP32 **não** funciona em redes 5 GHz)
- Sensor **DHT11** (opcional: sem ele, o código gera dados simulados)

## Mapa das etapas

| Etapa | O que você faz | Resultado esperado |
|---|---|---|
| 1 | Instala a Arduino IDE e grava o primeiro programa | LED azul da placa piscando |
| 2 | Conecta o ESP32 ao Wi-Fi | IP do ESP32 aparece no Serial Monitor |
| 3 | Instala e testa o broker Mosquitto | Mensagem de teste chega entre duas janelas |
| 4 | ESP32 publica temperatura e umidade via MQTT | Dados chegando a cada 5 segundos e LED comandado pelo PC |

> **No laboratório:** o broker (Etapa 3) roda no PC do professor. Nesse caso, pule a Etapa 3 e use o IP que o professor passar. Faça a Etapa 3 quando for praticar em casa.

---

## Etapa 1 — Arduino IDE e primeiro programa

### 1.1 Instalar a Arduino IDE

1. Acesse **arduino.cc/en/software**.
2. Baixe a versão **Windows (Win 10 and newer, 64 bits)**, arquivo `.exe`.
3. Execute o instalador e aceite as opções padrão (Avançar → Avançar → Instalar).
4. Se o Windows perguntar se deseja instalar drivers da Arduino, clique em **Instalar**.

### 1.2 Adicionar o ESP32 à Arduino IDE

A Arduino IDE não vem com o ESP32. Precisamos adicionar o pacote da fabricante (Espressif).

1. Abra a Arduino IDE.
2. Vá em **File → Preferences** (Arquivo → Preferências).
3. No campo **Additional boards manager URLs** (URLs adicionais para gerenciadores de placas), cole:

   ```
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
   ```

4. Clique em **OK**.
5. Clique no ícone de **placa** na barra lateral esquerda (Boards Manager).
6. Pesquise **esp32** e instale o pacote **esp32 by Espressif Systems**.
   - O download é grande e pode levar vários minutos. Aguarde terminar.

### 1.3 Conectar a placa e escolher a porta

1. Conecte o ESP32 ao PC com o cabo USB.
2. Em **Tools → Board → esp32**, escolha **DOIT ESP32 DEVKIT V1** (ou **ESP32 Dev Module**).
3. Em **Tools → Port**, escolha a porta **COM** que apareceu (ex.: `COM3`, `COM5`).

**A porta COM não aparece?** Siga esta ordem:

1. **Troque o cabo USB.** Esta é a causa mais comum: cabo que só carrega.
2. Abra o **Gerenciador de Dispositivos** (clique com o botão direito no menu Iniciar → Gerenciador de Dispositivos).
3. Procure em **Portas (COM e LPT)** ou em **Outros dispositivos**. Se houver um item com **triângulo amarelo**, falta o driver.
4. Olhe o chip pequeno próximo ao conector USB da placa e instale o driver correspondente:
   - Chip **CP2102**: pesquise "CP210x USB to UART Bridge VCP Drivers" no site da **Silicon Labs**.
   - Chip **CH340**: pesquise "CH341SER" no site da **WCH**.
5. Desconecte e reconecte a placa. A porta COM deve aparecer.

### 1.4 Gravar o primeiro programa (Blink)

1. Vá em **File → New Sketch** (Arquivo → Novo).
2. Apague o conteúdo e cole o código:

```cpp
#define LED 2   // LED azul da placa

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
}

void loop() {
  digitalWrite(LED, HIGH);
  Serial.println("LED ligado");
  delay(1000);
  digitalWrite(LED, LOW);
  Serial.println("LED desligado");
  delay(1000);
}
```

3. Clique em **Upload** (botão com a seta →).
4. Se a gravação travar na mensagem `Connecting........`, **segure o botão BOOT** da placa até a gravação começar e depois solte.

### 1.5 Conferir

- O **LED azul** da placa deve piscar a cada 1 segundo.
- Abra o **Serial Monitor** (ícone de lupa no canto superior direito).
- No canto do Serial Monitor, selecione **115200 baud**.
- Devem aparecer as mensagens "LED ligado" e "LED desligado".

> Se aparecerem caracteres estranhos no Serial Monitor, a velocidade está errada. Confirme **115200 baud**.

✅ **Etapa 1 concluída quando:** o LED pisca e as mensagens aparecem.

---

## Etapa 2 — ESP32 no Wi-Fi

### 2.1 Gravar o código de conexão

1. Crie um sketch novo (**File → New Sketch**).
2. Cole o código abaixo e troque **NOME_DA_REDE** e **SENHA_DA_REDE** pelos dados da sua rede (maiúsculas e minúsculas fazem diferença):

```cpp
#include <WiFi.h>

const char* ssid  = "NOME_DA_REDE";
const char* senha = "SENHA_DA_REDE";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, senha);
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado!");
  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.localIP());
}

void loop() { }
```

3. Faça o **Upload** e abra o **Serial Monitor** em 115200 baud.
4. Se a tela estiver vazia, aperte o botão **EN** (ou RST) da placa para reiniciá-la.

Resultado esperado:

```
Conectando.....
Conectado!
IP do ESP32: 192.168.0.60
```

**Anote o IP do ESP32.**

### 2.2 Descobrir o IP do seu PC

1. Aperte **Windows + R**, digite `cmd` e aperte Enter.
2. No Prompt de Comando, digite:

   ```
   ipconfig
   ```

3. Procure o bloco **Adaptador de Rede sem Fio Wi-Fi** (ou **Ethernet**, se o PC estiver no cabo).
4. Anote o **Endereço IPv4** (ex.: `192.168.0.49`).

Os dois IPs devem começar iguais (ex.: `192.168.0.60` e `192.168.0.49`). Isso confirma que ESP32 e PC estão na mesma rede.

> **Fica só imprimindo pontinhos?** Confira nome e senha da rede e se a rede é **2,4 GHz**. Redes com "5G" no nome não funcionam.

✅ **Etapa 2 concluída quando:** o IP do ESP32 aparece no Serial Monitor.

---

## Etapa 3 — Broker Mosquitto no Windows

> **No laboratório, pule esta etapa:** o broker já está rodando no PC do professor. Faça quando for praticar em casa.

O **broker** é o "carteiro" do MQTT: recebe todas as mensagens e entrega para quem assinou cada tópico.

### 3.1 Instalar

1. Acesse **mosquitto.org/download**.
2. Na seção **Windows**, baixe o arquivo **mosquitto-…-install-windows-x64.exe**.
3. Execute o instalador com as opções padrão.
   - Ele instala em `C:\Program Files\mosquitto`.
   - Ele também cria um **serviço do Windows** chamado "Mosquitto Broker".

### 3.2 Liberar o acesso pela rede (arquivo de configuração)

Por padrão, o Mosquitto só aceita conexões do próprio PC. Precisamos liberar para o ESP32.

1. No menu Iniciar, digite **Bloco de Notas**.
2. Clique com o **botão direito** em Bloco de Notas → **Executar como administrador**.
3. No Bloco de Notas, vá em **Arquivo → Abrir**.
4. Navegue até `C:\Program Files\mosquitto`.
5. No canto inferior direito da janela, troque o filtro de "Documentos de texto (*.txt)" para **Todos os arquivos (\*.\*)**.
6. Abra o arquivo **mosquitto.conf**.
7. Vá até o **final do arquivo** (Ctrl + End) e acrescente estas duas linhas:

   ```
   listener 1883
   allow_anonymous true
   ```

8. Salve (Ctrl + S) e feche.

> Se aparecer "Acesso negado" ao salvar, o Bloco de Notas não foi aberto como administrador. Repita a partir do passo 2.

### 3.3 Liberar a porta 1883 no Firewall

1. No menu Iniciar, digite **PowerShell**.
2. Clique com o **botão direito** → **Executar como administrador**.
3. Cole o comando abaixo e aperte Enter:

   ```
   New-NetFirewallRule -DisplayName "Mosquitto MQTT" -Direction Inbound -Protocol TCP -LocalPort 1883 -Action Allow
   ```

4. Verifique se a rede está como **Privada**: **Configurações → Rede e Internet → Wi-Fi → (nome da sua rede)** → Tipo de perfil de rede: **Privada**.

### 3.4 Iniciar o broker

Vamos rodar o broker em uma janela visível, para ver as conexões acontecendo.

1. No menu Iniciar, digite **cmd**.
2. Clique com o **botão direito** em Prompt de Comando → **Executar como administrador**.
3. Pare o serviço automático (para ele não ocupar a porta):

   ```
   net stop mosquitto
   ```

4. Entre na pasta do Mosquitto:

   ```
   cd "C:\Program Files\mosquitto"
   ```

5. Inicie o broker usando o arquivo de configuração:

   ```
   mosquitto -c mosquitto.conf -v
   ```

6. Devem aparecer linhas terminando em:

   ```
   Opening ipv4 listen socket on port 1883.
   mosquitto version 2.x.x running
   ```

**Deixe esta janela aberta** durante todo o teste. Ela é a **Janela 1 (broker)**.

> **Erro "Only one usage of each socket address"?** O serviço ainda está rodando. Execute `net stop mosquitto` novamente.

### 3.5 Testar o broker

Abra **mais duas janelas** do Prompt de Comando (não precisam ser de administrador). Em **cada uma**, entre primeiro na pasta do Mosquitto:

```
cd "C:\Program Files\mosquitto"
```

**Janela 2 — quem escuta (subscriber).** Troque o IP pelo IPv4 do seu PC:

```
mosquitto_sub -h 192.168.0.49 -t "senai/#" -v
```

A janela fica "parada", esperando mensagens. Isso é normal.

**Janela 3 — quem envia (publisher):**

```
mosquitto_pub -h 192.168.0.49 -t senai/teste -m "ola turma"
```

Resultado esperado na **Janela 2**:

```
senai/teste ola turma
```

**Entendendo os comandos:**

| Parte do comando | Significado |
|---|---|
| `-h 192.168.0.49` | endereço (IP) do broker |
| `-t senai/teste` | tópico da mensagem |
| `-t "senai/#"` | assina **todos** os tópicos que começam com `senai/` |
| `-m "ola turma"` | conteúdo da mensagem |
| `-v` | mostra também o nome do tópico junto da mensagem |

✅ **Etapa 3 concluída quando:** a mensagem enviada na Janela 3 aparece na Janela 2.

---

## Etapa 4 — ESP32 publicando dados via MQTT

### 4.1 Instalar as bibliotecas

1. Na Arduino IDE, clique no ícone de **livros** na barra lateral (Library Manager).
2. Pesquise e instale:
   - **PubSubClient** (autor: Nick O'Leary)
   - **ArduinoJson** (autor: Benoit Blanchon), **versão 7**
3. **Só se for usar o DHT11**, instale também:
   - **DHT sensor library** (Adafruit). Quando perguntar, clique em **Install All** para instalar junto a **Adafruit Unified Sensor**.

### 4.2 Montar o circuito (somente com DHT11)

| DHT11 | ESP32 |
|---|---|
| VCC (+) | 3V3 |
| DATA | GPIO 4 (D4) |
| GND (−) | GND |

> Se o seu DHT11 for o sensor "puro" (4 pinos, sem placa), coloque um **resistor de 10 kΩ entre DATA e 3V3**. Os módulos com placa (3 pinos) já têm esse resistor.
>
> **Sobre o DHT11:** mede de 0 a 50 °C (precisão de ±2 °C) e umidade de 20 a 80 % (±5 %), com valores inteiros. É mais simples que o DHT22, mas atende bem ao nosso projeto. Ele só aceita uma leitura por segundo, e nosso código lê a cada 5 segundos.

Sem o sensor, não monte nada: o código gera valores simulados.

### 4.3 Configurar o código

Crie um sketch novo, cole o código abaixo e altere **somente** a parte marcada como CONFIGURAÇÃO:

1. **Nome e senha do Wi-Fi.**
2. **IP do broker:** o IPv4 do PC onde o Mosquitto está rodando (no laboratório, o IP do professor).
3. **Seu número de aluno:** troque `aluno01` pelo seu número (ex.: `aluno07`) nas **três** linhas indicadas. Cada ESP32 precisa de um nome único: se dois alunos usarem o mesmo, um derruba a conexão do outro.
4. **USAR_SENSOR:** deixe `0` para dados simulados ou troque para `1` se tiver o DHT11 ligado.

```cpp
// =====================================================
//  Aula 8 - ESP32 + MQTT
// =====================================================
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ---------- CONFIGURACAO ----------
const char* ssid        = "NOME_DA_REDE";
const char* password    = "SENHA_DA_REDE";
const char* mqtt_server = "192.168.0.49";   // IP do PC com o Mosquitto
const int   mqtt_port   = 1883;

// TROQUE "aluno01" pelo seu numero nas 3 linhas abaixo!
const char* client_id = "ESP32_aluno01";
const char* topic_pub = "senai/aula8/aluno01/sensor";
const char* topic_sub = "senai/aula8/aluno01/led";

#define USAR_SENSOR 0   // 0 = dados simulados | 1 = sensor DHT11 no GPIO 4
// -----------------------------------

#define LEDPIN 2   // LED azul da placa

#if USAR_SENSOR
  #include <DHT.h>
  DHT dht(4, DHT11);
#endif

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long ultimoEnvio = 0;
float tempSimulada = 25.0;

// Chamada automaticamente quando chega mensagem no topico assinado
void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  Serial.print("Comando recebido: ");
  Serial.println(msg);
  if (msg == "ON")  digitalWrite(LEDPIN, HIGH);
  if (msg == "OFF") digitalWrite(LEDPIN, LOW);
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
      client.subscribe(topic_sub);
    } else {
      Serial.print("falhou, codigo = ");
      Serial.println(client.state());   // -2 = broker inacessivel
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LEDPIN, OUTPUT);
  randomSeed(esp_random());
  #if USAR_SENSOR
    dht.begin();
  #endif
  conectaWiFi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) conectaMQTT();
  client.loop();

  if (millis() - ultimoEnvio > 5000) {   // a cada 5 segundos
    ultimoEnvio = millis();

    #if USAR_SENSOR
      float temperatura = dht.readTemperature();
      float umidade     = dht.readHumidity();
      if (isnan(temperatura) || isnan(umidade)) {
        Serial.println("Falha na leitura do DHT11");
        return;
      }
    #else
      tempSimulada += random(-5, 6) / 10.0;
      tempSimulada = constrain(tempSimulada, 20.0, 35.0);
      float temperatura = tempSimulada;
      float umidade     = 55.0 + random(0, 100) / 10.0;
    #endif

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
```

### 4.4 Gravar e conferir no Serial Monitor

1. Faça o **Upload** e abra o **Serial Monitor** em 115200 baud.
2. Resultado esperado:

   ```
   Conectando ao Wi-Fi....
   Wi-Fi OK - IP: 192.168.0.60
   Conectando ao broker... OK
   Publicado: {"temperatura":25.3,"umidade":61.2}
   Publicado: {"temperatura":25.1,"umidade":58.7}
   ```

   Com o DHT11 os valores chegam inteiros (ex.: `{"temperatura":26,"umidade":58}`). Isso é normal.

### 4.5 Ver os dados chegando no PC

Com o broker rodando (Janela 1) e a **Janela 2** assinando `senai/#`, a cada 5 segundos deve aparecer:

```
senai/aula8/aluno01/sensor {"temperatura":25.3,"umidade":61.2}
```

> **No laboratório**, o professor mostra os dados de toda a turma chegando no telão.

### 4.6 Comandar o LED pelo PC

Na **Janela 3**, envie (troque o IP e o número do aluno):

```
mosquitto_pub -h 192.168.0.49 -t senai/aula8/aluno01/led -m ON
```

O **LED azul** da placa acende e o Serial Monitor mostra `Comando recebido: ON`. Para apagar:

```
mosquitto_pub -h 192.168.0.49 -t senai/aula8/aluno01/led -m OFF
```

✅ **Etapa 4 concluída quando:** os dados chegam a cada 5 segundos e o LED responde a ON/OFF.

---

## Problemas comuns

| Sintoma | Causa provável | Solução |
|---|---|---|
| Porta COM não aparece | Cabo só de carga ou falta de driver | Troque o cabo; instale o driver CP2102 ou CH340 (Etapa 1.3) |
| Upload trava em `Connecting....` | Placa não entrou em modo de gravação | Segure o botão **BOOT** até começar a gravar |
| Caracteres estranhos no Serial Monitor | Velocidade errada | Selecione **115200 baud** |
| Serial Monitor vazio | Programa já rodou antes de abrir o monitor | Aperte o botão **EN/RST** da placa |
| Só imprime pontinhos no Wi-Fi | Senha errada ou rede 5 GHz | Confira nome/senha e use rede **2,4 GHz** |
| `falhou, codigo = -2` | ESP32 não alcança o broker | Confira o IP do broker, se a Janela 1 está rodando e a regra do firewall (Etapa 3.3) |
| ESP32 conecta e desconecta sem parar | Dois ESP32 com o mesmo `client_id` | Use o **seu** número de aluno no código |
| `Only one usage of each socket address` | Serviço do Mosquitto ocupando a porta | Rode `net stop mosquitto` como administrador |
| `'mosquitto_sub' não é reconhecido` | Prompt fora da pasta do Mosquitto | Rode antes `cd "C:\Program Files\mosquitto"` |
| `Falha na leitura do DHT11` | Fiação ou resistor de pull-up | Confira VCC no 3V3, DATA no GPIO 4 e o resistor de 10 kΩ |
| LED não responde ao comando | Tópico ou mensagem diferente | Confira o tópico `.../led` com seu número e envie `ON`/`OFF` em maiúsculas |

---

## Checklist final

- [ ] LED da placa piscou com o programa Blink
- [ ] IP do ESP32 apareceu no Serial Monitor
- [ ] IP do PC anotado com `ipconfig`
- [ ] Broker testado com `mosquitto_sub` e `mosquitto_pub` (em casa)
- [ ] Meu número de aluno trocado nas 3 linhas do código
- [ ] Dados JSON chegando a cada 5 segundos
- [ ] LED acendendo e apagando com ON/OFF

**Próxima aula:** vamos receber esses dados no **Node-RED** e guardá-los no banco de dados **InfluxDB**. Traga o ESP32 com o código desta aula funcionando!
