#include <iostream>
#include <vector>

#include <fstream>
#include <sstream>

#include "northWest.h"
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

    if (k1 < k2) {cout << "cung < cau, khong can bang"; return;}
    else if (k1 > k2) {cout << "cung > cau, khong can bang"; return;}

    int member = 2 + m + n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {cost[i][j] = data[member++];}
    }
}

void writeOutputToCSV() {
    ofstream file("result.csv");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {file << x[i][j] << ",";}
        file << endl;
    } file << "Total cost: " << totalCost << endl; 
    file.close();
}

int main() {
    readInputFromCSV(); //khi tao file input.csv, nhap du lieu theo thu tu: m, n \n source[] \n receive[] \n cost[][]
    northWestMethod(m, supply, n, demand, cost, x, &totalCost);   
    writeOutputToCSV();
    cout << "Check the result in result.csv! Happy Ending!" << endl;
    return 0;
}
/*4,4,
20,40,70,30
50,20,40,50
1,4,5,7
9,6,9,3
4,6,1,5
1,2,3,5,*/
