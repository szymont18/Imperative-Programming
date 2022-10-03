#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1000
#define MEMORY_ALLOCATION_ERROR  -1

// list node
typedef struct Node {
    int *data;
    int array_size;
    struct Node* next;
    struct Node* prev;
} Node;

// doubly linked list
typedef struct List {
    Node *head;
    Node *tail;
    int size;
} List;

// iterator
typedef struct iterator {
    int position;
    struct Node* node_ptr;
} iterator;

// forward initialization
iterator begin(Node* head) {
    iterator it = {0, head};
    return it;
}

// backward initialization
iterator end(Node* tail) {
    iterator it = {tail->array_size - 1, tail};
    return it;
}

// initialize list
void init(List *list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

// ---------------------------------------------- to implement ...

// append element to the list
// Działa
void push_back(List *list, int *data, int array_size) {
    Node *array2add = malloc(sizeof (Node));

    array2add->array_size = array_size;
    array2add->data = malloc(array_size * sizeof(int));

    for(int i = 0; i < array_size; i++)
        array2add->data[i] = data[i];

    if (list->head == NULL){
        list->head = array2add;
        list->tail = array2add;
        list->size = 1;

        array2add->next = NULL;
        array2add->prev = NULL;
        return;
    }

    list->tail->next = array2add;
    array2add->prev = list->tail;
    list->tail = array2add;
    array2add->next= NULL;

    list->size++;

}

// set iterator to move n elements forward from its current position
// Zwraca pozycję n - tego elementu
// Indeksuje od zera, to znaczy, że dla n = 1, zwracam element pod indeksem 0
void skip_forward(iterator* itr, int n) {

    while(itr->node_ptr->next != NULL){
        int size = itr->node_ptr->array_size;

        if (n - size <= 0) break;

        n -= size;
        itr->node_ptr = itr->node_ptr->next;
    }
    itr->position = n - 1;
}

// forward iteration - get n-th element in the list
int get_forward(List *list, int n) {
    if (list->head == NULL) return 0;

    iterator itr = begin(list->head);
    skip_forward(&itr, n);

    int el2return = itr.node_ptr->data[itr.position];
    return el2return;
}

// set iterator to move n elements backward from its current position
void skip_backward(iterator* itr, int n) {
    while (itr->node_ptr->prev != NULL){
        int size = itr->node_ptr->array_size;

        if (n - size <= 0) break;
        n -= size;

        itr->node_ptr = itr->node_ptr->prev;
    }
    itr->position = itr->node_ptr->array_size - n;
}

// backward iteration - get n-th element from the end of the list
// Działa
int get_backward(List *list, int n) {
    if(list->head == NULL) return 0;

    iterator itr = end(list->tail);
    skip_backward(&itr, n);

    int el2return = itr.node_ptr->data[itr.position];
    return el2return;
}

// remove n-th element; if array empty remove node
// Nie działa usuwanie 1.szego elementu i ostatniego elementu( w sensie węzła)
// Niektóre usuwanie elementów nie działa

void remove_at(List *list, int n) {
    iterator itr = begin(list->head);
    skip_forward(&itr, n);
    Node *p = itr.node_ptr;
    for(int i = itr.position + 1; i < p->array_size; i++) p->data[i - 1] = p->data[i];

    p->array_size -= 1;

    if (p->array_size == 0){ //Trzeba usunąć Node'a
        if (p == list->head){
            list->head = p->next;
            list->head->prev = NULL;
        }

        else if (p == list->tail){
            list->tail = p->prev;
            list->tail->next = NULL;
        }

        else{
            p->next->prev = p->prev;
            p->prev->next = p->next;
        }
        free(itr.node_ptr->data);
        free(itr.node_ptr);
    }


}

// -------------------- helper functions

// print list
void dumpList(const List *list) {
    for(Node *node = list->head; node != NULL; node = node->next) {
        printf("-> ");
        for (int k = 0; k < node->array_size; k++) {
            printf("%d ", node->data[k]);
        }
        printf("\n");
    }
}

// remove the first element
void delete_front(List *list) {
    Node *to_delete;
    if(list->head == NULL) return;
    to_delete = list->head;
    list->head = list->head->next;
    if(list->head == NULL) list->tail = NULL;
    free(to_delete->data);
    free(to_delete);
    list->size--;
}

// free list
void freeList(List *list) {
    while(list->head) {
        delete_front(list);
    }
}

// read int vector
void read_vector(int tab[], int n) {
    for (int i = 0; i < n; ++i) {
        scanf("%d", tab + i);
    }
}

// initialize the list and push data
void read_list(List *list) {
    int size, n;
    init(list);
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &size);
        int *tab = (int*) malloc((size_t)size * sizeof(int));
        read_vector(tab, size);
        push_back(list, tab, size);
    }
}

int main() {
    int to_do, size, m;
    List list;

    scanf("%d", &to_do);
    read_list(&list);
    switch (to_do) {
        case 1:
            dumpList(&list);
            break;
        case 2:
            scanf("%d", &size);
            for (int i = 0; i < size; i++) {
                scanf("%d", &m);
                printf("%d ", get_forward(&list, m));
            }
            printf("\n");
            break;
        case 3:
            scanf("%d", &size);
            for (int i = 0; i < size; i++) {
                scanf("%d", &m);
                printf("%d ", get_backward(&list, m));
            }
            printf("\n");
            break;
        case 4:
            scanf("%d", &size);
            for (int i=0; i < size; i++) {
                scanf("%d", &m);
                remove_at(&list, m);
            }
            dumpList(&list);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    freeList(&list);

    return 0;
}