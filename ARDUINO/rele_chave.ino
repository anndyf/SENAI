/*
  Chave de partida selada (toggle) com debounce simplificado
  - 1º clique: liga carga (relé + LED)
  - 2º clique: desliga carga
*/

const uint8_t BOTAO_PIN = 2;
const uint8_t RELE_PIN  = 8;
const uint8_t LED_PIN   = 13;
const unsigned long DEBOUNCE_DELAY = 50; // ms

bool cargaLigada = false;        // estado atual da carga
int estadoAnterior = HIGH;       // último estado estável do botão
unsigned long ultimoTempo = 0;   // marca de tempo da última mudança

void setup() {
  pinMode(BOTAO_PIN, INPUT_PULLUP);
  pinMode(RELE_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int leitura = digitalRead(BOTAO_PIN);

  // Só avalia se o botão estiver pressionado (LOW) e já tiver
  // passado tempo suficiente desde a última mudança válida
  if (leitura != estadoAnterior && (millis() - ultimoTempo) > DEBOUNCE_DELAY) {
    ultimoTempo = millis();       // atualiza o tempo da mudança
    estadoAnterior = leitura;     // guarda o novo estado

    if (leitura == LOW) {         // clique real detectado
      cargaLigada = !cargaLigada; // alterna o estado da carga
      digitalWrite(RELE_PIN, cargaLigada);
      digitalWrite(LED_PIN, cargaLigada);
    }
  }
}
