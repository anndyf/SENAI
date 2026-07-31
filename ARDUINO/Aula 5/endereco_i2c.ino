#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  
  // Aguarda a inicialização da porta serial
  while (!Serial); 
  
  Serial.println("\n--- Varredor de Endereço I2C ---");
}

void loop() {
  byte error, address;
  int nDevices = 0;

  Serial.println("Escaneando barramento I2C...");

  // O protocolo I2C usa endereços de 7 bits (de 1 a 127)
  for (address = 1; address < 127; address++) {
    // Tenta iniciar a comunicação no endereço atual
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Dispositivo I2C encontrado no endereço: 0x");
      if (address < 16) {
        Serial.print("0"); // Adiciona o zero à esquerda para endereços menores que 0x10
      }
      Serial.print(address, HEX);
      Serial.println(" !");

      nDevices++;
    } 
    else if (error == 4) {
      Serial.print("Erro desconhecido no endereço: 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }    
  }

  if (nDevices == 0) {
    Serial.println("Nenhum dispositivo I2C encontrado.\n");
  } else {
    Serial.println("Escaneamento concluído.\n");
  }

  // Aguarda 5 segundos antes de escanear novamente
  delay(5000);           
}
