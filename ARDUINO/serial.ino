/*
  Demonstração de Serial.begin(), Serial.print() e Serial.println()
  - Não usa nenhum sensor ou componente externo.
  - Basta ligar o Arduino via USB e abrir o Monitor Serial
    (Ferramentas > Monitor Serial, ou Ctrl+Shift+M).
  - IMPORTANTE: o baud rate selecionado no Monitor Serial
    precisa ser 9600, igual ao definido no código.
*/

int contador = 0; // Variável simples só para gerar valores diferentes

void setup() {
  Serial.begin(9600); // Abre a comunicação serial a 9600 bps

  Serial.println("=== Demonstração de Comunicação Serial ===");
  Serial.println();
}

void loop() {
  // --- print() x println() ---
  Serial.print("Contador: ");   // Fica na mesma linha
  Serial.println(contador);     // Fecha a linha

  // --- Formatação de números ---
  Serial.print("Em HEX: ");
  Serial.println(contador, HEX);

  Serial.print("Em BIN: ");
  Serial.println(contador, BIN);

  // --- Concatenando texto e valores na mesma linha ---
  Serial.print("O dobro de ");
  Serial.print(contador);
  Serial.print(" é ");
  Serial.println(contador * 2);

  // --- Um separador visual entre as leituras ---
  Serial.println("-------------------------");

  contador++;       // Incrementa o contador a cada volta do loop
  delay(1000);      // Espera 1 segundo antes de repetir
}
