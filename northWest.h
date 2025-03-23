#define MAX 10000
void northWestMethod(int m, int supply[MAX], int n, int demand[MAX], float c[MAX][MAX], int x[MAX][MAX], float *totalCost) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {x[i][j] = 0;}
    }
    int i = 0, j = 0;
    int totalrow[100] = {0}, totalcol[100] = {0}; 
    do {
        int summer = (supply[i] - totalrow[i] < demand[j] - totalcol[j]) ? supply[i] - totalrow[i]: demand[j] - totalcol[j];
        x[i][j] = summer;
        *totalCost += summer * c[i][j];
        totalrow[i] += summer;
        totalcol[j] += summer;
        if (totalrow[i] == supply[i]) {i++;}
        if (totalcol[j] == demand[j]) {j++;}
    } while (i < m && j < n);
}
/* cac vi du:
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
