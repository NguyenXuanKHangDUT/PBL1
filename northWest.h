#define MAX 10000
void northWestMethod(int n, int a[MAX], int m, int b[MAX], float c[MAX][MAX], int x[MAX][MAX], float *totalCost) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {x[i][j] = 0;}
    }
    int i = 0, j = 0;
    int totalrow[100] = {0}, totalcol[100] = {0}; 
    do {
        int summer = (b[i] - totalrow[i] < a[j] - totalcol[j]) ? b[i] - totalrow[i]: a[j] - totalcol[j];
        x[i][j] = summer;
        *totalCost += summer * c[i][j];
        totalrow[i] += summer;
        totalcol[j] += summer;
        if (totalrow[i] == b[i]) {i++;}
        if (totalcol[j] == a[j]) {j++;}
    } while (i < m && j < n);
}
