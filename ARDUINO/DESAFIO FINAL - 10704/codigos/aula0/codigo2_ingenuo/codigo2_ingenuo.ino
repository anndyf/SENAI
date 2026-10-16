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
