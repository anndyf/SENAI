/*
  Exemplo básico de controle de Servomotor
  - O servo não usa PWM "puro" como o LED: ele espera pulsos
    numa frequência fixa de 50Hz (1 pulso a cada 20ms).
  - A LARGURA do pulso (não o duty cycle médio) é o que define
    o ângulo: ~1ms = 0°, ~1,5ms = 90°, ~2ms = 180°.
  - A biblioteca Servo.h já gera esses pulsos automaticamente -
    não usamos analogWrite() aqui.
*/

#include <Servo.h>

Servo meuServo;              // Cria o objeto que representa o servo
const uint8_t SERVO_PIN = 9; // Pino de sinal do servo

void setup() {
  meuServo.attach(SERVO_PIN); // Associa o objeto ao pino físico
}

void loop() {
  meuServo.write(0);    // Move para 0 graus
  delay(1000);          // Espera 1s para o servo chegar na posição

  meuServo.write(90);   // Move para 90 graus (posição central)
  delay(1000);

  meuServo.write(180);  // Move para 180 graus
  delay(1000);
}
