/*
  TESTE 1: LED
  Ligação: pino 8 -> LED (perna longa, +) -> perna curta (-) -> resistor 220R -> GND
  Resultado esperado: LED pisca a cada meio segundo.
  Se não acender: verifique a polaridade do LED.
*/

const int LED = 8;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  Serial.println("Teste do LED iniciado");
}

void loop() {
  digitalWrite(LED, HIGH);
  Serial.println("LED LIGADO");
  delay(500);

  digitalWrite(LED, LOW);
  Serial.println("LED DESLIGADO");
  delay(500);
}
