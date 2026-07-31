/* STREAMING_CHUNK:Definindo pinos e variaveis de controle */
// Definição dos Pinos Digitais
const byte pinoBotaoEmergencia = 2; // Pino INT0 (Suporta interrupção externa por hardware)
const byte pinoLEDOperacao = 13;    // LED Sinalizador da Esteira (Pisca a cada 500ms)
const byte pinoLEDEmergencia = 12;  // LED de Emergência (Acende em caso de parada)

// Variável compartilhada com a ISR (Obrigatório o uso do qualificador 'volatile')
volatile bool emergenciaAtiva = false;

// Variáveis de controle de tempo com millis() (sem bloqueio da CPU)
unsigned long tempoAnteriorPisca = 0;
unsigned long tempoAnteriorSegundo = 0;

// Constantes de intervalos de tempo (em milissegundos)
const unsigned long intervaloPisca = 500;   // 500 ms
const unsigned long intervaloSegundo = 1000; // 1000 ms = 1 segundo

// Contador do tempo de operação da esteira
unsigned long contadorSegundos = 0;

/* STREAMING_CHUNK:Criando a Rotina de Serviço de Interrupcao (ISR) */
// Rotina de Serviço de Interrupção (ISR) - Disparada no momento do clique do botão
void ISR_tratarEmergencia() {
  emergenciaAtiva = true; // Ação ultrarrápida: apenas altera a flag de controle
}

/* STREAMING_CHUNK:Configurando os periféricos no setup */
void setup() {
  // Inicialização da Comunicação Serial
  Serial.begin(9600);
  
  // Configuração das Saídas Digitais
  pinMode(pinoLEDOperacao, OUTPUT);
  pinMode(pinoLEDEmergencia, OUTPUT);
  
  // Configuração da Entrada Digital com Resistor de Pull-Up Interno
  pinMode(pinoBotaoEmergencia, INPUT_PULLUP);
  
  // Garantia dos estados iniciais dos LEDs
  digitalWrite(pinoLEDOperacao, LOW);
  digitalWrite(pinoLEDEmergencia, LOW);
  
  // Associação da Interrupção Externa no pino 2 (INT0) disparada na borda de descida (FALLING)
  attachInterrupt(digitalPinToInterrupt(pinoBotaoEmergencia), ISR_tratarEmergencia, FALLING);
  
  Serial.println(">>> SISTEMA INICIALIZADO: ESTEIRA EM OPERAÇÃO <<<");
}

/* STREAMING_CHUNK:Implementando o laço principal de controle */
void loop() {
  /* ==================================================================
   * 1. VERIFICAÇÃO DE EMERGÊNCIA (PRIORIDADE CRÍTICA)
   * ================================================================== */
  if (emergenciaAtiva) {
    // Desliga a sinalização da esteira e acende o LED de emergência
    digitalWrite(pinoLEDOperacao, LOW);
    digitalWrite(pinoLEDEmergencia, HIGH);
    
    // Emissão do alerta na comunicação serial (apenas uma vez)
    static bool alertaEnviado = false;
    if (!alertaEnviado) {
      Serial.println("\n--------------------------------------------------");
      Serial.println("!!! ALERTA CRÍTICO: PARADA DE EMERGÊNCIA ATIVADA !!!");
      Serial.println("Esteira Parada. Processo Interrompido.");
      Serial.println("--------------------------------------------------");
      alertaEnviado = true;
    }
    
    // Retorna imediatamente, travando a execução do fluxo normal da esteira
    return;
  }

  /* ==================================================================
   * 2. FLUXO NORMAL DE OPERAÇÃO DA ESTEIRA (TEMPO NÃO BLOQUEANTE)
   * ================================================================== */
  unsigned long tempoAtual = millis();

  // A) Pisca do LED Sinalizador da Esteira (a cada 500 ms)
  if (tempoAtual - tempoAnteriorPisca >= intervaloPisca) {
    tempoAnteriorPisca = tempoAtual;
    digitalWrite(pinoLEDOperacao, !digitalRead(pinoLEDOperacao)); // Inverte o estado do LED
  }

  // B) Incremento do Contador de Segundos de Operação (a cada 1000 ms)
  if (tempoAtual - tempoAnteriorSegundo >= intervaloSegundo) {
    tempoAnteriorSegundo = tempoAtual;
    contadorSegundos++;
    
    Serial.print("Tempo de Operação: ");
    Serial.print(contadorSegundos);
    Serial.println(" s");
  }
}
