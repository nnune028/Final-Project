#include "defs.h"
/*
  Returns the characteristic list of a graph.
  The characteristic list is a list with 1 number for each vertex,
  and each number is how many red edges there are leaving that vertex.
  Two graphs having the same characteristic list is a necessary but
  not sufficient condition for them to be color isomorphic.

  Remember to free this list when you're done with it!
*/
int * getCharList(Graph * g, Color c){
  int * charList = mallocDB(g->n * sizeof *charList, "getCharList");

  int i, j;
  for(i = 0; i < g->n; i++){
    int colorEdges = 0;
    for(j = 0; j < g->n; j++){
      if(getEdgeColor(g, i, j) == c){
        colorEdges++;
      }
    }
    *(charList + i) = colorEdges;
  }
  //qsort(charList,g->n,sizeof(int),cmpfunc);

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
    }
  }
}

void printGraphL(Graph * g){
  for(int i = 0; i < g->n*(g->n - 1)/2; i++){
    printf("%d", *(g->edges + i));
  }
  printf("\n");
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
  Graph * Kn = mallocDB(sizeof(*Kn), "createKn, graph*");
  Kn->isNull = FALSE;
  Kn->edges =  mallocDB(numEdges * sizeof *(Kn->edges), "createKn, edges");

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
  GraphList * gL = mallocDB(sizeof *gL, "newGraphList, *gL");
  gL->size = numGraphs;
  gL->graphs = mallocDB(sizeof *(gL->graphs), "newGraphList, gL->graphs");
  //printf("allocating for %d\n", numGraphs);
  *gL->graphs = mallocDB(numGraphs * sizeof *(gL->graphs), "newGraphList, *(gL->graphs)");
  /*for(int i = 0; i < numGraphs; i++){
    *(*(gL->graphs) + i) = mallocDB(sizeof *(*(gL->graphs) + i), "newGraphList, *(*(gL->graphs) + i)");
    //printf("%u\n", *(gL->graphs) + i);
  }*/
  return gL;
}
/*
  Creates and returns a list of 2^n graphs, where n is the number of
  vertices in g. Each graph has a different combination of edge colors,
  which is why there are so many of them.
  Currently bugged. Generates the right graphs but cannot store them.
*/

/*
  This graphs returns a pointer to a new graph, identical to g.
  Remember to call free this memory when you're done with it.
  See destroyGraph().
*/
Graph * copyGraph(Graph * g){
  Graph * out = mallocDB(sizeof *out, "copyGraph, *out");
  int n = g->n;
  out->edges = mallocDB(n*(n-1)/2 * sizeof *(out->edges), "copyGraph, out->edges");
  for(int i = 0; i < n*(n-1)/2; i++){
    *(out->edges + i) = *(g->edges + i);
  }
  out->n = g->n;
  out->isNull = g->isNull;
  return out;
}

/*
  Frees not only the graph pointer but also its edge array.
  Extremely important to avoid memory leaks.
*/
void destroyGraph(Graph * g){
  if(g != NULL){
    if(g->isNull){
      free(g->edges);
      int n = g->n;
      free(g);
    }else{
      printf("woops tried to free non null graph\n");
    }
  }else{
    printf("woops tried to free null graph pointer\n");
  }
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
void setEdgeColor(Graph * g, int n, int m, Color c){
  if (n > m){
    int base = n*(n-1)/2;
    *(g->edges + base + m) = c;
  }else{
    int base = m*(m-1)/2;
    *(g->edges + base + n) = c;
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

/*
  Returns a graph with all the same vertices as inGraph but only with edges
  that are color C.
  Remember to free the subgraph when you're done!
*/
Graph * getSubGraph(Graph * inGraph, Color col){

  Graph * outGraph = mallocDB(sizeof(*outGraph), "getSubGraph, *outGraph");
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

void shrinkGraphList(GraphList * gL, int newSize){
  //printf("Starting shrinking process\n");
  for(int i = newSize; i < gL->size; i++){
    //printf("getting %d out of %d", i, gL->size - 1);

    Graph * current = getGraph(gL, i);
    //printf("shrink checking %p\n", current);
    //printf("...got %d out of %d\n", i, gL->size - 1);
    if(current->isNull){
      destroyGraph(current);

    }
    //printf("freed %d out of %d\n", i, gL->size - 1);
  }
  //printf("done destroying null graphs\n");
  int oldSize = gL->size;
  gL->size = newSize;
  *gL->graphs = realloc(*gL->graphs, newSize * sizeof **gL->graphs);

}

void destroyGraphList(GraphList * gL){
  for(int i = 0; i < gL->size; i++){
    //printf("destroy checking %p\n", getGraph(gL, i));
    if(!(getGraph(gL, i) == NULL)){
      if(getGraph(gL, i)->isNull){
        //printf("-----------destroyed a graph %p\n", getGraph(gL, i));
        destroyGraph(getGraph(gL, i));

      }
    }
    //printf("Checked %d out of %d\n", i, gL->size);
  }
  //printf("done freeing null graphs\n");
  free(*gL->graphs);

  free(gL->graphs);

  free(gL);

}
void mergeGraphLists(GraphList * gLA, GraphList * gLB){
  //printf("doing it in merge %d\n", gLA->size + gLB->size);
  *gLA->graphs = realloc(*gLA->graphs, sizeof *gLA->graphs * (gLA->size + gLB->size));
  //printf("realloc'd gLA\n");
  for(int i = gLA->size; i < gLA->size + gLB->size; i++){
    if(getGraph(gLB, i - gLA->size)->isNull) printf("Found the bugger %d------------------\n", i - gLA->size);
    setGraph(gLA, getGraph(gLB, i - gLA->size), i);
  }


  //printf("Copied over gLB\n");
  //printf("%d\n", getGraph(gLA, 0)->isNull);
  gLA->size = gLA->size + gLB->size;
  destroyGraphList(gLB);
  //printf("Destroyed gLB\n");
  //destroyGraphList(gLA);
  //free(*gLA->graphs);
}


void setGraph(GraphList * gL, Graph * g, int n){
  *(*gL->graphs + n) = g;
  // /printf("isNull: %d\n", g->isNull);
}


Graph * getGraph(GraphList * gL, int n){
  return *(*gL->graphs + n);
}
