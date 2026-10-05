# Desafio: Monitor de Temperatura do Forno Industrial

## Contexto

Uma fábrica de cerâmica precisa de um painel simples para o operador acompanhar a temperatura de um forno. A temperatura ideal de trabalho fica entre **150 °C e 250 °C**. Acima de 250 °C, as peças trincam e o operador precisa ser avisado na hora, com sinal luminoso e sonoro.

Sua equipe vai construir esse sistema do zero no Arduino. Na bancada, o potenciômetro faz o papel do sensor de temperatura do forno.

## Objetivos

Ao final do desafio, você deve ser capaz de:

- Criar funções que **recebem parâmetros** e funções que **retornam valores**.
- Diferenciar uma função `void` de uma função com tipo de retorno (`int`, `String`).
- Dividir um programa em funções com uma única responsabilidade cada.
- Usar o resultado de uma função como entrada de outra.

## Materiais e ligações

Cada equipe recebe 1 Arduino Uno, 1 LCD 16x2 (sem I2C), 2 potenciômetros de 10 kΩ, 1 LED vermelho, 1 buzzer, 2 resistores de 220 Ω, protoboard e jumpers.

| Componente | Pino do componente | Liga em |
| --- | --- | --- |
| Potenciômetro (sensor) | pernas externas / meio | 5V e GND / A0 |
| LED de alarme | perna longa (+) | pino 8 (perna curta → 220 Ω → GND) |
| Buzzer | + / − | pino 9 / GND |
| LCD 1 (VSS) e 2 (VDD) | alimentação | GND e 5V |
| LCD 3 (V0) | contraste | meio do 2º potenciômetro (pernas externas em 5V e GND) |
| LCD 4 (RS) | controle | pino 12 |
| LCD 5 (RW) | leitura/escrita | GND |
| LCD 6 (E) | habilita | pino 11 |
| LCD 7 a 10 (D0 a D3) | dados | não ligar |
| LCD 11 a 14 (D4 a D7) | dados | pinos 5, 4, 3 e 2 |
| LCD 15 (A) e 16 (K) | luz de fundo | 5V com 220 Ω e GND |

## Funcionamento esperado

| Temperatura | Linha 2 do LCD | LED | Buzzer |
| --- | --- | --- | --- |
| abaixo de 150 °C | AQUECENDO | apagado | desligado |
| 150 a 250 °C | IDEAL | apagado | desligado |
| acima de 250 °C | ALERTA | aceso | 3 bipes |

A linha 1 do LCD deve mostrar sempre a temperatura atual, por exemplo `Forno: 213 C`.

## Estrutura sugerida das funções

Use as estruturas abaixo como ponto de partida. Os nomes, parâmetros e tipos de retorno já estão definidos; o corpo de cada função é com vocês.

```cpp
// Lê o sensor no pino recebido.
// Retorna a temperatura de 0 a 300 °C.
int lerTemperatura(int pino) {
  // sua lógica aqui
}

// Recebe a temperatura.
// Retorna "AQUECENDO", "IDEAL" ou "ALERTA".
String verificarStatus(int temperatura) {
  // sua lógica aqui
}

// Recebe a temperatura e o status.
// Escreve a temperatura na linha 1 e o status na linha 2 do LCD.
// Não retorna nada.
void atualizarPainel(int temperatura, String status) {
  // sua lógica aqui
}

// Recebe a quantidade de bipes.
// Faz o buzzer apitar esse número de vezes.
void acionarBuzzer(int vezes) {
  // sua lógica aqui
}

// Recebe o status.
// Acende o LED e chama acionarBuzzer() quando o status for "ALERTA".
void controlarAlarme(String status) {
  // sua lógica aqui
}
```

O `loop()` deve ficar curto, apenas chamando as funções na ordem certa:

```cpp
void loop() {
  int temp = lerTemperatura(A0);
  String status = verificarStatus(temp);
  atualizarPainel(temp, status);
  controlarAlarme(status);
  delay(300);
}
```

## Tarefas

Faça as tarefas em ordem e teste cada uma no circuito antes de seguir.

1. **Montagem e testes.** Monte o circuito e rode os códigos de teste de cada componente (LED, potenciômetro, LCD e buzzer). Só avance quando todos funcionarem separadamente.
2. **Leitura do sensor.** Escreva `lerTemperatura()` e mostre o valor no Monitor Serial. Gire o potenciômetro e confirme que a temperatura vai de 0 a 300 °C.
3. **Status do forno.** Escreva `verificarStatus()`. Mostre no Monitor Serial a temperatura e o status lado a lado e confira as três faixas.
4. **Painel do operador.** Escreva `atualizarPainel()` para mostrar as informações no LCD.
5. **Alarme.** Escreva `acionarBuzzer()` e `controlarAlarme()`. Na faixa de alerta, o LED acende e o buzzer dá 3 bipes.

### Desafios extras

6. Crie a função `int converterParaFahrenheit(int celsius)`, usando F = C × 1,8 + 32, e mostre as duas escalas na linha 1 do LCD, por exemplo `200C / 392F`.
7. Guarde a maior temperatura atingida desde que o Arduino ligou e mostre-a na linha 2 quando o forno estiver na faixa ideal, por exemplo `IDEAL  Max:235C`.

## Entrega e avaliação

Entregue o arquivo `.ino` final e demonstre o circuito funcionando para o professor. Cada função deve ter um comentário acima dela dizendo o que recebe e o que retorna.

| Critério | Pontos |
| --- | --- |
| Tarefas 1 e 2: circuito montado e leitura do sensor | 2,0 |
| Tarefa 3: `verificarStatus()` retornando as três faixas | 2,0 |
| Tarefa 4: LCD mostrando temperatura e status | 2,0 |
| Tarefa 5: alarme com LED e buzzer | 2,0 |
| Código organizado, comentado e com `loop()` curto | 2,0 |
| **Total** | **10,0** |
| Desafios extras (bônus) | até +1,0 |

## Dicas

- Os códigos de teste já mostram como usar cada componente: `analogRead()` e `map()` no do potenciômetro, `lcd.setCursor()` e `lcd.print()` no do LCD, `digitalWrite()` no do LED e `tone()` no do buzzer. Reaproveite essas linhas dentro das funções.
- Uma função que retorna valor precisa terminar com `return` em todos os caminhos do `if`.
- O LCD tem só 16 colunas. Complete o texto com espaços para apagar restos do que estava escrito antes.
- Enquanto o buzzer apita, o programa não lê o sensor. Isso é esperado nesta versão.
- No desafio 7, a variável da temperatura máxima precisa ser **global**, ou será zerada a cada volta do `loop()`.
- Se o LCD mostrar só quadradinhos ou ficar em branco, rode de novo o código de teste do LCD e confira o contraste e os pinos RS, E e D4 a D7.
