/*
  Exemplo básico de Motor DC com Ponte H (L298N)
  - ENA controla a VELOCIDADE (via PWM, 0-255)
  - IN1/IN2 controlam a DIREÇÃO (sentido de giro):
      IN1=HIGH, IN2=LOW  -> gira em um sentido
      IN1=LOW,  IN2=HIGH -> gira no sentido contrário
      IN1=LOW,  IN2=LOW  -> motor parado (freio solto)
*/

const uint8_t ENA_PIN = 9;  // Pino PWM - velocidade
const uint8_t IN1_PIN = 8;  // Direção
const uint8_t IN2_PIN = 7;  // Direção

void setup() {
  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
}

void loop() {
  // --- Gira para frente, acelerando gradualmente ---
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);

  for (int velocidade = 0; velocidade <= 255; velocidade += 5) {
    analogWrite(ENA_PIN, velocidade); // Aplica o PWM na velocidade
    delay(30);
  }

  delay(1000); // Mantém na velocidade máxima por 1s

  // --- Para o motor ---
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  delay(1000);

  // --- Gira para trás (sentido invertido) ---
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
  analogWrite(ENA_PIN, 200); // Velocidade fixa, ~78% de duty cycle
  delay(2000);

  // --- Para novamente antes de repetir o ciclo ---
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  delay(1000);
}
