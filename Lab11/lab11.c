#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define MEMORY_ALLOCATION_ERROR  -1
#define LIST_ERROR               -2
#define PROGRAM_ERROR            -3

struct tagList;
typedef void (*ConstDataFp)(const void*);
typedef void (*DataFp)(void*);
typedef  int (*CompareDataFp)(const void*, const void*);
typedef void (*InsertInOrder)(struct tagList*, void*);

typedef struct tagListElement {
    struct tagListElement *next;
    void                  *data;
} ListElement;

typedef struct tagList {
    ListElement *head;
    ListElement *tail;
    ConstDataFp dump_data;
    DataFp      free_data;
    CompareDataFp compare_data;
    InsertInOrder insert_sorted;
} List;

// -----------------------------------------------------------------
// generic functions - they are common for all instances of the list
// (independent of the data type)
// -----------------------------------------------------------------

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data,
               CompareDataFp compare_data, InsertInOrder insert_sorted) {
    p_list->head = NULL;
    p_list->tail = NULL;
    p_list->dump_data = dump_data;
    p_list->free_data = free_data;
    p_list->compare_data = compare_data;
    p_list->insert_sorted = insert_sorted;
}

// Print all elements of the list
void dump_list(const List* p_list) {

    ListElement *el2printf = p_list->head;

    while (el2printf != NULL) {
        p_list->dump_data(el2printf->data);

        el2printf = el2printf->next;

    }
}

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data) {

    ListElement *el2printf = p_list->head;

    while(el2printf != NULL){
        if (p_list->compare_data(el2printf->data, data) == 0) p_list->dump_data(el2printf->data);

        el2printf = el2printf->next;
    }

}

// Free all elements of the list
void free_list(List* p_list){

    ListElement *el2free = p_list->head;

    while(el2free != NULL){
        p_list->free_data(el2free->data);
        el2free = el2free->next;


    }
    p_list->head = NULL;
    p_list->tail = NULL;


}

// Push element at the beginning of the list

void push_front(List *p_list, void *data){

    ListElement *element = malloc(sizeof(ListElement));
    element->data = data;
    element->next = NULL;

    if (element == NULL)
        exit(MEMORY_ALLOCATION_ERROR);

    if (p_list->head == NULL)
    {
        p_list->head = element;
        p_list->tail = element;
        return;
    }

    element->next = p_list->head;
    p_list->head = element;

}

// Push element at the end of the list
void push_back(List *p_list, void *data) {
    ListElement *element = malloc(sizeof(ListElement));
    if(element == NULL) exit(MEMORY_ALLOCATION_ERROR);
    element->data = data;
    element->next = NULL;
    if(p_list->tail) p_list->tail->next = element;
    p_list->tail = element;
    if(!p_list->head) p_list->head = p_list->tail;
}

// Remove the first element
void pop_front(List *p_list) {

    if(p_list->head == NULL){
        return;
    }

    if(p_list->head == p_list->tail){ //Lista jednoelementowa
        p_list->free_data(p_list->head->data);
        p_list->head = NULL;
        p_list->tail = NULL;
    }

    else{
        p_list->free_data(p_list->head->data);
        p_list->head = p_list->head->next;
    }

}

// Reverse the list
void reverse(List *p_list) {

    if(p_list->head == NULL){ //Pusta lista - nic nie robimy
        return;
    }

    if(p_list->head == p_list->tail){
        ListElement *tmp = p_list->head;
        p_list->head = p_list->tail;
        p_list->tail = tmp;
        return;
    }

    ListElement *head_tmp = p_list->head;
    ListElement *tail_tmp = p_list->tail;

    ListElement *first = p_list->head->next;
    ListElement *prev = p_list->head;

    while(first != NULL){
        ListElement *tmp = first->next;
        first->next = prev;

        prev = first;
        first = tmp;
    }

   p_list->head = tail_tmp;
   p_list->tail = head_tmp;
   p_list->tail->next = NULL;

}

// insert element preserving the ordering (defined by insert_sorted function)
void insert_in_order(List *p_list, void *data) {
    p_list->insert_sorted(p_list, data);
}

// find element in sorted list after which to insert given element
ListElement* find_insertion_point(const List *p_list, ListElement *p_element) {
    //Funkcja zwraca  NULL jeżeli taki element nie istnieję
    //                wskaźnik do konkretnego elementu

    if (p_list->head == NULL) return NULL;

    ListElement *first = p_list->head;

    while (first != NULL){
        if (p_list->compare_data(p_element->data, first->data) == 0) return first;
        first = first->next;
    }
    return NULL;

}

// Insert element after 'previous'
void push_after(List *p_list, void *data, ListElement *previous) {
    ListElement *prev = find_insertion_point(p_list, previous);

    if (prev == NULL) push_back(p_list, data);

    else{
        ListElement *element = malloc(sizeof (ListElement));
        element->data = data;

        element->next = prev->next;
        prev->next = element;
    }
}

// Insert element preserving order (no counter)
void insert_elem(List *p_list, void *p_data) {

    ListElement *el2insert = malloc(sizeof (ListElement));
    el2insert->data = p_data;

    if(p_list ->head == NULL) push_back(p_list, p_data);

    else{
        ListElement *first = p_list->head;
        ListElement *prev = NULL;

        while (first != NULL){

            if(p_list->compare_data(p_data, first->data) < 0) break;

            prev = first;
            first = first->next;
        }

        if(prev != NULL && p_list->compare_data(p_data,prev->data) == 0) return; //Nie dodaje duplikatów

        if (first == p_list->head){
            el2insert->next = p_list->head;
            p_list->head = el2insert;
            return;
        }

        if(first == NULL) {
            push_back(p_list, p_data);
            return;
        }

        el2insert->next = prev->next;
        prev->next = el2insert;
    }

}

// ---------------------------------------------------------------
// type-specific definitions
// ---------------------------------------------------------------

// int element
//To się robi w mainie(Zamiania inta na wskaźnik na tego int
typedef struct DataInt {
    int id;
} DataInt;

void dump_int(const void *d) {
    DataInt el2dump = *(DataInt*)d;

    printf("%d ", el2dump.id);
}

void free_int(void *d) {
    free(d);
}

int cmp_int(const void *a, const void *b) {
    int el1 = *(int *)a;
    int el2 = *(int *)b;

    return el1 - el2;
}

DataInt *create_data_int(int v) {
    DataInt *element = malloc(sizeof (DataInt));
    element->id = v;
    return element;
}

// Word element

typedef struct DataWord {
    char *word;
    int counter;
} DataWord;

void dump_word (const void *d) {
    DataWord el2dump = *(DataWord *)d;

    printf("%s\n", el2dump.word);

}

void dump_word_lowercase (const void *d) {
    DataWord el2dump = *(DataWord * )d;

    for (int i = 0; el2dump.word[i]; i++){
        printf("%c", tolower(el2dump.word[i]));
    }
    printf("\n");

}

void free_word(void *d) {
    DataWord el2free = *(DataWord *)d;
    free(el2free.word);
    free(d);
}

// conpare words case insensitive
int cmp_word_alphabet(const void *a, const void *b) {
    DataWord el1 = *(DataWord *)a;
    DataWord el2 = *(DataWord *)b;
    return strcasecmp(el1.word, el2.word);
}

int cmp_word_counter(const void *a, const void *b) {
    DataWord el1 = *(DataWord *)a;
    DataWord el2 = *(DataWord *)b;
    //printf("%d --> %s,      %d ---> %s\n",el1.counter,el1.word,el2.counter,el2.word);
    return el1.counter - el2.counter;
}

// insert element; if present increase counter
// Ma dodawać posortowane
void insert_elem_counter(List *p_list, void *data) {

    if (p_list->head == NULL) {
        push_front(p_list, data);
        return;
    }

    ListElement *new_el = malloc(sizeof (ListElement));
    new_el->data = data;

    ListElement *first = p_list->head;
    ListElement *prev = NULL;

    while (first != NULL){
        if (p_list->compare_data(first->data, data) == 0){
            DataWord x = *(DataWord*)first->data;

            DataWord *new_data = malloc(sizeof (DataWord));
            new_data->word = x.word;
            new_data->counter = x.counter + 1;

            first->data = new_data;

            return;
        }

        if(p_list->compare_data(data, first->data) < 0){
            if(prev == NULL){
                push_front(p_list,data);
                return;
            }
            else{
                prev->next = new_el;
                new_el->next= first;
                return;
            }
        }

        prev = first;
        first = first->next;
    }

    if(first == NULL){
        push_back(p_list, data);
    }
}

// read text, parse it to words, and insert those words to the list
// in order given by the last parameter (0 - read order,
// 1 - alphabetical order)
void stream_to_list(List *p_list, FILE *stream, int order) {

    char buff[BUFFER_SIZE];
    char *ptr = fgets(buff, BUFFER_SIZE, stream);
    char *sep1 = ".,?!:;-\n\t\r ";

    while (ptr != NULL){
        char *word;
        for(word = strtok(buff, sep1); word; word = strtok(NULL, sep1)){
            char *element = malloc((strlen(word) + 1) * sizeof(char));

            element = strdup(word);

            DataWord *new_el = malloc(sizeof (DataWord));
            new_el->word = element;
            new_el->counter = 1;
            if (order == 0){
                push_back(p_list, new_el);
            }
            else{
                p_list->insert_sorted(p_list, new_el);
            }

        }
        ptr = fgets(buff, BUFFER_SIZE, stream);

    }
   //dump_list(p_list);

}

// test integer list
void list_test(List *p_list, int n) {
    char op[2];
    int v;
    for (int i = 0; i < n; ++i) {
        scanf("%s", op);
        switch (op[0]) {
            case 'f':
                scanf("%d", &v);
                push_front(p_list, create_data_int(v));
                break;
            case 'b':
                scanf("%d", &v);
                push_back(p_list, create_data_int(v));
                break;
            case 'd':
                pop_front(p_list);
                break;
            case 'r':
                reverse(p_list);
                break;
            case 'i':
                scanf("%d", &v);
                insert_in_order(p_list, create_data_int(v));
                break;
            default:
                printf("No such operation: %s\n", op);
                break;
        }
    }
}

int main(void) {
    int to_do, n;
    List list;

    scanf ("%d", &to_do);
    switch (to_do) {
        case 1: // test integer list
            scanf("%d",&n);
            init_list(&list, dump_int, free_int,
                      cmp_int, insert_elem);
            list_test(&list, n);
            dump_list(&list);
            free_list(&list);
            break;
        case 2: // read words from text, insert into list, and print
            init_list(&list, dump_word, free_word,
                      cmp_word_alphabet, insert_elem_counter);
            stream_to_list(&list, stdin, 0);
            dump_list(&list);
            free_list(&list);
            break;
        case 3: // read words, insert into list alphabetically, print words encountered n times
            scanf("%d",&n);
            init_list(&list, dump_word_lowercase, free_word,
                      cmp_word_alphabet, insert_elem_counter);
            stream_to_list(&list, stdin, 1);
            list.compare_data = cmp_word_counter;
            DataWord data = { NULL, n };
//			list.dump_data = dump_word_lowercase;
            dump_list_if(&list, &data);
            printf("\n");
            free_list(&list);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}
