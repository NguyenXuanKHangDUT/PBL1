#include <stdio.h>
void Smallest_Cost(int m, int a[10000], int n, int b[10000], float c[10000][10000], int total) {
    int x[100][100], y[100][100];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {x[i][j] = 0; y[i][j] = c[i][j];}
    }

    int totalcung = total, totalcau = total;
    int totalrow[100] = {0}, totalcol[100] = {0}; 
    while (totalcung > 0 && totalcau > 0) {
        float min = 1000000000000;
        int i, j;
        for (int I = 0; I < n; I++) {
            for (int J = 0; J < m; J++) {
                if (c[I][J] < min && y[I][J] != 1000000) {min = y[I][J]; i = I; j = J;}
            }
        }
        y[i][j] = 1000000;
        int summer = (b[i] - totalrow[i] < a[j] - totalcol[j]) ? b[i] - totalrow[i]: a[j] - totalcol[j];
        x[i][j] = summer;
        totalrow[i] += summer;
        totalcol[j] += summer;
        totalcung -= summer; 
        totalcau -= summer;
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%d ", x[i][j]);
        }
        printf("\n");
    }

    float tcost = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            tcost += x[i][j] * c[i][j];
        }
    }
    printf("tong chi phi: %.2f (dvtt)\n", tcost);
}