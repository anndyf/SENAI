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
