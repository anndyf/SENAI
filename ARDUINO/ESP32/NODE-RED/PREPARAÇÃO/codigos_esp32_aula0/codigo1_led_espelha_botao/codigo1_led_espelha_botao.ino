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
