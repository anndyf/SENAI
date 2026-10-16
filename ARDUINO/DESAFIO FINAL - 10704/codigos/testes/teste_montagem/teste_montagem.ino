// =====================================================
//  TESTE DA MONTAGEM (sem Wi-Fi, sem MQTT, sem Node-RED)
//  ESP32 + sensor/botao + LED verde + LED vermelho + ponte H L298N + motor
//
//  Abra o Serial Monitor em 115200 baud e envie (com Enter):
//    t = teste automatico (LEDs e motor, um de cada vez)
//    m = liga/desliga o MOTOR
//    d = inverte o sentido do motor (liga IN4 em vez de IN3)
//    v = liga/desliga o LED VERDE
//    r = liga/desliga o LED VERMELHO
//    p = PARA tudo
//  O sensor/botao e monitorado sempre: toda mudanca aparece no Serial.
// =====================================================

#define SENSOR       27   // botao ou sensor E18-D80NK (outro lado no GND)
#define LED_VERDE    23
#define LED_VERMELHO 22
#define MOTOR_IN1    26   // ponte H IN3  (motor em OUT3/OUT4)
#define MOTOR_IN2    25   // ponte H IN4  (motor em OUT3/OUT4)

// Logica do sensor: false = detecta em LOW (NPN normal); true = detecta em HIGH (invertido)
const bool SENSOR_INVERTIDO = false;
#define NIVEL_DETECTA (SENSOR_INVERTIDO ? HIGH : LOW)

bool motorLigado = false;
bool sentidoInvertido = false;
bool verde = false;
bool vermelho = false;

int estadoAnterior = HIGH;          // INPUT_PULLUP: solto = HIGH
unsigned long ultimaMudanca = 0;
int contagem = 0;

void aplicaSaidas() {
  if (motorLigado) {
    digitalWrite(MOTOR_IN1, sentidoInvertido ? LOW : HIGH);
    digitalWrite(MOTOR_IN2, sentidoInvertido ? HIGH : LOW);
  } else {
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, LOW);
  }
  digitalWrite(LED_VERDE, verde ? HIGH : LOW);
  digitalWrite(LED_VERMELHO, vermelho ? HIGH : LOW);
}

void paraTudo() {
  motorLigado = false;
  verde = false;
  vermelho = false;
  aplicaSaidas();
}

void testeAutomatico() {
  Serial.println("--- TESTE AUTOMATICO ---");
  paraTudo();

  Serial.println("1/4  LED VERDE por 1,5 s");
  verde = true; aplicaSaidas(); delay(1500);
  verde = false; aplicaSaidas();

  Serial.println("2/4  LED VERMELHO por 1,5 s");
  vermelho = true; aplicaSaidas(); delay(1500);
  vermelho = false; aplicaSaidas();

  Serial.println("3/4  MOTOR para frente por 2 s (verde aceso)");
  sentidoInvertido = false; motorLigado = true; verde = true;
  aplicaSaidas(); delay(2000);
  paraTudo(); delay(500);

  Serial.println("4/4  MOTOR no sentido contrario por 2 s (vermelho aceso)");
  sentidoInvertido = true; motorLigado = true; vermelho = true;
  aplicaSaidas(); delay(2000);
  sentidoInvertido = false;
  paraTudo();

  Serial.println("--- FIM. Aperte o botao / passe a mao no sensor: deve aparecer no Serial ---");
}

void verificaSensor() {
  int leitura = digitalRead(SENSOR);
  if (leitura != estadoAnterior && millis() - ultimaMudanca > 50) {   // debounce 50 ms
    ultimaMudanca = millis();
    estadoAnterior = leitura;
    if (leitura == NIVEL_DETECTA) {
      contagem++;
      Serial.print("SENSOR: DETECTANDO  (acionamento n. ");
      Serial.print(contagem);
      Serial.println(")");
    } else {
      Serial.println("SENSOR: livre");
    }
  }
}

void tratarComando(char c) {
  switch (c) {
    case 't': testeAutomatico(); break;
    case 'm':
      motorLigado = !motorLigado; aplicaSaidas();
      Serial.println(motorLigado ? "Motor LIGADO" : "Motor PARADO");
      break;
    case 'd':
      sentidoInvertido = !sentidoInvertido; aplicaSaidas();
      Serial.println(sentidoInvertido ? "Sentido: INVERTIDO (IN4)" : "Sentido: NORMAL (IN3)");
      break;
    case 'v':
      verde = !verde; aplicaSaidas();
      Serial.println(verde ? "LED verde ACESO" : "LED verde apagado");
      break;
    case 'r':
      vermelho = !vermelho; aplicaSaidas();
      Serial.println(vermelho ? "LED vermelho ACESO" : "LED vermelho apagado");
      break;
    case 'p':
      paraTudo(); Serial.println("Tudo PARADO");
      break;
    default: break;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(SENSOR, INPUT_PULLUP);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  paraTudo();
  estadoAnterior = digitalRead(SENSOR);

  delay(500);
  Serial.println();
  Serial.println("=== TESTE DA MONTAGEM ===");
  Serial.println("t=teste automatico  m=motor  d=sentido  v=verde  r=vermelho  p=parar");
  Serial.print("Sensor agora: ");
  Serial.println(estadoAnterior == NIVEL_DETECTA ? "DETECTANDO" : "livre");
}

void loop() {
  verificaSensor();
  while (Serial.available()) {
    char c = Serial.read();
    if (c != '\n' && c != '\r') tratarComando(c);
  }
}
