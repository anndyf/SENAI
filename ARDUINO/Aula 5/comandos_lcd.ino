#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Definindo o endereço I2C do display (geralmente 0x27 ou 0x3F) e o tamanho (16 colunas x 2 linhas)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();       // Inicializa a comunicação com o display LCD
  lcd.backlight();  // Liga a luz de fundo (Backlight)
}

void loop() {
  // =================================================================
  // 1. DEMONSTRAÇÃO DO setCursor(coluna, linha)
  // =================================================================
  lcd.clear(); // Limpa o display antes de começar
  
  // Posiciona o cursor na Coluna 0, Linha 0 (Início da primeira linha)
  lcd.setCursor(0, 0);
  lcd.print("Linha 0, Col 0");

  // Posiciona o cursor na Coluna 0, Linha 1 (Início da segunda linha)
  lcd.setCursor(0, 1);
  lcd.print("Linha 1, Col 0");
  
  delay(3000);

  // Escrevendo em posições personalizadas (centralizando texto)
  lcd.clear();
  
  lcd.setCursor(3, 0); // Avança 3 colunas na primeira linha
  lcd.print("SISTEMA OK");

  lcd.setCursor(1, 1); // Avança 1 coluna na segunda linha
  lcd.print("Temp: 25.5 C");

  delay(3000);


  // =================================================================
  // 2. DEMONSTRAÇÃO DO backlight() e noBacklight()
  // =================================================================
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("ALERTA VISUAL!");
  lcd.setCursor(0, 1);
  lcd.print("Piscando Backlight");

  // Simula um alarme/pisca pisca ou modo de economia de energia
  for (int i = 0; i < 5; i++) {
    lcd.noBacklight(); // Desliga a iluminação traseira
    delay(400);
    lcd.backlight();   // Liga a iluminação traseira
    delay(400);
  }

  delay(2000);


  // =================================================================
  // 3. DEMONSTRAÇÃO DO clear()
  // =================================================================
  // Limpa o conteúdo da tela e coloca o cursor de volta em (0, 0)
  lcd.clear(); 
  
  // Como o clear() redefine o cursor para (0, 0), o próximo print sai direto no topo
  lcd.print("Tela limpa!");
  
  lcd.setCursor(0, 1);
  lcd.print("Reiniciando...");

  delay(3000);
}
```eof

### Resumo das Aplicações no Código:

1. **`setCursor(col, lin)`**: Usado para definir exatamente em qual posição o próximo caractere será impresso na tela. Lembre-se que a contagem inicia em `0` (Coluna 0, Linha 0 = Canto superior esquerdo).
2. **`backlight()` e `noBacklight()`**: Utilizados no laço `for` para fazer a tela piscar (criar alertas) e demonstrar como desligar a iluminação do visor quando necessário economizar energia.
3. **`clear()`**: Usado para apagar tudo que estava escrito na tela e redefinir a posição do cursor automaticamente para `(0,0)`.

*Dica: Certifique-se de ter a biblioteca **LiquidCrystal_I2C** instalada no seu Arduino IDE.*
