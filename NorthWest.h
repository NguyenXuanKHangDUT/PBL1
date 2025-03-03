#include <stdio.h>
void North_West(int m, int a[10000], int n, int b[10000], float c[10000][10000]) {
    int x[100][100];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {x[i][j] = 0;}
    }
    int i = 0, j = 0;
    int totalrow[100] = {0}, totalcol[100] = {0}; 
    do {
        int summer = (b[i] - totalrow[i] < a[j] - totalcol[j]) ? b[i] - totalrow[i]: a[j] - totalcol[j];
        x[i][j] = summer;
        totalrow[i] += summer;
        totalcol[j] += summer;
        if (totalrow[i] == b[i]) {i++;}
        if (totalcol[j] == a[j]) {j++;}
    } while (i < n && j < m);

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