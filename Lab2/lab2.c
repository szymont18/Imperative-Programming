#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define N 100

// Losuje jedna liczbe calkowita z przedzialu [a,b] stosujac funkcje rand() i operator %
// Jezeli a>b to zwraca INT_MIN
// Jezeli b-a > RAND_MAX to zwraca INT_MAX
// Jezeli a=b to zwraca a (bez wywolywania funkcji rand)
// Else zwraca liczbe z przedzialu [a,b]
int rand_from_interval(int a, int b){
    if (a > b){
        return INT_MIN;
    }
    if ((b - a) > RAND_MAX){
        return INT_MAX;
    }
    if (a == b){
        return a;
    }
    int answer = (rand() % (b - a + 1)) + a;
    return answer;

}

// Losowa permutacja elementow zbioru liczb {0, 1, 2,..., n-1} (z rozkladem rownomiernym)
// wg algorytmu przedstawionego w opisie zadania
// jest zapisywana w tablicy tab.
// 0 < n <= 100, jezeli nie to elementy tablicy tab maja wartosci nieokreslone.
void rand_permutation(int n, int tab[]){
    for(int i = 0; i <= n-1; ++i){
        tab[i] = i;
    }

    for (int i = 0; i <= n -2; ++i){
        int k = rand_from_interval(i, n - 1);
        int tmp = tab[i];
        tab[i] = tab[k];
        tab[k] = tmp;
    }
}

// Metoda babelkowa sortowania n elementow tablicy tab w porzadku od wartosci najmniejszej do najwiekszej.
// Zwraca numer iteracji petli zewnetrznej (liczony od 1), po ktorej tablica byla uporzadkowana,
// np. dla { 0 1 2 3 7 4 5 6 } -> 1,
//     dla { 1 2 3 7 4 5 6 0 } -> 7,
//     dla { 0 1 2 3 4 5 6 7 } -> 0.
int bubble_sort(int n,int tab[]){
    int counter = 1;
    for(int i = 0; i <= n-1; ++i){
        int sorted = 1; //Jeżeli sorted = 0 to tablica nieposortowana; wpp Tablica posortowana
        for(int j = 0; j <= n-i-2; ++j){
            if (tab[j] > tab[j+1]){
                sorted = 0;
                int tmp = tab[j];
                tab[j] = tab[j+1];
                tab[j+1]= tmp;
            }
        }
        if (sorted == 1){
            return counter - 1;
        }
        counter++;
    }
    return counter - 1;

}

int main(void) {
    int nr_testu, seed;
    int a, b, n;
    int tab[N];

    scanf("%d %d",&nr_testu, &seed);
    srand((unsigned)seed); // ustawienie ziarna generatora (dla powtarzalnosci wynikow)

    switch(nr_testu) {
        case 1:
            scanf("%d %d",&a, &b);
            for(int i = 0; i < 3; ++i) {
                printf("%d ", rand_from_interval(a, b));
            }
            printf("\n");
            break;
        case 2:
            scanf("%d", &n);
            rand_permutation(n, tab);
            for(int i = 0; i < n; ++i) printf("%d ",tab[i]);
            printf("\n");
            break;
        case 3:
            scanf("%d", &n);
            rand_permutation(n, tab);
            printf("%d\n", bubble_sort(n, tab));
            break;
        default:
            printf("NOTHING TO DO!\n");
            break;
    }
    return 0;
}


