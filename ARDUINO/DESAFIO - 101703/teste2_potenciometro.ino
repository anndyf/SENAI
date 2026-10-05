/*
  TESTE 2: POTENCIÔMETRO
  Ligação: perna da esquerda -> GND
           perna do meio     -> A0
           perna da direita  -> 5V
  Resultado esperado: no Monitor Serial, o valor vai de 0 a 1023
  conforme você gira o eixo. Se só der 0 ou 1023, verifique a perna do meio.
  Se o sentido estiver invertido, troque GND e 5V de lugar.
*/

const int POT = A0;

void setup() {
  Serial.begin(9600);
  Serial.println("Teste do potenciometro iniciado");
}

void loop() {
  int leitura = analogRead(POT);
  int porcentagem = map(leitura, 0, 1023, 0, 100);
  float tensao = leitura * 5.0 / 1023.0;

  Serial.print("Leitura: ");
  Serial.print(leitura);
  Serial.print("  |  ");
  Serial.print(porcentagem);
  Serial.print("%  |  ");
  Serial.print(tensao, 2);
  Serial.println(" V");

  delay(300);
}
