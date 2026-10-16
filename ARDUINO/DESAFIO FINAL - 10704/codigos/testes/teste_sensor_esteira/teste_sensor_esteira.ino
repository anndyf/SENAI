// =====================================================
//  TESTE DA MONTAGEM: sensor -> esteira + LEDs
//  (sem Wi-Fi, sem MQTT, sem Node-RED)
//
//  - Ao ligar: teste rapido de LEDs e motor.
//  - Depois, o sensor (ou botao) e lido o tempo todo:
//      DETECTANDO -> esteira GIRA + LED VERDE aceso
//      livre      -> esteira PARADA + LED VERMELHO aceso
//  - O Serial Monitor (115200) mostra o status continuamente.
// =====================================================

#define SENSOR       27   // botao ou sensor E18-D80NK (outro lado no GND)
#define LED_VERDE    23
#define LED_VERMELHO 22
#define LED_AMARELO  21   // acende enquanto uma peca esta sendo contada
#define MOTOR_IN1    26   // ponte H IN3 (motor em OUT3/OUT4)
#define MOTOR_IN2    25   // ponte H IN4 (sempre LOW)

// Logica do sensor:
//   false = detecta quando o pino vai a LOW  (NPN normal)
//   true  = detecta quando o pino vai a HIGH (sinal INVERTIDO)
const bool SENSOR_INVERTIDO = false;
#define NIVEL_DETECTA (SENSOR_INVERTIDO ? HIGH : LOW)

const unsigned long MANTER_MS  = 1000;  // esteira continua girando este tempo apos o sensor liberar (0 = para na hora)
const unsigned long STATUS_MS  = 300;   // intervalo do status no Serial

unsigned long ultimaDeteccao = 0;
unsigned long ultimoStatus = 0;
bool detectou = false;       // ja detectou alguma vez desde que ligou
bool esteiraLigada = false;
int contagem = 0;
int leituraAnterior = HIGH;

void esteira(bool ligar) {
  esteiraLigada = ligar;
  digitalWrite(MOTOR_IN1, ligar ? HIGH : LOW);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(LED_VERDE, ligar ? HIGH : LOW);
  digitalWrite(LED_VERMELHO, ligar ? LOW : HIGH);
}

void setup() {
  Serial.begin(115200);
  pinMode(SENSOR, INPUT_PULLUP);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);

  delay(500);
  Serial.println();
  Serial.println("=== TESTE: SENSOR -> ESTEIRA ===");

  Serial.println("Teste inicial: os tres LEDs acesos por 1 s");
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_AMARELO, HIGH);
  delay(1000);
  digitalWrite(LED_AMARELO, LOW);

  Serial.println("Teste inicial: esteira girando por 1,5 s");
  esteira(true);
  delay(1500);
  esteira(false);

  Serial.println("Pronto. Aperte o botao / passe a mao no sensor.");
  leituraAnterior = digitalRead(SENSOR);
}

void loop() {
  int leitura = digitalRead(SENSOR);
  bool detectando = (leitura == NIVEL_DETECTA);

  if (detectando) {
    ultimaDeteccao = millis();
    detectou = true;
  }

  // conta cada novo acionamento (borda: livre -> detectando)
  if (leitura == NIVEL_DETECTA && leituraAnterior != NIVEL_DETECTA) {
    contagem++;
    Serial.print(">>> NOVA DETECCAO n. ");
    Serial.println(contagem);
  }
  leituraAnterior = leitura;

  // esteira gira enquanto detecta (e por MANTER_MS depois)
  bool deveLigar = detectou && (detectando || millis() - ultimaDeteccao < MANTER_MS);
  if (deveLigar != esteiraLigada) {
    esteira(deveLigar);
  }
  if (!detectou && !esteiraLigada) {
    digitalWrite(LED_VERMELHO, HIGH);   // estado inicial: parada
    digitalWrite(LED_VERDE, LOW);
  }

  // LED amarelo: acende enquanto uma peca esta sendo contada (esteira ligada + sensor detectando)
  digitalWrite(LED_AMARELO, (esteiraLigada && detectando) ? HIGH : LOW);

  // status constante no Serial
  if (millis() - ultimoStatus >= STATUS_MS) {
    ultimoStatus = millis();
    Serial.print("sensor: ");
    Serial.print(detectando ? "DETECTANDO" : "livre     ");
    Serial.print(" | esteira: ");
    Serial.print(esteiraLigada ? "GIRANDO" : "parada ");
    Serial.print(" | LED verde: ");
    Serial.print(digitalRead(LED_VERDE) ? "ON " : "off");
    Serial.print(" | LED vermelho: ");
    Serial.print(digitalRead(LED_VERMELHO) ? "ON " : "off");
    Serial.print(" | amarelo: ");
    Serial.print(digitalRead(LED_AMARELO) ? "ON " : "off");
    Serial.print(" | deteccoes: ");
    Serial.println(contagem);
  }
}
