#include <iostream>
#include <iomanip>
#include <vector>
#include <stdlib.h>

#include <fstream>
#define ll long long
using namespace std;

ll m, n, k;
vector<ll> supply, demand;
vector<vector<ll>> x;
float totalCost = 0;
vector<vector<float>> cost;

void readInputFromCSV() {
    ifstream file("input.csv"); //input.csv la ten file, co the thay doi
    vector<float> data; 
    string line;

    while (getline(file, line)) { 
        stringstream ss(line); 
        string piece;

        while (getline(ss, piece, ',')) { 
            try {
                data.push_back(stod(piece));
            } catch (const invalid_argument& e) {
                cerr << "error: '" << piece << "' type number only!" << endl;
                exit(0);
            }
        }
    } file.close();

    m = data[0]; n = data[1];
    for (ll i = 0; i < m; i++) supply.push_back(data[i+2]);
    for (ll i = 0; i < n; i++) demand.push_back(data[i+2+m]);
    ll k1 = 0, k2 = 0;
    for (ll j = 0; j < m; j++) k1 += supply[j];
    for (ll i = 0; i < n; i++) k2 += demand[i];

    if (k1 < k2) {cout << "supply < demand, unbalanced!"; exit(0);}
    else if (k1 > k2) {cout << "cung > cau, unbalanced!"; exit(0);}
    else k = k1;

    ll member = 2 + m + n;
    for (ll i = 0; i < m; ++i) {
        vector<float> row;  
        for (ll j = 0; j < n; ++j) 
            row.push_back(data[member++]);  
        cost.push_back(row);  
    }
    
}

void northWestMethod(ll m, vector<ll>& supply, ll n, vector<ll>& demand, vector<vector<float>>& c, vector<vector<ll>>& x, float *totalCost) {
    for (ll i = 0; i < m; i++) 
        for (ll j = 0; j < n; j++) 
            x[i][j] = 0;
    
    ll i = 0, j = 0;
    vector<ll> totalrow(m, 0);
    vector<ll> totalcol(n, 0); 
    do {
        ll summer = (supply[i] - totalrow[i] < demand[j] - totalcol[j]) ? supply[i] - totalrow[i]: demand[j] - totalcol[j];
        x[i][j] = summer;
        *totalCost += summer * c[i][j];
        totalrow[i] += summer;
        totalcol[j] += summer;
        if (totalrow[i] == supply[i]) {i++;}
        if (totalcol[j] == demand[j]) {j++;}
    } while (i < m && j < n);
}

void smallestCostMethod(ll m, vector<ll>& supply, ll n, vector<ll>& demand, vector<vector<float>>& c, vector<vector<ll>>& x, ll total, float *totalCost) {
    vector<vector<ll>> y(m, vector<ll>(n, 0));
    for (ll i = 0; i < m; i++) 
        for (ll j = 0; j < n; j++) {x[i][j] = 0; y[i][j] = c[i][j];}
    
    ll totalSupply = total, totalDemand = total;
    vector<ll> totalrow(m, 0);
    vector<ll> totalcol(n, 0); 
    while (totalSupply > 0 && totalDemand > 0) {
        float min = 1e9;
        ll i, j;
        for (ll I = 0; I < m; I++) 
            for (ll J = 0; J < n; J++) 
                if (c[I][J] < min && y[I][J] != 1e9) {min = y[I][J]; i = I; j = J;}
        y[i][j] = 1e9;
        
        ll summer = (supply[i] - totalrow[i] < demand[j] - totalcol[j]) ? supply[i] - totalrow[i]: demand[j] - totalcol[j];
        x[i][j] = summer;
        *totalCost += x[i][j] * c[i][j];
        totalrow[i] += summer;
        totalcol[j] += summer;
        totalSupply -= summer;
        totalDemand -= summer;
    }
}

float Penalty(ll iORj, ll restSize, vector<vector<float>> c, vector<ll>& done, ll Row) {
    vector<vector<float>> copy = c; 
    float m1 = 1e9, m2 = 1e9;
    
    if (Row) {
        for (ll j = 0; j < restSize; j++) 
            if (!done[j]) 
                if (copy[iORj][j] < m1)
                    m1 = copy[iORj][j];
        for (ll j = 0; j < restSize; j++) 
            if (!done[j] && copy[iORj][j] == m1) {
                copy[iORj][j] = 1e9;
                break;
            }
        for (ll j = 0; j < restSize; j++) 
            if (!done[j]) 
                if (copy[iORj][j] < m2)
                    m2 = copy[iORj][j];
    }

    else {
        for (ll i = 0; i < restSize; i++) 
            if (!done[i]) 
                if (copy[i][iORj] < m1)
                    m1 = copy[i][iORj];
        for (ll i = 0; i < restSize; i++) 
            if (!done[i] && copy[i][iORj] == m1) {
                copy[i][iORj] = 1e9;
                break;
            }
        for (ll i = 0; i < restSize; i++)
            if (!done[i]) 
                if (copy[i][iORj] < m2) 
                    m2 = copy[i][iORj];
    }
    return m2 - m1;
}

//Ham tim penalty cao nhat:
void findCR7(ll m, ll n, vector<float>& penRow, vector<float>& penCol, vector<ll>& rowDone, vector<ll>& colDone, ll &I, ll &J, ll &row) {
    float maxPen = -1;
    I = -1; J = -1;
    row = 1;
    for (ll i = 0; i < m; i++)
        if (!rowDone[i] && penRow[i] > maxPen) {
            maxPen = penRow[i];
            I = i;
            row = 1;
        }

    for (ll j = 0; j < n; j++) 
        if (!colDone[j] && penCol[j] > maxPen) {
            maxPen = penCol[j];
            J = j;
            row = 0;
        }
}
//Ham tim phan tu be nhat tren Hang/Cot
void minInRowOrCol(ll m, ll n, vector<vector<float>>& c, vector<ll>& rowDone, vector<ll>& colDone, ll &I, ll &J, ll Row) {
    if (Row) {
        float min = 1e9;
        for (ll j = 0; j < n; j++)
            if (!colDone[j] && c[I][j] < min) {
                min = c[I][j];
                J = j;
            }
    } 
    else {
        float min = 1e9;
        for (ll i = 0; i < m; i++) 
            if (!rowDone[i] && c[i][J] < min) {
                min = c[i][J];
                I = i;
            }
    }
}

void vogelMethod(ll m, vector<ll>& supply, ll n, vector<ll>& demand, vector<vector<float>>& c, vector<vector<ll>>& x, float *totalCost) {
    vector<ll> rowDone(m, 0); vector<ll> colDone(n, 0);
    vector<ll> totalRow(m, 0); vector<ll> totalCol(n, 0);

    while (1) {
        vector<float> penRow(m, 0); vector<float> penCol(n, 0);

        for (ll i = 0; i < m; i++) 
            penRow[i] = rowDone[i]? -1 : Penalty(i, n, c, colDone, 1);
        for (ll j = 0; j < n; j++) 
            penCol[j] = colDone[j]? -1 : Penalty(j, m, c, rowDone, 0);
    
        ll I = -1, J = -1;
        ll row = 1;
        findCR7(m, n, penRow, penCol, rowDone, colDone, I, J, row);
        minInRowOrCol(m, n, c, rowDone, colDone, I, J, row);
        if (I == -1 || J == -1) break;

        ll summer = supply[I] - totalRow[I] < demand[J] - totalCol[J]? supply[I] - totalRow[I] : demand[J] - totalCol[J];
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

void Answer(ll m, ll n, vector<vector<ll>>& x, float totalcost) {
    ll w = 5; 
    for (ll j = 0; j < n; j++) 
        cout << "+" << string(w, '-');
    cout << "+\n";
    for (ll i = 0; i < m; i++) {
        for (ll j = 0; j < n; j++) 
            cout << "| " << setw(w - 2) << x[i][j] << " ";
        cout << "|\n";
        for (ll j = 0; j < n; j++) 
            cout << "+" << string(w, '-');
        cout << "+\n";
    }
    cout << "Total cost = " <<  totalcost << endl << endl;
}

int main() {
    readInputFromCSV(); //khi tao file input.csv, nhap du lieu theo thu tu: m, n \n supply[] \n demand[] \n cost[][]: ma tran m hang n cot
    x.resize(m, vector<ll>(n, 0));

    cout << "Read all data!\n Choose the initial basic feasible solution Method:" << endl;
    while (1) {
        cout << "\n 1. North West method\n 2. Least Cost method\n 3. VAM method \n 0. exit program\n Your choose: ";
        char choose; cin >> choose;
        if (choose == '0') 
            return 0;
        else if (choose == '1') {
            northWestMethod(m, supply, n, demand, cost, x, &totalCost);
            cout << "THE RESULT:  \n" << endl;
            Answer(m, n, x, totalCost);
        }
        else if (choose == '2') {
            smallestCostMethod(m, supply, n, demand, cost, x, k, &totalCost);
            cout << "THE RESULT:  \n" << endl;
            Answer(m, n, x, totalCost);
        }    
        else if (choose == '3') {
            vogelMethod(m, supply, n, demand, cost, x, &totalCost);
            cout << "THE RESULT:  \n" << endl;
            Answer(m, n, x, totalCost);
        }
        else {
            cout << "\nInvalid character, type again" << endl;
            continue;
        }
        totalCost = 0;

        cout << "Continue with the initial basic feasible solution methods choose the methods to find the optimal solution:\n";
        cout << "1. basic feasible solution methods\n2. the methods to find the optimal solution\n0. exit program\n Your choose: ";
        cin >> choose;
        if (choose == '0') 
            return 0;
        else if (choose == '2') {
            cout << "Choose the method to find the optimal solution\n";
            cout << " 1. MODI method\n 2. Stepping Stone method\n 3. Back to the basic feasible solution methods\n 0. exit program\n Your choose: ";
            while (1) {
                cin >> choose;
                if (choose == '0')
                    return 0;
                else if (choose == '1') {
                    cout << "COMING SOON\n Your choose:";
                }
                else if (choose == '2') {
                    cout << "COMING SOON\n Your choose:";
                }
                else if (choose == '3') 
                    break;
                else {
                    cout << "\nInvalid character, type again" << endl;
                    continue;
                }
            }
        }

    }
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
