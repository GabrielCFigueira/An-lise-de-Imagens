---
title: Relatório do projeto 2 de ASA - grupo AL040
documentclass: article
author:
- Gabriel Figueira, n. 86426
- Rafael Andrade, n. 86503
papersize: A4
fontsize: 12pt
geometry: margin=3cm
geometry: vmargin=2cm
---

## Introdução

O problema apresentado consiste em segmentar os píxeis de uma imagem,
identificando-os como sendo de primeiro plano ou de cenário. A solução
lê as dimensões da imagem (tamanho $M\times N$), os possíveis pesos para cada
píxel (de primeiro plano -- $\ell_p$ -- e de cenário -- $c_p$), e um peso
possível atribuído às relações de vizinhança ($f_{pp'}$), primeiro para vizinhos
imediatamente à esquerda e direita, e depois para os imediatamente acima e
abaixo. Estes pesos são dados em forma matricial em que as dimensões são
$M\times N$ para os pesos $\ell_p$ e $c_p$, $M\times (N-1)$ para as relações de
vizinhança na horizontal, e $(M-1)\times N$ para as relações verticais.

A solução deve devolver o peso mínimo possível para a imagem, calculado
da seguinte forma:

$$
\normalsize{Peso} =
\left(\sum_{p\in L}\ell_p\right) +
\left(\sum_{p\in C}c_p\right) +
\left(\sum_{v\in V}f_v\right)
$$

em que $L$ é o conjunto dos píxeis de primeiro plano, $C$ é o conjunto dos
de cenário e o $f_v$, em que $v$ liga os píxeis $p$ e $p'$ é calculado da
seguinte forma:

$$
f_v =
\begin{cases}f_{pp'} &  p\in L \wedge p'\in C
             \\0     & c.c.
\end{cases}
$$

Além de mostrar o peso mínimo, também é necessário mostrar a classificação
dos píxeis que realiza o peso mínimo; se houver múltiplas segmentações
possíveis, deve ser mostrada a que maximiza o tamanho de $L$.


## Solução

A imagem pode ser vista como um grafo pesado e não-dirigido, em que os píxeis
são os vértices e as relações de vizinhança são os arcos. Além disso, criamos dois
vértices "virtuais", i.e., que não são píxeis, que são a *source* e o *target*.
O peso dos arcos entre a *source* e os píxeis é $\ell_p$, e entre os píxeis e o
*target* é $c_p$. Estes arcos são dirigidos.

Visto desta maneira, o problema de encontrar uma segmentação torna-se no
problema de encontrar uma bipartição no grafo que minimize o peso -- o que
significa que o problema é, na verdade, encontrar o corte mínimo no grafo,
e calcular a capacidade do corte e mostrar que vértices pertencem a que parte do
corte. E, tendo em conta o teorema do fluxo máximo/corte mínimo, decidimos usar
o algoritmo de *Edmonds-Karp* para resolver este problema.

-----

Dada a estrutura do problema, fizemos algumas alterações a nível de
implementação, descritas em seguida.

Em vez de usarmos uma lista ou matriz de adjacências, tirámos partido do
facto de que um píxel tem como vizinhos, no máximo, os píxeis imediatamente
acima, abaixo, e à esquerda e direita. Portanto, guardámos o *input*, que vinha
em forma de matriz, exatamente no mesmo formato matricial. Por exemplo, quando
queremos aceder à capacidade do arco que liga o píxel $p$ ao seu vizinho da
direita basta ir à posição de $p$ na matriz das relações entre vizinhos; mas se
quisermos a capacidade do arco que liga $p$ ao vizinho da esquerda, temos de ir
consultar a posição do vizinho da esquerda. Isto é possível porque os arcos
entre vizinhos são não-dirigidos, e por isso é ver a capacidade do arco $(p,p')$
e do arco $(p',p)$ é equivalente.

Para guardar o fluxo que passa nos arcos, usamos a mesma estratégia e estrutura
em memória que as capacidades fixas. Existe uma subtileza nesta solução:
nos arcos entre píxeis, que são não dirigidos, a posição onde podemos consultar
o fluxo que vai no sentido de um píxel $p$ para a sua direita é exatamente a
mesma posição onde está guardado o fluxo no sentido da direita de $p$ para $p$,
tal como com as capacidades, mas com o sinal contrário, i.e., se o fluxo é
positivo de $p$ para a sua direita, então logicamente o fluxo é negativo no
sentido contrário e vice-versa.

Além disto, antes de iniciar a procura de caminhos com BFS, percorremos os
píxeis, e tentamos saturar os caminhos de aumento que contêm apenas 1 píxel, ou
seja, os caminhos do tipo {(*source*, $p$), ($p$, *target*)}.

-----


Quando o algoritmo de *Edmonds-Karp* termina, para obter o corte mínimo,
percorremos a matriz que contém o fluxo que sai da *source*, somando os fluxos.
O teorema do corte mínimo/fluxo máximo permite-nos afirmar que a capacidade do
corte mínimo é igual ao fluxo que emana da *source*, e portanto o resultado da
soma é o corte mínimo, e portanto o peso mínimo para a imagem dada como *input*.

Para classificar os píxeis individuais, e pela lógica já exposta anteriormente,
analisamos o resultado da última BFS; mais concretamente, para cada píxel, vemos
se ele foi visitado. Se foi visitado, então classificamo-lo como de cenário ($p
\in C$); senão, afirmamos que o píxel é de primeiro plano ($p\in L$).


Resumidamente, a solução consiste em:

  - Ler o *input* e guardar em matrizes de tamanho apropriado
  - Percorrer uma vez todos os píxeis, saturando caminhos de aumento do tipo
    {(*source*, $p$),($p$, *target*)}
  - Executar o algoritmo de *Edmonds-Karp* no grafo
  - Percorrer uma vez o fluxo das ligações do tipo (*source*, $p$), somando os
    fluxos, e imprimir
  - Percorrer uma vez todos os píxeis, classificando-os como de
    1\textsuperscript{o} plano ou de cenário


## Análise Teórica

Antes de mais, importa notar que para este problema, temos que o número de
arestas no grafo, $E$, é na verdade menor a $6V$, onde $V$ é o número de
vértices no grafo ($V = M\times N + 2$). O número $6$ é obtido tendo em conta
que cada vértice pode ter, no máximo, 6 arcos ligados a ele: 4 arcos das
relações de vizinhança, um arco proveniente da *source* e outro arco que tem
como destino o *target*.

A nossa solução, como é baseada no algoritmo *Edmonds-Karp*, à partida tem
complexidade temporal $O(VE^2)$. No entanto, como $E < 6V$, então podemos
afirmar que a complexidade assimptótica deste algoritmo é $V^3$.

Além disso, durante o algoritmo *Edmonds-Karp* o espaço utilizado é $O(V)$, uma
vez que este algoritmo precisa apenas das mesmas estruturas de memória que uma
BFS, que tem complexidade $O(V)$, mais o espaço necessário para guardar os
fluxos, que é $O(E) = O(V)$ no nosso problema.

Posto isto, as complexidades temporal e espacial da nossa solução são:

|                       | Edmonds-Karp | Calcular peso mínimo | Classificar píxeis |
|:---------------------:|:------------:|:--------------------:|:------------------:|
| Complexidade Temporal | $O(V^3)$     | $O(V)$               |   $O(V)$           |
| Complexidade Espacial | $O(V)$       | $O(V)$               |   $O(V)$           |


Logo, podemos afirmar que a nossa solução tem uma complexidade teórica temporal
de $O(V^3)$ e complexidade espacial $O(V)$.

## Análise Prática

TODO: CHANGE THIS!!!

Para testar experimentalmente a eficiência do algoritmo, foram gerados
aleatoriamente 24 testes, com a dimensão em píxeis da imagem a variar entre 40
000 e 810 000.
Os testes foram executados nos servidores do sistema Sigma da DSI, correndo num
computador com processadores Intel(R) Xeon(R) CPU E5-2620 @ 2.00GHz, 8-core, com
16 GB de RAM.

Para medir memória usada, foi utilizado o *Valgrind*, mais especificamente a
ferramenta *Massif*; para medir tempos de execução, foi utilizado o comando
*time*, somando os tempos de execução em modo utilizador e sistema.


![](./docs/chart.png)


## Referências:
