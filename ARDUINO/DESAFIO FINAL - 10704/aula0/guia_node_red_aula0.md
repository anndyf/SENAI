# Guia do Node-RED — Aula 0

**SENAI Alagoinhas · Sistemas Eletrônicos e Microcontrolados**

Este guia mostra, **passo a passo e com prints**, como montar no Node-RED os painéis da **Aula 0** (esteira e alarme de porta). Guarde este guia como referência para as próximas aulas.

> **Atalho:** o arquivo `fluxo_aula0_node_red.json` traz os dois fluxos prontos. Para importar, vá ao **Passo 6**. Mas **monte uma vez à mão** (Passos 2 a 5): é assim que você aprende o que cada nó faz.

Os prints foram tirados do Node-RED 5.0 com `node-red-dashboard` 3.6, que é o que usamos na aula 10.

---

## 0. A ideia em uma figura

```
ESP32 ──publica──► HiveMQ ──► [mqtt in] ──► [nós de tela]      (o que o ESP32 conta)
ESP32 ◄─assina──── HiveMQ ◄── [mqtt out] ◄── [botões / regra]  (o que o Node-RED manda)
```

| Nó | Para que serve |
|---|---|
| **mqtt in** | **Recebe** mensagens do ESP32 |
| **mqtt out** | **Envia** comandos para o ESP32 |
| **ui_button** | Botão no dashboard |
| **ui_text** | Texto no dashboard |
| **function** | Onde mora a **regra** (a decisão) |
| **debug** | Mostra a mensagem na barra lateral, para você **enxergar** o que chegou |

**Regra de ouro dos tópicos:** o que o ESP32 **publica**, o Node-RED **assina** (mqtt in). O que o ESP32 **assina**, o Node-RED **publica** (mqtt out). O texto do tópico precisa ser **idêntico** nos dois lados.

---

## 1. Preparação (uma vez só)

### 1.1 Abrir o Node-RED

Abra o `cmd`, rode `node-red` e acesse **http://localhost:1880**. Você deve ver o editor assim:

![Editor do Node-RED com o fluxo da esteira](prints_node_red/01_editor_esteira.png)

- **Esquerda:** paleta de nós (e a lista de fluxos).
- **Centro:** área de trabalho (cada aba é um **fluxo**).
- **Direita:** informações e **mensagens de depuração**.
- **Botão vermelho "implementar" (Deploy):** **só depois de clicar nele** o que você montou passa a valer.

### 1.2 Conferir se o dashboard está instalado

Menu (☰ no canto superior direito) → **Gerenciar paleta**.

![Menu do Node-RED](prints_node_red/13_menu.png)

Na aba **Nós**, procure por `node-red-dashboard`. Se não estiver, vá em **Instalar**, pesquise `node-red-dashboard` e instale.

![Gerenciar paleta, aba Instalar](prints_node_red/13_paleta.png)

Se estiver instalado, a paleta passa a ter a seção **dashboard** (button, text, gauge…).

---

## 2. Dados que cada grupo troca (anote o seu número)

Troque `grupo01` pelo **número do seu grupo** em **todos** os tópicos. Eles precisam ser **os mesmos do ESP32**.

| Tópico | Quem publica | Conteúdo |
|---|---|---|
| `senai_alagoinhas_2026/aula0/grupo01/estado` | ESP32 | `{"esteira":"ON","acionamentos":3}` |
| `senai_alagoinhas_2026/aula0/grupo01/esteira` | Node-RED | `ON` ou `OFF` |
| `senai_alagoinhas_2026/aula0/grupo01/porta` | ESP32 | `{"porta":"ABERTA"}` ou `{"porta":"FECHADA"}` |
| `senai_alagoinhas_2026/aula0/grupo01/led_verde` | Node-RED | `ON` ou `OFF` |
| `senai_alagoinhas_2026/aula0/grupo01/led_vermelho` | Node-RED | `ON` ou `OFF` |

---

## 3. Painel da esteira (Fluxo 1)

Crie uma aba de fluxo: **+** (ao lado das abas) e dê o nome `Aula 0 - Esteira` (duplo clique na aba).

O fluxo final fica assim (veja o print do Passo 1.1):

```
[mqtt in estado] ──┬─► [debug]
                   ├─► [ui_text Esteira]
                   └─► [ui_text Acionamentos]

[ui_button Ligar esteira]  ──┐
                             ├─► [mqtt out esteira]
[ui_button Desligar esteira]─┘
```

### 3.1 Receber o estado do ESP32 — nó **mqtt in**

Arraste um **mqtt in** (seção *rede*) para a área de trabalho e dê **duplo clique**.

![Configuração do mqtt in](prints_node_red/02_mqtt_in.png)

| Campo | Valor |
|---|---|
| **Servidor** | `HiveMQ publico` (veja 3.2 para criar) |
| **Ação** | Assinar um tópico único |
| **Tópico** | `senai_alagoinhas_2026/aula0/grupo01/estado` |
| **QoS** | `0` |
| **Saída** | **um objeto JSON analisado sintaticamente** ← importante! |
| **Nome** | `estado do ESP32` |

> Com "objeto JSON analisado", o ESP32 manda `{"esteira":"ON","acionamentos":3}` e você acessa `msg.payload.esteira` e `msg.payload.acionamentos`. Com a opção errada, vem um texto e os campos não funcionam.

### 3.2 Criar o servidor (broker) — **só na primeira vez**

No campo **Servidor**, clique no **lápis** (ou no **+** para criar um novo).

![Configuração do broker HiveMQ](prints_node_red/06_broker.png)

| Campo | Valor |
|---|---|
| **Nome** | `HiveMQ publico` |
| **Servidor** | `broker.hivemq.com` |
| **Porta** | `1883` |
| **Usar TLS** | desmarcado |
| **Protocolo** | MQTT V3.1.1 |
| **ID do cliente** | **em branco** (o Node-RED gera um) |

Clique em **Atualizar** (ou **Adicionar**) e depois em **Feito**. Todos os outros nós mqtt **reaproveitam** este servidor.

> ⚠️ O **ID do cliente** do Node-RED **nunca** pode ser igual ao do ESP32 (`client_id` do código). Deixando em branco, isso não acontece.

### 3.3 Mostrar o estado — nós **ui_text**

Arraste um **text** (seção *dashboard*) e configure:

![Configuração do ui_text](prints_node_red/03_ui_text.png)

| Campo | Valor |
|---|---|
| **Group** | `[Aula 0] Esteira` (clique no **+**, crie a aba `Aula 0` e o grupo `Esteira`) |
| **Label** | `Esteira` |
| **Value format** | `{{msg.payload.esteira}}` |

Repita para o segundo texto: **Label** `Acionamentos`, **Value format** `{{msg.payload.acionamentos}}`.

Ligue a saída do **mqtt in** nos dois **text**. Para também ver a mensagem crua, ligue um **debug** (seção *comum*).

### 3.4 Botões de comando — **ui_button** + **mqtt out**

Arraste dois **button**.

![Configuração do ui_button](prints_node_red/04_ui_button.png)

| Campo | Botão 1 | Botão 2 |
|---|---|---|
| **Group** | `[Aula 0] Esteira` | `[Aula 0] Esteira` |
| **Label** | `Ligar esteira` | `Desligar esteira` |
| **Payload** | tipo **string**: `ON` | tipo **string**: `OFF` |
| **Topic** | (deixe como está) | (deixe como está) |

Depois arraste um **mqtt out** e ligue os **dois** botões nele.

![Configuração do mqtt out](prints_node_red/05_mqtt_out.png)

| Campo | Valor |
|---|---|
| **Servidor** | `HiveMQ publico` |
| **Tópico** | `senai_alagoinhas_2026/aula0/grupo01/esteira` |
| **QoS** | `0` |
| **Reter** | **falso** |
| **Nome** | `esteira` |

> **Reter = falso:** com `true`, o broker guarda o último comando e **reenvia** quando o ESP32 reconectar. A esteira poderia ligar sozinha ao ligar a placa.

### 3.5 Implementar e testar

1. Clique no botão vermelho **implementar**.
2. O texto **conectado** (bolinha verde) deve aparecer sob os nós mqtt. Se aparecer *desconectado*, confira o servidor e a internet.
3. Com o ESP32 ligado e conectado, abra o dashboard (**Passo 5**).

Para testar **sem** o ESP32, use o **MQTT Web Client do HiveMQ** (já usado nas aulas 9 e 10) e publique em `.../grupo01/estado`:

```
{"esteira":"ON","acionamentos":3}
```

A barra de depuração mostra o objeto recebido:

![Barra de depuração com a mensagem recebida](prints_node_red/07_debug.png)

---

## 4. Painel do alarme de porta (Fluxo 2)

Crie outra aba de fluxo: `Aula 0 - Alarme`.

![Fluxo do alarme de porta](prints_node_red/08_editor_alarme.png)

```
[mqtt in porta]      ──┬─► [debug]
[ui_button Armar]    ──┼─► [function regra do alarme] ─┬─ saída 1 ─► [mqtt out led_verde]
[ui_button Desarmar] ──┘                               ├─ saída 2 ─► [mqtt out led_vermelho]
                                                       └─ saída 3 ─► [ui_text Estado]
```

Aqui **três fontes** (porta + dois botões) entram na **mesma função**. A função decide o estado e comanda tudo.

### 4.1 Nós de entrada

| Nó | Configuração |
|---|---|
| **mqtt in** | Tópico `.../grupo01/porta`, QoS 0, saída **objeto JSON** |
| **ui_button** "Armar" | Group `[Aula 0] Alarme de porta`, Label `Armar`, **Payload** (string) `ARMAR`, **Topic** `botao` |
| **ui_button** "Desarmar" | Group `[Aula 0] Alarme de porta`, Label `Desarmar`, **Payload** (string) `DESARMAR`, **Topic** `botao` |

![ui_button Armar](prints_node_red/10_ui_button_armar.png)

> ⚠️ O **Topic `botao`** é o que a função usa para saber que a mensagem veio de um **botão** e não do ESP32. Se esquecer, o alarme não arma.

### 4.2 A função "regra do alarme"

Arraste um nó **function**, dê o nome `regra do alarme`, e na aba **Configurar** coloque **Saídas: 3**. Na aba **Na mensagem**, cole:

![Nó function com a regra](prints_node_red/09_function.png)

```javascript
// ===== ALARME DE PORTA =====
// O que a regra lembra entre uma mensagem e outra:
var estado = context.get("estado") || "DESARMADO";   // estado atual

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
    ARMADO: "Armado - vigiando",
    DISPARADO: "ALARME: porta aberta!"
};
var texto = { payload: textos[estado] };

return [verde, vermelho, texto];   // saida 1, saida 2, saida 3
```

Como a função pensa:

| Estado | Entra por | Vai para |
|---|---|---|
| DESARMADO | botão **ARMAR** | ARMADO |
| ARMADO | porta **ABERTA** | DISPARADO |
| qualquer um | botão **DESARMAR** | DESARMADO |
| DISPARADO | porta fechando ou **ARMAR** | **continua** DISPARADO (só o DESARMAR sai) |

### 4.3 Nós de saída

| Nó | Configuração |
|---|---|
| **mqtt out** `LED verde` | Tópico `.../grupo01/led_verde`, QoS 0, Reter **falso** ← **saída 1** |
| **mqtt out** `LED vermelho` | Tópico `.../grupo01/led_vermelho`, QoS 0, Reter **falso** ← **saída 2** |
| **ui_text** `Estado` | Group `[Aula 0] Alarme de porta`, Label `Estado`, Value format `{{msg.payload}}` ← **saída 3** |

![mqtt out do LED verde](prints_node_red/11_mqtt_out_verde.png)

**Implemente** (botão vermelho).

### 4.4 Roteiro de testes (sem ESP32 também funciona)

Para simular a porta, publique no HiveMQ Web Client em `.../grupo01/porta`.

| # | Ação | Esperado |
|---|---|---|
| 1 | Clicar **Desarmar** | Verde e vermelho `OFF`; texto `Desarmado` |
| 2 | Porta **ABERTA** (desarmado) | Nada acontece |
| 3 | Clicar **Armar** | LED verde `ON`; texto `Armado - vigiando` |
| 4 | Porta **ABERTA** (armado) | Verde `OFF`, vermelho `ON`; `ALARME: porta aberta!` |
| 5 | Porta **FECHADA** | Continua em alarme |
| 6 | Clicar **Armar** de novo | Continua em alarme |
| 7 | Clicar **Desarmar** | Tudo `OFF`; `Desarmado` |

> Estes 7 passos foram conferidos num Node-RED real antes da aula. Se o seu falhar em algum, o erro está na configuração (topic `botao`, saídas da função ou tópicos MQTT).

---

## 5. O dashboard

Abra **http://localhost:1880/ui**. As duas partes aparecem na aba **Aula 0**.

Alarme **desarmado**:

![Dashboard desarmado](prints_node_red/14_dashboard_desarmado.png)

Alarme **armado**:

![Dashboard armado](prints_node_red/15_dashboard_armado.png)

Porta aberta com o alarme armado, **disparado**:

![Dashboard disparado](prints_node_red/16_dashboard_disparado.png)

> Para abrir o dashboard no celular, use o IP do computador: `http://IP-DO-PC:1880/ui` (mesma rede Wi-Fi).

---

## 6. Importar o fluxo pronto (atalho e plano B)

Se travou ou quer comparar com o seu:

1. Menu ☰ → **Importar** (ou `Ctrl+I`).
2. Clique em **selecione um arquivo para importar** e escolha `fluxo_aula0_node_red.json` (ou cole o conteúdo).
3. Escolha **novo fluxo** e clique em **Importar**.
4. **Troque `grupo01` pelo seu número** em todos os nós mqtt (duplo clique em cada um).
5. Clique em **implementar**.

![Janela de importação](prints_node_red/12_importar.png)

> Dica: para trocar `grupo01` em todos os nós de uma vez, use `Ctrl+F` ("Procurar fluxos") e digite `grupo01`: ele lista todos os nós que o usam.

---

## 7. Problemas comuns

| Sintoma | Causa provável | Solução |
|---|---|---|
| Nó mqtt com *desconectado* ou sem bolinha | Sem internet, ou servidor/porta errados | Conferir `broker.hivemq.com` e porta `1883` |
| Nada aparece no **debug** | Esqueceu de **implementar**; ou tópico diferente do ESP32 | Clicar em implementar; conferir cada letra do tópico |
| Texto do dashboard mostra `{{msg.payload.esteira}}` ou vazio | Mqtt in com saída **texto**, não **objeto JSON** | Trocar a saída para *objeto JSON analisado* |
| Botões não fazem nada | mqtt out sem servidor, ou fio solto | Conferir servidor e a ligação botão → mqtt out |
| Alarme não arma | Botão sem **Topic `botao`**, ou payload com letras diferentes | Conferir `ARMAR`/`DESARMAR` e o topic |
| Função dá erro vermelho | Saídas da função diferentes de 3 | Aba **Configurar**: ajustar o nº de saídas |
| ESP32 desconecta o tempo todo | `client_id` repetido entre grupos | Cada grupo com `client_id` próprio |
| Dashboard não abre | `/ui` digitado errado, ou Node-RED fechado | Conferir `http://localhost:1880/ui` e a janela do `cmd` |
| A esteira liga sozinha ao reiniciar o ESP32 | **Reter** = verdadeiro em algum mqtt out | Trocar para **falso** |
| Mudou algo e nada mudou | Não clicou em **implementar** | Clicar em implementar |

---

## 8. Cola rápida

```
Receber do ESP32  →  mqtt in   (Saída: objeto JSON, QoS 0)
Mandar ao ESP32   →  mqtt out  (Reter: falso, QoS 0)
Botão             →  ui_button (Payload string, Topic "botao")
Mostrar           →  ui_text   ({{msg.payload}} ou {{msg.payload.campo}})
Decisão           →  function  (context.get/set + return [saida1, saida2, ...])
Depois de mexer   →  IMPLEMENTAR
```
