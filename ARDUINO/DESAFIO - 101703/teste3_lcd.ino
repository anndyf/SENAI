/*
  TESTE 3: LCD 16x2 (SEM I2C — ligação paralela de 4 bits)
  Biblioteca: LiquidCrystal (já vem com a Arduino IDE, não precisa instalar)

  LIGAÇÃO (pinos do LCD numerados da esquerda para a direita):
   1  VSS -> GND
   2  VDD -> 5V
   3  V0  -> pino do MEIO de um potenciômetro de 10k (contraste)
             (as outras pernas desse potenciômetro: 5V e GND)
   4  RS  -> Arduino 12
   5  RW  -> GND
   6  E   -> Arduino 11
   7 a 10 (D0 a D3) -> não ligar
   11 D4  -> Arduino 5
   12 D5  -> Arduino 4
   13 D6  -> Arduino 3
   14 D7  -> Arduino 2
   15 A   -> 5V através de resistor de 220R (luz de fundo)
   16 K   -> GND

  Resultado esperado: mensagem na linha 1 e um contador na linha 2.

  Problemas comuns:
  - Só aparecem quadradinhos na linha 1: o LCD liga, mas não recebe dados.
    Confira RS, E, D4 a D7 e se RW está no GND.
  - Tela acesa sem nada: gire o potenciômetro de contraste.
  - Tela apagada: confira os pinos 15 e 16 (luz de fundo).
  - Sem potenciômetro de contraste? Ligue o pino 3 (V0) ao GND
    com um resistor de 1k a 2,2k.
*/

#include <LiquidCrystal.h>

//                RS  E  D4 D5 D6 D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int contador = 0;

void setup() {
  lcd.begin(16, 2);          // 16 colunas, 2 linhas

  lcd.setCursor(0, 0);
  lcd.print("Teste do LCD OK!");
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print("Contador: ");
  lcd.print(contador);
  lcd.print("   ");

  contador++;
  delay(1000);
}
