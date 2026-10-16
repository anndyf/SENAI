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
