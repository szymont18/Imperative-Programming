#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STR_LEN 64
#define MAX_PERSONS 1024

typedef struct Person {
    int age;
    char first_name[MAX_STR_LEN];
    char last_name[MAX_STR_LEN];
} Person;

// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int cmp_person(const void *p1, const void *p2){
    Person P1 = *(Person*)p1;
    Person P2 = *(Person*)p2;

    if (P1.age > P2.age) return -1;
    if (P1.age < P2.age) return 1;
    if (P1.age == P2.age){
        int x = strcmp(P1.first_name, P2.first_name);

        if (x == 0) return strcmp(P1.last_name, P2.last_name);

        return x;
    }

}

// Read data to Person array (till EOF)
int read_person_array(Person *persons){
    int person_index = 0;

    char buffor[MAX_STR_LEN * 2 + 3]; //Dwa razy bo imię i nazwisko

    char x = getchar();

    while (x != EOF){
        int buffor_index = 0;

        while(x != EOF && x != '\n'){
            buffor[buffor_index] = x;
            buffor_index++;
            x = getchar();
        }

        Person new_person;
        buffor[buffor_index] = '\0';
        sscanf(buffor, "%d %s %s", &new_person.age, &new_person.first_name, &new_person.last_name);
        if (x == '\n') x = getchar();
        persons[person_index] = new_person;
        person_index++;
    }

    return person_index;
}

// Print Person array
void print_person_array(Person *persons, int n){
    for(int i = 0; i < n; i++){
        printf("%d ", persons[i].age);
        printf("%s ", persons[i].first_name);
        printf("%s ", persons[i].last_name);
        printf("\n");
    }
}

// Sort women first (woman's first name ends with 'a');
// Then sort women by age and men by last name
// Line consists of: age, first_name, last_name
// (int that order)
int cmp_lines(const void *l1, const void *l2){
    char *L1 = (char *)l1;
    char *L2 = (char *)l2;

    Person P1;
    sscanf(L1, "%d %s %s", &P1.age, &P1.first_name, &P1.last_name);

    Person P2;
    sscanf(L2, "%d %s %s", &P2.age, &P2.first_name, &P2.last_name);

    int gender1 = 1, gender2 = 1;
    int size1 = strlen(P1.first_name), size2 = strlen(P2.first_name);

    if (P1.first_name[size1 - 1] == 'a') gender1 = 0;
    if (P2.first_name[size2 - 1] == 'a') gender2 = 0;

    if(gender1 == 1 && gender2 == 0) return 1;
    if(gender1 == 0 && gender2 == 1) return -1;

    if(gender1 == 1 && gender2 == 1) return strcmp(P1.last_name, P2.last_name);

    return P1.age - P2.age;

}

// Read lines with students' data (as text)
int read_lines(char lines[][MAX_STR_LEN]){
    char x = getchar();
    int person_index = 0;

    while(x != EOF){
        int in_person_index = 0;
        while(x != EOF && x != '\n'){
            lines[person_index][in_person_index] = x;
            in_person_index++;
            x = getchar();
        }
        if (in_person_index > 0) {
            lines[person_index][in_person_index] = '\0';
            person_index++;
        }

        if (x == '\n') x = getchar();
    }
    return person_index;
}

// Print sorted lines
void print_lines(char lines[][MAX_STR_LEN], int n){
    for(int i = 0; i < n; i++){
        printf("%s\n", lines[i]);
    }
}

// -------------------------------------------------

int read_int() {
    char buf[MAX_STR_LEN];
    int n;
    fgets(buf, MAX_STR_LEN, stdin);
    sscanf(buf, "%d", &n);
    return n;
}

int main(void) {
    int to_do = read_int();
    int n;
    Person persons[MAX_PERSONS];
    char lines[MAX_PERSONS][MAX_STR_LEN];
    switch (to_do) {
        case 1:
            n = read_person_array(persons);
            qsort(persons, (size_t)n, sizeof(Person), cmp_person);
            print_person_array(persons, n);
            break;
        case 2:
            n = read_lines(lines);
            qsort(lines, (size_t) n, MAX_STR_LEN, cmp_lines);
            print_lines(lines, n);
            break;
        default:
            printf("Nothing to do for %d\n", to_do);
            break;
    }
}

