#include <iostream>
#include <iomanip>
#include <vector>
#include <stdlib.h>

#include <fstream>
#define MAX 100 //khong the chay noi voi kich thuoc > 100

using namespace std;

int m, n, supply[MAX], demand[MAX], x[MAX][MAX];
float cost[MAX][MAX], totalCost = 0;

void readInputFromCSV() {
    ifstream file("input.csv"); //input.csv la ten file, co the thay doi
    vector<double> data; 
    string line;

    while (getline(file, line)) { 
        stringstream ss(line); 
        string piece;

        while (getline(ss, piece, ' ')) { 
            try {
                data.push_back(stod(piece));
            }
            catch (const invalid_argument& e) {
                cerr << "error: " << piece << "only type number" << endl;
            }
        }
    } file.close();

    m = data[0]; n = data[1];
    for (int i = 0; i < m; i++) supply[i] = data[i+2];
    for (int i = 0; i < n; i++) demand[i] = data[i+2 + m];
    int k1 = 0, k2 = 0;
    for (int j = 0; j < m; j++) k1 += supply[j];
    for (int i = 0; i < n; i++) k2 += demand[i];

    if (k1 < k2) {cout << "cung < cau, khong can bang"; exit(0);}
    else if (k1 > k2) {cout << "cung > cau, khong can bang"; exit(0);}

    int member = 2 + m + n;
    for (int i = 0; i < m; i++) 
        for (int j = 0; j < n; j++) 
            cost[i][j] = data[member++];
}

float Penalty(int iORj, int restSize, float c[MAX][MAX], int done[MAX], int Row) {
    float copy[MAX][MAX], m1 = 1e9, m2 = 1e9;
    for (int i = 0; i < MAX; i++) 
        for (int j = 0; j < MAX; j++) 
            copy[i][j] = c[i][j];

    if (Row) {
        for (int j = 0; j < restSize; j++) 
            if (!done[j]) 
                if (copy[iORj][j] < m1) 
                    m1 = copy[iORj][j];
        for (int j = 0; j < restSize; j++) 
            if (!done[j] && copy[iORj][j] == m1) {
                copy[iORj][j] = 1e9;
                break;
            }
        for (int j = 0; j < restSize; j++) 
            if (!done[j]) 
                if (copy[iORj][j] < m2) 
                    m2 = copy[iORj][j];
    }

    else {
        for (int i = 0; i < restSize; i++) 
            if (!done[i]) 
                if (copy[i][iORj] < m1) 
                    m1 = copy[i][iORj];
        for (int i = 0; i < restSize; i++) 
            if (!done[i] && copy[i][iORj] == m1) {
                copy[i][iORj] = 1e9;
                break;
            }
        for (int i = 0; i < restSize; i++)
            if (!done[i]) 
                if (copy[i][iORj] < m2) 
                    m2 = copy[i][iORj];
    }
    return m2 - m1;
}

//Ham tim penalty cao nhat:
void findCR7(int m, int n, float penRow[MAX], float penCol[MAX], int rowDone[MAX], int colDone[MAX], int &I, int &J, int &row) {
    float maxPen = -1;
    I = -1; J = -1;
    row = 1;
    for (int i = 0; i < m; i++) 
        if (!rowDone[i] && penRow[i] > maxPen) {
            maxPen = penRow[i];
            I = i;
            row = 1;
        }

    for (int j = 0; j < n; j++) 
        if (!colDone[j] && penCol[j] > maxPen) {
            maxPen = penCol[j];
            J = j;
            row = 0;
        }
}
//Ham tim phan tu be nhat tren Hang/Cot
void minInRowOrCol(int m, int n, float c[MAX][MAX], int rowDone[MAX], int colDone[MAX], int &I, int &J, int Row) {
    if (Row) {
        float min = 1e9;
        for (int j = 0; j < n; j++)
            if (!colDone[j] && c[I][j] < min) {
                min = c[I][j];
                J = j;
            }
    } 
    else {
        float min = 1e9;
        for (int i = 0; i < m; i++) 
            if (!rowDone[i] && c[i][J] < min) {
                min = c[i][J];
                I = i;
            }
    }
}

void vogelMethod(int m, int supply[MAX], int n, int demand[MAX], float c[MAX][MAX], int x[MAX][MAX], float *totalCost) {
    int rowDone[MAX] = {0}, colDone[MAX] = {0};
    int totalRow[MAX] = {0}, totalCol[MAX] = {0};

    while (1) {
        float penRow[MAX], penCol[MAX];

        for (int i = 0; i < m; i++) 
            penRow[i] = rowDone[i]? -1 : Penalty(i, n, c, colDone, 1);
        for (int j = 0; j < n; j++) 
            penCol[j] = colDone[j]? -1 : Penalty(j, m, c, rowDone, 0);
    
        int I = -1, J = -1;
        int row = 1;
        findCR7(m, n, penRow, penCol, rowDone, colDone, I, J, row);
        minInRowOrCol(m, n, c, rowDone, colDone, I, J, row);
        if (I == -1 || J == -1) break;

        int summer = supply[I] - totalRow[I] < demand[J] - totalCol[J]? supply[I] - totalRow[I] : demand[J] - totalCol[J];
        x[I][J] = summer;
        *totalCost += summer * c[I][J];
        totalRow[I] += summer;
        totalCol[J] += summer;

        if (totalRow[I] == supply[I]) 
            rowDone[I] = 1;
        if (totalCol[J] == demand[J]) 
            colDone[J] = 1;
    }
}
void Answer(int m, int n, int x[][MAX], float totalcost) {
    int w = 5; 
    for (int j = 0; j < n; j++) 
        cout << "+" << string(w, '-');
    cout << "+\n";
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) 
            cout << "| " << setw(w - 2) << x[i][j] << " ";
        cout << "|\n";
        for (int j = 0; j < n; j++) 
            cout << "+" << string(w, '-');
        cout << "+\n";
    }
    cout << "Total cost = " <<  totalcost << endl;
}

int main() {
    readInputFromCSV(); //khi tao file input.csv, nhap du lieu theo thu tu: m, n \n supply[] \n demand[] \n cost[][]
    vogelMethod(m, supply, n, demand, cost, x, &totalCost);
    cout << "THE RESULT: \n" << endl;
    Answer(m, n, x, totalCost);
    return 0;
}
/* cac vi du:
5,4,
10,50,20,80,20,
30,80,10,60,
3,2,2,3,
4,2,2,3,
6,4,2,2,
8,5,3,4,
9,5,2,2,

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
