// Aula 0 - Codigo 3: cada APERTO alterna o LED (borda + debounce)
#define BOTAO      27
#define LED_VERDE  23
#define DEBOUNCE   0     // comece com 0 para ver o tremor; depois troque para 50

bool ledLigado = false;
int  estadoAnterior = HIGH;          // lembra a leitura anterior
unsigned long ultimaMudanca = 0;     // quando foi a ultima mudanca aceita

void setup() {
  Serial.begin(115200);
  pinMode(BOTAO, INPUT_PULLUP);
  pinMode(LED_VERDE, OUTPUT);
}

void loop() {
  int leitura = digitalRead(BOTAO);

  // so reage se MUDOU e se passou tempo suficiente desde a ultima mudanca
  if (leitura != estadoAnterior && millis() - ultimaMudanca > DEBOUNCE) {
    ultimaMudanca = millis();
    estadoAnterior = leitura;

    if (leitura == LOW) {            // so na DESCIDA: o momento do aperto
      ledLigado = !ledLigado;
      digitalWrite(LED_VERDE, ledLigado);
      Serial.println(ledLigado ? "LED ligado" : "LED desligado");
    }
  }
}
