#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <vector>  

#include <fstream>
#define MAX 10000

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

        while (getline(ss, piece, ',')) { 
            try {
                data.push_back(stod(piece));
            } catch (const invalid_argument& e) {
                cerr << "error: " << piece << "chi nhap so vao file csv thoi" << endl;
            }
        }
    } file.close();

    m = data[0]; n = data[1];
    for (int i = 0; i < m; i++) supply[i] = data[i + 2];
    for (int i = 0; i < n; i++) demand[i] = data[i + 2 + m];
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
void northWestMethod(int m, int supply[MAX], int n, int demand[MAX], float c[MAX][MAX], int x[MAX][MAX], float *totalCost) {
    for (int i = 0; i < m; i++) 
        for (int j = 0; j < n; j++) 
            x[i][j] = 0;
    
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
    readInputFromCSV(); //khi tao file input.csv, nhap du lieu theo thu tu: m, n \n supply[] \n demand[] \n cost[][]: ma tran m hang n cot
    northWestMethod(m, supply, n, demand, cost, x, &totalCost);   
    cout << "THE RESULT:  |_> <_|\n" << endl;
    Answer(m, n, x, totalCost);
    return 0;
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
