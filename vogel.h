#define MAX 10000
float findTwoLowestValueInRow(int n, int m, float c[MAX][MAX]) {
    float a[MAX];
    for (int i = 0; i < n; i++) {a[i] = c[n][i];}
    int i,I = -1;
    float m1 = 1e9, m2 = 1e9;
    for (i = 0; i < n; i++) {
        if (a[i] < m1) {m1 = a[i]; I = i;}
    } a[I] = 1e9;
    for (i = 0; i < n; i++) {
        if (a[i] < m2) {m2 = a[i];}
    } return m2 - m1;
}
float findTwoLowestValueInCol(int n, int m, float c[MAX][MAX]) {
    float a[MAX];
    for (int j = 0; j < m; j++) {a[j] = c[j][m];}
    int j,J = -1;
    float m1 = 1e9, m2 = 1e9;
    for (j = 0; j < m; j++) {
        if (a[j] < m1) {m1 = a[j]; J = j;}
    } a[J] = 1e9;
    for (j = 0; j < m; j++) {
        if (a[j] < m2) {m2 = a[j];}
    } return m2 - m1;
}

void vogelMethod(int n, int a[MAX], int m, int b[MAX], float c[MAX][MAX], int x[MAX][MAX], float *totalCost) {
    int rowDoneYet[MAX] = {0}, colDoneYet[MAX] = {0}, totalRow[MAX] = {0}, totalCol[MAX] = {0};
    while (1) {
        float penRow[MAX], penCol[MAX];
        for (int i = 0; i < m; i++) {
			penRow[i] = rowDoneYet[i]? -1e9 :findTwoLowestValueInRow(m, i, c);
		}
        for (int j = 0; j < n; j++) {
			penCol[j] = colDoneYet[j]? -1e9 :findTwoLowestValueInCol(j, m, c);
		}
        
        int I = -1, J = -1;
        float maxPenRow = -1e9, maxPenCol = -1e9;
        for (int i = 0; i < m; i++) {
			if (penRow[i] > maxPenRow) {maxPenRow = penRow[i]; I = i;}
		}
        for (int j = 0; j < n; j++) {
			if (penCol[j] > maxPenCol) {maxPenCol = penCol[j]; J = j;}
		}
        
        if (maxPenRow > maxPenCol) {
            J = -1;
            for (int j = 0; j < n; j++) {
				if (!colDoneYet[j] && (J == -1 || c[I][j] < c[I][J])) {J = j;}
			} 
        } else {
            I = -1;
            for (int i = 0; i < m; i++) {
				if (!rowDoneYet[i] && (I == -1 || c[i][J] < c[I][J])) {I = i;}
			} 
        }
        if (I == -1 || J == -1) break;

        int summer = (b[I] - totalRow[I] < a[J] - totalCol[J]) ? b[I] - totalRow[I]: a[J] - totalCol[J];
        x[I][J] = summer;
		*totalCost += summer * c[I][J];
        totalRow[I] += summer;
        totalCol[J] += summer;
        if (totalCol[J] == a[J]) {colDoneYet[J] = 1;}
        if (totalRow[I] == b[I]) {rowDoneYet[I] = 1;}
		
        int congratulation = 1;
        for (int i = 0; i < m; i++) {
			if (!rowDoneYet[i]) {congratulation = 0;}
		}
        for (int j = 0; j < n; j++) {
			if (!colDoneYet[j]) {congratulation = 0;}
		}

        if (congratulation) 
			break;
    }
}
