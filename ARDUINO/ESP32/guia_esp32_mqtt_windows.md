# Node-RED Recebendo os Dados do ESP32 (Windows)

**SENAI · Sistemas Eletrônicos e Microcontroladores · Aula 9 (parte 1)**

Neste guia você vai instalar o **Node-RED** e montar um fluxo que recebe os dados que o ESP32 publica via MQTT. Também vai comandar o LED da placa pelo Node-RED.

---

## Antes de começar

Confira se a Aula 8 está funcionando:

- [ ] ESP32 ligado e publicando a cada 5 segundos (veja no Serial Monitor: `Publicado: {...}`)
- [ ] Dados do ESP32 aparecendo no cliente web da HiveMQ (tópico `senai_ssa_2026/#`)
- [ ] Você sabe o **seu número de aluno** usado no código (ex.: `aluno07`)

## O que é o Node-RED?

O Node-RED é uma ferramenta de **programação visual**. Em vez de escrever código, você arrasta **nós** (blocos) e liga um ao outro com fios. As mensagens viajam pelos fios dentro de um objeto chamado **msg**, e o conteúdo principal fica em **msg.payload**.

O fluxo que vamos montar:

```
[mqtt in] ──► [json] ──► [debug]
```

| Nó | Função |
|---|---|
| **mqtt in** | Assina um tópico no broker e recebe as mensagens do ESP32 |
| **json** | Garante que o texto JSON vire um objeto com campos separados |
| **debug** | Mostra as mensagens na barra lateral |

---

## Etapa 1 — Instalar o Node.js

O Node-RED roda sobre o **Node.js**, então ele vem primeiro.

1. Acesse **nodejs.org**.
2. Baixe a versão **LTS** (botão da esquerda, "Recomendado para a maioria").
3. Execute o instalador `.msi` e aceite as opções padrão (Next → Next → Install).
   - Na tela "Tools for Native Modules", **não** precisa marcar a caixa.
4. Ao terminar, abra um **Prompt de Comando** novo: aperte **Windows + R**, digite `cmd` e aperte Enter.
5. Confirme a instalação:

   ```
   node --version
   ```

   Deve aparecer algo como `v22.x.x`. A versão precisa ser **18 ou maior**.

> **Use o Prompt de Comando (cmd), não o PowerShell.** No PowerShell pode aparecer o erro "a execução de scripts foi desabilitada neste sistema" ao usar `npm` ou `node-red`.

---

## Etapa 2 — Instalar e iniciar o Node-RED

1. No Prompt de Comando, digite:

   ```
   npm install -g node-red
   ```

   Aguarde terminar. No final aparece uma linha como `added 277 packages`.

   > Avisos amarelos (`npm warn`) podem ser ignorados. Só se preocupe com linhas vermelhas escritas `ERR!`.

2. Inicie o Node-RED:

   ```
   node-red
   ```

3. Aguarde aparecer a linha:

   ```
   Server now running at http://127.0.0.1:1880/
   ```

4. Se o Windows perguntar se o **Node.js** pode acessar a rede, marque **Redes privadas** e clique em **Permitir acesso**.

5. **Deixe esta janela aberta** enquanto usa o Node-RED. Se fechar, o Node-RED para.

6. Abra o navegador e acesse: **http://localhost:1880**

<!-- PRINT: tela inicial do Node-RED -->
![Tela inicial do Node-RED](imagens/aula9_00_editor.png)

### Conhecendo a tela

| Área | Onde fica | Para que serve |
|---|---|---|
| **Paleta** | Esquerda | Todos os nós disponíveis, separados por categoria |
| **Área de trabalho** | Centro | Onde você monta o fluxo |
| **Barra lateral** | Direita | Aba **Debug** (🐞) mostra as mensagens |
| **Deploy** | Botão vermelho, canto superior direito | Coloca o fluxo para funcionar. **Nada funciona antes do Deploy!** |

---

## Etapa 3 — Montar o fluxo

### Passo 1 — Criar uma aba para o projeto

1. Clique no botão **+** no topo da área de trabalho para criar uma nova aba.
2. Dê duplo clique no nome da aba e renomeie para **Aula 9 - ESP32**.
3. Clique em **Done**.

<!-- PRINT: passo 1 -->
![Passo 1 - nova aba](imagens/aula9_01_aba.png)

### Passo 2 — Arrastar os nós

Arraste da paleta da esquerda para a área central, da esquerda para a direita:

1. **mqtt in**: na seção **network**
2. **json**: na seção **parser**
3. **debug**: na seção **common**

> Dica: se não achar um nó, digite o nome no campo **filter nodes**, no topo da paleta.

<!-- PRINT: passo 2 -->
![Passo 2 - nós na área de trabalho](imagens/aula9_02_nos.png)

### Passo 3 — Ligar os nós

1. Clique na **bolinha cinza da direita** do nó **mqtt in** e arraste até a **bolinha da esquerda** do nó **json**.
2. Faça o mesmo do **json** para o **debug**.

O resultado é `mqtt in → json → debug`.

<!-- PRINT: passo 3 -->
![Passo 3 - nós ligados](imagens/aula9_03_ligados.png)

### Passo 4 — Configurar o mqtt in

1. Dê **duplo clique** no nó **mqtt in**.
2. No campo **Server**, clique no **lápis ✏️** ao lado de "Add new mqtt-broker...".
3. Preencha:
   - **Server:** `broker.hivemq.com`
   - **Port:** `1883`
4. Clique em **Add**.
5. De volta à janela do nó, preencha:
   - **Topic:** `senai_ssa_2026/aula8/+/sensor`
   - **Name:** `ESP32 sensores`
6. Clique em **Done**.

> **O que significa o `+` no tópico?** Ele é um curinga que substitui um nível do tópico. `senai_ssa_2026/aula8/+/sensor` recebe os dados de `aluno01`, `aluno02`, `aluno03`... ou seja, de **toda a turma**. Para ver só o seu ESP32, troque o `+` pelo seu número (ex.: `senai_ssa_2026/aula8/aluno07/sensor`).

<!-- PRINT: passo 4 -->
![Passo 4 - configuração do mqtt in](imagens/aula9_04_mqtt_in.png)

### Passo 5 — Configurar o json

1. Dê **duplo clique** no nó **json**.
2. Em **Action**, escolha **Always convert to JavaScript Object**.
3. Clique em **Done**.

> **Por que essa opção?** As versões atuais do Node-RED já podem converter o JSON no próprio mqtt in. Com a opção padrão ("Convert between JSON String & Object"), o nó json faria o caminho inverso e transformaria o objeto de volta em texto. Com "Always convert to JavaScript Object", o resultado sai certo nos dois casos.

<!-- PRINT: passo 5 -->
![Passo 5 - configuração do json](imagens/aula9_05_json.png)

### Passo 6 — Configurar o debug

1. Dê **duplo clique** no nó **debug**.
2. Em **Name**, digite `Dados do ESP32`.
3. Clique em **Done**.

### Passo 7 — Fazer o Deploy

1. Clique no botão vermelho **Deploy** (canto superior direito).
2. Abaixo do nó **mqtt in** deve aparecer um quadradinho **verde** escrito **connected**.

<!-- PRINT: passo 7 -->
![Passo 7 - deploy e connected](imagens/aula9_07_deploy.png)

> **Apareceu "disconnected" ou "connecting" em vermelho/amarelo?** O Node-RED não alcançou o broker. Confira se o Server é `broker.hivemq.com`, porta `1883`, e se o PC tem internet.

### Passo 8 — Ver os dados chegando

1. Na barra lateral direita, clique no ícone de **inseto 🐞** (aba **Debug**).
2. A cada 5 segundos chega uma mensagem de cada ESP32 ligado.
3. Clique na setinha ao lado de uma mensagem para expandir. Você vê os campos **temperatura** e **umidade** separados, o que confirma que a mensagem é um **objeto**.

<!-- PRINT: passo 8 -->
![Passo 8 - dados na aba Debug](imagens/aula9_08_debug.png)

✅ **Etapa 3 concluída quando:** os dados do ESP32 aparecem na aba Debug.

---

## Etapa 4 — Bônus: comandar o LED pelo Node-RED

Até agora o Node-RED só **recebe**. Agora ele também vai **enviar** comandos.

### Passo 9 — Montar o fluxo do LED

Abaixo do fluxo anterior, arraste:

- **2 nós inject** (seção **common**)
- **1 nó mqtt out** (seção **network**)

Configure cada um com duplo clique:

**Primeiro inject:**
1. Em **msg.payload**, clique na setinha do tipo e escolha **string** (ícone `az`).
2. No valor, digite `ON`.
3. Em **Name**, digite `LED ON`.
4. Clique em **Done**.

**Segundo inject:** faça igual, com valor `OFF` e nome `LED OFF`.

**mqtt out:**
1. Em **Server**, escolha o mesmo broker configurado no Passo 4.
2. Em **Topic**, digite `senai_ssa_2026/aula8/alunoXX/led`, **trocando `alunoXX` pelo seu número**.
3. Em **Name**, digite `Meu LED`.
4. Clique em **Done**.

Ligue **os dois inject** à entrada do **mqtt out**.

<!-- PRINT: passo 9 -->
![Passo 9 - fluxo do LED](imagens/aula9_09_led.png)

> **Atenção:** confira o número no tópico do mqtt out. Se você usar o número de um colega, vai acender o LED **dele**!

### Passo 10 — Testar

1. Clique em **Deploy**.
2. Clique no **quadradinho à esquerda** do nó **LED ON**. O LED azul da sua placa acende.
3. Clique no quadradinho do **LED OFF**. O LED apaga.

<!-- PRINT: passo 10 -->
![Passo 10 - fluxo completo](imagens/aula9_10_final.png)

✅ **Etapa 4 concluída quando:** o LED acende e apaga pelos botões do Node-RED.

---

## Atalho: importar o fluxo pronto

Se quiser conferir o seu fluxo com a versão pronta (ou recuperar depois de um erro), o professor disponibiliza o arquivo `fluxo_aula9_hivemq.json`.

1. No Node-RED, clique no menu **☰** (canto superior direito) → **Import**.
2. Clique em **select a file to import** e escolha o arquivo.
3. Clique em **Import** e clique na área de trabalho para soltar os nós.
4. Dê duplo clique no **mqtt out** e troque `aluno01` pelo seu número.
5. Clique em **Deploy**.

---

## Problemas comuns

| Sintoma | Causa provável | Solução |
|---|---|---|
| `'node' não é reconhecido` | Prompt aberto antes de instalar o Node.js | Feche e abra um **novo** Prompt de Comando |
| `'node-red' não é reconhecido` | Prompt aberto antes de instalar o Node-RED | Feche e abra um **novo** Prompt de Comando |
| "A execução de scripts foi desabilitada" | Comando rodado no PowerShell | Use o **Prompt de Comando (cmd)** |
| Página localhost:1880 não abre | Janela do Node-RED foi fechada | Rode `node-red` novamente e deixe a janela aberta |
| `Error: port 1880 in use` | Node-RED já está rodando em outra janela | Use a janela que já está aberta ou feche-a e rode de novo |
| mqtt in mostra "disconnected" | Servidor errado ou rede bloqueando | Confira `broker.hivemq.com` e porta `1883` no Passo 4; na escola, avise o professor |
| Nada aparece no Debug | Esqueceu o Deploy, ESP32 desligado ou tópico errado | Clique em **Deploy**; confira o Serial Monitor e o tópico `senai_ssa_2026/aula8/+/sensor` |
| Debug mostra texto `"{\"temperatura\"..."` | Nó json na opção errada | No json, use **Always convert to JavaScript Object** |
| LED não responde | Número do aluno errado no tópico do mqtt out | Confira `senai_ssa_2026/aula8/alunoXX/led` com o **seu** número |

---

## Checklist final

- [ ] `node --version` mostra versão 18 ou maior
- [ ] Node-RED aberto em http://localhost:1880
- [ ] mqtt in com **connected** em verde
- [ ] Dados chegando na aba Debug com temperatura e umidade separadas
- [ ] LED acendendo e apagando pelos botões LED ON e LED OFF

**Próximo passo:** trocar o nó **debug** por um nó que **grava os dados no banco InfluxDB**, para guardar o histórico e depois criar gráficos no Grafana.
