#define MAX 10000
void northWestMethod(int m, int a[MAX], int n, int b[MAX], float c[MAX][MAX], int x[MAX][MAX], float *totalCost) {
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

    float t = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            t += x[i][j] * c[i][j];
        }
    }
    *totalCost = t;
}