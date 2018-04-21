#include <stdio.h>
#include <stdlib.h>


typedef struct graph {
  int M, N;
  int **whiteCapacity;
  int **blackCapacity;
  int **verticalEdges;
  int **horizontalEdges;
  int **verticalFlow;
  int **horizontalFlow;
  int **whiteFlow;
  int **blackFlow;
} Graph;

int lines(Graph g) {return g->M;}

int columns(Graph g) {return g->N;}

int **whiteCapacity(Graph g) {return g->whiteCapacity;}

int **blackCapacity(Graph g) {return g->blackCapacity;}

int **verticalEdges(Graph g) {return g->verticalEdges;}

int **horizontalEdges(Graph g) {return g->horizontalEdges;}

int **verticalFlow(Graph g) {return g->verticalFlow;}

int **horizontalFlow(Graph g) {return g->horizontalFlow;}

int **whiteFlow(Graph g) {return g->whiteFlow;}

int **blackFlow(Graph g) {return g->blackFlow;

}



/* returns a lxc matrix */
int **buildMatrix(int l, int c) {

  int i;
  int **res = (int**) calloc(1, l*sizeof(int*));
  for(i = 0; i < l; ++i)
    res[i] = (int*) calloc(1, c*sizeof(int));
  return res;

}

/* reads a matrix from input */
void readMatrix(int **matrix, int l, int c) {

  int i, j;

  for(i = 0; i < l; i++)
    for(j = 0; j < c; j++)
      scanf("%d ", &matrix[i][j]);

}


void EdmondsKarp(Graph g) {

  int M = lines(g);
  int N = columns(g);
  /*flag 0 for not visited, 1 for visited */
  int **colour = buildMatrix(M, N);
  int* pai = (int*) malloc(N*M*sizeof(int));
  int* queue = (int*) malloc(N*M*sizeof(int));
  int i,j, breakcondition = 0;
  //Enqueue every non 0 element of whiteCapacity - whiteFlow
  for (i = 0; i < M; i++)
    for (j = 0; j < N; j++)

}













int main() {
  // receives M and N as input
  // receives the weight of the black pixels, builds a MxN matrix with them
  // receives the weight of the white pixels, builds a MxN matrix with them
  // receives the capacity of each vertical edge, builds a Mx(N-1) matrix with them
  // receives the capacity of each horizontal edge, builds a (M-1)xN matrix with them
  /* builds two MxN matrix, one with the edges between the source (of the flow) and
  the pixels and another with the edges between the target and the pixels*/

  return 0;
}
