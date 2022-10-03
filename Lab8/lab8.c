#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

double get(int cols, int row, int col, const double *A){
    return A[row * cols + col];
}

void set(int cols, int row, int col, double *A, double value){
    A[row * cols +  col] = value;
}

void prod_mat(int rowsA, int colsA, int colsB, double *A, double *B, double *AB){
    for(int i = 0; i < rowsA; i++){
        for(int j = 0; j < colsB; j++){
            double sum = 0;

            for(int k = 0; k < colsA; k++){
                sum = sum + get(colsA,i, k, A) * get(colsB, k, j, B);
            }
            set(colsB, i, j, AB, sum);
        }
    }
}

void read_mat(int rows, int cols, double *t){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            scanf("%lf", &t[i * cols + j]);
        }
    }
}

void print_mat(int rows, int cols, double *t){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            printf("%.2f ", t[i * cols + j]);
        }
        printf("\n");
    }
}

int read_char_lines(char *tab[]){
    char buffor[BUF_SIZE];
    char c = getchar();
    int tab_index = 0, buffor_index = 0;

    c = getchar();
    while(c != EOF){
        buffor_index = 0;

        while(c != EOF && c != '\n'){
            buffor[buffor_index] = c;
            buffor_index++;
            c = getchar();
        }
        tab[tab_index] = malloc(sizeof(char) * (buffor_index + 1));
        for(int j = 0; j < buffor_index; j++){
            tab[tab_index][j] = buffor[j];
        }
        tab[tab_index][buffor_index] = '\0'; //Koniec linii
        tab_index++;

        if(c != EOF) c = getchar(); // Idziemy dalej
    }

    return tab_index;
}

void write_char_line(char *tab[], int n){
    for(int i = 0; tab[n - 1][i] != '\0'; i++){
        printf("%c", tab[n-1][i]);
    }
}

void delete_lines(char *tab[], int line_count){
    for(int i = 0; i < line_count; i++){
        free(tab[i]);
    }
}

int read_dbl_lines_v1(double *ptr_tab[]){
    char buffor[BUF_SIZE];
    double *double_buffor = ptr_tab[0]; //Tablica series_table z funkcji main

    int db = 0;
    int ptr_index = 0;
    char c = getchar();

    c = getchar();
    while(c != EOF){
        int i = 0;

        while(c != '\n' && c != EOF){
            buffor[i] = c;
            i++;
            c = getchar();
        }
        buffor[i] = '\n';

        double *start_line;
        start_line = &double_buffor[db];
        char *end = buffor;
        do {
            double_buffor[db] = strtod(end, &end);
            db += 1;
        }while(*end != '\n');

        ptr_tab[ptr_index] = start_line;
        ptr_index++;

        if( c != EOF) c = getchar();
    }
    ptr_tab[ptr_index] = &double_buffor[0 + db] ; //Żeby zapobiec błedowi w write
    return ptr_index;
}

void write_dbl_line_v1(double *ptr_tab[], int n){
   double *boundary = ptr_tab[n];
   double *iter = ptr_tab[n - 1];

   while(iter != boundary){
       printf("%.2f ", *iter);
       iter++;
   }

}

int main(void) {
    int to_do;

    scanf ("%d", &to_do);

    double A[TAB_SIZE], B[TAB_SIZE], C[TAB_SIZE];
    int n, lines_counter, rowsA,colsA,rowsB,colsB;
    char *char_lines_table[TAB_SIZE];
    double series_table[TAB_SIZE];
    double *ptr_table[TAB_SIZE];

    switch (to_do) {
        case 1:
            scanf("%d %d",&rowsA,&colsA);
            read_mat(rowsA, colsA, A);
            scanf("%d %d",&rowsB,&colsB);
            read_mat(rowsB, colsB, B);
            prod_mat(rowsA,colsA,colsB,A,B,C);
            print_mat(rowsA,colsB,C);
            break;
        case 2:
            scanf("%d",&n);
            ptr_table[0] = series_table;
            lines_counter = read_dbl_lines_v1(ptr_table);
            write_dbl_line_v1(ptr_table,n);
            break;
        case 3:
            scanf("%d", &n);
            lines_counter = read_char_lines(char_lines_table);
            write_char_line(char_lines_table,n);
            delete_lines(char_lines_table,lines_counter);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}