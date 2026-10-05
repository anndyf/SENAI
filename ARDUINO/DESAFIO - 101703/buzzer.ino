/*
  TESTE 4: BUZZER (para o desafio 4)
  Ligação: pino 9 -> perna + do buzzer | perna - -> GND
  Resultado esperado: três tons diferentes, depois silêncio, repetindo.

  Atenção no componente REAL:
  - Buzzer PASSIVO: precisa de tone() (é o que este teste usa).
  - Buzzer ATIVO: apita sozinho com digitalWrite(HIGH); com tone()
    o som pode sair igual nos três tons.
*/

const int BUZZER = 9;    // pino 11 fica livre para o LCD

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER, OUTPUT);
  Serial.println("Teste do buzzer iniciado");
}

void loop() {
  Serial.println("Tom grave (500 Hz)");
  tone(BUZZER, 500, 400);
  delay(600);

  Serial.println("Tom medio (1000 Hz)");
  tone(BUZZER, 1000, 400);
  delay(600);

  Serial.println("Tom agudo (2000 Hz)");
  tone(BUZZER, 2000, 400);
  delay(600);

  noTone(BUZZER);
  delay(1500);
}
