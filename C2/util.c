#include "defs.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int * getCharList(Graph * g){
  int * charList = malloc(g->n * sizeof *charList);
  int i, j;
  for(i = 0; i < g->n; i++){
    int reds = 0;
    for(j = 0; j < g->n; j++){
      if(getEdgeColor(g, i, j) == RED){
        reds++;
      }
    }
    *(charList + i) = reds;
  }
}

void printGraph(Graph * g){
  int n = g->n;
  int i, j;
  printf("  ");
  for(i = 0; i < n; i++){
    printf("%d ", i);
  }
  printf("\n");
  i = 0;
  while (i < n){
    printf("%d ", i);
    for(j = 0; j < i; j++){
      printf("%d ", *(g->edges + i*(i-1)/2 + j));
    }
    printf("\n");
    i++;
  }
}

Graph * createKn(int numVertices) {
  int numEdges = numVertices * (numVertices - 1) / 2;
  Graph * Kn = malloc(sizeof(*Kn));
  Kn->edges =  malloc(numEdges * sizeof *(Kn->edges));

  Kn->n = numVertices;
  int edgeCount;

  for (edgeCount = 0; edgeCount < numEdges; edgeCount++) {
    *(Kn->edges + edgeCount) = RED;
  }

  return Kn;
}

GraphList * getNextSize(Graph * g){
  int i, j;
  int n = g->n;
  GraphList * out = malloc(sizeof *out);
  out->graphs = malloc(pow(2, n) * sizeof *(out->graphs));
  for(i = 0; i < pow(2, n); i++){
    Graph * current = out->graphs + i;
    *current = *copyGraph(g);
    current->edges = realloc(current->edges, (n*(n+1)/2) * sizeof *(current->edges));
    current->n += 1;
    int k = i;
    for(j = n*(n-1)/2; j < n*(n+1)/2; j++){
      *(current->edges + j) = (k&1) + 1;
      k = k>>1;
    }
  }
  return out;
}

Graph * copyGraph(Graph * g){
  Graph * out = malloc(sizeof *out);
  int n = g->n;
  out->edges = malloc(n*(n-1)/2 * sizeof *(out->edges));
  for(int i = 0; i < n*(n-1)/2; i++){
    *(out->edges + i) = *(g->edges + i);
  }
  out->n = g->n;
  return out;
}

void destroyGraph(Graph * g){
  free(g->edges);
  free(g);
}

Color getEdgeColor(Graph * g, int n, int m){
  if( n == m ) {
    return NONE;
  }else if (n > m){
    int base = n*(n-1)/2;
    return *(g->edges + base + m);
  }else{
    int base = m*(m-1)/2;
    return *(g->edges + base + n);
  }
}

//Example of not very good K3 checking, has to check n choose 3 combos
//we can make an analogus one for checking for k4, we can probably
//even make a general has Kn
bool hasK3(Graph * g, Color c){
  int n = g->n;
  for(int i = 0; i < n - 2; i++){
    for(int j = i + 1; j < n - 1; j++){
      for(int k = j + 1; k < n; k++){
        if(
          getEdgeColor(g, i, j) == c &&
          getEdgeColor(g, j, k) == c &&
          getEdgeColor(g, i, k) == c
        ){
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

bool hasK4(Graph * g, Color c){
  int n = g->n;
  for(int h = 0; h < n - 3; h++) {
	  for(int i = h; i < n - 2; i++){
		  for(int j = i + 1; j < n - 1; j++){
		    for(int k = j + 1; k < n; k++){
			    if(
			      getEdgeColor(g, h, i) == c &&
			      getEdgeColor(g, h, j) == c &&
			      getEdgeColor(g, h, k) == c &&
			      getEdgeColor(g, i, j) == c &&
			      getEdgeColor(g, i, k) == c &&
			      getEdgeColor(g, j, k) == c
			    ){
			      return TRUE;
			    }
		    }
		  }
	  }
  }
  return FALSE;
}


int main(){
  char b = 0;
  Graph * g = createKn(4);
  printGraph(g);
  GraphList * next = getNextSize(g);
  for(int i = 0; i < pow(2, g->n); i++){
    printGraph((next->graphs + i));
  }
  return 0;
}
