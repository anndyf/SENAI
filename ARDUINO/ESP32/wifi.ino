#include <WiFi.h>

const char* ssid  = "NOME_DA_REDE";
const char* senha = "SENHA_DA_REDE";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, senha);
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado!");
  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.localIP());
}

void loop() { }


//descobrir o ip do pc: ipconfig getifaddr en0
