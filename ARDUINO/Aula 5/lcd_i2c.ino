#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Definir o endereço I2C do display (Geralmente 0x27 ou 0x3F)
// Se o seu visor tiver tamanho diferente de 16x2, ajuste os números (ex: 20, 4)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Frase que será exibida no carrossel
// Nota: Removemos os acentos, pois os LCDs padrão HD44780 não suportam caracteres acentuados nativamente.
String texto = "Sistemas Eletronicos e Microcontrolados";

void setup() {
  lcd.init();          // Inicializa o LCD
  lcd.backlight();     // Liga a luz de fundo (backlight)
  
  // Mensagem fixa na primeira linha (opcional)
  lcd.setCursor(0, 0);
  lcd.print("   PROJETO IR  "); 
}

void loop() {
  // Adiciona espaços ao final para dar um espaço visual antes de reiniciar
  String textoEspacado = texto + "   ";
  
  // Garante que haja espaços antes do texto para que ele "entre" no display da direita para a esquerda
  String textoParaExibir = "                " + textoEspacado; 
  
  // Percorre todo o tamanho da mensagem
  for (int i = 0; i < textoParaExibir.length() - 16; i++) {
    lcd.setCursor(0, 1); // Define a posição na segunda linha (Linha 1)
    
    // Recorta e exibe apenas os 16 caracteres visíveis no momento
    lcd.print(textoParaExibir.substring(i, i + 16));
    
    // Velocidade do carrossel (menor valor = mais rápido)
    delay(300); 
  }
}
