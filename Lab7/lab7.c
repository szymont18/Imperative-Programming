#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 1000
#define MAX_RANGE 1000

typedef struct {
    int first;
    int second;
} pair;

// Add pair to existing relation if not already there
int add_relation (pair* RoS, int size, pair P){
    int diff = 1;
    for (int i = 0; i < size; i++){
        if (RoS[i].first == P.first && RoS[i].second == P.second){
            diff = 0;
            break;
        }
    }
    if (diff == 1){
        RoS[size] = P;
        size++;
    }
    return size;
}

// Case 1:
// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair* R , int size){
    for(int i = 0; i < size; i++){
        int potential1 = R[i].first;
        int potential2 = R[i].second;
        int flag1 = 0;
        int flag2 = 0;

        for(int j = 0; j < size; j++){
            if (R[j].first == potential1 && R[j].second == potential1) flag1 = 1;
            if (R[j].first == potential2 && R[j].second == potential2) flag2 = 1;
        }

        if (flag1 == 0) return 0;
        if (flag2 == 0) return 0;

    }
    return 1;
}
// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair* relation, int size){
    int flag = 1;
    for(int i = 0 ; i < size; i++){
        if (relation[i].first == relation[i].second){
            flag = 0;
            break;
        }
    }
    return flag;
}
// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair* relation, int size){
    for (int i = 0 ; i < size; i++){
        int is_pair_symmetric = 0; //Zakładam że para nie jest symetryczna
        for (int j = 0; j < size; j++){
            if (relation[i].first == relation[j].second && relation[i].second == relation[j].first) {
                is_pair_symmetric = 1;
                break;
            }

        }
        if (is_pair_symmetric == 0) return 0;
    }
    return 1;
}
// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair* relation, int size){
    for(int i = 0 ; i < size; i++){
        for (int j = 0; j < size; j++){

                if(relation[i].first == relation[j].second && relation[i].second == relation[j].first){
                    if (relation[i].first == relation[i].second) continue; //CONTINUE CZY BREAK?
                    else return 0;
                }

        }
    }
    return 1;
}
// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair* R, int size){ //Antysymetryczna i irreflective
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if (R[i].first == R[j].second && R[i].second == R[j].first) return 0;
        }
    }
    return 1;
}
// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair* R , int size){
    for(int i = 0; i < size; i++){// xRy

        for(int j = 0; j < size; j++){//yRz

            if(i != j){
                if (R[i].second == R[j].first){// xRy and yRz
                    int flag = 0;
                    for(int k = 0; k < size; k++){ //xRz

                        if (R[i].first == R[k].first && R[j].second == R[k].second){
                            flag = 1;
                            break;
                        }

                    }
                    if (flag == 0) return 0;
                }
            }
        }
    }
    return 1;
}

// Case 2:
// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair* R, int size){
    if (is_reflexive(R,size) == 1 && is_antisymmetric(R, size) == 1 && is_transitive(R, size) == 1) return 1;
    return 0;
}
int is_connected(pair*, int);

// A total order relation is a partial order relation that is connected
int is_total_order(pair* R, int size){
    if (is_partial_order(R, size) == 1 && is_connected(R, size) == 1) return 1;
    return 0;
}

int find_max_elements(pair* R, int size, int* max_tab){
    int curr = 0; //Indeks w tablicy max_tab

    for(int i = 0; i < size; i++){
        int potential = R[i].second;
        int is_max = 1;

        for(int j = 0; j < size; j++){
            if (R[j].first == potential && R[j].second != potential) {
                is_max = 0;
                break;
            }
        }
        if (is_max == 1) {
            int include = 0;
            for (int k = 0; k < curr; k++) {
                if (max_tab[k] == potential) {
                    include = 1;
                    break;
                }
            }
            if (include == 0){
                int pos = 0;
                while (pos < curr && max_tab[pos] < potential) pos++;
                for (int k = curr; k > pos; k--) max_tab[k] = max_tab[k - 1];
                max_tab[pos] = potential;
                curr++;
            }
        }
    }
    return curr;
}
int find_min_elements(pair* R, int size, int* min_tab){
    int curr = 0; //Indeks w tablicy min_tab

    for(int i = 0; i < size; i++){
        int potential = R[i].first;
        int is_min = 1;

        for(int j = 0; j < size; j++){
            if (R[j].second == potential && R[j].first != potential) {
                is_min = 0;
                break;
            }
        }
        if (is_min == 1) {
            int include = 0;
            for (int k = 0; k < curr; k++) {
                if (min_tab[k] == potential) {
                    include = 1;
                    break;
                }
            }
            if (include == 0){
                int pos = 0;
                while (pos < curr && min_tab[pos] < potential) pos++;
                for (int k = curr; k > pos; k--) min_tab[k] = min_tab[k - 1];
                min_tab[pos] = potential;
                curr++;
            }
        }
    }
    return curr;
}
int get_domain(pair* R, int size, int* domain){
    int domain_size = 0;
    for (int i = 0; i < size; i++){
        int left = R[i].first;
        int right = R[i].second;
        int is_left = 0;
        int is_right = 0;

        for(int j = 0; j < domain_size; j++){
            if (domain[j] == left) is_left = 1;
        }

        if (is_left == 0){
            int pos = 0;
            while (pos < domain_size && domain[pos] < left) pos++;
            for(int k = domain_size; k > pos; k--) domain[k] = domain[k - 1];
            domain[pos] = left;
            domain_size++;
        }

        for(int j = 0; j < domain_size; j++){
            if (domain[j] == right) is_right = 1;
        }
        if (is_right == 0){
            int pos = 0;
            while (pos < domain_size && domain[pos] < right) pos++;
            for(int k = domain_size; k > pos; k--) domain[k] = domain[k - 1];
            domain[pos] = right;
            domain_size++;
        }
    }
    return domain_size;
}
// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair* R, int size){ //TUTAJ ZMIANA KOLEJNOŚCI FUNKCJI
    int D[MAX_REL_SIZE];
    int d_size = get_domain(R, size, D);

    for(int i = 0; i < d_size; i++){
        for(int j = 0; j < d_size; j++){

            int flag = 0;
            for(int k = 0; k < size; k++){
                if (R[k].first == D[i] && R[k].second == D[j]){
                    flag = 1;
                    break;
                }
                if (R[k].first == D[j] && R[k].second == D[i]){
                    flag = 1;
                    break;
                }
            }
            if (flag == 0) return 0;
        }
    }
    return 1;
}


// Case 3:
int composition (pair* R, int size, pair* S, int size2 , pair* RoS){
    int pos = 0;
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size2; j++){
            if (R[i].second == S[j].first){
                pair P;
                P.first = R[i].first;
                P.second = S[j].second;
                pos = add_relation(RoS, pos,P);
            }
        }
    }
    return pos;
}

int cmp (pair p1, pair p2) {
    if (p1.first == p2.first) return p1.second - p2.second;
    return p1.first - p2.first;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair* relation){
    int no_reletions;
    scanf("%d", &no_reletions);
    for(int i = 0; i < no_reletions; i++){
        int first, second;
        scanf("%d", &first);
        scanf("%d", &second);
        relation[i].first = first;
        relation[i].second = second;
    }
    return no_reletions;
}

void print_int_array(int *array, int n) {
    printf("%d\n", n);
    for (int i = 0; i < n; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main(void) {
    int to_do;
    pair relation[MAX_REL_SIZE];
    pair relation_2[MAX_REL_SIZE];
    pair comp_relation[MAX_REL_SIZE];
    int domain[MAX_REL_SIZE];

    scanf("%d",&to_do);
    int size = read_relation(relation);
    int ordered, size_2, n_domain;

    switch (to_do) {
        case 1:
            printf("%d ", is_reflexive(relation, size));
            printf("%d ", is_irreflexive(relation, size));
            printf("%d ", is_symmetric(relation, size));
            printf("%d ", is_antisymmetric(relation, size));
            printf("%d ", is_asymmetric(relation, size));
            printf("%d\n", is_transitive(relation, size));
            break;
        case 2:
            ordered = is_partial_order(relation, size);
            n_domain = get_domain(relation, size, domain);
            printf("%d %d\n", ordered, is_total_order(relation, size));
            print_int_array(domain, n_domain);
            if (!ordered) break;
            int max_elements[MAX_REL_SIZE];
            int min_elements[MAX_REL_SIZE];
            int no_max_elements = find_max_elements(relation, size, max_elements);
            int no_min_elements = find_min_elements(relation, size, min_elements);
            print_int_array(max_elements, no_max_elements);
            print_int_array(min_elements, no_min_elements);
            break;
        case 3:
            size_2 = read_relation(relation_2);
            printf("%d\n", composition(relation, size,
                                       relation_2, size_2, comp_relation));
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}