// Definição dos pinos e variáveis globais de controle
const int pinLED = 13;               // Pino digital conectado ao LED (LED interno do UNO)
unsigned long tempoAnterior = 0;      // Armazena o instante da última alteração (em ms)
const long intervalo = 1000;         // Tempo de espera desejado entre estados (1000 ms = 1 s)
int estadoLED = LOW;                 // Estado lógico atual do LED (LOW = Apagado, HIGH = Aceso)

void setup() {
  // Configura o pino do LED como saída digital
  pinMode(pinLED, OUTPUT);
}

void loop() {
  // Captura o tempo decorrido desde que a placa foi ligada
  unsigned long tempoAtual = millis();

  // Verifica se o tempo decorrido desde a última mudança é maior ou igual ao intervalo estipulado
  if (tempoAtual - tempoAnterior >= intervalo) {
    tempoAnterior = tempoAtual;      // Salva o instante atual para a próxima comparação
    estadoLED = !estadoLED;          // Inverte o estado do LED (Se era LOW vira HIGH, e vice-versa)
    digitalWrite(pinLED, estadoLED); // Aplica o novo estado ao pino do LED
  }

  // A CPU permanece livre para executar outras tarefas aqui continuamente!
}
