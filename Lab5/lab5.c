#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
   for(int i = 0; i < n; ++i) {
      scanf("%lf", x++);
   }
}

void print_vector(double x[], int n) {
   for(int i = 0; i < n; ++i) {
      printf("%.4f ", x[i]);
   }
   printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
   for(int i = 0; i < m; ++i) {
      for(int j = 0; j < n; ++j) {
         scanf("%lf", &A[i][j]);
      }
   }
}

void print_mat(double A[][SIZE], int m, int n) {
   for(int i = 0; i < m; ++i) {
      for(int j = 0; j < n; ++j) {
         printf("%.4f ", A[i][j]);
      }
      printf("\n");
   }
}

void print_mat_ind(double A[][SIZE], int m, int n, const int indices[]);

// 5.1
// Calculate matrix product, AB = A X B
// A[m][p], B[p][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n){
    double suma = 0;
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            suma = 0;

            for(int k = 0; k < p; k++){
                suma = suma + A[i][k] * B[k][j];
            }
        AB[i][j] = suma;
        }
    }
}

// Calculate matrix - vector product
void mat_vec_product(double A[][SIZE], const double b[], double Ab[], int m, int n);

void backward_substit(double A[][SIZE], double x[], int n);

void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n);

// 5.2
// Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n){
    double product = 1;
    for (int i = 0; i < n - 1; i++){ // Pętla idąca po przekątnych(ostatni wiesz już się nie wlicza bo on nie ma co zerować
        if(A[i][i] == 0){
            return NAN;
        }
        product *= A[i][i];


//        printf("\n");
//        printf("Iteracja %d", i);
//        for(int a = 0; a < n; a++){
//            printf("\n");
//            for (int b = 0; b < n; b++){
//                printf("%lf ", A[a][b]);
//            }
//        }


        for(int j = i + 1; j < n; j++){ // Metoda Gaussa - pętla po wierszach
            double mnoz = A[j][i] / A[i][i];
            for (int k = i; k < n; k++){ // Metoda Gaussa - pętla po kolumnach
                A[j][k] -= (A[i][k] * mnoz);
            }
        }
    }// Tutaj już macierz jest trójkątna
    if (A[n - 1][n - 1] == 0) return NAN;
    product *= A[n - 1][n - 1];
    return product;

}

// 5.3
// Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps){
    int indeks[n];
    for(int i = 0; i < n; i++){// Indeks przechowuje informację o kolejności wierszy
        indeks[i] = i;
    }
    //Przepisanie stałych z b do x

    for (int i = 0; i < n; ++i) {
        x[i] = b[i];
    }

    double product = 1;
    for(int i = 0; i < n - 1; i++){ // Czy do ostatniego elementu?
        double maks = 0;
        int actual_indeks = 0;
        for(int j = i; j < n; j++){
            if (fabs(A[indeks[j]][i]) > maks){

                maks = fabs(A[indeks[j]][i]);
                actual_indeks = j;
            }
        }
        //Podmiana
        if(i != actual_indeks) product *= -1; //Zamiana wierszy powoduję zmianę znaku wyznacznika
        int tmp = indeks[i];
        indeks[i] = indeks[actual_indeks];
        indeks[actual_indeks] = tmp;

        double tmp2 = x[i];
        x[i] = x[actual_indeks];
        x[actual_indeks] = tmp2;

//        printf("\n\n\n");
//        printf("%d-ty raz %lf:\n",i, fabs(A[indeks[i]][i]));

        if(fabs(A[indeks[i]][i]) < eps) return NAN;

        //Zerowanie
        for(int j = i + 1; j < n; j++){
            double mnoznik = A[indeks[j]][i] / A[indeks[i]][i];
            x[j] -= (mnoznik * x[i]);
            for(int k = i; k < n; k++){
                A[indeks[j]][k] -= (A[indeks[i]][k] * mnoznik);
            }
        }
//        for(int k = 0; k < n; k++){
//            for(int l = 0; l < n; l++){
//                printf("%lf ", A[indeks[k]][l]);
//            }
//            printf("\n");
//        }
    }

    for(int i = 0; i < n; i++){
        product *= A[indeks[i]][i];
    }

    if(product != 0){
        for(int i = n - 1; i >=0; i--){
            for(int j = n - 1; j > i; j--){
                x[i] -= (A[indeks[i]][j] * x[j]);
            }
            x[i] = x[i] / A[indeks[i]][i];
        }
    }

    return product;
}

// 5.4
// Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps){
    double tmp[SIZE][SIZE];
    int indeks[n];

    for(int i = 0 ; i < n; i++){
        for(int j = 0; j < n; j++){
            tmp[i][j] = A[i][j];
        }
    }
    for(int i = 0; i < n; i++){// Indeks przechowuje informację o kolejności wierszy
        indeks[i] = i;
    }

    //Wyznacznik z zadania wyżej
    double product = 1;
    for(int i = 0; i < n - 1; i++) { // Czy do ostatniego elementu?
        double maks = 0;
        int actual_indeks = 0;
        for (int j = i; j < n; j++) {
            if (fabs(tmp[indeks[j]][i]) > maks) {

                maks = fabs(tmp[indeks[j]][i]);
                actual_indeks = j;
            }
        }

        if (i != actual_indeks) product *= -1; //Zamiana wierszy powoduję zmianę znaku wyznacznika
        int tmp2 = indeks[i];
        indeks[i] = indeks[actual_indeks];
        indeks[actual_indeks] = tmp2;

        if (fabs(tmp[indeks[i]][i]) < eps) return NAN;

        //Zerowanie
        for (int j = i + 1; j < n; j++) {
            double mnoznik = tmp[indeks[j]][i] / tmp[indeks[i]][i];

            for (int k = i; k < n; k++) {
                tmp[indeks[j]][k] -= (tmp[indeks[i]][k] * mnoznik);
            }
        }
    }
    for(int i = 0; i < n; i++){
        product *= tmp[indeks[i]][i];
    }

    //Tworzenie macierzy jednoskowej

    for(int i = 0 ; i < n; i++){
        B[i][i] = 1;
    }

    for(int i = 0; i<n; i++){
        double divide = A[i][i];
        for(int j = 0; j < n; j++){
            A[i][j] = A[i][j] / divide;
            B[i][j] = B[i][j] / divide;
        }

        for(int a = 0; a < n; a++){
            if(a != i) {

                double diff = A[a][i];

                for (int b = 0; b < n; b++) {
                    A[a][b] -= (diff * A[i][b]);
                    B[a][b] -= (diff * B[i][b]);
                }
            }
        }
    }
    return product;
}

int main(void) {

   double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
   double b[SIZE], x[SIZE], det, eps = 1.e-13;

   int to_do;
   int m, n, p;

   scanf ("%d", &to_do);

   switch (to_do) {
      case 1:
         scanf("%d %d %d", &m, &p, &n);
         read_mat(A, m, p);
         read_mat(B, p, n);
         mat_product(A, B, C, m, p, n);
         print_mat(C, m, n);
         break;
      case 2:
         scanf("%d", &n);
         read_mat(A, n, n);
         printf("%.4f\n", gauss_simplified(A, n));
         break;
    case 3:
       scanf("%d", &n);
       read_mat(A,n, n);
       read_vector(b, n);
       det = gauss(A, b, x, n, eps);
       printf("%.4f\n", det);
       if(det) print_vector(x, n);
       break;
    case 4:
       scanf("%d", &n);
       read_mat(A,n,n);
       printf("%.4f\n",matrix_inv(A, B, n, eps));
       print_mat(B, n, n);
       break;
      default:
         printf("NOTHING TO DO FOR %d\n", to_do);
         break;
   }
   return 0;
}