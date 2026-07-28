/*
  Sensor Ultrassônico HC-SR04 - Medição de Distância
  - TRIG: envia um pulso ultrassônico (saída)
  - ECHO: recebe o eco de volta e mede quanto tempo levou (entrada)
  - Distância = (tempo do eco × velocidade do som) / 2
    (dividido por 2 porque o som percorre a distância de ida E volta)
*/

const uint8_t TRIG_PIN = 5;
const uint8_t ECHO_PIN = 4;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  // 1) Garante que o TRIG comece em LOW (pulso limpo)
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // 2) Envia o pulso ultrassônico: 10 microssegundos em HIGH
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // 3) Mede quanto tempo (em microssegundos) o ECHO ficou em HIGH
  //    -> esse é o tempo de ida e volta do som até o obstáculo
  long duracao = pulseIn(ECHO_PIN, HIGH);

  // 4) Converte o tempo em distância (cm)
  //    Velocidade do som ≈ 343 m/s ≈ 0,0343 cm/microssegundo
  //    Divide por 2 (ida + volta) e multiplica pela velocidade
  float distanciaCm = (duracao * 0.0343) / 2;

  Serial.print("Distância: ");
  Serial.print(distanciaCm);
  Serial.println(" cm");

  delay(300); // Pequena pausa entre as leituras
}
