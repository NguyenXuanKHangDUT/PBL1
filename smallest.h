#define MAX 10000
void smallestCostMethod(int m, int supply[MAX], int n, int demand[MAX], float c[MAX][MAX], int x[MAX][MAX], int total, float *totalCost) {
    int y[MAX][MAX];
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
        int summer = (supply[i] - totalrow[i] < demand[j] - totalcol[j]) ? supply[i] - totalrow[i]: demand[j] - totalcol[j];
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
/*cac vi du:
4,4,
20,40,70,30
50,20,40,50
1,4,5,7
9,6,9,3
4,6,1,5
1,2,3,5,

5,4,
10,50,20,80,20,
30,80,10,60,
3,2,2,3,
4,2,2,3,
6,4,2,2,
8,5,3,4,
9,5,2,2,
*/
