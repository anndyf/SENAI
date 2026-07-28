/*
  Leitura básica de Temperatura e Umidade com DHT11
  - O DHT11 se comunica por um único fio de dados (protocolo próprio),
    diferente do potenciômetro/LDR que usam analogRead().
  - A biblioteca DHT.h já decodifica esse sinal e entrega os valores
    prontos em Celsius (temperatura) e % (umidade).
*/

#include <DHT.h>

#define DHT_PIN  2       // Pino digital ligado ao DATA do sensor
#define DHT_TYPE DHT11   // Informa à biblioteca qual modelo estamos usando

DHT dht(DHT_PIN, DHT_TYPE); // Cria o objeto do sensor

void setup() {
  Serial.begin(9600);
  dht.begin(); // Inicializa a comunicação com o sensor
}

void loop() {
  // O DHT11 é lento: leva ~250ms a 2s entre leituras confiáveis
  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature(); // Celsius por padrão

  // Verifica se a leitura falhou (é comum acontecer de vez em quando)
  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Falha ao ler o sensor DHT11!");
    return; // Pula essa rodada do loop e tenta de novo na próxima
  }

  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.print("% | Temperatura: ");
  Serial.print(temperatura);
  Serial.println("°C");

  delay(2000); // O DHT11 não deve ser lido mais rápido que isso
}   
