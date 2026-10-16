# Código do ESP32: os 4 TODOs do desafio

**SENAI Alagoinhas · Sistemas Eletrônicos e Microcontrolados**

O arquivo **`desafio_base_esp32.ino`** já traz tudo que não é lógica: Wi-Fi, MQTT, velocidade da esteira, LEDs, publicação do status e monitoramento no Serial. Vocês completam **4 trechos** marcados com `// TODO`.

O código **compila e roda mesmo vazio**. O que faltar aparece como um comportamento errado, descrito no **SINTOMA** de cada TODO.

> **Antes de gravar:** coloque o seu Wi-Fi e troque **`grupo01`** pelo número do seu grupo nas **8 linhas** de configuração (client_id e os 7 tópicos).

## Os 4 TODOs

| TODO | Onde | O que fazer | **Referência (Aula 0)** | Como conferir |
|---|---|---|---|---|
| **1** | `setup` | Configurar o pino do sensor como entrada com pull-up interno | Qualquer código, por exemplo `codigo1_led_espelha_botao` | Sem tocar no sensor, o Serial deve mostrar `sensor=livre` e não ficar mudando sozinho |
| **2a** | `verificaSensor` | Dizer quando a leitura **mudou** e já passou o tempo de **debounce** | **`codigo3_borda_debounce`**: o `if` com `estadoAnterior` e `millis()` | O Serial deve reagir a cada acionamento do sensor |
| **2b** | `verificaSensor` | Dizer quando a peça **chegou**: leitura `LOW` **e** esteira ligada | **`anexoA_sala`**: `leitura == LOW && salaAberta` (aqui, a "sala aberta" vira "esteira ligada") | Com a esteira ligada, 3 acionamentos mostram `Contagem: 1, 2, 3`. Segurando o botão por 3 s, conta só **1**. Com a esteira parada, **não conta** |
| **3a** | `callback` | Tratar o comando `RESET`: zerar a contagem e avisar o Node-RED | **`anexoA_sala`**: o tratamento do comando `ZERAR` no `callback` | Clique em **Reiniciar** no dashboard: o Serial mostra a contagem zerada |
| **3b** | `callback` | Ligar ou desligar a esteira conforme `ON` / `OFF` no tópico da esteira | **`codigo4_esteira_e_alarme`**: o `callback` que chama `controlaEsteira` | Clique em **Iniciar**: a esteira (ou o LED amarelo da bancada) liga |
| **4** | `publicaContagem` | Colocar a contagem no JSON, com a chave `"contagem"` | **`codigo4_esteira_e_alarme`** e **`anexoA_sala`**: o `JsonDocument` com `doc["..."] = ...` | O debug do Node-RED mostra `{"contagem":1}` a cada peça |

## Códigos da Aula 0 para deixar abertos

Tenham ao lado, na Arduino IDE, estes três sketches da Aula 0. A resposta de cada TODO é uma adaptação de algo que vocês já escreveram:

| Sketch da Aula 0 | Ajuda nos TODOs |
|---|---|
| `codigo3_borda_debounce` | 1, 2a |
| `codigo4_esteira_e_alarme` | 3b, 4 |
| `anexoA_sala` | 2b, 3a, 4 |

O que muda são os **nomes** (sala vira esteira, visitas vira contagem). A lógica é a mesma.

## Ordem sugerida

1. **TODO 1 e 2** primeiro, **sem rede**: testem pelo Serial Monitor com o botão. Só avancem quando a contagem estiver certa.
2. **TODO 4** e **TODO 3**: agora com o Node-RED montado (guia do dashboard), testem o ciclo completo.

## Dicas (abram só se travarem)

**Nível 1: a ideia**
- TODO 1: pino de entrada, mas que **não fica solto**: o ESP32 tem uma resistência interna que puxa o pino para cima.
- TODO 2a: duas condições juntas. Uma compara `leitura` com `estadoAnterior`; a outra compara o tempo atual (`millis()`) com `ultimaMudanca`.
- TODO 2b: de novo duas condições. Lembre que o sensor **ativo** vai para `LOW`, e que só vale contar com a esteira ligada (`esteiraLigada`).
- TODO 3: compare a mensagem (`msg`) com o texto esperado. Para a esteira, a variável `estado` já traz o valor.
- TODO 4: o JSON funciona como um dicionário: `doc["chave"] = valor`.

**Nível 2: o passo a passo**
- TODO 1: use a função que define o modo de um pino, com o modo `INPUT_PULLUP`.
- TODO 2a: "diferente" é `!=`; "e" é `&&`; "passou mais que o tempo de debounce" é `millis() - ultimaMudanca > DEBOUNCE`.
- TODO 2b: `leitura == LOW` e `esteiraLigada` ligados por `&&`.
- TODO 3a: `if (msg == "RESET") { ... }`. Dentro: zere `contagem` e chame `publicaContagem()` e `publicaStatus()`.
- TODO 3b: `if (t == topic_esteira) { ... }`. Dentro: chame `controlaEsteira(...)` com `true` quando `estado` for `HIGH`.
- TODO 4: a chave é `"contagem"` e o valor é a variável `contagem`.

**Nível 3: pergunte ao professor** 🙂

## LED amarelo

O **LED amarelo (GPIO21)** já está pronto no código: ele acende **enquanto uma peça está sendo contada** (esteira ligada e sensor detectando). Se ele não acender, confira a ligação e o TODO 1.
