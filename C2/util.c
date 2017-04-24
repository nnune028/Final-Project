#include "defs.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc.h>

int memUsage = 0;
int memFreed = 0;
bool debug = FALSE;
void dumpMallinfo(){
  struct mallinfo m = mallinfo();
  printf("used kbytes = %.3f\n", m.uordblks/1000.0);
}
/*
  Returns the characteristic list of a graph.
  The characteristic list is a list with 1 number for each vertex,
  and each number is how many red edges there are leaving that vertex.
  Two graphs having the same characteristic list is a necessary but
  not sufficient condition for them to be color isomorphic.

  Remember to free this list when you're done with it!
*/
int * getCharList(Graph * g){
  int * charList = mallocDB(g->n * sizeof *charList, "getCharList");

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

void * mallocDB(int bytes, char* id){
  memUsage += bytes;
  if (debug) printf("Trying to allocate %.3f KB of memory at call %s\n", bytes/1000.0, id);
  return calloc(bytes, sizeof(char));
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
GraphList * getNextSize(Graph * g){
  int i, j;
  int n = g->n;
  GraphList * out = newGraphList(pow(2, n));
  for(i = 0; i < pow(2, n); i++){
    Graph * current = copyGraph(g);
    current->edges = realloc(current->edges, (n*(n+1)/2) * sizeof *(current->edges));
    memUsage += n * sizeof *(current->edges);
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
      memFreed += n*(n-1)/2 * sizeof(*g->edges);
      free(g);
      memFreed += sizeof(*g);
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

/*
  Returns true if the two graphs are color isomorphic and false otherwise.
  Currently not accurate.
*/
bool isColorIso(Graph * g, Graph * h){
  int * charListG = getCharList(g);
  int * charListH = getCharList(h);

  int result = memcmp(charListG, charListH, g->n*sizeof(int));
  free(charListG);
  memFreed += g->n * sizeof(int);
  free(charListH);
  if(result == 0){
    return TRUE;
  }else{
    return FALSE;
  }
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
  memFreed += (oldSize - newSize) * sizeof(**gL->graphs);
}

void destroyGraphList(GraphList * gL){
  for(int i = 0; i < gL->size; i++){
    //printf("destroy checking %p\n", getGraph(gL, i));
    if(!(getGraph(gL, i) == NULL)){
      if(getGraph(gL, i)->isNull){
        printf("-----------destroyed a graph %p\n", getGraph(gL, i));
        destroyGraph(getGraph(gL, i));

      }
    }
    //printf("Checked %d out of %d\n", i, gL->size);
  }
  //printf("done freeing null graphs\n");
  free(*gL->graphs);
  memFreed += gL->size * sizeof **(gL->graphs);
  free(gL->graphs);
  memFreed += sizeof *gL->graphs;
  free(gL);
  memFreed += sizeof *gL;
}
/*
  Accepts a graphList and modifies it so that it contains only one
  representative of each color isomorphism class. In other words, no
  duplicates. It also filters out any invalid graphs, which are
  graphs that have either a red K3 or a green K4.
  Currently broken.usage
*/
void clean(GraphList * gL){
  //printf("starting off with %d graphs\n", gL->size);
  //printf("%d\n", getGraph(gL, 0)->isNull);

  int numGraphs = gL->size;
  int i = 0;
  int foundGraphs = 0;
  //printf("doing it in clean\n");
  GraphList * cleanedGraphs = newGraphList(numGraphs);
  //printf("about to start checking isos\n");
  while(i < numGraphs){

    Graph * current = getGraph(gL, i);
    //printf("got graph %d out of %lu\n", i, numGraphs);
    if(!current->isNull){
      //printGraph(*(*gL->graphs + i));
      //**(*cleanedGraphs->graphs + foundGraphs) = **(*gL->graphs + i);
      //foundGraphs++;
      //printf("comparing to others..\n");
      for(int j = numGraphs - 1; j > i; j--){
        Graph * other = getGraph(gL, j);
        if(!other->isNull){
          if(isColorIso(current, other)){
            //printf("Were color iso %d %d\n", i, j);
            other->isNull = TRUE;
          }
        }
      }
    }
    i++;
  }
  //printf("Done checking color isomorphism in clean\n");
  i=0;

  while(i < numGraphs){
    Graph * current = getGraph(gL, i);
    if(!current->isNull){
      //printGraph(*(*gL->graphs + i));
      bool K3 = hasK3(current, RED);
      bool K4 = hasK4(current, GREEN);
      if(K3 | K4){
        current->isNull = TRUE;
        //printf("Woops, had red K3 or green K4\n");
      }else{
        //printf("Found a good one! %d\n", i);
        *(*cleanedGraphs->graphs + foundGraphs) = *(*gL->graphs + i);
        foundGraphs++;
      }
      //printf("Found another one\n");
    }
    i++;
  }
  //printf("done cheking for complete subgraphs\n");
  for(i = 0; i < foundGraphs; i++){
    //printf("Set %d to a found graph\n", i);
    *(*gL->graphs + i) = *(*cleanedGraphs->graphs + i);
  }
  //printf("done reassigning graph pointers\n");
  shrinkGraphList(gL, foundGraphs);
  destroyGraphList(cleanedGraphs);

  //printf("shrunk gL\n");

  //printf("destroyed cleanedGraphs\n");
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



/*
  Implements the bulk of the algorithm as described in the project proposal.
  Returns the smallest int n such that there are no valid colorings of Kn.
  Currently WIP.
*/

int run(){
  printf("starting mem usage:\n");
  dumpMallinfo;
  int tiers = 10;
  GraphList ** graphTiers = mallocDB(tiers * sizeof(*graphTiers), "run, **graphTiers");
  dumpMallinfo;
  printf("Generating First GraphList\n");
  *graphTiers = newGraphList(1);
  printf("Generating K1\n");
  **((*graphTiers)->graphs) = createKn(1);
  printf("starting mem usage:\n");
  dumpMallinfo;
  for(int i = 1; i < tiers; i++){
    printf("Starting row %d\n", i + 1);
    *(graphTiers + i) = newGraphList(0);
    printf("Before generating next size: ");
    dumpMallinfo();

    //printf("Current memory alloc: %.3f kilobytes\n", memUsage/1000.0);
    //printf("Current memory freed: %.3f kilobytes\n", memFreed/1000.0);
    int num = (*(graphTiers + i - 1))->size;
    for(int j = 0; j < num; j++){
      printf("Generating the next size: %3d%% done...", (int)(j*100/num));
      printf("\n\033[F\033[J");
      GraphList * gL = getNextSize(getGraph(*(graphTiers + i - 1), j));
      //printf("Got next size for graph %d\n", j);
      //printf("%d has %d graphs raw\n", gL->size);
      clean(gL);
      //printf("cleaned\n");
      mergeGraphLists(*(graphTiers + i), gL);
      //printf("Done merging in %d\n", j);
      //clean(*(graphTiers + i));
      //printf("%d\n", gL->size);
    }
    printf("After generating next size: ");
    dumpMallinfo();
    printf("%d has %d graphs raw\n", i+1, (*(graphTiers + i))->size);
    printf("Cleaning this set up...\n");
    clean(*(graphTiers + i));
    printf("%d has %d graphs cleaned\n",i+1, (*(graphTiers + i))->size);
    dumpMallinfo();
    printf("-------------------\n%d : %d\n", i + 1, (*(graphTiers + i))->size);

    /*for(int k = 0; k < (*(graphTiers + i))->size; k++){
      printf("Graph %d, %d:\n", i, k);
      printGraph(getGraph(*(graphTiers + i), k));
    }*/
  }
}



int main(){

  run();
  return 0;
}
