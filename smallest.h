#define MAX 10000
 void smallestCostMethod(int n, int a[MAX], int m, int b[MAX], float c[MAX][MAX], int x[MAX][MAX], int total, float *totalCost) {
     int y[100][100];
     for (int i = 0; i < m; i++) {
         for (int j = 0; j < n; j++) {x[i][j] = 0; y[i][j] = c[i][j];}
     }
 
     int totalcung = total, totalcau = total;
     int totalrow[100] = {0}, totalcol[100] = {0}; 
     while (totalcung > 0 && totalcau > 0) {
         float min = 1e9;
         int i, j;
         for (int I = 0; I < m; I++) {
             for (int J = 0; J < n; J++) {
                 if (c[I][J] < min && y[I][J] != 1e9) {min = y[I][J]; i = I; j = J;}
             }
         }
         y[i][j] = 1e9;
         int summer = (b[i] - totalrow[i] < a[j] - totalcol[j]) ? b[i] - totalrow[i]: a[j] - totalcol[j];
         x[i][j] = summer;
         totalrow[i] += summer;
         totalcol[j] += summer;
         totalcung -= summer; 
         totalcau -= summer;
     }
 
     float t = 0;
     for (int i = 0; i < m; i++) {
         for (int j = 0; j < n; j++) {
             t += x[i][j] * c[i][j];
         }
     }
     *totalCost = t;  
 }
