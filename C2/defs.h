

typedef enum Color {
  NONE = 0, RED = 1, GREEN = 2
} Color;

typedef struct Graph {
  int n;
  Color * edges;
} Graph;

typedef struct GraphList {
  int size;
  Graph * graphs;
} GraphList;

typedef enum bool {
  FALSE = 0, TRUE = 1
} bool;

//function headers
Graph * createKn(int n);

GraphList * getNextSize(Graph * g);
int * getCharList(Graph * g);
Graph * getSubgraph(Color c);
Color getEdgeColor(Graph * g, int m, int n);
void printGraph(Graph * g);
GraphList * getNextSize(Graph * g);
Graph * copyGraph(Graph * g);

int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}
