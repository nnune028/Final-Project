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
void freeListArray(Cell ** listArray, int n){

  for(int i = 0; i < n ; i++){
    freeList(listArray[i]);
  }
  free(listArray);

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
  if(current->size > 0){
    while(current->next != NULL){
      addToList(t, current->value);
      current = current->next;
    }
    addToList(t, current->value);
    Cell * ans = t->next;
    free(t);
    return ans;
  }else{
    t->size = 0;
    t->value = -1;
    t->next = NULL;
    return t;
  }
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
    //printList(array[i]);
    t[i] = copyList(array[i]);
  }
  return t;
}

Cell * permuteList(Cell * list, Cell * perm){

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
      free(getListCellIndex(copy, address));
      getListCellIndex(copy, address-1)->next=NULL;
      copySize--;
    }else{
      Cell * temp = getListCellIndex(copy, address);
      getListCellIndex(copy, address-1)->next = getListCellIndex(copy, address+1);
      free(temp);
      copySize--;
    }
    current = current->next;
  }

  addToList(t, getListIndex(copy,current->value));
  freeList(copy);
  Cell * ans = t->next;
  free(t);

  return ans;
}
