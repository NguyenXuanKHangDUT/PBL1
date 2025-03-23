#define MAX 10000
float findTwoLowestValueInRow(int row, int n, float c[MAX][MAX]) {
    float a[MAX];
    for (int j = 0; j < n; j++) { a[j] = c[row][j]; }
    int j, J = -1;
    float m1 = 1e9, m2 = 1e9;
    for (j = 0; j < n; j++) {
        if (a[j] < m1) { m1 = a[j]; J = j; }
    }
    a[J] = 1e9;
    for (j = 0; j < n; j++) {
        if (a[j] < m2) { m2 = a[j]; }
    }
    return m2 - m1;
}
float findTwoLowestValueInCol(int m, int col, float c[MAX][MAX]) {
    float a[MAX];
    for (int i = 0; i < m; i++) { a[i] = c[i][col]; }
    int i, I = -1;
    float m1 = 1e9, m2 = 1e9;
    for (i = 0; i < m; i++) {
        if (a[i] < m1) { m1 = a[i]; I = i; }
    }
    a[I] = 1e9;
    for (i = 0; i < m; i++) {
        if (a[i] < m2) { m2 = a[i]; }
    }
    return m2 - m1;
}

float minInEachRow(int row, int n, float c[MAX][MAX]) {
    float min = 1e9;
    for (int j = 0; j < n; j++) {
        if (c[row][j] < min) { min = c[row][j]; }
    }
    return min;
}
float minInEachCol(int m, int col, float c[MAX][MAX]) {
    float min = 1e9;
    for (int i = 0; i < m; i++) {
        if (c[i][col] < min) { min = c[i][col]; }
    }
    return min;
}

void vogelMethod(int m, int supply[MAX], int n, int demand[MAX], float c[MAX][MAX], int x[MAX][MAX], float *totalCost) {
    int rowDoneYet[MAX] = {0}, colDoneYet[MAX] = {0}, totalRow[MAX] = {0}, totalCol[MAX] = {0};

    float minInRow[MAX], minInCol[MAX];
    for (int i = 0; i < m; i++) {
        minInRow[i] = minInEachRow(i, n, c);
    }
    for (int j = 0; j < n; j++) {
        minInCol[j] = minInEachCol(m, j, c);
    }

    while (1) {
        float penRow[MAX], penCol[MAX];
        for (int i = 0; i < m; i++) {
            penRow[i] = rowDoneYet[i]? -1e9 : findTwoLowestValueInRow(i, n, c);
        }
        for (int j = 0; j < n; j++) {
            penCol[j] = colDoneYet[j]? -1e9 : findTwoLowestValueInCol(m, j, c);
        }

        int I = -1, J = -1;
        float maxPenRow = -1e9, maxPenCol = -1e9;
        for (int i = 0; i < m; i++) {
            if (penRow[i] > maxPenRow || (penRow[i] == maxPenRow && !rowDoneYet[i] && (I == -1 || minInRow[i] < minInRow[I]) )  ) {
                maxPenRow = penRow[i]; I = i;
            }
        }
        for (int j = 0; j < n; j++) {
            if (penCol[j] > maxPenCol || (penCol[j] == maxPenCol && !colDoneYet[j] && (J == -1 || minInCol[j] < minInCol[J]) )  ) {
                maxPenCol = penCol[j]; J = j;
            }
        }

        if (maxPenRow > maxPenCol || 
            (maxPenRow == maxPenCol && (minInRow[I] < minInCol[J] || 
             (minInRow[I] == minInCol[J] && c[I][J] < c[I][J])      )   )      ) 
            {
                J = -1;
                for (int j = 0; j < n; j++) {
                    if (!colDoneYet[j] && (J == -1 || c[I][j] < c[I][J])) J = j;
                }
        }
        else {
            I = -1;
            for (int i = 0; i < m; i++) {
                if (!rowDoneYet[i] && (I == -1 || c[i][J] < c[I][J])) I = i;
            }
        }
        if (I == -1 || J == -1) break;

        int summer = (supply[I] - totalRow[I] < demand[J] - totalCol[J]) ? supply[I] - totalRow[I] : demand[J] - totalCol[J];
        x[I][J] = summer;
        *totalCost += summer * c[I][J];
        totalRow[I] += summer;
        totalCol[J] += summer;
        if (totalRow[I] == supply[I]) rowDoneYet[I] = 1;
        if (totalCol[J] == demand[J]) colDoneYet[J] = 1;

        for (int i = 0; i < m; i++) {
            if (rowDoneYet[i]) {
                minInRow[i] = 1e9;
            }
        }
        for (int j = 0; j < n; j++) {
            if (colDoneYet[j]) {
                minInCol[j] = 1e9;
            }
        }

        int allDone = 1;
        for (int i = 0; i < m; i++) {
            if (!rowDoneYet[i]) allDone = 0;
        }
        for (int j = 0; j < n; j++) {
            if (!colDoneYet[j]) allDone = 0;
        }
        if (allDone) break;
    }
}
/*
3,4
30,50,20
20,40,30,10
1,2,1,4
3,3,2,1
4,2,5,9

3,4
7,9,18
5,8,7,14
19,30,50,10
70,30,40,60
40,8,70,20
*/
//có các trường hợp khi penalty trùng nhiều, code này sẽ cho kết quả khác với VAM nguyên gốc, nhưng vẫn là VAM
