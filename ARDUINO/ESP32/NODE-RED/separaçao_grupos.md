# Desafio prático: grupos e datas

**SENAI Alagoinhas · Sistemas Eletrônicos e Microcontrolados**

Este documento mostra **quem vem em cada dia**, **como o dia se organiza** e **o que levar**. O enunciado será apresentado pelo professor no primeiro dia.

---

## 1. Datas e grupos

A turma foi dividida em **dois dias**, com **4 grupos de 4 pessoas** em cada um.

| | Dia 1 | Dia 2 |
|---|---|---|
| **Data** | **Sexta-feira, 16/10/2026** | **Sexta-feira, 23/10/2026** |
| **Grupos** | grupo01, grupo02, grupo03, grupo04 | grupo05, grupo06, grupo07, grupo08 |
| **Alunos** | 16 | 16 |

> Quem vem em cada dia é definido pelo **grupo**. Não troquem de dia sem combinar com o professor, porque os grupos e a planta foram organizados para isso.

### Dia 1 — sexta-feira, 16/10

| Grupo | Integrantes |
|---|---|
| **grupo01** | Nathan Ferreira Dos Santos · Ícaro Alcantara Souza · Marcus Vinicius dos Santos Souza · Nicolas Gabriel Ramos das Virgens |
| **grupo02** | Állan Freitas dos Santos · Isabelle da Conceição Ramos · Rafael Silva Rabelo · Whislimar Alves da Silva |
| **grupo03** | Ariel Muniz de Brito · Jennifer Lavinie Araújo Moura Sá Barreto · Maurício Santos de Almeida · Tayná Lima da Silva |
| **grupo04** | Felipe Carneiro dos Santos · Leandro Castro de Oliveira · Alisson Miguel Campos Silva · Victoria Santana Batista |

### Dia 2 — sexta-feira, 23/10

| Grupo | Integrantes |
|---|---|
| **grupo05** | Álef Gabriel Santiago dos Santos · Iasmin dos Santos Machado · Lucas Cerqueira Costa · Neilton Matos Moura |
| **grupo06** | Allan Albert Mendes de Brito · Ikaro Ryan Almeida dos Santos · Maria Eduarda Batista Barradas de Lima · Pedro Arthur de Jesus Nascimento |
| **grupo07** | Ana Claudia da Silva Santos · Jean Santos Santana · Matheus Vinício de Santana Cerqueira · Táfne Camille Araujo Santos |
| **grupo08** | Eder Gabriel Ferreira dos Santos da Paixão · Kaique Santana Pereira · Naelson Cardoso dos Santos · Thiago Alves da Hora |

O **número do grupo** é o que vocês usam nos tópicos MQTT (`senai_alagoinhas_2026/desafio/grupoXX/...`), no ESP32 **e** no Node-RED. O broker é público: se dois grupos usarem o mesmo número, um atrapalha o outro. **Cada grupo usa só o seu número**, mesmo que teste em casa.

---

## 2. Como o dia se organiza

- Cada grupo trabalha no **seu dia**, em **três etapas de 2h30**: Etapa 1, Etapa 2 e Etapa 3.
- As etapas 1 e 2 são feitas na **bancada** de cada grupo.
- A etapa 3 inclui a **planta real**. Existe **uma só**, então os grupos do dia se revezam **em ordem de número**, cerca de **35 minutos** cada. Quem espera continua trabalhando na bancada.

| Dia | Ordem na planta |
|---|---|
| **Dia 1 (16/10)** | grupo01 → grupo02 → grupo03 → grupo04 |
| **Dia 2 (23/10)** | grupo05 → grupo06 → grupo07 → grupo08 |

Na planta, só o professor encaixa o ESP32 do grupo. Ninguém mexe na fiação da planta.

Os horários exatos são definidos pelo professor.

---

## 3. O que levar

| Item | Observação |
|---|---|
| **Notebook** com **Arduino IDE** e as bibliotecas **PubSubClient** e **ArduinoJson (v7)** | Testem a gravação antes |
| **Node-RED** instalado, com o `node-red-dashboard` | Abram o `localhost:1880` antes do dia |
| **ESP32**, cabo USB, protoboard, botão, 3 LEDs, 3 resistores de 220 Ω e jumpers | O kit da bancada |
| **Os códigos e fluxos da Aula 0** (`aula0_guia_do_aluno.md`) | Em pen drive, e-mail ou nuvem: a internet pode falhar |
| **Nome e senha do Wi-Fi** | Para o ESP32 |
| **Celular** | Para gravar vídeo |
