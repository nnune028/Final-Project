#include "defs.h"


int memUsage = 0;
int memFreed = 0;
bool debug = FALSE;

void dumpMallinfo(){
  struct mallinfo m = mallinfo();
  printf("used kbytes = %.3f\n", m.uordblks/1000.0);
}

void * mallocDB(int bytes, char* id){
  memUsage += bytes;
  if (debug) printf("Trying to allocate %.3f KB of memory at call %s\n", bytes/1000.0, id);
  return calloc(bytes, sizeof(char));
}

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


bool hasK3(Graph * g, Color c){
  int n = g->n;
  int * numEdges = getCharList(g, c);

  for(int i = 0; i < n - 2; i++){
    if( *(numEdges+i) >= 2) {
      for(int j = i + 1; j < n - 1; j++){
        if( *(numEdges+j) >= 2) {
          for(int k = j + 1; k < n; k++){
            if(
              *(numEdges+k) >= 2  &&
              getEdgeColor(g, i, j) == c &&
              getEdgeColor(g, j, k) == c &&
              getEdgeColor(g, i, k) == c
            ){
              free(numEdges);
              return TRUE;
            }
          }
        }
      }
    }
  }
  free(numEdges);
  return FALSE;
}

bool hasK4(Graph * g, Color c){
  int n = g->n;
  int * numEdges = getCharList(g, c);

  for(int h = 0; h < n - 3; h++) {
    if( *(numEdges+h) >= 3) {
      for(int i = h + 1; i < n - 2; i++){
        if( *(numEdges+i) >= 3) {
          for(int j = i + 1; j < n - 1; j++){
            if( *(numEdges+j) >= 3) {
              for(int k = j + 1; k < n; k++){
                if(
                  *(numEdges+k) >= 3  &&
                  getEdgeColor(g, h, i) == c &&
                  getEdgeColor(g, h, j) == c &&
                  getEdgeColor(g, h, k) == c &&
                  getEdgeColor(g, i, j) == c &&
                  getEdgeColor(g, i, k) == c &&
                  getEdgeColor(g, j, k) == c
                ){
                  free(numEdges);
                  return TRUE;
                }
              }
            }
          }
        }
      }
    }
  }
  free(numEdges);
  return FALSE;
}



int fact(int n){
  int t = 1;
  for(int  i = 2; i <= n; i++){
    t *= i;
  }
  return t;
}

Cell * decToFact(int n, int dig){
  int temp[dig];
  int num = n;
  Cell * ans = malloc(sizeof *ans);
  ans->size = 0;
  ans->next = NULL;
  ans->value = -1;
  for(int i = 0; i < dig; i++){
    //printf("%d / %d = %d remainder %d\n", num, i + 1, num/(i+1), num % (i+1));
    temp[i] = num % (i+1);
    num /= (i+1);
  }
  for(int i = dig-1; i >= 0; i--){
    addToList(ans, temp[i]);
  }

  Cell * real = ans->next;
  free(ans);
  return real;
}

int * collapseVerts(Cell ** verts, int n){
  //printf("expecting %d\n", n);
  int * t = malloc(n * sizeof(int));
  int found = 0;
  for(int i = 0; i < n; i++){
    //printf("looking at row %d ", i);
    Cell * current = verts[i];
    //printList(current);
    //printf("got row\n");
    if(current->size > 0){
      while(current->next != NULL){
        t[found] = current->value;
        found++;
        current = current->next;
      }
      t[found] = current->value;
      found++;
      //printf("found 1\n");
    }
  }
  return t;
}
bool recIsoCheck(Cell *vertsG[], Cell *vertsH[], int depth, Graph * g, Graph * h){
  //printf("depth = %d / %d\n", depth, g->n-1);
  if(depth >= g->n-1){

    int * gList = collapseVerts(vertsG, g->n);
    int * hList = collapseVerts(vertsH, h->n);

    for(int i = 0; i < g->n-1; i++){
      for(int j = i + 1; j < g->n; j++){
        //printf("gonna check edge between %d %d\n", i, j);
        //printf("%d and %d in g, %d and %d in h\n", gList[i], gList[j], hList[i], hList[j]);
        if(getEdgeColor(g, gList[i], gList[j]) != getEdgeColor(h, hList[i], hList[j])){
          //printf("gonna free something %d %d\n", i, j);
          free(gList);
          free(hList);
          //printf("done freeing\n");

          return FALSE;
        }
      }
    }
    //printf("gonna free outside loop\n");
    free(gList);
    free(hList);

    return TRUE;
  }else{
    if(vertsG[depth]->size > 0){
      for(int i = 0; i < fact(vertsG[depth]->size); i++){

        Cell * perm = decToFact(i, vertsG[depth]->size);


        Cell * permVertsG = permuteList(vertsG[depth], perm);
        freeList(perm);
        Cell **copy = copyListArray(vertsG, g->n);
        freeList(copy[depth]);
        copy[depth] = permVertsG;
        bool ans = recIsoCheck(copy, vertsH, depth + 1, g, h);
        freeListArray(copy, g->n);
        if(ans) return TRUE;

      }
      return FALSE;
    }else{
      //printf("entering recursive call 2...\n");
      return recIsoCheck(vertsG, vertsH, depth + 1, g, h);

    }
  }
}
int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}
/*
  Returns true if the two graphs are color isomorphic and false otherwise.
  Currently not accurate.
*/
bool isColorIso(Graph * g, Graph * h){
  //printf("starting the isomorphism checking process\n");
  int * charListG = getCharList(g);
  int * charListGSorted = getCharList(g);
  int * charListH = getCharList(h);
  int * charListHSorted = getCharList(h);
  int n = g->n;
  qsort(charListGSorted,n,sizeof(int),cmpfunc);
  qsort(charListHSorted,n,sizeof(int),cmpfunc);
  Cell * vertsG[n];
  Cell * vertsH[n];
  int result = memcmp(charListGSorted, charListHSorted, n*sizeof(int));
  free(charListGSorted);
  free(charListHSorted);

  if(result == 0){
      //printf("--------------------\n");

      for(int i = 0; i < n; i++){
        vertsG[i] = calloc(1, sizeof *vertsG[i]);
        vertsH[i] = calloc(1, sizeof *vertsH[i]);
      }
      for(int i = 0 ; i < n; i++){
        addToList(vertsG[charListG[i]], i);
        addToList(vertsH[charListH[i]], i);
      }
      for(int i = 0; i < n; i++){
        if (vertsG[i]->next != NULL) {
          vertsG[i] = vertsG[i]->next;
          //printf("G: ");
          //printList(vertsG[i]);
        }
        if (vertsH[i]->next != NULL) {
          vertsH[i] = vertsH[i]->next;
          //printf("                   H: ");
          //printList(vertsH[i]);
        }
      }

      bool ans = recIsoCheck(vertsG, vertsH, 0, g, h);

      for(int i = 0; i < n; i++){
        freeList(vertsG[i]);
        freeList(vertsH[i]);
      }
      free(charListH);
      free(charListG);
      return ans;
  }else{
    free(charListH);
    free(charListG);
    return FALSE;
  }
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
  printf("about to start checking isos                   ");
  dumpMallinfo();
  while(i < numGraphs){
    //printf("-----------------------------------------------\n");
    Graph * current = getGraph(gL, i);
  //  printf("got graph %d out of %lu\n", i, numGraphs);
    if(!current->isNull){
      //printGraph(*(*gL->graphs + i));
      //**(*cleanedGraphs->graphs + foundGraphs) = **(*gL->graphs + i);
      //foundGraphs++;
      //printf("comparing to others...\n");
      for(int j = numGraphs - 1; j > i; j--){
        //printf("comparing to %d\n", j);
        Graph * other = getGraph(gL, j);
        //printf("got other\n");
        if(!other->isNull){
          //printf("before isColorIso: ");
          //dumpMallinfo();
          if(isColorIso(current, other)){
            //printf("Were color iso %d %d\n", i, j);
            other->isNull = TRUE;
          }
          //printf("after isColorIso: ");
          //dumpMallinfo();
        }
      }
    }
    i++;
  }
  printf("Done checking color isomorphism in clean      ");
  dumpMallinfo();
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
      //clean(gL);
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
    printf("before cleaning: ");
    dumpMallinfo();
    clean(*(graphTiers + i));
    printf("after  cleaning: ");
    dumpMallinfo();
    printf("%d has %d graphs cleaned\n",i+1, (*(graphTiers + i))->size);
    dumpMallinfo();
    printf("-------------------\n%d : %d\n", i + 1, (*(graphTiers + i))->size);
/*
    for(int k = 0; k < (*(graphTiers + i))->size; k++){
      printf("Graph %d, %d:\n", i, k);
      printGraph(getGraph(*(graphTiers + i), k));
    }*/
  }
}



int main(){

  run();

  return 0;
}
