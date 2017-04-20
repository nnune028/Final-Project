#include "defs.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


/*
  Returns the characteristic list of a graph.
  The characteristic list is a list with 1 number for each vertex,
  and each number is how many red edges there are leaving that vertex.
  Two graphs having the same characteristic list is a necessary but
  not sufficient condition for them to be color isomorphic.

  Remember to free this list when you're done with it!
*/
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
  qsort(charList,g->n,sizeof(int),cmpfunc);

  return charList;
}

/*
  Prints one half of the graph's adjacency matrix, to have a
  human readable representation of the graph.
*/
void printGraph(Graph * g){
  int n = g->n;
  int i, j;
  printf("  ");
  if(n > 10){
    printf("Uh oh n is %d\n", n);
  }else{
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
  }}
}

/*
  Creates a pointer to a complete graph with numVertices vertices
  and where every vertex is connected to every other vertex.
  Note: Remember to free graphs when you are done using them to
  avoid a memory leak.
  See the destroyGraph().
*/
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

/*
  Returns a pointer to an initialized graphList that can store
  numGraphs graphs
*/
GraphList * newGraphList(int numGraphs){
  GraphList * gL = malloc(sizeof *gL);
  gL->size = numGraphs;
  gL->graphs = malloc(sizeof *(gL->graphs));
  *(gL->graphs) = malloc(numGraphs * sizeof **(gL->graphs));
  for(int i = 0; i < numGraphs; i++){
    *(*(gL->graphs) + i) = malloc(sizeof **(*(gL->graphs) + i));
    //printf("%u\n", *(gL->graphs) + i);
  }
  return gL;
}
/*
  Creates and returns a list of 2^n graphs, where n is the number of
  vertices in g. Each graph has a different combination of edge colors,
  which is why there are so many of them.
  Currently bugged. Generates the right graphs but cannot store them.
*/
GraphList * getNextSize(Graph * g){
  int i, j;
  int n = g->n;
  GraphList * out = newGraphList(pow(2, n));
  for(i = 0; i < pow(2, n); i++){
    Graph * current = copyGraph(g);
    current->edges = realloc(current->edges, (n*(n+1)/2) * sizeof *(current->edges));
    current->n += 1;
    int k = i;
    for(j = n*(n-1)/2; j < n*(n+1)/2; j++){
      *(current->edges + j) = (k&1) + 1;
      k = k>>1;
    }
    *(*(out->graphs) + i) = current;
    //printGraph(&current);
  }
  return out;
}

/*
  This graphs returns a pointer to a new graph, identical to g.
  Remember to call free this memory when you're done with it.
  See destroyGraph().
*/
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

/*
  Frees not only the graph pointer but also its edge array.
  Extremely important to avoid memory leaks.
*/
void destroyGraph(Graph * g){
  free(g->edges);
  free(g);
}

/*
  Returns the color of the edge between vertices m and n.
*/
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

/*
  Returns the number of edges in g adjacent to vertex that
  are color c.
*/
int numColorEdges (Graph * g, Color c, int vertex) {
	int counter = 0;
	int numVertices = g->n;

	for (int i = 0; i < numVertices; i++) {
		if (getEdgeColor(g, vertex, i) == c) {
			counter++;
		}
	}

	return counter;
}


bool hasK3(Graph * g, Color c){
  int n = g->n;
  for(int i = 0; i < n - 2; i++){
    if(numColorEdges(g, c, i) >= 2) {
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
  }
  return FALSE;
}




bool hasK4(Graph * g, Color c){
  int n = g->n;
  for(int h = 0; h < n - 3; h++) {
    if(numColorEdges(g, c, h) >= 3) {
      for(int i = h + 1; i < n - 2; i++){
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
  }
  return FALSE;
}

/*
  Returns a graph with all the same vertices as inGraph but only with edges
  that are color C.
  Remember to free the subgraph when you're done!
*/
Graph * getSubGraph(Graph * inGraph, Color col){

  Graph * outGraph = malloc(sizeof(*outGraph));
  int i;
  int n = inGraph->n;
  outGraph->n = n;
  int numEdges = n * (n - 1) / 2;

  int k = 0;
  for(i = 0; i < numEdges; i++){
    if(*(inGraph->edges + i) == col){
      *(outGraph->edges + k) = *(inGraph->edges + i);
      k++;
    }
  }
  return outGraph;
}

/*
  Returns true if the two graphs are color isomorphic and false otherwise.
  Currently not accurate.
*/
bool isColorIso(Graph * g, Graph * h){
  int * charListG = getCharList(g);

  int * charListH = getCharList(h);
  int result = memcmp(charListG, charListH, g->n);
  free(charListG);
  free(charListH);
  if(result == 0){
    return TRUE;
  }else{
    return FALSE;
  }
}

void shrinkGraphList(GraphList * gL, int newSize){
  for(int i = newSize; i < gL->size - 1; i++){
    destroyGraph(*(*gL->graphs + i));
  }
  gL->size = newSize;
  *gL->graphs = realloc(*gL->graphs, newSize * sizeof **gL->graphs);
}

void destroyGraphList(GraphList * gL, bool deleteGraphs){
  for(int i = 0; i < gL->size; i++){
    if(deleteGraphs){
      destroyGraph(*(*gL->graphs + i));
    }else{
      free(*(*gL->graphs + i));
    }
  }

  free(*gL->graphs);
  free(gL);
}
/*
  Accepts a graphList and modifies it so that it contains only one
  representative of each color isomorphism class. In other words, no
  duplicates. It also filters out any invalid graphs, which are
  graphs that have either a red K3 or a green K4.
  Currently broken.
*/
void clean(GraphList * gL){
  int numGraphs = gL->size;
  int i = 0;
  int foundGraphs = 0;
  GraphList * cleanedGraphs = newGraphList(numGraphs);
  while(i < numGraphs){
    if(!(*(*(gL->graphs) + i))->isNull){

      **(*cleanedGraphs->graphs + foundGraphs) = **(*gL->graphs + i);
      foundGraphs++;
      for(int j = numGraphs - 1; j > i; j--){
        if(isColorIso(*(*gL->graphs + i), *(*gL->graphs + j))){
          (*(*gL->graphs + j))->isNull = TRUE;
        }
      }
    }
    i++;
  }
  for(i = 0; i < foundGraphs; i++){
    **(*gL->graphs + i) = **(*cleanedGraphs->graphs + i);
  }

  destroyGraphList(cleanedGraphs, FALSE);
  shrinkGraphList(gL, foundGraphs);
  gL->size = foundGraphs;
}

/*
  Implements the bulk of the algorithm as described in the project proposal.
  Returns the smallest int n such that there are no valid colorings of Kn.
  Currently WIP.
*/
/*
int run(){
  int tiers = 10;
  GraphList * graphTiers = malloc(tiers * sizeof(*graphTiers));
  graphTiers->size = 1;
  *(graphTiers->graphs) = *createKn(1);
  for(int i = 0; i < tiers; i++){

  }
}
*/


int main(){

  char b = 0;
  Graph * g = createKn(9);
  printGraph(g);
  GraphList * next = getNextSize(g);
  clean(next);
  for(int i = 0; i < next->size; i++){
    printGraph(*(*(next->graphs) + i));
    //printf("%d\n", (*(*(next->graphs) + i))->n);
  }
  return 0;
}
