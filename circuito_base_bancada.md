# Circuito Base da Bancada — ESP32 + Botão + 3 LEDs

**SENAI Alagoinhas · Sistemas Eletrônicos e Microcontrolados**

Este é o circuito que **cada grupo monta na sua bancada**. O botão faz o papel de um **sensor** e o LED amarelo faz o papel do **motor de uma esteira**. Depois, o ESP32 pode ser levado para a **esteira real**.

![Circuito base da bancada](imagens/circuito_base_bancada.png)

*(Esquema feito no Cirkit Designer. Arquivo da imagem: `circuito_base_bancada.png`.)*

---

## 1. O que cada componente representa

| Componente na bancada | Pino | Na esteira real vira... |
|---|---|---|
| **Botão** ("Sensor") | GPIO **27** | Sensor infravermelho **E18-D80NK** (saída NPN) |
| **LED verde** | GPIO **23** | Indicador (o mesmo LED) |
| **LED vermelho** | GPIO **22** | Indicador (o mesmo LED) |
| **LED amarelo** ("motor") | GPIO **26** | **Ponte H L298N** (IN1) + motor da esteira |

O botão e o sensor NPN se comportam **do mesmo jeito**: quando acionados, o pino vai para **GND** (leitura `LOW`). Por isso o código escrito na bancada roda na esteira real **sem mudança**.

---

## 2. Ligações do ESP32

| Pino do ESP32 | Fio no esquema | Vai para |
|---|---|---|
| **GPIO 27** | azul | Protoboard, **coluna 4** (linha A) → botão |
| **GPIO 22** | vermelho | Protoboard, **coluna 11** (linha E) → resistor → LED vermelho |
| **GPIO 23** | verde | Protoboard, **coluna 21** (linha E) → resistor → LED verde |
| **GPIO 26** | amarelo | Protoboard, **coluna 30** (linha A) → resistor → LED amarelo |
| **GND** (lado esquerdo da placa) | azul-petróleo | **Trilho de GND** da protoboard (coluna 1) |

> O **GPIO 25** (IN2 da ponte H na esteira real) **não é usado na bancada**.

---

## 3. Mapa da protoboard (por coluna)

As colunas são numeradas de 1 a 30. Cada grupo de cinco furos de uma coluna (**A–E** embaixo, **F–J** em cima) é ligado entre si; o vão do meio separa os dois lados.

| Componente | Colunas | Como fica |
|---|---|---|
| **Botão** | 2 e 4 | Atravessa o vão central (linhas **E** e **F**). **Coluna 2** → jumper (linha A) para o trilho de GND. **Coluna 4** ← fio azul do GPIO 27 |
| **LED vermelho** | 5 e 6 | Pernas na linha **C**. **Coluna 5** → jumper (linha A) para o trilho de GND. **Coluna 6** → resistor 220 Ω (linha B, da coluna 6 à coluna 11) |
| **LED verde** | 15 e 16 | Pernas na linha **C**. **Coluna 15** → jumper (linha A) para o trilho de GND. **Coluna 16** → resistor 220 Ω (linha B, da coluna 16 à coluna 21) |
| **LED amarelo** | 24 e 25 | Pernas na linha **C**. **Coluna 24** → jumper (linha A) para o trilho de GND. **Coluna 25** → resistor 220 Ω (linha B, da coluna 25 à coluna 30) |

### Caminho elétrico

```
GPIO 22 ──fio──► [coluna 11] ──[220 Ω]──► [coluna 6]  ──► LED vermelho ──► [coluna 5]  ──► GND
GPIO 23 ──fio──► [coluna 21] ──[220 Ω]──► [coluna 16] ──► LED verde    ──► [coluna 15] ──► GND
GPIO 26 ──fio──► [coluna 30] ──[220 Ω]──► [coluna 25] ──► LED amarelo  ──► [coluna 24] ──► GND
GPIO 27 ──fio──► [coluna 4]  ──► botão ──► [coluna 2] ──► GND
```

---

## 4. Montagem passo a passo (10 min)

1. **GND primeiro:** ligue o pino **GND** do ESP32 ao **trilho de GND** da protoboard.
2. **Botão:** encaixe atravessando o vão central, nas colunas **2 e 4**.
3. **LEDs:** encaixe os três LEDs na linha **C**, nas colunas indicadas (**5–6**, **15–16**, **24–25**).
4. **Resistores de 220 Ω:** ligue cada um na linha **B**, entre as colunas indicadas (**6→11**, **16→21**, **25→30**).
5. **Jumpers curtos para o GND:** colunas **2, 5, 15 e 24** (linha A) até o trilho de GND.
6. **Fios do ESP32:** GPIO **27** → coluna 4, GPIO **22** → coluna 11, GPIO **23** → coluna 21, GPIO **26** → coluna 30.
7. **Confira** com o checklist abaixo, **antes** de ligar o cabo USB.

---

## 5. Checklist de conferência

- [ ] **Polaridade dos LEDs:** a **perna longa (ânodo +)** fica do lado do **resistor**; a **perna curta (cátodo −)** fica do lado do **GND**
- [ ] **Os três LEDs têm resistor de 220 Ω** (sem ele, o LED e o pino do ESP32 podem queimar)
- [ ] O **botão não tem resistor** externo (o pull-up é feito por software)
- [ ] O **GND do ESP32** está no trilho de GND
- [ ] Nenhum fio está no furo errado da coluna
- [ ] **GPIO 27, 22, 23 e 26** estão nos fios certos (as cores ajudam: azul, vermelho, verde, amarelo)

---

## 6. Teste da bancada

Grave este sketch. Se tudo estiver certo, os três LEDs acendem **um de cada vez, em sequência**, e o Serial mostra o estado do botão.

```cpp
// =====================================================
//  Teste da bancada: 3 LEDs em sequencia + leitura do botao
// =====================================================
#define BOTAO        27
#define LED_VERDE    23
#define LED_VERMELHO 22
#define LED_AMARELO  26   // na esteira real, este pino e o IN1 da ponte H

void setup() {
  Serial.begin(115200);
  pinMode(BOTAO, INPUT_PULLUP);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
}

void piscaLed(int pino) {
  digitalWrite(pino, HIGH);
  delay(300);
  digitalWrite(pino, LOW);
}

void loop() {
  piscaLed(LED_VERDE);
  piscaLed(LED_VERMELHO);
  piscaLed(LED_AMARELO);

  if (digitalRead(BOTAO) == LOW) {
    Serial.println("botao APERTADO");
  } else {
    Serial.println("botao solto");
  }
}
```

| Teste | Esperado |
|---|---|
| Gravar e observar | Verde → vermelho → amarelo, um de cada vez, repetindo |
| Botão **solto** | Serial mostra `botao solto` |
| Botão **apertado** (segure durante o ciclo) | Serial mostra `botao APERTADO` |

---

## 7. Problemas comuns

| Sintoma | Causa provável | Solução |
|---|---|---|
| Um LED não acende | Polaridade invertida ou resistor desconectado | Inverta o LED; confira a linha B |
| Nenhum LED acende | GND do ESP32 não ligado ao trilho | Conferir o fio de GND |
| LED acende na cor errada | Fios do GPIO trocados | Conferir 22 (vermelho), 23 (verde), 26 (amarelo) |
| Serial mostra **sempre** `botao APERTADO` | Botão girado 90°, e os contatos internos ficam sempre ligados | Gire o botão em 90° ou mude-o de colunas |
| Serial mostra **sempre** `botao solto` | Botão sem contato com a coluna 4 ou com o GND | Conferir o botão, a coluna 4 e o jumper da coluna 2 |
| Leituras oscilando | Fio do GPIO 27 solto | Reencaixar o fio |

---

## 8. Ligação na esteira real

| Bancada | Esteira real |
|---|---|
| Botão (colunas 2 e 4) | Sensor **E18-D80NK**: **preto** → GPIO 27, **marrom** → VIN (5 V), **azul** → GND |
| LED amarelo (GPIO 26) | Ponte H **L298N**: IN1 = GPIO 26, IN2 = GPIO 25 |
| LED verde e LED vermelho | Os mesmos LEDs, GPIO 23 e GPIO 22 |
| GND da protoboard | **GND comum** com a ponte H e a fonte do motor |

> ⚠️ **Na esteira real, ninguém mexe na fiação do L298N, da fonte ou do sensor.** O professor troca apenas o ESP32 do grupo.
