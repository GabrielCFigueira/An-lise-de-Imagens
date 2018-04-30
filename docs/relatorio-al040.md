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
(que toma os pixeis como vértices de um grafo e as ligações com os seus
vizinhos como arcos) lê as dimensões da imagem, o peso de cada pixel
(de primeiro plano e de cenário), e o peso dos arcos. Com isto, determina
que píxeis pertecem a que plano, e a soma dos pesos dos arcos entre píxeis
de diferentes planos, tendo em conta que esta soma tem de ser a mínima
possível.


## Solução

Como a imagem pode ser vista como um grafo pesado, em que os píxeis são os
vértices e as ligações entre vértices vizinhos como arcos, e o objectivo é
calcular o mínimo da soma dos pesos dos arcos entre píxeis de planos diferentes,
escolheu-se usar um algoritmo de propagação de fluxo com vista o cálculo do
corte mínimo. A formatação do *input* foi preservada (ou seja, os pesos dos
vértices e arcos são guardados em estruturas semelhantes a matrizes): matrizes
de MxN para os pesos dos vértices e matrizes de (M-1)xN e Mx(N-1) para os pesos
dos arcos horizontais e verticais, respectivamente.

As ligações entre um vértice e os seus vizinhos são assim facilmente obtíveis.
Os vizinhos de um vértices estão nas posições imediatamente acima, em baixo, à
direita e à esquerda dele próprio.

Para aplicar o algoritmo do fluxo, foi criada uma *source* e um *target*. Ambos
têm um arco por cada vértice da matriz, cuja capacidade é, respetivamente para
a *source* e para o *target*, o peso primeiro plano e o peso de cenário de cada
vértice. Em termos práticos, isto significa que o "peso" dos vértices é na
realidade o peso dos arcos acima descritos.

O algoritmo consiste em encontrar caminhos de aumento da *source* para o
*target*, em que o peso dos arcos é a capacidade que eles têm de propagar fluxo.
Para isto foi usado uma adaptação do algoritmo de Edmonds-Karp, em que os
caminhos de aumento são encontrados com recurso a uma *BFS*.

No entanto, a nossa adaptação do algoritmo tem várias optimizações:

-fazer uma *BFS* a partir *source* para encontrar um caminho poderia se tornar
ineficiente tendo em conta que a *source* tem como vizinhos todos os vértices
excepto o *target*. Assim, a nossa adaptação primeiro escolhe um vértice válido
para propagar fluxo, e só a partir daí é que inicia a *BFS* (reduzindo o
número de vizinhos inicial de V para 4).

-antes do algoritmo sequer começar, é feita a propagação direta da *source* para
o *target* por todos os vértices possíveis.

-quando uma *BFS* não chega ao *target* a partir de um dado vértice, os vértices
por onde esse *BFS* passou são "pintados" como inválidos para que futuros *BFS*
a partir de outros vértices sejam mais rápidos.

O algoritmo chega ao fim quando não encontra caminhos de aumento a partir do
último vértice considerado. Aqui é feita uma *BFS* a partir da *source* para
determinar o corte mínimo.

Por fim, é dado como *output* a capacidade do corte mínimo e uma representação
em formato de matriz dos píxeis que são de primeiro plano ou de cenário.


## Análise Teórica

O algoritmo usado na solução é uma adaptação do algoritmo de Edmonds-Karp, cuja
complexidade teórica é a mesma deste último (O(VE²)).

## Análise Prática

## Referências:
