# Pacote do Desafio: Contadora de Peças

**SENAI Alagoinhas · Sistemas Eletrônicos e Microcontrolados**

## O que tem em cada pasta

| Item | Para que serve |
|---|---|
| [`desafio_enunciado.md`](desafio_enunciado.md) | **Comece por aqui**: o desafio, os comportamentos e as entregas |
| [`circuito_bancada_desafio.md`](circuito_bancada_desafio.md) | Montagem da **bancada** (botão + 3 LEDs) |
| [`circuito_final_desafio.md`](circuito_final_desafio.md) | Circuito da **planta real** (sensor, ponte H, motor) |
| [`desafio_todos_esp32.md`](desafio_todos_esp32.md) | Os 4 trechos de código para completar |
| [`guia_dashboard_node_red_desafio.md`](guia_dashboard_node_red_desafio.md) | Passo a passo para montar o dashboard no Node-RED |
| [`codigos/desafio_base_esp32/`](codigos/desafio_base_esp32/desafio_base_esp32.ino) | **Código base do desafio** (complete os 4 TODOs) |
| [`codigos/testes/teste_montagem/`](codigos/testes/teste_montagem/teste_montagem.ino) | Testa botão, LEDs e motor por menu no Serial |
| [`codigos/testes/teste_sensor_esteira/`](codigos/testes/teste_sensor_esteira/teste_sensor_esteira.ino) | Teste contínuo do sensor e da esteira |
| [`codigos/aula0/`](codigos/aula0/) | Códigos da Aula 0 (consulta para resolver os TODOs) |
| [`aula0/`](aula0/aula0_guia_do_aluno.md) | Guia da Aula 0, circuito da bancada da Aula 0 e fluxo Node-RED da Aula 0 |
| `imagens/` e `*.png` | Imagens usadas nos guias |

## Como abrir no Arduino IDE

Cada código fica em uma pasta com **o mesmo nome do arquivo `.ino`**. Abra o `.ino` de dentro da pasta dele.

Bibliotecas: **PubSubClient** e **ArduinoJson** (versão 7).

## Antes de começar

1. Troque `grupo01` pelo número do seu grupo no código base (client_id e 7 tópicos).
2. Preencha o Wi-Fi (`ssid` e `password`).
3. Na bancada, o **LED amarelo fica no GPIO 21** (na Aula 0 estava no GPIO 26).
