# Aula 0 — Sensores, Eventos e Acionamento Remoto

**SENAI Alagoinhas · Sistemas Eletrônicos e Microcontrolados · Aula 0 · Grupos de 4**

---

## Grupos

Cada grupo tem **4 integrantes**. O **número do grupo** é o que vocês usam nos tópicos (`grupo01` a `grupo08`), tanto no ESP32 quanto no Node-RED. Dois grupos com o mesmo número se atrapalham, porque o broker é público.

### Dia 1

| Grupo | Integrantes |
|---|---|
| **grupo01** | Nathan Ferreira Dos Santos · Ícaro Alcantara Souza · Marcus Vinicius dos Santos Souza · Nicolas Gabriel Ramos das Virgens |
| **grupo02** | Állan Freitas dos Santos · Isabelle da Conceição Ramos · Rafael Silva Rabelo · Whislimar Alves da Silva |
| **grupo03** | Ariel Muniz de Brito · Jennifer Lavinie Araújo Moura Sá Barreto · Maurício Santos de Almeida · Tayná Lima da Silva |
| **grupo04** | Felipe Carneiro dos Santos · Leandro Castro de Oliveira · Alisson Miguel Campos Silva · Victoria Santana Batista |

### Dia 2

| Grupo | Integrantes |
|---|---|
| **grupo05** | Álef Gabriel Santiago dos Santos · Iasmin dos Santos Machado · Lucas Cerqueira Costa · Neilton Matos Moura |
| **grupo06** | Allan Albert Mendes de Brito · Ikaro Ryan Almeida dos Santos · Maria Eduarda Batista Barradas de Lima · Pedro Arthur de Jesus Nascimento |
| **grupo07** | Ana Claudia da Silva Santos · Jean Santos Santana · Matheus Vinício de Santana Cerqueira · Táfne Camille Araujo Santos |
| **grupo08** | Eder Gabriel Ferreira dos Santos da Paixão · Kaique Santana Pereira · Naelson Cardoso dos Santos · Thiago Alves da Hora |

---

Na aula 9 vocês **mediram** (potenciômetro) e publicaram **a cada 5 segundos**. Hoje o ESP32 passa a **reagir a eventos** (um sensor foi acionado) e a **comandar uma esteira pelo dashboard**, e o Node-RED ganha **memória**.

| Bloco | Ideia nova | Aplicação |
|---|---|---|
| 1 | Entrada digital, `INPUT_PULLUP` e **lógica invertida** | Ler sensores industriais, como o E18-D80NK |
| 2 | **Borda** (a mudança do sinal) e **debounce** | Reagir a um acionamento **uma única vez** |
| 3 | Publicar **quando algo muda** e comandar pelo dashboard | **Ligar e desligar uma esteira** à distância |
| 4 | Node-RED com **memória** (estados com `context`) | Um sistema que lembra em que estado está (alarme de porta) |
| 5 | Comandos de texto, contagem com condição e medidor | Controle de lotação de uma sala (**Anexo A**) |

---

## Como usar este guia

Este guia foi feito para o grupo **fazer sozinho, só replicando**. Cada prática tem o mesmo formato:

1. **Monte / confira** o circuito.
2. **Grave** o código (todos estão **completos**: é só copiar e colar).
3. **Monte o Node-RED** (nos blocos que usam painel) para **ver as ações acontecerem** no dashboard.
4. **Teste** com a tabela "Esperado" e marque ✅.
5. **Responda** as perguntas: elas é que fixam o conteúdo.

> 📌 **Guardem este material.** Os códigos, os fluxos do Node-RED e as tabelas de teste são a **base de referência** para as próximas aulas. Anotem o que deu certo e o que deu errado.

**Regra de ouro dos blocos 3, 4 e 5:** o que o ESP32 **publica**, o Node-RED **assina** (`mqtt in`). O que o ESP32 **assina**, o Node-RED **publica** (`mqtt out`). O texto do tópico precisa ser **idêntico** nos dois lados, inclusive o **número do grupo**.

Os papéis giram a cada bloco: **piloto** (teclado), **leitor** (confere o código), **testador** (aciona o circuito e anota) e **relator** (registra o que deu certo e errado). Todo mundo precisa conseguir explicar o que o grupo fez.

**Arquivos que acompanham este guia:** pasta `imagens/` (esquema da bancada e as imagens de referência dos fluxos do Node-RED), `codigos_esp32_aula0/` (os sketches) e `circuito_base_bancada.md` (montagem da bancada).

---

# PREPARAÇÃO — façam antes do Bloco 1

## P1. Número do grupo

Anotem aqui: **grupo \_\_ → `grupo01`, `grupo02` ... `grupo08`**. Sempre que este guia mostrar `grupo01`, **troquem pelo número do seu grupo**, no ESP32 **e** no Node-RED.

## P2. Arduino IDE

1. Placa: **Ferramentas → Placa → ESP32 Arduino → ESP32 Dev Module** (se não aparecer, instale o pacote **esp32 (Espressif)** em *Gerenciador de Placas*).
2. Porta: **Ferramentas → Porta** → a que aparece ao conectar o ESP32 (COM3, COM4...).
3. Bibliotecas (**Sketch → Incluir Biblioteca → Gerenciar Bibliotecas**), as mesmas da aula 9: **PubSubClient** (Nick O'Leary) e **ArduinoJson** (Benoit Blanchon, **versão 7**). Os blocos 1 e 2 não precisam delas; os blocos 3, 4 e 5 precisam.
4. **Serial Monitor** (lupa no canto superior direito): velocidade **115200 baud**.
5. Para gravar: botão **Upload** (seta →). Se aparecer `Connecting....`, **segure o botão BOOT** da placa até começar a gravar.

## P3. Circuito da bancada

Montem conforme **`circuito_base_bancada.md`** (esquema, colunas da protoboard e checklist).

| Componente | Ligação | Papel |
|---|---|---|
| **Botão** | um lado no **GPIO 27**, o outro no **GND** (sem resistor externo) | Faz o papel de um **sensor** |
| LED **verde** + resistor 220 Ω | ânodo (perna longa) no **GPIO 23**, cátodo no GND | Indicador |
| LED **vermelho** + resistor 220 Ω | ânodo no **GPIO 22**, cátodo no GND | Indicador |
| LED **amarelo** + resistor 220 Ω | ânodo no **GPIO 26**, cátodo no GND | Faz o papel do **motor da esteira** |

![Circuito da bancada](imagens/circuito_base_bancada.png)

*Esquema da bancada: botão no GPIO 27, LED vermelho no 22, LED verde no 23 e LED amarelo (o "motor") no 26.*

✅ Confiram: perna longa do LED do lado do resistor · os 3 LEDs com resistor · botão **sem** resistor · GND do ESP32 no trilho de GND da protoboard.

## P4. Node-RED (iniciar e instalar o dashboard)

1. Abram o `cmd` (Prompt de Comando), digitem `node-red` e esperem a mensagem **Started flows**. **Não fechem essa janela** durante a aula.
2. No navegador: **http://localhost:1880**.

3. Menu **☰** (canto superior direito) → **Gerenciar paleta**.

4. Aba **Nós**: procurem `node-red-dashboard`. Se **não** estiver, aba **Instalar** → pesquisem `node-red-dashboard` → **instalar**. (Se já instalaram na aula 10, está pronto.)

## P5. Cliente web do HiveMQ (para testar sem o Node-RED)

1. Abram **https://www.hivemq.com/demos/websocket-client/**.
2. Em *Connection*: host `broker.hivemq.com`, clique **Connect** (deve ficar verde, *Connected*).
3. **Subscribe**: em *Add New Topic Subscription*, digitem `senai_alagoinhas_2026/aula0/grupo01/#` (com o **seu** número) e cliquem **Subscribe**. O `#` recebe **tudo** do seu grupo.
4. **Publish**: preencham *Topic* e *Message* e cliquem **Publish**.

---

# RECEITAS DO NODE-RED (consultem nos blocos 3, 4 e 5)

Os blocos 3, 4 e 5 pedem para **montar um painel** do zero, **sem fluxo pronto**: cada nó e cada ligação estão descritos nos blocos, e cada bloco traz uma **imagem de referência** de como o fluxo deve ficar. Todos usam as mesmas receitas. **Leiam uma vez**; depois é só voltar aqui quando o bloco indicar.

### Os três fluxos desta aula

| Fluxo (aba) | Bloco | Nós usados | Lógica |
|---|---|---|---|
| **`Aula 0 - Esteira`** | 3 | `mqtt in`, 2 × `text`, `debug`, 2 × `button`, `mqtt out` | **Sem função**: o Node-RED só **repassa**. O estado chega do ESP32 e vira texto; os botões mandam `ON`/`OFF` |
| **`Aula 0 - Alarme`** | 4 | `mqtt in`, `debug`, 2 × `button`, **`function`** (3 saídas), 2 × `mqtt out`, `text` | **Máquina de estados** com `context`: DESARMADO → ARMADO → DISPARADO. `msg.topic === "botao"` separa o botão do ESP32; o estado decide as 3 saídas |
| **`Aula 0 - Anexo A (Sala)`** | 5 | `mqtt in`, `debug`, 2 × `button`, **`function`** (4 saídas), 3 × `mqtt out`, `text`, `gauge` | Máquina de estados **com número**: FECHADA → ABERTA → LOTADA quando `visitas >= LOTACAO`. O botão **Zerar** tem **duas** saídas (função e `mqtt out comando`) |

> Em todos os fluxos: **um** servidor `HiveMQ publico` compartilhado, `QoS 0`, `Reter` **falso**, `mqtt in` com saída **objeto JSON**, e **o mesmo número de grupo** em todos os tópicos.

### R1. Criar uma aba de fluxo

Clique no **+** ao lado das abas, depois **duplo clique** na aba nova e dê o **nome** indicado pelo bloco.

### R2. Colocar e ligar nós

- Os nós ficam na **paleta** (à esquerda). Use o campo **filtrar nós** (digite `mqtt`, `button`, `text`...).
- **Arraste** o nó para a área de trabalho.
- Para **ligar**: arraste da **bolinha cinza da direita** (saída) de um nó até a **bolinha da esquerda** (entrada) de outro.
- **Duplo clique** em um nó abre a configuração. Confirme em **Feito**.

### R3. Nó **mqtt in** (receber do ESP32)

Seção *rede*. Duplo clique:

| Campo | Valor |
|---|---|
| **Servidor** | `HiveMQ publico` (crie na primeira vez, **R3.1**) |
| **Ação** | Assinar um tópico único |
| **Tópico** | o tópico indicado no bloco, com **o seu número de grupo** |
| **QoS** | `0` |
| **Saída** | **um objeto JSON analisado sintaticamente** ← importante |
| **Nome** | o nome indicado no bloco |

> Com "objeto JSON analisado", o ESP32 envia `{"esteira":"ON","acionamentos":3}` e vocês acessam `msg.payload.esteira`. Com a opção errada, chega um texto e os campos não funcionam.

**R3.1. Criar o servidor (só na primeira vez).** No campo **Servidor**, clique no **+** (ou no lápis para editar):

| Campo | Valor |
|---|---|
| **Nome** | `HiveMQ publico` |
| **Servidor** | `broker.hivemq.com` |
| **Porta** | `1883` |
| **Usar TLS** | desmarcado |
| **ID do cliente** | **em branco** |

Clique em **Adicionar** e depois **Feito**. Todos os outros nós mqtt **reaproveitam** este servidor.

### R4. Nó **mqtt out** (mandar comandos ao ESP32)

Seção *rede*. Duplo clique:

| Campo | Valor |
|---|---|
| **Servidor** | `HiveMQ publico` |
| **Tópico** | o tópico indicado no bloco, com **o seu número de grupo** |
| **QoS** | `0` |
| **Reter** | **falso** |
| **Nome** | o nome indicado no bloco |

> **Reter = falso:** com `true`, o broker guarda o último comando e **reenvia** quando o ESP32 reconectar.

### R5. Nó **button** (botão do dashboard)

Seção *dashboard*. Duplo clique:

| Campo | Valor |
|---|---|
| **Group** | o grupo indicado no bloco (veja **R5.1** na primeira vez) |
| **Label** | o texto que aparece no botão |
| **Payload** | tipo **string** (az) e o valor indicado (`ON`, `ARMAR`...) |
| **Topic** | o indicado no bloco. Se o bloco não indicar, deixe como está |

**R5.1. Criar a aba e o grupo do dashboard (só na primeira vez).** No campo **Group**, clique no **+**:

- **Name:** o nome do grupo indicado no bloco (`Esteira`, `Alarme de porta`...).
- **Tab:** clique no **+** ao lado, crie a aba **`Aula 0`** (só uma vez; os grupos seguintes escolhem essa mesma aba).
- Cliquem **Adicionar** nas duas janelas.

### R6. Nó **text** (mostrar um valor)

Seção *dashboard*. Duplo clique:

| Campo | Valor |
|---|---|
| **Group** | o grupo indicado no bloco |
| **Label** | o rótulo indicado |
| **Value format** | o indicado (por exemplo `{{msg.payload.esteira}}` ou `{{msg.payload}}`) |

### R7. Nó **function** (a regra)

Seção *função*. Duplo clique:

1. **Nome:** o indicado no bloco.
2. Aba **Configurar**: campo **Saídas** com o número indicado (3, 4...).
3. Aba **Na mensagem**: **apaguem tudo** e **colem** o código do bloco.
4. **Feito.** O nó passa a mostrar as bolinhas de saída (uma por saída). **A ordem das saídas importa**: a saída 1 é a de cima.

### R8. Nó **debug** (enxergar a mensagem)

Seção *comum*. Ligue na saída do `mqtt in`. Ao implementar, as mensagens aparecem na barra lateral direita (ícone do **inseto**).

### R9. Nó **gauge** (medidor)

Seção *dashboard*. Duplo clique:

| Campo | Valor |
|---|---|
| **Group** | o grupo indicado |
| **Type** | Gauge |
| **Label** | `Visitas` |
| **Value format** | `{{value}}` |
| **Range** | min `0`, max `3` |

### R10. Implementar e abrir o dashboard

1. Clique no botão vermelho **implementar** (canto superior direito). **Sem isso, nada do que vocês montaram passa a valer.**
2. A **bolinha verde** com *conectado* aparece embaixo dos nós mqtt. Se aparecer *desconectado*: internet, servidor `broker.hivemq.com` e porta `1883`.
3. Abram o dashboard: **http://localhost:1880/ui**.

### R11. Procurar por `grupo01`

Para achar todos os nós que ainda têm o número errado: **Ctrl+F** (Procurar fluxos) e digitem `grupo01`.

---

# BLOCO 1 — Entrada digital e lógica invertida

> Neste bloco **não usa Node-RED**: só ESP32 e Serial Monitor.

## Ideia

Um pino configurado como **entrada** precisa de um nível conhecido. Se ele não estiver ligado a nada, "flutua" e lê valores aleatórios. O ESP32 tem um **resistor de pull-up interno** que puxa o pino para 3,3 V quando nada está ligado:

```
      3V3
       │
     [pull-up interno]
       │
GPIO 27 ┬──────────── lê HIGH (botão solto)
        │
     [botão]
        │
       GND ─────────── lê LOW (botão apertado)
```

| Situação | Leitura do pino |
|---|---|
| Botão **solto** | `HIGH` |
| Botão **apertado** | `LOW` |

Ou seja, **a lógica é invertida**: "ativo" significa `LOW`.

> 🔎 **Por que importa?** Os sensores industriais do tipo **NPN**, como o **E18-D80NK**, se comportam exatamente assim: quando detectam um objeto, a saída é puxada para o **GND**. O botão serve para treinar o sensor.

## Prática 1.1 — O LED espelha o botão (Código 1)

**Roteiro:**

1. Abram um **sketch novo** na Arduino IDE e **colem** o código abaixo.
2. **Gravem** (Upload) e abram o **Serial Monitor** (115200).
3. **Apertem e soltem** o botão e observem o LED verde e o Serial.
4. Preencham a tabela de teste.

```cpp
// Aula 0 - Codigo 1: entrada digital com pull-up
#define BOTAO      27
#define LED_VERDE  23

void setup() {
  Serial.begin(115200);
  pinMode(BOTAO, INPUT_PULLUP);    // entrada com pull-up interno
  pinMode(LED_VERDE, OUTPUT);
}

void loop() {
  int leitura = digitalRead(BOTAO);

  if (leitura == LOW) {            // LOW = apertado
    digitalWrite(LED_VERDE, HIGH);
  } else {
    digitalWrite(LED_VERDE, LOW);
  }

  Serial.println(leitura);         // veja 1 (solto) e 0 (apertado)
  delay(100);
}
```

| # | Teste | Esperado | ✅ |
|---|---|---|---|
| 1 | Botão **solto** | LED verde apagado; Serial mostra `1` | |
| 2 | Botão **apertado** | LED verde aceso; Serial mostra `0` | |

## Prática 1.2 — Experimento: `INPUT` × `INPUT_PULLUP`

**Roteiro:**

1. No Código 1, troquem `INPUT_PULLUP` por **`INPUT`** e **gravem de novo**.
2. **Soltem o botão** e olhem o Serial **sem tocar em nada**. Anotem o que aconteceu.
3. Voltem para `INPUT_PULLUP` e gravem de novo.

> **Respondam por escrito:** o que aconteceu com o `INPUT` simples? Por que a leitura ficou instável? *(Dica: o pino não está ligado a nada em nível conhecido.)*

## Prática 1.3 — LED vermelho invertido (Código 1B)

O **LED vermelho** (GPIO 22) fica **aceso quando o botão está SOLTO** e apagado quando apertado, junto com o verde, que faz o contrário. *(Cuidado com a lógica invertida!)*

**Roteiro:** colem, gravem e testem o código completo:

```cpp
// Aula 0 - Codigo 1B: LED verde espelha o botao, LED vermelho faz o contrario
#define BOTAO         27
#define LED_VERDE     23
#define LED_VERMELHO  22

void setup() {
  Serial.begin(115200);
  pinMode(BOTAO, INPUT_PULLUP);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
}

void loop() {
  int leitura = digitalRead(BOTAO);

  if (leitura == LOW) {            // apertado
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_VERMELHO, LOW);
  } else {                         // solto
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, HIGH);
  }

  Serial.println(leitura);
  delay(100);
}
```

| # | Teste | Esperado | ✅ |
|---|---|---|---|
| 1 | Botão **solto** | Vermelho **aceso**, verde apagado | |
| 2 | Botão **apertado** | Verde **aceso**, vermelho apagado | |

> **Respondam:** em qual linha o código decide qual LED acender? Por que o `if` testa `== LOW` para "apertado"?

## Extra (se o professor liberar o sensor E18-D80NK)

Trocando o botão pelo **sensor E18-D80NK** (**marrom** → VIN/5 V, **azul** → GND, **preto** → GPIO 27), o **mesmo Código 1** funciona sem mudar nada: o LED acende quando um objeto passa na frente. O parafuso de trás ajusta a distância.

---

# BLOCO 2 — Borda e debounce

> Neste bloco também **não usa Node-RED**.

## O problema: "apertar uma vez" não é "estar apertado"

Queremos que **cada aperto alterne o LED** (liga → desliga → liga...). O loop roda **milhares de vezes por segundo**.

## Prática 2.1 — A versão ingênua (Código 2)

**Roteiro:**

1. Colem e gravem:

```cpp
// Aula 0 - Codigo 2: alterna o LED enquanto o botao esta apertado
#define BOTAO      27
#define LED_VERDE  23

bool ledLigado = false;

void setup() {
  Serial.begin(115200);
  pinMode(BOTAO, INPUT_PULLUP);
  pinMode(LED_VERDE, OUTPUT);
}

void loop() {
  if (digitalRead(BOTAO) == LOW) {     // enquanto estiver apertado...
    ledLigado = !ledLigado;            // ...inverte o LED
    digitalWrite(LED_VERDE, ledLigado);
    Serial.println("alternou");
  }
}
```

2. **Aperte e segure** o botão por 2 segundos, com o Serial aberto.
3. Anotem: quantas vezes apareceu `alternou`? O LED terminou ligado ou desligado? Era previsível?

> **Resultado esperado:** dezenas de `alternou`, e o LED fica piscando rápido ou em estado imprevisível. O código pensa que cada volta do `loop` é um novo aperto.

## A solução: reagir à **mudança**

O que interessa não é "estar apertado", e sim o **momento em que mudou de solto para apertado**. A isso chamamos **borda de descida** (o sinal desce de `HIGH` para `LOW`).

```
leitura:   HIGH ─────┐         ┌───── HIGH
                     └─────────┘
                     ▲         ▲
                  APERTOU    SOLTOU
              (borda de descida)  (borda de subida)
```

Para detectar a mudança, o código precisa **lembrar a leitura anterior** (`estadoAnterior`). *É o mesmo raciocínio do `context.get(...)` na histerese da aula 10: guardar o valor anterior.*

## O tremor do botão (*bounce*)

Um botão mecânico não muda de estado "limpo": os contatos **tremem por alguns milissegundos** e o ESP32, muito rápido, enxerga várias bordas.

```
real:        HIGH ──┐
                    └──────────────────
lido:        HIGH ──┐ ┌┐ ┌─┐
                    └─┘└─┘ └───────────   ← várias "bordas" em poucos ms
                    |<-- ~5 a 20 ms -->|
```

**Debounce:** depois de aceitar uma mudança, **ignoramos novas mudanças por uns 50 ms**, usando `millis()` como nas aulas anteriores (sem `delay`).

## Prática 2.2 — Borda + debounce (Código 3)

**Roteiro:**

1. Colem e gravem (com `DEBOUNCE 0`, para **ver o tremor**):

```cpp
// Aula 0 - Codigo 3: cada APERTO alterna o LED (borda + debounce)
#define BOTAO      27
#define LED_VERDE  23
#define DEBOUNCE   0     // comece com 0 para ver o tremor; depois troque para 50

bool ledLigado = false;
int  estadoAnterior = HIGH;          // lembra a leitura anterior
unsigned long ultimaMudanca = 0;     // quando foi a ultima mudanca aceita

void setup() {
  Serial.begin(115200);
  pinMode(BOTAO, INPUT_PULLUP);
  pinMode(LED_VERDE, OUTPUT);
}

void loop() {
  int leitura = digitalRead(BOTAO);

  // so reage se MUDOU e se passou tempo suficiente desde a ultima mudanca
  if (leitura != estadoAnterior && millis() - ultimaMudanca > DEBOUNCE) {
    ultimaMudanca = millis();
    estadoAnterior = leitura;

    if (leitura == LOW) {            // so na DESCIDA: o momento do aperto
      ledLigado = !ledLigado;
      digitalWrite(LED_VERDE, ledLigado);
      Serial.println(ledLigado ? "LED ligado" : "LED desligado");
    }
  }
}
```

2. Façam o **Experimento** com `DEBOUNCE 0`: **20 apertos normais** e depois **segurar 3 s**. Anotem na tabela.
3. Troquem para **`#define DEBOUNCE 50`**, **gravem de novo** e repitam os mesmos testes.

| Teste | `DEBOUNCE 0` | `DEBOUNCE 50` |
|---|---|---|
| 20 apertos normais | anote | anote |
| Segurar 3 s | anote | anote |

**Esperado:** com `0`, alguns apertos alternam duas vezes ou "não pegam"; com `50`, **cada aperto alterna uma vez**. Segurar o botão alterna **uma vez só**, nos dois casos.

> **Respondam:** (a) por que `estadoAnterior` é atualizado **dentro** do `if` do debounce? (b) o que mudaria se reagíssemos à **borda de subida** (`leitura == HIGH`) em vez da de descida?

## Prática 2.3 — Interruptor de 3 modos

Cada aperto avança um **modo**: `apagado → verde → vermelho → apagado → ...`

**Roteiro:** colem, gravem e testem:

```cpp
// Aula 0 - Exercicio 2: interruptor de 3 modos
#define BOTAO         27
#define LED_VERDE     23
#define LED_VERMELHO  22
#define DEBOUNCE      50

int  modo = 0;                // 0 = apagado, 1 = verde, 2 = vermelho
int  estadoAnterior = HIGH;
unsigned long ultimaMudanca = 0;

void aplicaModo() {
  // acende so o LED do modo atual (0 = nenhum, 1 = verde, 2 = vermelho)
  digitalWrite(LED_VERDE,    modo == 1 ? HIGH : LOW);
  digitalWrite(LED_VERMELHO, modo == 2 ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(BOTAO, INPUT_PULLUP);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  aplicaModo();
}

void loop() {
  int leitura = digitalRead(BOTAO);

  if (leitura != estadoAnterior && millis() - ultimaMudanca > DEBOUNCE) {
    ultimaMudanca = millis();
    estadoAnterior = leitura;

    if (leitura == LOW) {            // so no APERTO (borda de descida)
      modo = (modo + 1) % 3;         // 0 -> 1 -> 2 -> 0 ...
      aplicaModo();
      Serial.print("Modo: ");
      Serial.println(modo);
    }
  }
}
```

| # | Teste | Esperado | ✅ |
|---|---|---|---|
| 1 | Ligar | Tudo apagado (modo 0) | |
| 2 | 1º aperto | Verde | |
| 3 | 2º aperto | Vermelho (verde apaga) | |
| 4 | 3º aperto | Tudo apagado | |
| 5 | Segurar o botão | Muda **uma** vez só | |

> **Respondam:** como o modo "dá a volta" depois do 2 sem um `if` gigante? *(Observem o operador `%`: o resto da divisão por 3 nunca passa de 2.)* Em qual linha está a borda de descida?

---

# BLOCO 3 — Acionamento da esteira

## Ideia

Na aula 9, o ESP32 publicava a cada 5 s (`millis() - ultimoEnvio > 5000`). Isso serve para **medir** uma grandeza que varia devagar, mas **não** para um **evento**: um aperto dura décimos de segundo e se perderia entre duas leituras.

| Tipo | Quando publicar | Exemplo |
|---|---|---|
| **Medição** (aula 9) | Em intervalos fixos | Temperatura |
| **Evento** (hoje) | **Na hora em que muda** | Esteira ligou ou desligou |

Além disso, o dashboard também **comanda** o ESP32 (como os LEDs da aula 10), e o ESP32 **avisa de volta** o novo estado para o painel ficar sempre correto.

Na bancada, o **LED amarelo (GPIO 26)** faz o papel do **motor da esteira**. Em uma esteira real, o mesmo pino aciona uma **ponte H (L298N)**.

```
 botão físico ──┐
                ├─► ESP32 (controlaEsteira) ──► LED amarelo
 dashboard  ────┘          │
   ▲  (mqtt out: ON/OFF)   │ publica o estado: {"esteira":"ON","acionamentos":3}
   └───────── HiveMQ ◄─────┘          (mqtt in → ui_text)
```

## Passo 1 — Gravar o Código 4 (modo esteira)

> Antes de começar este bloco, **confiram o Wi-Fi do ESP32** (nome e senha da rede) e **o número do grupo**: o Bloco 3 usa a internet.

O **Código 4** tem **dois modos**, escolhidos na linha `#define MODO`. Para este bloco: **`#define MODO 1`**.

**Antes de gravar, troquem:**
- `NOME_DA_REDE` e `SENHA_DA_REDE` pelos dados do Wi-Fi;
- `grupo01` pelo **número do seu grupo nas 6 linhas** (`client_id` e os 5 tópicos).

```cpp
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
```

> ⚠️ Repare: **não existe temporizador de 5 s**. A publicação acontece **dentro** do `if` da borda, no instante do aperto.

Depois de gravar, abram o **Serial Monitor**. Esperado: `Wi-Fi OK - IP: ...` e `Conectando ao broker... OK`, seguido de `Publicado: {"esteira":"OFF","acionamentos":0}`.

## Passo 2 — Testar pelo HiveMQ (ainda sem o Node-RED)

Conectem o cliente web (**P5**), com a assinatura em `senai_alagoinhas_2026/aula0/grupo01/#` (seu número) e testem:

| Ação | Esperado | ✅ |
|---|---|---|
| Ligar o ESP32 | Chega `{"esteira":"OFF","acionamentos":0}` | |
| Apertar o botão | LED amarelo acende e chega `{"esteira":"ON","acionamentos":1}` | |
| Apertar de novo | LED amarelo apaga e chega `{"esteira":"OFF","acionamentos":1}` | |
| **Publicar** `ON` em `senai_alagoinhas_2026/aula0/grupo01/esteira` | LED amarelo acende **e** chega `{"esteira":"ON","acionamentos":2}` | |
| **Publicar** `OFF` no mesmo tópico | LED amarelo apaga **e** chega `{"esteira":"OFF","acionamentos":2}` | |

## Passo 3 — Montar o Node-RED: painel da esteira

Agora o painel faz, com botões, o que vocês acabaram de fazer à mão. Usem as **receitas (R1 a R10)**.

**R1:** criem uma aba de fluxo chamada **`Aula 0 - Esteira`**. O resultado deve ficar **assim** (imagem de referência):

![Fluxo da esteira no Node-RED](imagens/fluxo_esteira.png)

```
[mqtt in estado] ─┬──► [debug]
                  ├──► [ui_text Esteira]
                  └──► [ui_text Acionamentos]

[ui_button Ligar esteira]    ─┐
[ui_button Desligar esteira] ─┴─► [mqtt out esteira]
```

| # | Nó | Configuração |
|---|---|---|
| 1 | **mqtt in** (R3) | Nome `estado do ESP32` · Tópico `senai_alagoinhas_2026/aula0/grupo01/estado` · QoS `0` · Saída **objeto JSON**. Na primeira vez, crie o servidor **R3.1** |
| 2 | **debug** (R8) | Ligado à saída do `mqtt in` |
| 3 | **text** (R6) | Group **`Esteira`** (aba `Aula 0`, **R5.1**) · Label `Esteira` · Value format `{{msg.payload.esteira}}` |
| 4 | **text** (R6) | Mesmo grupo · Label `Acionamentos` · Value format `{{msg.payload.acionamentos}}` |
| 5 | **button** (R5) | Group `Esteira` · Label `Ligar esteira` · **Payload (string)** `ON` |
| 6 | **button** (R5) | Group `Esteira` · Label `Desligar esteira` · **Payload (string)** `OFF` |
| 7 | **mqtt out** (R4) | Nome `esteira` · Tópico `senai_alagoinhas_2026/aula0/grupo01/esteira` · QoS `0` · Reter **falso**. **Os dois botões ligam neste mesmo nó** |

Ligações: `mqtt in` → `debug`, `text Esteira`, `text Acionamentos`. `button Ligar` → `mqtt out`. `button Desligar` → `mqtt out`.

**R10:** cliquem em **implementar** e abram **http://localhost:1880/ui**.

> **Novidade:** o **ui_button** envia um `msg.payload` fixo toda vez que é clicado. O campo **Payload** precisa ser do tipo **texto** (*string*).

## Passo 4 — Testes do painel

| # | Teste | Esperado | ✅ |
|---|---|---|---|
| 1 | Implementar o fluxo e ligar o ESP32 | Dashboard: `Esteira: OFF`, `Acionamentos: 0`, LED amarelo apagado | |
| 2 | Apertar o botão **físico** | LED amarelo acende e o texto muda para `ON` | |
| 3 | Apertar de novo | LED amarelo apaga e o texto volta para `OFF` | |
| 4 | Clicar **Ligar esteira** no dashboard | LED amarelo acende e o texto muda para `ON` | |
| 5 | Apertar o botão físico depois | LED apaga (alterna **a partir do estado real**) | |
| 6 | Segurar o botão físico | Muda **uma** vez só | |
| 7 | Ligar a esteira pelo botão **e** pelo dashboard | `Acionamentos` soma 1 a cada vez que a esteira é **ligada** (desligar não soma) | |

## Para entender o código (respondam por escrito)

- A esteira muda de estado em **dois lugares** (o botão físico e o comando do dashboard), mas a soma de `acionamentos` está em **um só**: dentro de `controlaEsteira`. Por que esse é o melhor lugar?
- Como `if (ligar && !esteiraLigada)` evita somar quando a esteira já estava ligada, ou quando ela é desligada?
- A variável `acionamentos` é global. O que aconteceria se ela fosse declarada **dentro** da função?
- Entre o clique no dashboard e o LED acender, o que existe **no caminho**? *(Navegador → Node-RED → HiveMQ → Wi-Fi → ESP32 → pino.)* E se o Wi-Fi cair com a esteira ligada, o que acontece?

## Extra (com a esteira real, se o professor liberar)

O **mesmo Código 4 (`MODO 1`)** comanda uma esteira real sem mudar nada: o GPIO 26 aciona o **IN1** de uma ponte H L298N e o GPIO 25 (sempre `LOW`) o **IN2**. **Só o professor mexe na fiação da esteira.**

---

# BLOCO 4 — Node-RED com memória: alarme de porta

## Ideia

No Bloco 3 o Node-RED só repassava mensagens. Agora ele precisa **lembrar** em que **estado** o sistema está, porque a resposta a um evento **depende do estado**:

> *Porta abrir com o alarme **armado** é invasão. Porta abrir com o alarme **desarmado** é só uma porta abrindo.*

| Estado | Verde | Vermelho | Texto |
|---|---|---|---|
| **DESARMADO** | apagado | apagado | `Desarmado` |
| **ARMADO** | **aceso** | apagado | `Armado - vigiando` |
| **DISPARADO** | apagado | **aceso** | `ALARME: porta aberta!` |

```
DESARMADO ──Armar──► ARMADO ──porta ABERTA──► DISPARADO
    ▲                   │                         │
    └────Desarmar───────┴───────Desarmar──────────┘
```

Repare: **depois de disparado, o alarme continua disparado mesmo que a porta feche.** Só o botão **Desarmar** limpa. Quem guarda isso é o `context`.

## Passo 1 — Regravar o Código 4 no modo alarme

No **Código 4** do Bloco 3, mudem **só a primeira linha de configuração**:

```cpp
#define MODO 2
```

e **gravem de novo** (os tópicos com o seu número de grupo continuam os mesmos). Agora o botão simula o **sensor da porta**: **apertado = ABERTA**, **solto = FECHADA**, e o ESP32 publica **as duas bordas**, a de descida **e** a de subida.

Teste rápido no HiveMQ (**P5**): ao apertar o botão chega `{"porta":"ABERTA"}`; ao soltar, `{"porta":"FECHADA"}`.

## Passo 2 — Montar o Node-RED: alarme de porta

**R1:** criem uma aba de fluxo chamada **`Aula 0 - Alarme`**. O resultado deve ficar **assim** (imagem de referência):

![Fluxo do alarme no Node-RED](imagens/fluxo_alarme.png)

```
[mqtt in porta] ──────┐
[ui_button Armar]    ─┼─► [regra do alarme] ─┬─ 1 ─► [mqtt out led_verde]
[ui_button Desarmar] ─┘                      ├─ 2 ─► [mqtt out led_vermelho]
                                             └─ 3 ─► [ui_text Estado]
```

| # | Nó | Configuração |
|---|---|---|
| 1 | **mqtt in** (R3) | Nome `porta do ESP32` · Tópico `senai_alagoinhas_2026/aula0/grupo01/porta` · QoS `0` · Saída **objeto JSON** |
| 2 | **debug** (R8) | Ligado à saída do `mqtt in` (para ver `ABERTA`/`FECHADA`) |
| 3 | **button** (R5) | Group **`Alarme de porta`** (novo grupo, na aba `Aula 0`, **R5.1**) · Label `Armar` · **Payload (string)** `ARMAR` · **Topic** `botao` |
| 4 | **button** (R5) | Group `Alarme de porta` · Label `Desarmar` · **Payload (string)** `DESARMAR` · **Topic** `botao` |
| 5 | **function** (R7) | Nome `regra do alarme` · **Saídas: 3** · código abaixo |
| 6 | **mqtt out** (R4) | Nome `LED verde` · Tópico `senai_alagoinhas_2026/aula0/grupo01/led_verde` · QoS `0` · Reter **falso** |
| 7 | **mqtt out** (R4) | Nome `LED vermelho` · Tópico `senai_alagoinhas_2026/aula0/grupo01/led_vermelho` · QoS `0` · Reter **falso** |
| 8 | **text** (R6) | Group `Alarme de porta` · Label `Estado` · Value format `{{msg.payload}}` |

**Ligações:** `mqtt in` → `debug` **e** `regra do alarme`. `Armar` → `regra do alarme`. `Desarmar` → `regra do alarme`. Saída **1** da função → `LED verde`. Saída **2** → `LED vermelho`. Saída **3** → `Estado`.

> ⚠️ O **Topic `botao`** dos dois botões é o que a função usa para saber que a mensagem veio de um **botão** e não do ESP32. Se esquecerem, o alarme não arma.

### Código da função "regra do alarme" (cole na aba *Na mensagem*)

```javascript
// ===== ALARME DE PORTA =====
// O que a regra lembra entre uma mensagem e outra:
var estado = context.get("estado") || "DESARMADO";   // DESARMADO, ARMADO ou DISPARADO

// --- 1) O que chegou? ---
if (msg.topic === "botao") {
    // veio de um botao do dashboard
    if (msg.payload === "ARMAR" && estado === "DESARMADO") {
        estado = "ARMADO";
    } else if (msg.payload === "DESARMAR") {
        estado = "DESARMADO";
    }
} else {
    // veio o JSON do ESP32: {"porta":"ABERTA"} ou {"porta":"FECHADA"}
    if (msg.payload && msg.payload.porta === "ABERTA" && estado === "ARMADO") {
        estado = "DISPARADO";
    }
}

context.set("estado", estado);   // guarda o estado para a proxima mensagem

// --- 2) Monta as saidas de acordo com o ESTADO ---
var verde = { payload: estado === "ARMADO" ? "ON" : "OFF" };

var vermelho = { payload: estado === "DISPARADO" ? "ON" : "OFF" };

var textos = {
    DESARMADO: "Desarmado",
    ARMADO:    "Armado - vigiando",
    DISPARADO: "ALARME: porta aberta!"
};
var texto = { payload: textos[estado] };

return [verde, vermelho, texto];
```

**R10:** **implementar**, abram o dashboard (**http://localhost:1880/ui**) e **cliquem em Desarmar** para sincronizar os LEDs.

## Passo 3 — Testes do alarme

| # | Teste | Esperado | ✅ |
|---|---|---|---|
| 1 | Implementar e clicar **Desarmar** | Texto `Desarmado`, LEDs apagados | |
| 2 | Apertar o botão físico (porta abre) | **Nada** acontece | |
| 3 | Clicar **Armar** | Verde aceso, texto `Armado - vigiando` | |
| 4 | Apertar o botão físico | Verde apaga, **vermelho acende**, texto `ALARME: porta aberta!` | |
| 5 | Soltar o botão (porta fecha) | **Continua** disparado | |
| 6 | Clicar **Armar** com o alarme disparado | Nada acontece (só `Desarmar` limpa) | |
| 7 | Clicar **Desarmar** | Tudo apagado, texto `Desarmado` | |

> ℹ️ Ao **implementar** o fluxo, o `context` é zerado e o estado volta a `DESARMADO`. Clique em **Desarmar** para sincronizar os LEDs.

## Perguntas-guia (respondam por escrito)

- Por que a função testa `msg.topic === "botao"`? *(O que a função recebe nas duas entradas?)*
- O que aconteceria sem o `context.get/set`? Em que estado estaria a regra a cada mensagem?
- Por que `ARMAR` só vale se o estado for `DESARMADO`? O que mudaria se retirássemos essa condição?
- Por que a mensagem `FECHADA` não muda o estado? Quem decide que o alarme continua disparado?
- O objeto `textos` evita três `if`. Como ele funciona?

---

# FECHAMENTO

O grupo responde **por escrito** (todos precisam conseguir explicar):

1. O que é a **lógica invertida** e por que o botão serve para treinar o sensor NPN?
2. Por que "o botão está apertado" **não** é a mesma coisa que "o botão foi apertado"? Como o código percebe a diferença?
3. Para que serve o **debounce**? O que acontece sem ele?
4. Quando publicar **a cada X segundos** e quando publicar **na hora que muda**?
5. Para que serve o `context` no Node-RED? Dê um exemplo.

### Autoavaliação

Marquem o que o grupo consegue fazer **sem ajuda**:

- [ ] Ler um botão ou sensor NPN com `INPUT_PULLUP`
- [ ] Reagir a um acionamento **uma única vez**, com borda e debounce
- [ ] Publicar um JSON **quando algo muda**
- [ ] Comandar uma saída pelo dashboard com `ui_button` e `mqtt out`
- [ ] Guardar um estado no Node-RED com `context` e agir de acordo com ele
- [ ] Receber um **comando de texto** (como `ZERAR`) no `callback` e zerar uma variável *(Bloco 5)*
- [ ] Contar **só quando uma condição permite** (ex.: sala aberta) *(Bloco 5)*
- [ ] Comparar um **número** recebido com um limite no Node-RED e mudar o estado *(Bloco 5)*
- [ ] Mostrar um valor no dashboard com **ui_gauge** *(Bloco 5)*

---

# BLOCO 5 — Anexo A: controle de lotação de uma sala (tarefa de casa ou sobra de tempo)

Este bloco fecha **quatro técnicas** que ainda não apareceram: **comando de texto** no ESP32, **contar só com uma condição**, **comparar um número** no Node-RED e o **gauge**. Cenário: o **controle de lotação de uma sala**.

- O **botão** é a catraca: cada aperto é uma pessoa que entra.
- A sala **abre** pelo dashboard e só conta **enquanto estiver aberta**.
- Ao chegar a **3 pessoas**, a sala fica **LOTADA** (LED vermelho aceso) e o botão **Zerar** recomeça tudo.

Usa o **mesmo circuito** da bancada. O LED **verde** mostra "sala aberta"; o **vermelho**, "lotada".

### Tópicos (prefixo `senai_alagoinhas_2026/aula0/grupoXX/`)

| Tópico | Quem publica | Conteúdo |
|---|---|---|
| `visitas` | ESP32 | `{"visitas":2}` |
| `sala` | Node-RED | `ON` (aberta) ou `OFF` (fechada) |
| `comando` | Node-RED | `ZERAR` |
| `led_vermelho` | Node-RED | `ON` / `OFF` |

### Estados

| Estado | `sala` | LED vermelho | Texto |
|---|---|---|---|
| FECHADA | `OFF` | `OFF` | `Fechada` |
| ABERTA | `ON` | `OFF` | `Aberta: X de 3` |
| LOTADA | `OFF` | `ON` | `LOTADA` |

## Passo 1 — Gravar o sketch completo no ESP32

Sketch **inteiro** (é um programa novo; não mexe no Código 4). Troquem o Wi-Fi e o `grupo01` pelo seu número (**5 linhas**: `client_id` e 4 tópicos).

```cpp
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
```

**O que cada técnica faz neste código:**

| Técnica | Onde está |
|---|---|
| **1. Comando de texto** | No `callback`, `if (t == topic_comando)` trata `ZERAR` **antes** do filtro `ON`/`OFF`, que descartaria o texto |
| **2. Contar só com condição** | Em `verificaVisita`, `if (leitura == LOW && salaAberta)`. O `estadoAnterior` é atualizado **sempre**, para não "contar atrasado" |

Esperado no Serial: `Conectando ao broker... OK` e `Publicado: {"visitas":0}`.

## Passo 2 — Montar o Node-RED: controle de lotação

**R1:** criem uma aba de fluxo chamada **`Aula 0 - Anexo A (Sala)`**. O resultado deve ficar **assim** (imagem de referência):

![Fluxo da sala no Node-RED](imagens/fluxo_sala.png)

```
[mqtt in visitas]  ──┬──► [debug]
[ui_button Abrir]  ──┼─► [regra da sala] ─┬─ 1 ─► [mqtt out sala]
[ui_button Zerar]  ──┤                    ├─ 2 ─► [mqtt out led_vermelho]
                     │                    ├─ 3 ─► [ui_text Estado]
                     │                    └─ 4 ─► [ui_gauge Visitas]
[ui_button Zerar]  ──┴─► [mqtt out comando]
```

| # | Nó | Configuração |
|---|---|---|
| 1 | **mqtt in** (R3) | Nome `visitas do ESP32` · Tópico `.../grupo01/visitas` · QoS `0` · Saída **objeto JSON** |
| 2 | **debug** (R8) | Ligado à saída do `mqtt in` |
| 3 | **button** (R5) | Group **`Sala (Anexo A)`** (novo grupo, aba `Aula 0`) · Label `Abrir sala` · Payload (string) `ABRIR` · **Topic** `botao` |
| 4 | **button** (R5) | Group `Sala (Anexo A)` · Label `Zerar` · Payload (string) `ZERAR` · **Topic** `botao` |
| 5 | **function** (R7) | Nome `regra da sala` · **Saídas: 4** · código abaixo |
| 6 | **mqtt out** (R4) | Nome `sala` · Tópico `.../grupo01/sala` · QoS `0` · Reter **falso** |
| 7 | **mqtt out** (R4) | Nome `LED vermelho` · Tópico `.../grupo01/led_vermelho` · QoS `0` · Reter **falso** |
| 8 | **text** (R6) | Group `Sala (Anexo A)` · Label `Estado` · Value format `{{msg.payload}}` |
| 9 | **gauge** (R9) | Group `Sala (Anexo A)` · Label `Visitas` · Range `0` a `3` · Value format `{{value}}` |
| 10 | **mqtt out** (R4) | Nome `comando` · Tópico `.../grupo01/comando` · QoS `0` · Reter **falso** |

(`...` = `senai_alagoinhas_2026/aula0`.)

**Ligações:**
- `mqtt in` → `debug` **e** `regra da sala`.
- `Abrir sala` → `regra da sala`.
- `Zerar` → `regra da sala` **e também** → `mqtt out comando` (**duas** ligações saindo do mesmo botão).
- Saída **1** da função → `sala`; saída **2** → `LED vermelho`; saída **3** → `Estado`; saída **4** → `gauge`.

> O botão **Zerar** tem **duas** saídas: uma vai para a função (que zera o estado) e outra direto para o `mqtt out comando` (que zera o ESP32).

### Código da função "regra da sala" (aba *Na mensagem*)

```javascript
var LOTACAO = 3;
var estado = context.get("estado") || "FECHADA";
var visitas = context.get("visitas") || 0;

if (msg.topic === "botao") {
    if (msg.payload === "ABRIR" && estado === "FECHADA") {
        estado = "ABERTA";
    } else if (msg.payload === "ZERAR") {
        estado = "FECHADA";
        visitas = 0;
    }
} else if (msg.payload && msg.payload.visitas !== undefined) {
    visitas = msg.payload.visitas;
    if (estado === "ABERTA" && visitas >= LOTACAO) {
        estado = "LOTADA";
    }
}

context.set("estado", estado);
context.set("visitas", visitas);

var sala     = { payload: estado === "ABERTA" ? "ON" : "OFF" };
var vermelho = { payload: estado === "LOTADA" ? "ON" : "OFF" };
var texto    = { payload: estado === "FECHADA" ? "Fechada"
                        : estado === "ABERTA"  ? "Aberta: " + visitas + " de " + LOTACAO
                        : "LOTADA" };
var medidor  = { payload: visitas };

return [sala, vermelho, texto, medidor];
```

**Técnica 3:** o estado muda quando um **número** (`visitas`) atinge um limite (`>= LOTACAO`). **Técnica 4:** a **saída 4** manda o número para o `gauge`.

**R10:** **implementar** e abrir o dashboard.

## Passo 3 — Testes do Anexo A

| # | Ação | Esperado | ✅ |
|---|---|---|---|
| 1 | Apertar o botão com a sala **fechada** | Nada conta | |
| 2 | Clicar **Abrir sala** | LED verde aceso; texto `Aberta: 0 de 3` | |
| 3 | Apertar o botão 2 vezes | Texto `Aberta: 2 de 3`, medidor em 2 | |
| 4 | Apertar a 3ª vez | LED verde apaga, vermelho acende, `LOTADA` | |
| 5 | Apertar de novo | Não conta mais (a sala foi fechada pelo Node-RED) | |
| 6 | Clicar **Zerar** | Medidor em 0, tudo apagado, `Fechada` | |
| 7 | **Abrir sala** de novo | O ciclo recomeça do zero | |

> ⚠️ Os **dois lados** precisam concordar: o ESP32 só conta com a sala aberta, e o Node-RED manda `sala OFF` ao lotar. Se o ESP32 reiniciar no meio, a contagem volta a 0 e o Node-RED ainda guarda o número antigo. O **Zerar** sincroniza os dois.

**Respondam:** (a) por que `estadoAnterior` é atualizado mesmo com a sala fechada? (b) o que acontece com a contagem se o ESP32 reiniciar? (c) por que o `ZERAR` precisa chegar **aos dois lados**?

---

# PROBLEMAS COMUNS

| Sintoma | Causa provável | Solução |
|---|---|---|
| Não grava (`Connecting....`) | Placa não entrou em modo de gravação | Segurar o botão **BOOT** durante o Upload; conferir porta e cabo |
| Leitura do botão oscila sozinha | Pino sem `INPUT_PULLUP` | Conferir o `pinMode` |
| Serial mostra **sempre** `0` | Botão girado 90° (contatos sempre ligados) | Girar o botão ou mudar de colunas |
| LED alterna várias vezes ao segurar | Falta a borda (comparar com `estadoAnterior`) | Rever o Código 3 |
| Aperto "não pega" ou alterna duas vezes | Tremor do botão | Usar `DEBOUNCE 50` |
| LED não acende | Polaridade invertida ou sem resistor/GND | Conferir perna longa e GND |
| `Conectando ao Wi-Fi...` sem fim | Nome ou senha da rede errados | Conferir `ssid` e `password` |
| `Conectando ao broker... falhou, codigo = -2` | Broker inacessível (rede ou porta 1883 bloqueada) | Testar o HiveMQ pelo celular na mesma rede |
| ESP32 conecta e desconecta toda hora | `client_id` repetido entre grupos | Cada grupo com seu `client_id` (número do grupo) |
| Nada chega no Node-RED ou no HiveMQ | Número do grupo diferente no ESP32 e no Node-RED | Conferir **letra por letra** os tópicos (**Ctrl+F** no Node-RED) |
| Nó mqtt *desconectado* | Sem internet ou servidor errado | Conferir `broker.hivemq.com` e porta `1883` |
| Nada aparece no debug | Esqueceu de **implementar** | Clicar em **implementar** |
| Texto do dashboard vazio ou `{{msg.payload.esteira}}` | `mqtt in` com saída texto | Trocar a saída para **objeto JSON analisado** |
| Botão do dashboard não faz nada | **Payload** vazio ou do tipo errado; botão sem ligação | Abrir o botão e conferir (string) e a ligação com o `mqtt out` |
| Dashboard em branco | Widget com Group *nenhum* | Escolher o grupo no widget |
| Alarme não arma | Botão sem **Topic `botao`**; payload com letras diferentes | Conferir `ARMAR`/`DESARMAR` e o topic |
| Função com erro vermelho | Nº de **saídas** da função diferente do indicado | Aba **Configurar** do nó function |
| Alarme não dispara | Fluxo reimplementado (estado voltou a `DESARMADO`) | Clicar **Armar** de novo |
| Esteira liga sozinha ao reiniciar o ESP32 | **Reter** verdadeiro em algum `mqtt out` | Trocar para **falso** |
| Mudou algo e nada mudou | Não clicou em **implementar** | Clicar em **implementar** |

---

# COLA RÁPIDA

| Item | Valor |
|---|---|
| Botão (ou sensor) | GPIO **27**, `INPUT_PULLUP`, ativo = `LOW` |
| LED verde / vermelho | GPIO **23** / GPIO **22** |
| Esteira (LED amarelo na bancada) | GPIO **26** (IN1) · GPIO **25** (IN2, sempre `LOW`) |
| Tópicos (blocos 3 e 4) | `senai_alagoinhas_2026/aula0/grupoXX/` + `estado`, `esteira`, `porta`, `led_verde`, `led_vermelho` |
| Tópicos (bloco 5) | mesmo prefixo + `visitas`, `sala`, `comando`, `led_vermelho` |
| Borda de descida | `leitura != estadoAnterior` **e** `leitura == LOW` |
| Debounce | `millis() - ultimaMudanca > 50` |
| Publicar evento | dentro do `if` da borda (não a cada X segundos) |
| Memória no Node-RED | `context.get("x")` / `context.set("x", valor)` |
| Comparar número (Node-RED) | `if (msg.payload.campo >= LIMITE)` |
| Comando de texto no ESP32 | tratar `if (t == topic_comando)` **antes** do filtro ON/OFF |
| Receber do ESP32 | `mqtt in` (Saída: objeto JSON, QoS 0) |
| Mandar ao ESP32 | `mqtt out` (Reter: falso, QoS 0) |
| Botão do dashboard | `button` (Payload **string**, Topic `botao` quando a regra pede) |
| Depois de mexer no Node-RED | **implementar** |
| Broker | `broker.hivemq.com` : `1883` |
| Dashboard | `http://localhost:1880/ui` |
