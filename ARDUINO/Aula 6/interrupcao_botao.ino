// Definição dos pinos e variáveis globais de controle
const byte pinoBotao = 2;              // Pino digital com suporte a interrupção externa (INT0 no UNO)
const byte pinoLED = 13;               // Pino digital conectado ao LED interno
volatile bool eventoDetectado = false; // Flag modificada dentro da ISR (uso obrigatório de 'volatile')

// Rotina de Serviço de Interrupção (ISR)
void tratarInterrupcao() {
  eventoDetectado = true;              // Ação ultrarrápida dentro da ISR (apenas seta a flag)
}

void setup() {
  pinMode(pinoLED, OUTPUT);            // Configura o pino do LED como saída digital
  pinMode(pinoBotao, INPUT_PULLUP);   // Habilita o resistor de Pull-Up interno para o botão
  
  // Associa a interrupção externa no pino do botão acionando na borda de descida (FALLING)
  attachInterrupt(digitalPinToInterrupt(pinoBotao), tratarInterrupcao, FALLING);
}

void loop() {
  // O loop principal verifica se a ISR sinalizou um evento
  if (eventoDetectado) {
    digitalWrite(pinoLED, !digitalRead(pinoLED)); // Inverte o estado atual do pino do LED
    eventoDetectado = false;                      // Reseta a flag de evento
  }
}
