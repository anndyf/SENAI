/*
  Desafio: Luz Proporcional ao Potenciômetro
  - Lê um potenciômetro (ADC) e usa o valor para controlar
    o brilho de um LED via PWM, proporcionalmente.
  - Opcional: acende um segundo LED (digital) quando o
    potenciômetro passar de 50% do curso.
*/

const uint8_t POT_PIN   = A0;  // Pino analógico do potenciômetro
const uint8_t LED_PWM   = 9;   // LED de brilho proporcional (pino PWM ~)
const uint8_t LED_ALERTA = 7;  // LED de alerta (liga/desliga simples)

void setup() {
  pinMode(LED_PWM, OUTPUT);
  pinMode(LED_ALERTA, OUTPUT);
  Serial.begin(9600); // Opcional, útil para debug/testes
}

void loop() {
  // 1) Amostragem ADC: lê o potenciômetro (0 a 1023)
  int leituraPot = analogRead(POT_PIN);

  // 2) Controle Proporcional via PWM:
  //    Converte a escala 0-1023 para 0-255 (faixa do analogWrite)
  int brilho = map(leituraPot, 0, 1023, 0, 255);
  analogWrite(LED_PWM, brilho);

  // 3) Alerta Simples: acende o segundo LED quando passar de 50%
  //    50% de 1023 ≈ 511
  if (leituraPot > 511) {
    digitalWrite(LED_ALERTA, HIGH);
  } else {
    digitalWrite(LED_ALERTA, LOW);
  }

  // Debug opcional via Serial (Ferramentas > Monitor Serial)
  Serial.print("Pot: ");
  Serial.print(leituraPot);
  Serial.print(" | Brilho: ");
  Serial.println(brilho);

  delay(20); // Pequena pausa para estabilizar a leitura
}
