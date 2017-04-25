#include "defs.h"
void freeList(Cell * list){
  Cell * current = list;
  Cell * next;
  while(current->next != NULL){
    next = current->next;
    free(current);
    current = next;
  }
  free(current);
}
void printList(Cell * list){
  Cell * current = list;
  printf("[");
  while(current->next != NULL){
    printf("%d ", current->value);
    current = current->next;
  }
  if(current->size == 1) printf("%d]\n", current->value );
  else printf("]\n");
}

void addToList(Cell * list, int val){
  Cell * current = list;
  while(current -> next != NULL){
    current->size++;
    current = current->next;
  }
  current->size++;
  current->next = malloc(sizeof *current->next);
  current = current->next;
  current->size = 1;
  current->next = NULL;
  current->value = val;
}


Cell * copyList(Cell * list){
  Cell * t = calloc(1, sizeof *t);
  Cell * current = list;
  while(current->next != NULL){
    addToList(t, current->value);
    current = current->next;

  }
  addToList(t, current->value);
  Cell * ans = t->next;
  free(t);
  return ans;
}
int getListIndex(Cell * list, int n){
  Cell * current = list;
  int i = 0;
  while(i != n){
    current = current->next;
    i++;
  }
  return current->value;
}
Cell* getListCellIndex(Cell * list, int n){
  Cell * current = list;
  int i = 0;
  while(i != n){
    current = current->next;
    i++;
  }
  return current;
}
Cell ** copyListArray(Cell *array[], int n){
  Cell **t = calloc(n, sizeof *t);
  for(int i = 0; i < n; i++){
    t[i] = array[i];
  }
  return t;
}

Cell * permuteList(Cell * list, Cell * perm){
  //printf("--permuting vertices\n");
  Cell * t = calloc(1, sizeof(t));
  Cell * current = perm;
  Cell * copy = copyList(list);
  //printf("beginning loop\n");
  int copySize = copy->size;
  while(current->next != NULL){
    //printf("current t: ");
    //printList(t);
    //printf("current copy: (%d) ", copySize);
    //printList(copy);
    int address = current->value;
    //printf("getting address %d\n", address);
    addToList(t, getListIndex(copy, address));
    //printf("removing vertex\n");
    if(address == 0){

      if(copy->next != NULL){

        Cell* toFree = copy;
        copy = copy->next;
        copySize--;
        free(toFree);
      }else{
        free(copy);
      }
    }else if(address == copySize - 1){
      getListCellIndex(copy, address-1)->next=NULL;
      copySize--;
    }else{
      getListCellIndex(copy, address-1)->next = getListCellIndex(copy, address+1);
      copySize--;
    }
    current = current->next;
  }
  addToList(t, getListIndex(copy,current->value));
  Cell * ans = t->next;
  free(t);
  return ans;
}
