#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  -1
#define LIST_ERROR               -2
#define PROGRAM_ERROR            -3

typedef union data_union {
    int int_data;
    char char_data;
    void *ptr_data;
} data_union;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union*);
typedef  int (*CompareDataFp)(data_union, data_union);
typedef  int (*HashFp)(data_union, int);

typedef struct ht_element {
    struct ht_element *next;
    data_union data;
} ht_element;

typedef struct hash_table {
    int size;
    int no_elements;
    ht_element **ht;
    DataFp dump_data;
    DataFp free_data;
    CompareDataFp compare_data;
    HashFp hash_function;
    DataPFp modify_data;
} hash_table;

void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if(ptr) return ptr;
    exit(MEMORY_ALLOCATION_ERROR);
}

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, int size, DataFp dump_data, DataFp free_data,
             CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data) {
    p_table->dump_data = dump_data;
    p_table->free_data = free_data;
    p_table->compare_data = compare_data;
    p_table->hash_function = hash_function;
    p_table->modify_data = modify_data;

    p_table->no_elements = 0;
    p_table->size = size;

    if (size > 0){
        p_table->ht = safe_malloc(size * sizeof (ht_element*));

        for (int i = 0; i < size; i++){
            p_table->ht[i] = safe_malloc(sizeof (ht_element));
            p_table->ht[i]->next = NULL;
        }
    }

}

// print elements of the list with hash n
void dump_list(const hash_table* p_table, int n) {
    ht_element *el2dump = p_table->ht[n]->next;

    while(el2dump != NULL){
        p_table->dump_data(el2dump->data);
        el2dump = el2dump->next;
    }
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete) {
    if (free_data != NULL) {
        free_data(to_delete->data);
    }
    free(to_delete);
}

// free all elements from the table (and the table itself)
void free_table(hash_table* p_table) {

    for(int i = 0; i < p_table->size; i++) {

        ht_element *el2del = p_table->ht[i]->next;

        while (el2del != NULL) {
            ht_element *tmp = el2del->next;
            free_element(p_table->free_data, el2del);
            el2del = tmp;
        }
        free(p_table->ht[i]);
    }

    free(p_table->ht);

}

// calculate hash function for integer k
int hash_base(int k, int size) {
    static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
    double tmp = k * c;
    return (int)floor(size * (tmp - floor(tmp)));
}

void rehash(hash_table *p_table) {
    ht_element **new_table = safe_malloc(2 * p_table->size * sizeof(ht_element *));

    ht_element **ht2rel = p_table->ht;

    //Inicjalizacja nowej tablicy

    for(int i = 0; i < p_table->size * 2; i++){
        new_table[i] = safe_malloc(sizeof (ht_element));
        new_table[i]->next = NULL;
    }

    for(int i = 0; i < p_table->size; i++){
        ht_element *old_ht = ht2rel[i]->next;

        while(old_ht != NULL){
            int h_index = p_table->hash_function(old_ht->data,p_table->size * 2);

            ht_element *tmp = old_ht->next;

            old_ht->next = new_table[h_index]->next;
            new_table[h_index]->next = old_ht;

            old_ht = tmp;
        }
    }

    //Aktualizacja danych starej listy

    p_table->ht = new_table;
    p_table->size = 2 * p_table->size;

}

// find element; return pointer to previous
//Nie rozumiem ???
ht_element *find_previous(hash_table *p_table, data_union data, int *first) {

}

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data) {
    int h_index = p_table->hash_function(*data, p_table->size);

    ht_element *first = p_table->ht[h_index]->next;

    while (first != NULL){

        if (!p_table->compare_data(*data, first->data)) return first;

        first = first->next;
    }
    return first;
}

// insert element

void insert_element(hash_table *p_table, data_union *data) {

    //MAX_RATE
    if(p_table->no_elements / p_table->size > MAX_RATE){
        rehash(p_table);
    }

    //Insert
    int h_index = p_table->hash_function(*data, p_table->size);

    ht_element *s_Table = p_table->ht[h_index]->next;

    while(s_Table != NULL){

        if (!p_table->compare_data(*data, s_Table->data)){
            if (p_table->modify_data != NULL) {
                p_table->modify_data(&s_Table->data);
            }
            return;
        }

        s_Table = s_Table->next;
    }

    ht_element *new_element = safe_malloc(sizeof (ht_element));
    new_element->data = *data;

    ht_element *s_Table2 = p_table->ht[h_index];

    new_element->next = s_Table2->next;
    s_Table2->next = new_element;


    p_table->no_elements++;

}

// remove element
// Do poprawy
void remove_element(hash_table *p_table, data_union data) {
    int h_index = p_table->hash_function(data, p_table->size);

    ht_element* prev = p_table->ht[h_index];
    ht_element* s_ht = prev->next;

    while (s_ht != NULL){
        if (!p_table->compare_data(data, s_ht->data)) break;

        prev = s_ht;
        s_ht = s_ht->next;
    }

    if (s_ht == NULL) return;

    if(s_ht != NULL) prev->next = s_ht->next;

    if (p_table->free_data != NULL) {
        p_table->free_data(s_ht->data);
    }
    free(s_ht);

    p_table->no_elements--;



}

// type-specific definitions

// int element

int hash_int(data_union data, int size) {
    return hash_base(data.int_data, size);
}

void dump_int(data_union data) {
    printf("%d ", data.int_data);
}

int cmp_int(data_union a, data_union b) {
    return a.int_data - b.int_data;
}

// read int value and insert to the union
data_union create_data_int() {
    int id;
    scanf("%d", &id);

    data_union new_Data;

    new_Data.int_data = id;

    return new_Data;

}

// char element

int hash_char(data_union data, int size) {
    return hash_base((int)data.char_data, size);
}

void dump_char(data_union data) {
    printf("%c ", data.char_data);
}

int cmp_char(data_union a, data_union b) {
    return strcmp(&a.char_data, &b.char_data);
}

// read char value and insert to the union
data_union create_data_char() {
    int cd;
    scanf(" %c", &cd);

    data_union new_Data;

    new_Data.int_data = cd;

    return new_Data;
}

// Word element

typedef struct DataWord {
    char *word;
    int counter;
} DataWord;

void dump_word(data_union data) {
    DataWord el2dump = *(DataWord*) data.ptr_data;
    printf("%s %d", el2dump.word, el2dump.counter);
}


void free_word(data_union data) {
    DataWord *el2free = (DataWord*) data.ptr_data;
    free(el2free->word);
    free(el2free);
}

int cmp_word(data_union a, data_union b) {
    DataWord *adw = (DataWord*)a.ptr_data;
    DataWord *bdw = (DataWord*)b.ptr_data;

    return strcmp(adw->word, bdw->word);

}

int hash_word(data_union data, int size) {
    int s = 0;
    DataWord *dw = (DataWord*)data.ptr_data;
    char *p = dw->word;
    while (*p) {
        s += *p++;
    }
    return hash_base(s, size);
}

// increase the counter
void modify_word(data_union *data) {
    DataWord *el = (DataWord*) data->ptr_data;
    el->counter++;
}

// allocate DataWord structure and insert to the union
data_union create_data_word(char *value) {
    char *word = strdup(value);

    for(int i = 0; i < strlen(word); i++){
        word[i] = tolower(word[i]);
    }

    DataWord *new_dw = safe_malloc(sizeof(DataWord));
    new_dw->word = word;
    new_dw->counter = 1;

    data_union new_du;
    new_du.ptr_data = new_dw;

    return new_du;
}

// read text, parse it to words, and insert those words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream) {
    char buffer[BUFFER_SIZE];
    char *sep = ".,?!:;-\n\t\r ";

    char *ptr = fgets(buffer, BUFFER_SIZE, stream);

    while(ptr != NULL){

        char *word;
        for(word = strtok(buffer, sep); word; word = strtok(NULL,sep)){
            data_union new = create_data_word(word);

            insert_element(p_table, &new);
        }
        ptr = fgets(buffer, BUFFER_SIZE, stream);
    }
}

// test primitive type list
void table_test(hash_table *p_table, int n, data_union (*create_data)()) {
    char op;
    data_union data;
    for (int i = 0; i < n; ++i) {
        scanf(" %c", &op);
        data = create_data();
        switch (op) {
            case 'r':
                remove_element(p_table, data);
                break;
            case 'i':
                insert_element(p_table, &data);
                break;
            default:
                printf("No such operation: %c\n", op);
                break;
        }
    }
}

int main(void) {
    int to_do, n, index;
    hash_table table;
    char buffer[BUFFER_SIZE];
    data_union data;

    scanf ("%d", &to_do);
    switch (to_do) {
        case 1: // test integer table
            scanf("%d %d",&n, &index);
            init_ht(&table, 4, dump_int,NULL,
                    cmp_int, hash_int, NULL);
            table_test(&table, n, create_data_int);
            printf ("%d\n", table.size);
            dump_list(&table, index);
            free_table(&table);
            break;
        case 2: // test char table
            scanf("%d %d",&n, &index);
            init_ht(&table, 4, dump_char,NULL,
                    cmp_char, hash_char, NULL);
            table_test(&table, n, create_data_char);
            printf ("%d\n", table.size);
            dump_list(&table, index);
            free_table(&table);
            break;
        case 3: // read words from text, insert into table, and print
            scanf("%s", buffer);
            init_ht(&table, 8, dump_word, free_word,
                    cmp_word, hash_word, modify_word);
            stream_to_ht(&table, stdin);
            printf ("%d\n", table.size);
            data = create_data_word(buffer);
            ht_element *e = get_element(&table, &data);
            if (e) table.dump_data(e->data);
            if (table.free_data) table.free_data(data);
            free_table(&table);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}
