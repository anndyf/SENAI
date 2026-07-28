/*
  Exemplo básico de PWM - Fade de LED
  - PWM simula um valor "analógico" variando a largura do pulso
    (duty cycle) numa frequência fixa (~490Hz no Arduino Uno).
  - analogWrite() aceita valores de 0 (sempre desligado / 0%)
    a 255 (sempre ligado / 100%).
*/

const uint8_t LED_PIN = 9;      // Pino PWM (tem o símbolo ~)
int brilho = 0;                 // Valor atual do PWM (0-255)
int passo = 5;                  // Quanto o brilho muda a cada ciclo

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  analogWrite(LED_PIN, brilho); // Aplica o duty cycle atual no LED

  brilho += passo;              // Incrementa (ou decrementa) o brilho

  // Ao chegar nos limites (0 ou 255), inverte o sentido do incremento
  if (brilho <= 0 || brilho >= 255) {
    passo = -passo;
  }

  delay(15); // Pequena pausa para o efeito ficar visível a olho nu
}
