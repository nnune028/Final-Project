

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
void * mallocDB(int bytes, char * id);
void setGraph(GraphList * gL, Graph * g, int n);
Graph * getGraph(GraphList * gL, int n);
Graph * createKn(int n);
void destroyGraphList(GraphList * gL);
GraphList * getNextSize(Graph * g);
int * getCharList(Graph * g);
Graph * getSubgraph(Color c);
Color getEdgeColor(Graph * g, int m, int n);
void printGraph(Graph * g);
GraphList * getNextSize(Graph * g);
Graph * copyGraph(Graph * g);
bool hasK3(Graph * g, Color c);
bool hasK4(Graph * g, Color c);
int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}
