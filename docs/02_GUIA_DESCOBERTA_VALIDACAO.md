# Guia de Descoberta & Roteiro de Validação — DevHub C++
**Metodologia HOUS3**  
**Objetivo:** Transformar hipóteses e lacunas `[PENDENTE]` em dados concretos por meio de 3 a 5 entrevistas com desenvolvedores C++ em atividade.

---

## 1. Regras de Condução da Entrevista (Baseado em The Mom Test)

1. **Nunca pergunte se o desenvolvedor "usaria uma ferramenta que faz X":** As pessoas sempre dizem sim por cortesia.
2. **Pergunte sempre sobre fatos e ações reais do passado recente:** *"Quando foi a última vez que você...", "Como você resolveu...", "Quanto tempo levou..."*
3. **Preste atenção em workarounds e gambiarras existentes:** Se o desenvolvedor já criou um script batch, um bookmark no navegador ou um alias no terminal, a dor é real. Se ele não fez nada, a dor pode ser tolerável.
4. **Deixe o usuário falar 80% do tempo:** Não venda a ideia do DevHub durante a entrevista de descoberta.

---

## 2. Roteiro de Entrevista Qualitativa

### Bloco A: Contexto e Volume (10 minutos)
1. *Quantos projetos C++ você diria que tem no seu computador hoje? (Entre trabalho, pet projects e estudos)*
2. *Como esses projetos estão distribuídos em discos e pastas? Você tem um padrão rígido (ex.: `C:\dev\`) ou eles acabam se espalhando?*
3. *Em um dia normal de trabalho, entre quantos projetos diferentes você precisa alternar?*
4. *Pode me descrever o passo a passo exato desde o momento em que você decide mexer no projeto X até o momento em que o código está pronto para rodar?*

### Bloco B: Ferramental e Heterogeneidade (10 minutos)
5. *Quais IDEs e editores você usa para C++ no dia a dia? Você usa a mesma ferramenta para todos os projetos ou ferramentas diferentes para projetos diferentes? Por quê?*
6. *Como você sabe qual gerador de CMake, compilador ou versão do C++ aquele projeto específico exige quando você volta nele depois de semanas sem mexer?*
7. *Você já abriu um projeto na IDE errada ou com a toolchain errada? O que aconteceu e como percebeu?*

### Bloco C: Validação de Hipóteses de Risco (15 minutos)

#### Teste da Hipótese: "Executar Build pelo DevHub" (F06)
8. *Onde e como você costuma compilar seus projetos? (Linha de comando pura, atalho da IDE, script customizado?)*
9. *Quando ocorre um erro de compilação, como você investiga? Você preferiria iniciar uma compilação fora da IDE ou isso atrapalharia seu fluxo de depuração?*
   * **Critério de Validação:** Se a maioria afirmar que nunca compila fora da IDE ou que precisa do clique no log de erro para saltar na linha do código, a funcionalidade de Build deve ser rebaixada para atalho ou descartada do MVP.

#### Teste da Hipótese: "Status Git no DevHub" (F07)
10. *Como você acompanha o estado do Git dos seus projetos hoje? (GitLens, terminal com starship/oh-my-posh, GUI como Fork/GitKraken?)*
11. *Com que frequência você esquece branches com alterações pendentes em projetos locais? Você sente falta de um alerta sobre isso antes de abrir o projeto?*
    * **Critério de Validação:** Identificar se o valor do Git no DevHub é apenas saber a branch atual (baixo valor) ou avisar sobre trabalho pendente esquecido / dirty state (médio valor).

#### Teste da Hipótese: "Templates de Projetos" (F08)
12. *Quando você inicia um novo projeto C++, como você faz? Começa do zero, copia e cola uma pasta anterior, usa um gerador CLI (ex.: cmake-init) ou clona um template do GitHub?*
13. *Com que frequência no último ano você criou um projeto C++ totalmente novo?*
    * **Critério de Validação:** Se a frequência for menor que 1 vez a cada dois meses e a cópia manual atender sem estresse, a funcionalidade de Templates deve ficar no Backlog.

---

## 3. Matriz de Tabulação das Pendências

Preencher esta tabela durante as entrevistas para fechar as pendências do documento de personas:

| Pergunta / Pendência | Resposta Desenvolvedor 1 | Resposta Desenvolvedor 2 | Resposta Desenvolvedor 3 | Síntese / Conclusão |
|---|---|---|---|---|
| Quantidade média de projetos simultâneos | | | | |
| Diretório único vs pastas espalhadas | | | | |
| Frequência de alternância entre projetos | | | | |
| IDEs principais utilizadas | | | | |
| Compilação fora da IDE agrega valor? | | | | |
| Consulta Git no dashboard é útil? | | | | |
| Criação de projetos por templates é frequente? | | | | |
| Scripts/Workarounds já existentes | | | | |

---

## 4. Gatilhos de Decisão para a Release MVP

- **Se o Desenvolvedor compila 100% via IDE:** F06 (Build) vai para o **Backlog** como melhoria futura.
- **Se o Desenvolvedor raramente cria projetos novos:** F08 (Templates) vai para o **Backlog**.
- **Se o Desenvolvedor valoriza encontrar e abrir rapidamente:** F01, F02, F03 e F04 formam o **Núcleo de Ouro do MVP (5 BP cada)**.
