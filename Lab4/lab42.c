#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8


#define MAX_LINE 128
#define MAX_ID_LEN 64
#define MAX_IDS 1024
#define KEYWORDS_LEN 32

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char tab[MAX_IDS][MAX_ID_LEN];
int actual_index = 0;

char *keywords[] = {
        "auto", "break", "case", "char",
        "const", "continue", "default", "do",
        "double", "else", "enum", "extern",
        "float", "for", "goto", "if",
        "int", "long", "register", "return",
        "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while"
};

int is_identifier(int potencial_id[], int id_lenght){ //Returns 0 if potencial id is not id and 1 otherwise
    if (id_lenght == 0) return 0;

    if (potencial_id[0] != 95 && !(potencial_id[0] >= 97 && potencial_id[0] <= 122) &&
        !(potencial_id[0] >= 65 && potencial_id[0] <= 90)) return 0;

    for(int i = 1; i < id_lenght; i++){
        if (potencial_id[i] != 95 && !(potencial_id[i] >= 97 && potencial_id[i] <= 122) &&
            !(potencial_id[i] >= 65 && potencial_id[i] <= 90) &&
            !(potencial_id[i] >= 48 && potencial_id[i] <= 57)) return 0; // Additionally after first pos. there can be numbers

    }
    return 1; // Everything is ok
}



int keywords_include(int potencial_unique[], int lenght){ //Returns 1 if NOT keyword

    for(int i = 0; i < KEYWORDS_LEN; i++){
        if (lenght != strlen(keywords[i])) continue;
        int flag = 0;
        for (int j = 0; j < lenght; j++){
            if (potencial_unique[j] != keywords[i][j]){
                flag = 1;
                break;
            }
        }
        if (flag == 0) return 0;
    }
    return 1;
}

int is_unique(int potencial_unique[], int lenght){ //Returns 1 is this IS unique
    for(int i = 0; i < actual_index; i++){
        if (lenght != strlen(tab[i])) continue;
        int flag = 0;
        for (int j = 0; j < lenght; j++){
            if (potencial_unique[j] != tab[i][j]) {
                flag = 1;
                break;
            }
        }
        if (flag == 0) return 0;
    }
    return 1;

}

void add(int unique[], int unique_lenght){

    for(int i = 0; i < unique_lenght; i++){
        tab[actual_index][i] = unique[i];
    }
    tab[actual_index][unique_lenght] = '\0';
    actual_index++;
}

int is_another_word(int character){ //Returns 1 if there is new world and 0 otherwise
    if (character == 9) return 1;
    if ((character >= 40 && character <= 47) || (character >= 58 && character <= 64)) return 1; // Mathematics operand or brackets
    if ((character >= 32 && character <= 40) || (character >= 91 && character <= 93) ||
        (character >= 123 && character <= 125)) return 1; // Another characters which can not be included in acceptable ID

    return 0;
}

int find_idents(){
    int line[MAX_LINE];
    int ID[MAX_ID_LEN] = {0};
    int ID_index = 0;
    int c;
    int i;
    for(i = 0; i < MAX_LINE && (c=getchar()) !='\n' && c != EOF; i++){
        line[i] = c;
    }


    int block_flag = 0;


    while(i != 0 || c != EOF) {

        for (int j = 0; j < i; j++) {
            //Check if not in string or comment
            if(block_flag == 1){
                if(line[j - 1] == 42 && line[j] == 47){
                    block_flag = 0;
                }
                continue;
            }

            if (line[j-1] == 47 && line[j] == 42){ //Block comment
                block_flag = 1;
                continue;
            }

            if(line[j-1] == 47 && line[j] == 47) break; //Inline comment

            if(line[j] == '\''){ // Char '
                j++;
                while (line[j] != '\''){
                    if(line[j] == 92)j++;
                    j++;
                }
                continue;
            }

            if(line[j] == '\"'){ //Char "
                j++;
                while (line[j] != '\"'){
                    if(line[j] == 92)j++;
                    j++;
                }
                continue;
            }

            // Not in comment etc
            if(is_another_word(line[j]) == 1){

                if(is_identifier(ID,ID_index) == 1
                && keywords_include(ID,ID_index) == 1
                && is_unique(ID,ID_index) == 1){
                    add(ID,ID_index);

                }
                ID_index = 0;

            }
            else{
                ID[ID_index] = line[j];
                ID_index++;
            }
        }
        //Before \n it can be new ID
        if(is_identifier(ID,ID_index) == 1
           && keywords_include(ID,ID_index) == 1
           && is_unique(ID,ID_index) == 1){
            add(ID,ID_index);
            ID_index = 0;
        }

        // NEW LINE !
        for(i = 0; i < MAX_LINE && (c=getchar()) !='\n' && c != EOF; i++){
            line[i] = c;
        }
    }
    for(int k = 0; k < actual_index; k++){
        for(int j = 0; j < strlen(tab[k]); j++){
            printf("%c", tab[k][j]);
        }
        printf("\n");
    }
    return actual_index;
}

int cmp(const void* first_arg, const void* second_arg) {
    char *a = *(char**)first_arg;
    char *b = *(char**)second_arg;
    return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
    int a = *(int*)first_arg;
    int b = *(int*)second_arg;
    return strcmp(tab[a], tab[b]);
}

int main(void) {
    printf("%d\n", find_idents());
    return 0;
}