#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc.h>

/*
  Because C can't even have a built in bool
*/
typedef enum bool {
  FALSE = 0, TRUE = 1
} bool;


typedef struct Cell {
  int value;
  int size;
  struct Cell * next;
} Cell;
/*
  The color of an edge. NONE means no edge.
*/
typedef enum Color {
  NONE = 0, RED = 1, GREEN = 2
} Color;

/*
  Stores some data about a graph. n is the number of vertices.
  edges is an array that actually represents the graph as described in class.
  isNull is used to clear a graph before actually freeing it.
*/
typedef struct Graph {
  int n;
  Color * edges;
  bool isNull;
} Graph;

/*
  Stores an array of graph pointers, though as discussed in class
  we probably need to make it an array of pointers to pointers to graphs.
  Current:
  **graphs -> *Graph -> Graph
              .
              . size of them
              .
              *Graph -> Graph
  Future:
  ***graphs -> **Graph -> *Graph -> Graph
                .
                . size of them
                .
               **Graph -> *Graph -> Graph
*/
typedef struct GraphList {
  int size;
  Graph *** graphs;
} GraphList;



//function headers
//list.c
void freeList(Cell * list);
void freeListArray(Cell ** listArray, int n);
void printList(Cell * list);
void addToList(Cell * list, int val);
Cell * copyList(Cell * list);
int getListIndex(Cell * list, int n);
Cell * getListCellIndex(Cell * list, int n);
Cell ** copyListArray(Cell *array[], int n);
Cell * permuteList(Cell * list, Cell * perm);

//graph.c
int * getCharList(Graph * g);
void printGraph(Graph * g);
void printGraphL(Graph * g);
Graph * createKn(int numVertices);
GraphList * newGraphList(int numGraphs);
Graph * copyGraph(Graph * g);
void destroyGraph(Graph * g);
Color getEdgeColor(Graph * g, int u, int v);
void setEdgeColor(Graph * g, int u, int v, Color c);
bool * hasNMinusOneEdges(Graph * g, Color c, int N);
int numColorEdges(Graph * g, Color c, int vertex);
Graph * getSubGraph(Graph * inGraph, Color c);
void shrinkGraphList(GraphList * gL, int newSize);
void destroyGraphList(GraphList * gL);
void mergeGraphLists(GraphList * gLA, GraphList * gLB);
void setGraph(GraphList * gL, Graph * g, int n);
Graph * getGraph(GraphList * gL, int n);

//util.c
void * mallocDB(int bytes, char * id);
void dumpMallinfo();
GraphList * getNextSize(Graph * g);
bool hasK3(Graph * g, Color c);
bool hasK4(Graph * g, Color c);
int fact(int n);
Cell * decToFact(int n, int dig);
int * collapseVerts(Cell ** verts, int n);
bool recIsoCheck(Cell *vertsG[], Cell *vertsH[], int depth, Graph * g, Graph * h);
bool isColorIso(Graph * g, Graph * h);
void clean(GraphList * gL);
int run();
int cmpfunc(const void * a, const void * b);
