# Primeiros dados com o ESP32

**Aula 8 · Sistemas Eletrônicos e Microcontrolados**
Vamos mandar o dado de um sensor para o computador, para a rede local e para a internet. Do mais perto ao mais longe.

| Etapa | O que fazemos |
|---|---|
| 1. No computador | Ler o sensor e ver na tela |
| 2. Na rede local | Conectar o ESP32 ao Wi-Fi |
| 3. Na internet | Enviar o dado para um endereço na web |
| 4. Extra | O ESP32 serve uma página HTML na rede local |

## O que o ESP32 enxerga quando você gira o botão?

Ele lê uma tensão no pino e transforma em um número inteiro de **0 a 4095**.
Para ficar fácil de ler, convertemos esse número para uma escala de **0 a 100** (como uma porcentagem).

## O que você precisa na bancada

- Placa ESP32 e cabo USB
- Protoboard e três jumpers
- Potenciômetro (botão giratório)
- Computador com a IDE Arduino instalada
- Rede Wi-Fi de 2,4 GHz disponível

---

# Etapa 1: no computador

## Passo 1.1: Monte o circuito

> **Com o cabo USB desconectado.**

| Pino do potenciômetro | Vai para |
|---|---|
| Pino lateral 1 | 3V3 do ESP32 |
| Pino do meio | GPIO 34 |
| Pino lateral 2 | GND do ESP32 |

## Passo 1.2: Escolha a placa e a porta

1. Abra a IDE Arduino.
2. Conecte o cabo USB ao ESP32.
3. Em Ferramentas › Placa, escolha **ESP32 Dev Module**.
4. Em Ferramentas › Porta, escolha a porta do ESP32.

## Passo 1.3: Cole o Código A

Apague o que estiver no editor e cole este código:

```cpp
const int PINO_POT = 34;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int leitura = analogRead(PINO_POT);
  int nivel = map(leitura, 0, 4095, 0, 100);
  Serial.print("nivel:");
  Serial.println(nivel);
  delay(500);
}
```

## Passo 1.4: Envie para a placa

1. Clique no botão **Carregar** (seta à direita, no topo da IDE).
2. Aguarde a mensagem "Carregado" aparecer embaixo.

## Passo 1.5: Veja o dado na tela

1. Abra Ferramentas › **Monitor Serial**.
2. Ajuste a velocidade para **115200 baud**.
3. Gire o potenciômetro e veja o número mudar de 0 a 100.

## Destrinchando o Código A

| Código | O que faz |
|---|---|
| `const int PINO_POT = 34;` | Define que o sensor está no pino 34. |
| `Serial.begin(115200);` | Abre a conversa com o computador. |
| `analogRead(PINO_POT);` | Lê a tensão como número de 0 a 4095. |
| `map(leitura, 0, 4095, 0, 100);` | Converte esse número para 0 a 100. |
| `Serial.println(nivel);` | Envia o valor para a tela. |
| `delay(500);` | Espera meio segundo e repete. |

---

# Etapa 2: na rede local

## Passo 2.1: Cole o Código B e coloque o Wi-Fi

Troque `NOME_DA_REDE` e `SENHA_DA_REDE` pelos da sua rede, mantendo as aspas.

> ⚠ **Somente redes de 2,4 GHz.**

```cpp
const char* ssid     = "NOME_DA_REDE";
const char* password = "SENHA_DA_REDE";

// dentro de setup():
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
  delay(500); Serial.print(".");
}
Serial.println(WiFi.localIP());
```

## Passo 2.2: Envie e confira o IP

1. Carregue o Código B e abra o Monitor Serial.
2. Devem aparecer alguns pontos e depois um endereço do tipo `192.168.x.x`.
3. **Qual é o IP que apareceu na sua tela?**

---

# Etapa 3: na internet

## Passo 3.1: Crie sua caixa de correio na web

1. Abra **webhook.site** no navegador.
2. Copie a *Your unique URL* que aparece no topo.

> ⚠ Gratuito e sem cadastro, mas **não feche essa aba**. A URL gratuita expira em 7 dias.
> ⚠ A URL é só sua, não compartilhe.

## Passo 3.2: Teste pelo navegador

1. Na barra de endereço, cole sua URL e some `?nivel=42` no final.
2. Volte à aba do webhook.site: a requisição deve aparecer na lista.

```
https://webhook.site/SEU-ID?nivel=42
```

## Passo 3.3: Cole o Código C com a sua URL

- Troque `COLE-SEU-ID-AQUI` pela sua URL.
- Coloque também o nome e a senha do Wi-Fi.

```cpp
const char* url = "http://webhook.site/COLE-SEU-ID-AQUI";

// dentro de loop(), a cada 15 segundos:
int nivel = map(analogRead(PINO_POT), 0, 4095, 0, 100);
String endereco = String(url) + "?nivel=" + String(nivel);

HTTPClient http;
http.begin(endereco);
int codigo = http.GET();
http.end();
```

### Código C completo (pronto para colar na IDE)

> Esta versão junta os códigos A, B e C num só programa, com os `#include` necessários.
> O código completo original está no material da aula; confira se bate com o seu.

```cpp
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid     = "NOME_DA_REDE";
const char* password = "SENHA_DA_REDE";
const char* url      = "http://webhook.site/COLE-SEU-ID-AQUI";

const int PINO_POT = 34;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println(WiFi.localIP());
}

void loop() {
  int nivel = map(analogRead(PINO_POT), 0, 4095, 0, 100);
  String endereco = String(url) + "?nivel=" + String(nivel);

  HTTPClient http;
  http.begin(endereco);
  int codigo = http.GET();
  http.end();

  Serial.print("nivel:");
  Serial.print(nivel);
  Serial.print("  HTTP:");
  Serial.println(codigo);

  delay(15000);
}
```

## Passo 3.4: Envie e confira

1. Carregue o Código C na placa.
2. No Monitor Serial: veja o nível sendo enviado.
3. No webhook.site: veja a requisição chegando a cada 15 s.

## Destrinchando o Código C

| Código | O que faz |
|---|---|
| `const char* url = "...";` | Guarda sua caixa de correio na internet. |
| `analogRead + map` | Lê o sensor e converte para 0 a 100. |
| `String(url) + "?nivel=" + ...` | Monta o endereço com o valor no final. |
| `HTTPClient http;  http.begin(...)` | Prepara a chamada para o endereço. |
| `http.GET();` | Envia o dado pela internet. |
| `http.end();` | Fecha a conexão para liberar memória. |

---

# Etapa 4 (extra): o ESP32 serve uma página HTML

Até aqui a placa só **enviava** dados. Agora ela vira o **servidor**: o navegador de um computador ou celular, **na mesma rede Wi-Fi**, pede a página e a placa responde com o nível do potenciômetro.

## Passo 4.1: Cole o Código D e coloque o Wi-Fi

Troque `NOME_DA_REDE` e `SENHA_DA_REDE` pelos da sua rede. Rede de **2,4 GHz**.

```cpp
#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "NOME_DA_REDE";
const char* password = "SENHA_DA_REDE";

const int PINO_POT = 34;

WebServer server(80);   // porta 80 = porta padrão de páginas web

// A página HTML que o navegador vai receber
const char PAGINA[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 - Nível</title>
  <style>
    body  { font-family: Arial, sans-serif; text-align: center; margin-top: 60px; }
    h1    { color: #1f4e79; }
    #num  { font-size: 90px; font-weight: bold; color: #1f4e79; }
    #fundo{ width: 80%; max-width: 500px; height: 40px; margin: 20px auto;
            background: #e7f0f9; border-radius: 6px; overflow: hidden; }
    #barra{ height: 100%; width: 0%; background: #5b9bd5; transition: width 0.4s; }
  </style>
</head>
<body>
  <h1>Nível do potenciômetro</h1>
  <div id="num">--</div>
  <div id="fundo"><div id="barra"></div></div>
  <p>Atualiza a cada 1 segundo</p>

  <script>
    // De 1 em 1 segundo, pede o valor novo para o ESP32
    setInterval(function () {
      fetch("/nivel")
        .then(function (r) { return r.text(); })
        .then(function (valor) {
          document.getElementById("num").textContent = valor;
          document.getElementById("barra").style.width = valor + "%";
        });
    }, 1000);
  </script>
</body>
</html>
)rawliteral";

int lerNivel() {
  return map(analogRead(PINO_POT), 0, 4095, 0, 100);
}

// Quando o navegador pede o endereço principal "/", manda a página
void paginaInicial() {
  server.send(200, "text/html", PAGINA);
}

// Quando a página pede "/nivel", manda só o número
void enviarNivel() {
  server.send(200, "text/plain", String(lerNivel()));
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println();
  Serial.print("Abra no navegador: http://");
  Serial.println(WiFi.localIP());

  server.on("/", paginaInicial);
  server.on("/nivel", enviarNivel);
  server.begin();
}

void loop() {
  server.handleClient();   // atende os pedidos do navegador
}
```

## Passo 4.2: Envie e copie o IP

1. Carregue o Código D na placa.
2. Abra o Monitor Serial (115200 baud).
3. Copie o endereço `http://192.168.x.x` que aparece.

## Passo 4.3: Abra a página no navegador

1. No computador ou celular **ligado na mesma rede Wi-Fi da placa**, abra o endereço copiado.
2. Gire o potenciômetro e veja o número e a barra mudando a cada segundo.

## Destrinchando o Código D

| Código | O que faz |
|---|---|
| `WebServer server(80)` | A placa agora é o **servidor**; o navegador é o cliente. |
| `server.on("/", ...)` | Cada endereço (rota) chama uma função. `/` entrega a página. |
| `server.on("/nivel", ...)` | Rota que devolve só o número, para a página atualizar sem recarregar. |
| `fetch("/nivel")` no JavaScript | A página pergunta o valor novo ao ESP32 a cada segundo. |
| `server.handleClient()` | Precisa rodar sempre no `loop()` para atender os pedidos. |

## Página web × Webhook: quando usar cada um?

Os dois mostram o dado do sensor, mas funcionam de jeitos opostos.

| | **Página web no ESP32** (Código D) | **Webhook** (Código C) |
|---|---|---|
| Quem é o servidor? | O próprio ESP32 | O webhook.site, na internet |
| Quem inicia a conversa? | O navegador pede, e a placa responde | A placa envia o dado quando quiser |
| Onde funciona? | Só na mesma rede Wi-Fi | De qualquer lugar com internet |
| O dado fica guardado? | Não. Mostra só o valor de agora | Sim. Cada envio vira uma linha no histórico |
| Quando usar? | Ver o valor ao vivo, na bancada ou em sala | Registrar dados e acompanhar à distância |

**Como escolher**

- **Só quer ver o valor agora**, no celular ou no computador, com todo mundo na mesma rede? Use a **página no ESP32**.
- **Quer guardar o histórico, acompanhar de longe ou entregar o dado a outro sistema?** Use o **webhook**.
- Na indústria, o mais comum é o caminho do webhook (a máquina envia, e um sistema central recebe e guarda). A página local serve para diagnóstico e ajuste na própria máquina.

**Cuidados de cada um**

| Página web no ESP32 | Webhook |
|---|---|
| O IP pode mudar quando a placa reconecta ao Wi-Fi | A URL gratuita expira em 7 dias |
| Se a placa estiver desligada, a página não abre | Se a placa estiver sem internet, nada chega |
| Qualquer um na mesma rede consegue abrir a página | Quem tiver a URL consegue ver os dados: não compartilhe |

---

## Se não funcionar

| Problema | Solução |
|---|---|
| Nada aparece no Monitor Serial | Confira a velocidade: 115200 baud |
| Fica em "Carregando..." para sempre | Pressione o botão BOOT da placa ao enviar |
| Não conecta no Wi-Fi | Use rede 2,4 GHz e revise nome e senha |
| Nada chega no webhook.site | Confira a URL colada; teste pelo navegador |
| A página do Código D não abre | Confira se o aparelho está na **mesma rede Wi-Fi** da placa e se o IP digitado está certo |
| A página abre, mas o número fica `--` | Recarregue a página e confira o IP |

## E se o servidor quiser mandar um comando para a placa?

Hoje, não dá. Pela internet, o ESP32 só envia; o servidor web não consegue chamar a placa. Na rede local, o Código D mostra um caminho (o navegador consegue **pedir** dados à placa). Na próxima aula vamos resolver o caso geral com **MQTT**.

---

# Atividade principal

## Atividade: novas variáveis na página do ESP32

Parta do **Código D** e acrescente a leitura de **pelo menos duas novas variáveis**, mostrando-as na mesma página, na rede local.

**Sugestões de variáveis**

| Variável | Como ler | Observação |
|---|---|---|
| Segundo potenciômetro | `analogRead(35)` + `map(...)` | Ligar no GPIO 35 (3V3, sinal, GND) |
| Botão | `digitalRead(4)` com `pinMode(4, INPUT_PULLUP)` | Mostrar "pressionado" ou "solto" |
| Força do Wi-Fi | `WiFi.RSSI()` | Valor em dBm (quanto mais perto de 0, melhor) |
| Tempo ligado | `millis() / 1000` | Segundos desde que a placa ligou |

## Atividade de fixação (entrega)

1. Responda no caderno, numerando as questões.
2. Fotografe a página.
3. Poste no Classroom.
4. Anexe o print do webhook.site com pelo menos 3 valores recebidos.
