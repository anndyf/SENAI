/*
  Chave de partida selada (toggle) - SEM debounce
  - Pino 2: botão (pull-up externo -> usar INPUT)
  - Pino 8: relé
  - Pino 13: LED indicador
*/

const uint8_t BOTAO_PIN = 2;
const uint8_t RELE_PIN  = 8;
const uint8_t LED_PIN   = 13;

bool cargaLigada = false;
int estadoAnterior = HIGH;

void setup() {
  pinMode(BOTAO_PIN, INPUT);
  pinMode(RELE_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int leitura = digitalRead(BOTAO_PIN);

  if (leitura != estadoAnterior) {   // detecta qualquer mudança de estado
    estadoAnterior = leitura;

    if (leitura == LOW) {           // botão pressionado
      cargaLigada = !cargaLigada;
      digitalWrite(RELE_PIN, cargaLigada);
      digitalWrite(LED_PIN, cargaLigada);
    }
  }
}
