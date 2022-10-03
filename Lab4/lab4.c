#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_DIGRAMS (LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR)
#define MAX_LINE 128
#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_DIGRAMS] = { 0 };

// function to be used as comparator to qsort (compares counters and thes sorts
// alphabetically when equal)
int cmp (const void *a, const void *b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    if (count[va] == count[vb]) return va - vb; // sort alphabetically if counts equal
    return count[vb] - count[va];
}

// function to be used as comparator to qsort for digrams (compares counters and
// thes sorts alphabetically when equal)
int cmp_di (const void *a, const void *b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    // sort according to second char if counts and the first char equal
    if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
    // sort according to first char if counts equal
    if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
    return count[vb] - count[va];
}

// count number of lines (nl), number of words (nw) and number of characters
// (nc) in the text read from stdin
void wc(int *nl, int *nw, int *nc){
    // Wyzerowanie wskaźników
    *nl = 0;
    *nw = 0;
    *nc = 0;

    char one_line[MAX_LINE];

    while ((fgets(one_line, MAX_LINE, stdin)) != NULL){
        *nl += 1;

        for (int i = 0; i < MAX_LINE; i++){
            *nc += 1; // Znak zawsze się pojawi

            if (one_line[i] == 10){
                // Należy jeszcze sprawdzić czy już raz słowo nie zostało policzone
                if(i>=1 && one_line[i - 1] != 10 && one_line[i - 1] != 32 && one_line[i - 1] != 9){
                    *nw += 1;
                }
                //Znak końca lini, więc one_line nie posiada już żadnych znaków
                break;
            }

            if (i >= 1 && one_line[i - 1] != 10 && one_line[i - 1] != 32 && one_line[i - 1] != 9
            && (one_line[i] == 9 || one_line[i] == 10 || one_line[i] == 32)){
                *nw += 1;
            }
        }
    }
}

// count how many times each character from [FIRST_CHAR, LAST_CHAR) occurs
// in the text read from stdin. Sort chars according to their respective
// cardinalities (decreasing order). Set n_char and cnt to the char_no - th char
// in the sorted list and its cardinality respectively
void char_count(int char_no, int *n_char, int *cnt){
    for(int j = 0; j < MAX_CHARS; j++){
        count[j] = 0;
    }

    for (int i = getchar(); i != EOF; i = getchar()){
        if (i > 32) {
            count[i - 33] = count[i - 33] + 1;
        }
    }
    int indeks[MAX_CHARS];
    for (int i = 0; i < MAX_CHARS; i++){
        indeks[i] = i;
    }
    qsort(indeks,MAX_CHARS, sizeof(int), cmp);


    *cnt = count[indeks[char_no - 1]];
    *n_char = indeks[char_no - 1] + 33;
}

// count how many times each digram (a pair of characters, from [FIRST_CHAR,
// LAST_CHAR) each) occurs in the text read from stdin. Sort digrams according
// to their respective cardinalities (decreasing order). Set digram[0] and
// digram[1] to the first and the second char in the digram_no - th digram_char
// in the sorted list. Set digram[2] to its cardinality.
void digram_count(int digram_no, int digram[]){
    for(int i = 0 ; i < MAX_DIGRAMS; i++){
        count[i] = 0;
    }

    for(int i = getchar(); i != EOF;){
        int j = getchar();
        if(j != EOF && j > 32 && i > 32 && j < 127 && i < 127){
            count[(i-33) * 94 + (j - 33)]++;
        }
        i = j;
    }

    int indeks[MAX_DIGRAMS];
    for (int i = 0; i < MAX_DIGRAMS; i++){
        indeks[i] = i;
    }
    qsort(indeks,MAX_DIGRAMS, sizeof(int), cmp_di);

    digram[1] = indeks[digram_no - 1] % 94 + 33;
    digram[0] = ((indeks[digram_no - 1] - digram[1] + 33)/ 94) + 33;
    digram[2] = count[indeks[digram_no - 1]];

}

// Count block and line comments in the text read from stdin. Set
// line_comment_counter and block_comment_counter accordingly
void find_comments(int *line_comment_counter, int *block_comment_counter) {
    int  line4[MAX_LINE];
    int line_counter = 0;
    int block_counter = 0;
    //Zapisywanie do line aktualnej lini i ustawienie flagi
    int flag_block = 0; //Flaga ta mówi czy został rozpoczęty komentarz blokowy ( 1 - rozpoczęty)

    int c;
    int lenght;
    for (lenght = 0; lenght < MAX_LINE && (c = getchar()) != EOF && c != '\n'; lenght++){
        line4[lenght] = c;
    }

    while (lenght != 0 || c != EOF){
        for (int i = 1; i < lenght; ++i){
            if (flag_block == 0) {

                if (line4[i] == 47 && line4[i - 1] == 47) {

                    line_counter++;
                    break; //Bo to co za nim już nie ważne
                }

                if (line4[i] == 42 && line4[i - 1] == 47) {
                    block_counter++;
                    flag_block = 1;
                }
            }
            if (line4[i] == 47 && line4[i - 1] == 42){
                flag_block = 0;
                i++;
            }
        }
        lenght = 0;
        for (lenght = 0; lenght < MAX_LINE && (c = getchar()) != EOF && c != '\n'; lenght++){
            line4[lenght] = c;
        }

    }

    *block_comment_counter = block_counter;
    *line_comment_counter = line_counter;
}


int read_line() {
    char line[MAX_LINE];
    int n;

    fgets (line, MAX_LINE, stdin); // to get the whole line
    sscanf (line, "%d", &n);
    return n;
}

int main(void) {
    int to_do;
    int nl, nw, nc, char_no, n_char, cnt;
    int line_comment_counter, block_comment_counter;
    int digram[3];

    to_do = read_line();
    switch (to_do) {
        case 1: // wc()
            wc (&nl, &nw, &nc);
            printf("%d %d %d\n", nl, nw, nc);
            break;
        case 2: // char_count()
            char_no = read_line();
            char_count(char_no, &n_char, &cnt);
            printf("%c %d\n", n_char, cnt);
            break;
        case 3: // digram_count()
            char_no = read_line();
            digram_count(char_no, digram);
            printf("%c%c %d\n", digram[0], digram[1], digram[2]);
            break;
        case 4:
            find_comments(&line_comment_counter, &block_comment_counter);
            printf("%d %d\n", block_comment_counter, line_comment_counter);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}