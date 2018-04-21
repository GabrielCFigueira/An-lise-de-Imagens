#include <stdio.h>
#include <stdlib.h>


typedef struct graph {
  int M;
  int N;
  int **whiteCapacity;
  int **blackCapacity;
  int **verticalEdges;
  int **horizontalEdges;
  int **verticalFlow;
  int **horizontalFlow;
  int **whiteFlow;
  int **blackFlow;

} Graph;

/* returns a lxc matrix */
int **buildMatrix(int l, int c) {

  int i;
  int **res = (int**) malloc(l*sizeof(int*));
  for(i = 0; i < l; ++i)
    res[i] = (int*) malloc(c*sizeof(int));
  return res;

}

/* reads a matrix from input */
void readMatrix(int **matrix, int l, int c) {

  int i, j;

  for(i = 0; i < l; i++)
    for(j = 0; j < c; j++)
      scanf("%d ", &matrix[i][j]);

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
