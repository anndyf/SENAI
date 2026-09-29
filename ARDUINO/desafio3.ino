#include <Servo.h>
#include <DHT.h>

// --- Configuração do DHT11 ---
int pinoDHT = 2;        // Pino Digital onde o DHT11 está conectado
#define DHTTYPE DHT11   // Define o modelo do sensor
DHT dht(pinoDHT, DHTTYPE);

Servo comporta;

// --- Configuração dos Pinos ---
int pinoTrig = 9;       // Pino Trig do Sensor Ultrassônico
int pinoEcho = 10;      // Pino Echo do Sensor Ultrassônico
int pinoServo = 3;      // Servo Motor

int ledVerde = 4;       // LED Normal / Fechado
int ledAmarelo = 5;     // LED Comporta Aberta
int ledVermelho = 6;    // LED Alerta / Objeto Próximo

int botaoAbrir = 7;     // Botão de abertura manual
int botaoFechar = 8;    // Botão de fechamento manual

void setup() {
  // Inicializa a comunicação com o computador (Monitor Serial)
  Serial.begin(9600);
  
  // Inicializa o sensor DHT11
  dht.begin();

  // Configuração do motor
  comporta.attach(pinoServo);
  comporta.write(0); // Garante que a comporta começa fechada (0 graus)

  // Configuração dos pinos dos sensores e LEDs
  pinMode(pinoTrig, OUTPUT);
  pinMode(pinoEcho, INPUT);
  
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  
  // Usamos INPUT_PULLUP para não precisar de resistores físicos nos botões
  pinMode(botaoAbrir, INPUT_PULLUP);
  pinMode(botaoFechar, INPUT_PULLUP);
}

void loop() {
  // 1. LEITURA DA TEMPERATURA (Sensor DHT11)
  float temperatura = dht.readTemperature();
  
  // Proteção: Se o DHT11 falhar, definimos um valor seguro para não travar o sistema
  if (isnan(temperatura)) {
    temperatura = 25.0; 
  }

  // 2. LEITURA DA DISTÂNCIA (Sensor Ultrassônico)
  digitalWrite(pinoTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinoTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrig, LOW);
  
  long tempoEcho = pulseIn(pinoEcho, HIGH);
  float distancia = tempoEcho * 0.017; // Converte o tempo do eco em Centímetros

  // --- Imprime os valores no Monitor Serial para acompanhamento ---
  Serial.print("Temp: ");
  Serial.print(temperatura);
  Serial.print(" C | Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  // 3. LEITURA DOS BOTÕES (LOW significa que foi apertado)
  bool apertouAbrir = (digitalRead(botaoAbrir) == LOW);
  bool apertouFechar = (digitalRead(botaoFechar) == LOW);

  // 4. LÓGICA DE CONTROLE E SEGURANÇA
  
  // REGRA 1: SEGURANÇA MÁXIMA (Tem algo perto ou o botão de fechar foi apertado)
  if (distancia < 50.0 || apertouFechar) {
    comporta.write(0); // Fecha a comporta imediatamente
    
    digitalWrite(ledAmarelo, LOW); 
    
    if (distancia < 50.0) {
      digitalWrite(ledVermelho, HIGH); // Objeto perto: Acende LED vermelho
      digitalWrite(ledVerde, LOW);
    } else {
      digitalWrite(ledVermelho, LOW);
      digitalWrite(ledVerde, HIGH);    // Fechamento manual seguro: Acende LED verde
    }
  }
  
  // REGRA 2: ABERTURA (Muito quente OU botão de abrir apertado, E sem perigo por perto)
  else if (temperatura > 30.0 || apertouAbrir) {
    comporta.write(90); // Abre a comporta (90 graus)
    
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledAmarelo, HIGH); // Acende aviso de comporta aberta
  }
  
  // REGRA 3: ESTADO NORMAL (Temperatura boa, sem obstáculos, botões soltos)
  else {
    comporta.write(0); // Mantém fechada
    
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledVerde, HIGH); // Acende aviso de tudo OK (Seguro e fechado)
  }

  // Pausa de 250ms (O DHT11 precisa de um tempo entre leituras)
  delay(250);
}
