#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct graph {
  int M, N;
  int **whiteCapacity;
  int **blackCapacity;
  int **verticalCapacity;
  int **horizontalCapacity;
  int **verticalFlow;
  int **horizontalFlow;
  int **whiteFlow;
  int **blackFlow;
} *Graph;

int lines(Graph g) {return g->M;}

int columns(Graph g) {return g->N;}

int **whiteCapacity(Graph g) {return g->whiteCapacity;}

int **blackCapacity(Graph g) {return g->blackCapacity;}

int **verticalCapacity(Graph g) {return g->verticalCapacity;}

int **horizontalCapacity(Graph g) {return g->horizontalCapacity;}

int **verticalFlow(Graph g) {return g->verticalFlow;}

int **horizontalFlow(Graph g) {return g->horizontalFlow;}

int **whiteFlow(Graph g) {return g->whiteFlow;}

int **blackFlow(Graph g) {return g->blackFlow;}



// returns a lxc matrix
int **buildMatrix(int l, int c) {

  int i;
  int **res = (int**) calloc(l, sizeof(int*));
  for(i = 0; i < l; ++i)
    res[i] = (int*) calloc(c, sizeof(int));
  return res;

}

int **resetMatrix(int** matrix, int l, int c) {

  int i;
  for(i = 0; i < l; ++i)
    matrix[i] = (int*) memset(matrix[i], 0, c*sizeof(int));
  return matrix;

}


// Double matrix, used to keep ancestors during BFS
int (**buildDoubleMatrix(int l, int c))[2] {

  int i;
  int (**res)[2] = (int(**)[2]) calloc(l, sizeof(int(**)[2]));
  for(i = 0; i < l; ++i)
    res[i] = (int(*)[2]) calloc(c, sizeof(int(*)[2]));
  return res;
}

int (**resetDoubleMatrix(int (**matrix)[2], int l, int c))[2] {

  int i;
  for(i = 0; i < l; ++i)
    matrix[i] = (int(*)[2]) memset(matrix[i], 0, c*sizeof(int(*)[2]));
  return matrix;
}


// reads a matrix from input
void readMatrix(int **matrix, int l, int c) {
  int i, j;

  for(i = 0; i < l; i++)
    for(j = 0; j < c; j++)
      scanf("%d ", &matrix[i][j]);
}

// Macro to add stuff to the BFS queue
#define INSERT_IN_QUEUE(x_cord, y_cord) do {\
                  queue[endQueue][0] = x_cord; \
                  queue[endQueue][1] = y_cord; \
                  endQueue++; \
        } while(0)

#define INSERT_AS_PARENT(x_cord, y_cord, x_pai, y_pai) do {\
                  colour[x_cord][y_cord] = 1; \
                  pai[x_cord][y_cord][0] = x_pai; \
                  pai[x_cord][y_cord][1] = y_pai; \
        } while(0)



void EdmondsKarp(Graph g) {

  int M, N;
  int **colour, (**pai)[2];
  int (*queue)[2], startQueue=-1, endQueue=0;
  int i, j, p_i, p_j, found_path=0, min_capacity, res_capacity;

  M = lines(g);
  N = columns(g);
  colour = buildMatrix(M, N);
  pai = buildDoubleMatrix(M,N);
  queue = (int(*)[2]) calloc(N*M, sizeof(int(*)[2]));

  // I am sorry. I am sorry. I am sorry.
  // Only way to do this "cleanly" on a loop (?)
find_path:


  //Enqueue every non 0 element of whiteCapacity - whiteFlow
  for (i = 0; i < M; i++)
    for (j = 0; j < N; j++)
      if (whiteCapacity(g)[i][j] - whiteFlow(g)[i][j] > 0) {
        INSERT_IN_QUEUE(i, j);
        INSERT_AS_PARENT(i, j, -1, -1);
      }

  while(++startQueue < endQueue) {
    i = queue[startQueue][0];
    j = queue[startQueue++][1];

    // Check if we can reach the target directly now
    if(blackCapacity(g)[i][j] - blackFlow(g)[i][j] > 0){
      found_path = 1;
      break;
    }

    // Else, check neighbourhood
    if(i!=0 && !colour[i-1][j] && horizontalCapacity(g)[i-1][j]+horizontalFlow(g)[i-1][j]>0){
      INSERT_IN_QUEUE(i-1, j);
      INSERT_AS_PARENT(i-1, j, i, j);
    }
    if(i!=M-1 && !colour[i+1][j] && horizontalCapacity(g)[i][j]-horizontalFlow(g)[i][j]>0){
      INSERT_IN_QUEUE(i+1, j);
      INSERT_AS_PARENT(i+1, j, i, j);
    }
    if(j!=0 && !colour[i][j-1] && verticalCapacity(g)[i][j-1]+verticalFlow(g)[i][j-1]>0){
      INSERT_IN_QUEUE(i, j-1);
      INSERT_AS_PARENT(i, j-1, i, j);
    }
    if(j!=N-1 && !colour[i][j+1] && verticalCapacity(g)[i][j]-verticalFlow(g)[i][j]>0){
      INSERT_IN_QUEUE(i, j+1);
      INSERT_AS_PARENT(i, j+1, i, j);
    }

  }

  // Here we have a path OR we have a complete cut
  // Reutilize queue to save the found path as we go back to ancestors
  if(found_path) {
    endQueue = 0;

    // One hypotesis for the flow augmentation is the last edge,
    // the one that connects to the target
    min_capacity = blackCapacity(g)[i][j] - blackFlow(g)[i][j];

    INSERT_IN_QUEUE(i,j);
    p_i = pai[i][j][0];
    p_j = pai[i][j][1];

    // This loop only processes connections between vertexes on the matrix
    while(p_i != -1) {

      // Calculate the residual capacity of the edge i,j; to know that, we need to know
      // from were we came -- the p_i, p_j
      switch((p_i > i || p_j > j)*2 + (i!=p_i && j==p_j)) {
        // 00: The parent is in the north (p_j < j)
        case 0: res_capacity = verticalCapacity(g)[p_i][p_j] - verticalFlow(g)[p_i][p_j];
        break;
        // 01: The parent is in the west (p_i < i)
        case 1: res_capacity = horizontalCapacity(g)[p_i][p_j] - horizontalFlow(g)[p_i][p_j];
        break;
        // 10: The parent is in the south (p_j > j)
        case 2: res_capacity = verticalCapacity(g)[p_i][p_j] + verticalFlow(g)[p_i][p_j];
        break;
        // 11: The parent is in the east (p_i > i)
        case 3: res_capacity = horizontalCapacity(g)[p_i][p_j] + horizontalFlow(g)[p_i][p_j];
        break;
      }

      // Update the minimum residual capacity present
      min_capacity = (min_capacity > res_capacity) ? res_capacity : min_capacity;

      // Move on to the next parent->child edge
      INSERT_IN_QUEUE(p_i, p_j);
      i = p_i;
      j = p_j;
      p_i = pai[i][j][0];
      p_j = pai[i][j][1];
    }

    // Finally, see the first edge on the path: from the source to a point in the matrix
    res_capacity = whiteCapacity(g)[i][j] - whiteFlow(g)[i][j];
    min_capacity = (min_capacity > res_capacity) ? res_capacity : min_capacity;


    // Ok, now we are going to reprocess the edges, putting min_capacity on the flow
    // That may result in adding or subtracting, according to orientation of the path


    // Add in source->vertex1 edge
    whiteFlow(g)[i][j] += min_capacity;

    // Process all the vertex of the path that are in the matrix,
    // but using the Queue as a stack

    // However, at this stage we have a repeated element here: the current (i,j)
    // is duplicated, and the last position (first on the stack). So, before taking
    // any element, we remove this repeated element from the stack
    --endQueue;

    // Start processing the stack
    while(--endQueue >= 0) {
      p_i = i;
      p_j = j;
      i = queue[endQueue][0];
      j = queue[endQueue][1];

      // This is the same logic as before
      switch((p_i > i || p_j > j)*2 + (i!=p_i && j==p_j)) {
        // 00: The parent is in the north (p_j < j)
        case 0: verticalFlow(g)[p_i][p_j] += min_capacity;
        break;
        // 01: The parent is in the west (p_i < i)
        case 1: horizontalFlow(g)[p_i][p_j] += min_capacity;
        break;
        // 10: The parent is in the south (p_j > j)
        case 2: verticalFlow(g)[p_i][p_j] -= min_capacity;
        break;
        // 11: The parent is in the east (p_i > i)
        case 3: horizontalFlow(g)[p_i][p_j] -= min_capacity;
        break;
      }

    }

    // Add flow to vertexK->target edge
    blackFlow(g)[i][j] += min_capacity;


    // Restart now: do everything over again, until there are no more paths
    // Cleanup here
    startQueue = -1;
    endQueue = 0;
    colour = resetMatrix(colour, M, N);
    pai = resetDoubleMatrix(pai, M, N);
    found_path = 0;

    goto find_path;

  } else {
    // Here, we check the vertexes that are in queue. Those vertexes are white,
    // for reasons currently not completely understood
  }
}




int main() {
  // receives M and N as input
  // receives the weight of the black pixels, builds a MxN matrix with them: whiteCapacity
  // receives the weight of the white pixels, builds a MxN matrix with them: blackCapacity
  // receives the capacity of each horizontal edge, builds a Mx(N-1) matrix with them: horizontalCapacity
  // receives the capacity of each vertical edge, builds a (M-1)xN matrix with them: vertical Capacity
  // builds two MxN matrix, one with the edges between the source (of the flow) and
  // the pixels and another with the edges between the target and the pixels
  // builds a Mx(N-1) empty matrix: horizontalFlow
  // builds a (M-1)*N empty matrix: verticalFlow

  return 0;
}
