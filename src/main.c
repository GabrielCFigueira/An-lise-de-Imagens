#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Our graph */
int M, N;
int **whiteCapacity;
int **blackCapacity;
int **verticalCapacity;
int **horizontalCapacity;
int **verticalFlow;
int **horizontalFlow;
int **whiteFlow;
int **blackFlow;



/* returns a lxc matrix */
int **buildMatrix(int l, int c) {

  int **res;
  int i;
  res= (int**) calloc(l, sizeof(int*));
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

void freeMatrix(int **matrix, int l) {

  int i;
  for(i = 0; i < l; ++i)
    free(matrix[i]);
  free(matrix);
}

/* Reads lxc matrix to memory */
void readMatrix(int **matrix, int l, int c) {

  int i, j;
  for(i=0; i<l; ++i) {
    for(j=0; j<c; ++j){
      scanf("%d", &matrix[i][j]);
    }
  }


}


/* Double matrix, used to keep ancestors during BFS */
int (**buildDoubleMatrix(int l, int c))[2] {

  int i;
  int (**res)[2] = (int(**)[2]) calloc(l, sizeof(int(*)[2]));
  for(i = 0; i < l; ++i)
    res[i] = (int(*)[2]) calloc(c, sizeof(int[2]));
  return res;
}

int (**resetDoubleMatrix(int (**matrix)[2], int l, int c))[2] {

  int i;
  for(i = 0; i < l; ++i)
    matrix[i] = (int(*)[2]) memset(matrix[i], 0, c*sizeof(int(*)[2]));
  return matrix;
}

void freeDoubleMatrix(int (**matrix)[2], int l) {

  int i;
  for(i = 0; i < l; ++i)
    free(matrix[i]);
  free(matrix);
}


/* Macro to add stuff to the BFS queue */
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



void EdmondsKarp() {

  int **colour, (**pai)[2];
  int (*queue)[2], startQueue=-1, endQueue=0;
  int i, j, p_i, p_j, found_path=0, min_capacity, res_capacity;

  colour = buildMatrix(M, N);
  pai = buildDoubleMatrix(M,N);
  queue = (int(*)[2]) calloc(N*M, sizeof(int(*)[2]));


  /* Heuristic: try to fill all paths between source and target
     that are of the form source --> u --> target */
  for (i = 0; i < M; i++) {
    for (j = 0; j < N; j++) {
      res_capacity = whiteCapacity[i][j];
      min_capacity = blackCapacity[i][j];
      if (res_capacity > 0 && min_capacity > 0) {
        min_capacity = (res_capacity < min_capacity) ? res_capacity : min_capacity;
        whiteFlow[i][j] = blackFlow[i][j] = min_capacity;
      }
    }
  }




  /* I am sorry. I am sorry. I am sorry.
     Only way to do this "cleanly" on a loop (?) */
find_flow_augmentation_path:


  /* Enqueue every non 0 element of whiteCapacity - whiteFlow */
  for (i = 0; i < M; i++)
    for (j = 0; j < N; j++)
      if (whiteCapacity[i][j] - whiteFlow[i][j] > 0) {
        INSERT_IN_QUEUE(i, j);
        INSERT_AS_PARENT(i, j, -1, -1);
      }

  while(++startQueue < endQueue) {
    i = queue[startQueue][0];
    j = queue[startQueue][1];

    /* Check if we can reach the target directly now */
    if(blackCapacity[i][j] - blackFlow[i][j] > 0){
      found_path = 1;
      break;
    }

    /* Else, check neighbourhood; parent will be (i,j)
       This is a case where the parent is in the east */
    if(j!=0 && !colour[i][j-1] && horizontalCapacity[i][j-1]+horizontalFlow[i][j-1]>0){
      INSERT_IN_QUEUE(i, j-1);
      INSERT_AS_PARENT(i, j-1, i, j);
    }
    /* Parent in the west */
    if(j!=N-1 && !colour[i][j+1] && horizontalCapacity[i][j]-horizontalFlow[i][j]>0){
      INSERT_IN_QUEUE(i, j+1);
      INSERT_AS_PARENT(i, j+1, i, j);
    }
    /* Parent in the south */
    if(i!=0 && !colour[i-1][j] && verticalCapacity[i-1][j]+verticalFlow[i-1][j]>0){
      INSERT_IN_QUEUE(i-1, j);
      INSERT_AS_PARENT(i-1, j, i, j);
    }
    /* Parent in the north */
    if(i!=M-1 && !colour[i+1][j] && verticalCapacity[i][j]-verticalFlow[i][j]>0){
      INSERT_IN_QUEUE(i+1, j);
      INSERT_AS_PARENT(i+1, j, i, j);
    }

  }

  /* Here we have a path OR we have a complete cut
     Reutilize queue to save the found path as we go back to ancestors */
  if(found_path) {
    endQueue = 0;

    /* One hypotesis for the flow augmentation is the last edge,
       the one that connects to the target */
    min_capacity = blackCapacity[i][j] - blackFlow[i][j];

    INSERT_IN_QUEUE(i,j);
    p_i = pai[i][j][0];
    p_j = pai[i][j][1];

    /* This loop only processes connections between vertexes on the matrix */
    while(p_i != -1) {

      /* Calculate the residual capacity of the edge i,j; to know that, we need to know
         from were we came -- the p_i, p_j */
      switch((p_i > i || p_j > j)*2 + (i==p_i && j!=p_j)) {
        /* 00: The parent is in the north (p_i < i) [normal flow] */
        case 0: res_capacity = verticalCapacity[p_i][p_j] - verticalFlow[p_i][p_j];
        break;
        /* 01: The parent is in the west (p_j < j) [normal flow] */
        case 1: res_capacity = horizontalCapacity[p_i][p_j] - horizontalFlow[p_i][p_j];
        break;
        /* 10: The parent is in the south (p_i > i) [reverse flow] */
        case 2: res_capacity = verticalCapacity[i][j] + verticalFlow[i][j];
        break;
        /* 11: The parent is in the east (p_j > j) [reverse flow] */
        case 3: res_capacity = horizontalCapacity[i][j] + horizontalFlow[i][j];
        break;
      }

      /* Update the minimum residual capacity present */
      min_capacity = (min_capacity > res_capacity) ? res_capacity : min_capacity;

      /* Move on to the next parent->child edge */
      INSERT_IN_QUEUE(p_i, p_j);
      i = p_i;
      j = p_j;
      p_i = pai[i][j][0];
      p_j = pai[i][j][1];
    }

    /* Finally, see the first edge on the path: from the source to a point in the matrix */
    res_capacity = whiteCapacity[i][j] - whiteFlow[i][j];
    min_capacity = (min_capacity > res_capacity) ? res_capacity : min_capacity;


    /* Ok, now we are going to reprocess the edges, putting min_capacity on the flow
       That may result in adding or subtracting, according to orientation of the path */


    /* Add in source->vertex1 edge */
    whiteFlow[i][j] += min_capacity;

    /* Process all the vertex of the path that are in the matrix,
       but using the Queue as a stack */

    /* However, at this stage we have a repeated element here: the current (i,j)
       is duplicated on the last position of the queue (first on the stack). So, before taking
       any element, we remove this repeated element from the stack */
    --endQueue;

    /* Start processing the stack */
    while(--endQueue >= 0) {
      p_i = i;
      p_j = j;
      i = queue[endQueue][0];
      j = queue[endQueue][1];

      /* This is the same logic as before */
      switch((p_i > i || p_j > j)*2 + (i==p_i && j!=p_j)) {
        /* 00: The parent is in the north (p_i < i) [normal flow] */
        case 0: verticalFlow[p_i][p_j] += min_capacity;
        break;
        /* 01: The parent is in the west (p_j < j) [normal flow] */
        case 1: horizontalFlow[p_i][p_j] += min_capacity;
        break;
        /* 10: The parent is in the south (p_i > i) [reverse flow] */
        case 2: verticalFlow[i][j] -= min_capacity;
        break;
        /* 11: The parent is in the east (p_j > j) [reverse flow] */
        case 3: horizontalFlow[i][j] -= min_capacity;
        break;
      }

    }

    /* Add flow to vertexK->target edge */
    blackFlow[i][j] += min_capacity;


    /* Restart now: do everything over again, until there are no more paths
       Cleanup here */
    startQueue = -1;
    endQueue = 0;
    colour = resetMatrix(colour, M, N);
    pai = resetDoubleMatrix(pai, M, N);
    found_path = 0;

    goto find_flow_augmentation_path;

  } else {

    /* Here, we check the vertexes that are in queue. Those vertexes are white,
       for reasons currently not completely understood */

    /* Reutilize res_capacity to make sum of flow (max flow = min cut) */
    res_capacity = 0;
    for(i=0; i<M; ++i)
      for(j=0; j<N; ++j)
        res_capacity += whiteFlow[i][j];

    printf("%d\n\n", res_capacity);

    /* Now, see the colour. All the vertexes we hit are background (C) */
    for(i=0; i<M; ++i){
      for(j=0; j<N; ++j){
        if(colour[i][j])
          printf("C ");
        else
          printf("P ");
      }
      putchar('\n');
    }
  }

  /*Cleanup */
  free(queue);
  freeMatrix(colour, M);
  freeDoubleMatrix(pai, M);
  return;
}




int main() {

  scanf("%d %d\n\n", &M, &N);

  whiteCapacity = buildMatrix(M,N);
  blackCapacity = buildMatrix(M,N);
  horizontalCapacity = buildMatrix(M, N-1);
  verticalCapacity = buildMatrix(M-1, N);

  whiteFlow = buildMatrix(M, N);
  blackFlow = buildMatrix(M, N);
  horizontalFlow = buildMatrix(M, N-1);
  verticalFlow = buildMatrix(M-1, N);


  /* White capacity: weight if it is of foreground (1o plano) */
  readMatrix(whiteCapacity, M, N);
  /* Black capacity: weight if it is of background (cena'rio) */
  readMatrix(blackCapacity, M, N);
  readMatrix(horizontalCapacity, M, N-1);
  readMatrix(verticalCapacity, M-1, N);

  EdmondsKarp();

  freeMatrix(whiteCapacity, M);
  freeMatrix(blackCapacity, M);
  freeMatrix(horizontalCapacity, M);
  freeMatrix(verticalCapacity, M-1);

  freeMatrix(whiteFlow, M);
  freeMatrix(blackFlow, M);
  freeMatrix(horizontalFlow, M);
  freeMatrix(verticalFlow, M-1);


  return 0;
}
