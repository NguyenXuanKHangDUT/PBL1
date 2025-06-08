#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <fstream>
#include <algorithm>

#define ll long long
#define her 1608
#define endl cout << endl
using namespace std;

ll m, n, total;
vector<ll> supply, demand;
vector<vector<ll>> x;
float totalCost = 0;
vector<vector<float>> cost;

void readInputFromCSV() {
    supply.push_back(her);
    demand.push_back(her);

    ifstream file("input.csv"); //input.csv is file name, changeably
    if (!file.is_open()) {
    cerr << "Error: Cannot optimalen file!";
    exit(1);
    }
    vector<float> data; 
    data.push_back(her); 
    //eliminate the first element of this vector so that the first inserted data will be stored at index 1
    //the purpose is to simplify the way we handle an array or a matrix in code, using indices from 1 to n instead of 0 to n-1
    string line;

    //The idea of reading file is to push all the data from the file into a vector, 
    //then, based on the determined sequence, many numbers will be separated into variables, arrays, and matrices 
    while (getline(file, line)) { 
        stringstream ss(line); 
        string piece;

        while (getline(ss, piece, ',')) {
            try {
                data.push_back(stod(piece));
            } catch (const invalid_argument& e) {
                cerr << "error: '" << piece << "' type number only!"; endl;
                exit(0);
            }
        }
    } file.close();

    m = data[1]; n = data[2];
    if (m == 0 || n == 0) {
        cout << "supply or demand remains 0, invalid input!";
        exit(0);
    }
    for (ll i = 1; i <= m; i++) supply.push_back(data[i+2]);
    for (ll i = 1; i <= n; i++) demand.push_back(data[i+2+m]);
    ll k1 = 0, k2 = 0;
    for (ll j = 1; j <= m; j++) k1 += supply[j];
    for (ll i = 1; i <= n; i++) k2 += demand[i];

    if (k1 < k2) {cout << "supply < demand, unbalanced!"; exit(0);}
    else if (k1 > k2) {cout << "supply > demand, unbalanced!"; exit(0);}
    else total = k1;

    ll member = 1 + 2+m+n; 

    vector<float> wall;
    for (ll i = 1; i <= n; i++) 
        wall.push_back(her);
    cost.push_back(wall);
    //The vector "wall" and those steps are used to eliminate the first row of the cost matrix 
    //so that the first row will be the row with index 1

    for (ll i = 1; i <= m; i++) {
        vector<float> row; row.push_back(her);
        for (ll j = 1; j <= n; j++) 
            row.push_back(data[member++]);  
        cost.push_back(row);
    }
    
}
void Answer() {
    cout << "X ="; endl;
    ll w = 5; 
    for (ll j = 1; j <= n; j++) 
        cout << "+" << string(w, '-');
    cout << "+\n";
    for (ll i = 1; i <= m; i++) {
        for (ll j = 1; j <= n; j++) 
            cout << "| " << setw(w - 2) << x[i][j] << " ";
        cout << "|\n";
        for (ll j = 1; j <= n; j++) 
            cout << "+" << string(w, '-');
        cout << "+\n";
    }
}
// void Answer() {
//     cout << "X =" << endl;
//     for (ll i = 1; i <= m; i++) {
//         for (ll j = 1; j <= n; j++)
//             cout << x[i][j] << " ";
//         cout << endl;
//     }
// } //this function is to print the result without table, in case mxn is too big
void northWestMethod() {
    x = vector<vector<ll>>(m+1, vector<ll>(n+1, 0));

    ll i = 1, j = 1, cnt = 1;
    vector<ll> totalrow(m+1, 0); totalrow.push_back(her);
    vector<ll> totalcol(n+1, 0); totalcol.push_back(her);
    do {
        Answer();
        cout << "Step " << cnt++ <<":\n";
        ll summer = supply[i]-totalrow[i] < demand[j]-totalcol[j] ? supply[i]-totalrow[i]: demand[j]-totalcol[j];
        x[i][j] = summer;
        totalCost += summer * cost[i][j];
        totalrow[i] += summer;
        totalcol[j] += summer;
        if (totalrow[i] == supply[i]) i++;
        if (totalcol[j] == demand[j]) j++;
    } while (i <= m && j <= n);
}

void leastCostMethod() {
    vector<vector<float>> y = cost;
    x = vector<vector<ll>>(m+1, vector<ll>(n+1, 0));
    
    ll totalSupply = total, totalDemand = total, cnt = 1;
    vector<ll> totalrow(m+1, 0);
    vector<ll> totalcol(n+1, 0); 

    while (totalSupply > 0 && totalDemand > 0) {
        float min = 1e9;
        ll i = -1, j = -1, summer = 0;
        for (ll I = 1; I <= m; I++) 
            for (ll J = 1; J <= n; J++) 
                if (y[I][J] <= min && y[I][J] != 1e9) {

                    if (y[I][J] < min) {
                        min = y[I][J];
                        i = I; j = J;
                        summer = supply[i]-totalrow[i] < demand[j]-totalcol[j] ? supply[i]-totalrow[i]: demand[j]-totalcol[j];
                    }
                    else if (y[I][J] == min) {
                        ll tmp = supply[I]-totalrow[I] < demand[J]-totalcol[J] ? supply[I]-totalrow[I]: demand[J]-totalcol[J];
                        if (tmp > summer) {
                            min = y[I][J]; 
                            i = I; j = J;
                            summer = tmp;    
                        }
                    }      

                }
        if (i == -1 || j == -1) break;
        y[i][j] = 1e9;
        
        Answer();
        cout << "Step " << cnt++ <<":\n";
        x[i][j] = summer;
        totalCost += x[i][j] * cost[i][j];
        totalrow[i] += summer;
        totalcol[j] += summer;
        totalSupply -= summer;
        totalDemand -= summer;
    }
}
//this function is to calculate the penalty value of each row and column
float Penalty(ll Row, ll iORj, ll restSize, vector<vector<float>> c, vector<ll>& done) {
    vector<vector<float>> copy = c; 
    float m1 = 1e9, m2 = 1e9;
    
    //I decided to use only 1 function to calculate the penalty value instead of 2 function for either row or column
    //so here, the variable "Row", "Row" indicates whether the array being calculated is a row or a column (1 is row, 0 is column) 
    //the "done" array checks whether the row or column had been done yet
    if (Row) { // if Row == 1, this function will calc in row, "restSize" will be n (the number of columns)
        for (ll j = 1; j <= restSize; j++) 
            if (!done[j]) 
                if (copy[iORj][j] < m1)
                    m1 = copy[iORj][j];
        for (ll j = 1; j <= restSize; j++) 
            if (!done[j] && copy[iORj][j] == m1) {
                copy[iORj][j] = 1e9;
                break;
            }
        for (ll j = 1; j <= restSize; j++) 
            if (!done[j]) 
                if (copy[iORj][j] < m2)
                    m2 = copy[iORj][j];
    }
    else { //else if Row == 0, this function will calc in column, "restSize" will be m (the number of rows)
        for (ll i = 1; i <= restSize; i++) 
            if (!done[i]) 
                if (copy[i][iORj] < m1)
                    m1 = copy[i][iORj];
        for (ll i = 1; i <= restSize; i++) 
            if (!done[i] && copy[i][iORj] == m1) {
                copy[i][iORj] = 1e9;
                break;
            }
        for (ll i = 1; i <= restSize; i++)
            if (!done[i]) 
                if (copy[i][iORj] < m2) 
                    m2 = copy[i][iORj];
    }
    return m2 - m1;
}

//this function is to find the lowest element in the row or column, the result is location of that element, stored in I and J
float minInRowOrCol(ll m, ll n, vector<vector<float>>& c, vector<ll>& rowDone, vector<ll>& colDone, ll &I, ll &J, ll Row) {
    float min = 1e9;
    if (Row) {
        for (ll j = 1; j <= n; j++)
            if (!colDone[j] && c[I][j] < min) {
                min = c[I][j];
                J = j;
            }
    } 
    else {
        for (ll i = 1; i <= m; i++) 
            if (!rowDone[i] && c[i][J] < min) {
                min = c[i][J];
                I = i;
            }
    }
    return min;
}
//this function is to find the row or the column with the highest penalty value, the result is location of the row or columnn, stored in I or J
void findCR7(ll m, ll n, vector<float>& penRow, vector<float>& penCol, vector<ll>& rowDone, vector<ll>& colDone, ll &I, ll &J, ll &row) {
    float maxPen = -1, lowestElement = 1e9;
    I = -1; J = -1;
    row = 1;
    ll tmpI, tmpJ;
    for (ll i = 1; i <= m; i++) {
        float lE = minInRowOrCol(m, n, cost, rowDone, colDone, i, tmpJ, 1);
        if (!rowDone[i] && penRow[i] > maxPen) {
            maxPen = penRow[i];
            I = i;
            row = 1;
            lowestElement = lE;
        }
        else if (!rowDone[i] && penRow[i] == maxPen) 
            if (lE < lowestElement) {
                I = i;
                row = 1;
                lowestElement = lE;
            }
    }
    for (ll j = 1; j <= n; j++) {
        float lE = minInRowOrCol(m, n, cost, rowDone, colDone, tmpI, j, 0);
        if (!colDone[j] && penCol[j] > maxPen) {
            maxPen = penCol[j];
            J = j;
            row = 0;
            lowestElement = lE;
        }
        else if (!colDone[j] && penCol[j] == maxPen) 
            if (lE < lowestElement) {
                J = j;
                row = 0;
                lowestElement = lE;
            }
    }
}
//this function is the main function of VAM
void vogelMethod() {
    x = vector<vector<ll>>(m+1, vector<ll>(n+1, 0));
    ll cnt = 1;
    vector<ll> rowDone(m+1, 0); vector<ll> colDone(n+1, 0);
    vector<ll> totalRow(m+1, 0); vector<ll> totalCol(n+1, 0);
    
    while (1) {
        vector<float> penRow(m+1, 0); vector<float> penCol(n+1, 0);

        for (ll i = 1; i <= m; i++) 
            penRow[i] = rowDone[i]? -1 : Penalty(1, i, n, cost, colDone); //calc all penalty values in each rows
        for (ll j = 1; j <= n; j++) 
            penCol[j] = colDone[j]? -1 : Penalty(0, j, m, cost, rowDone); //calc all penalty values in each columns
    
        ll I = -1, J = -1;
        ll row = 1;
        findCR7(m, n, penRow, penCol, rowDone, colDone, I, J, row); //find the highest penalty value row or column, stored the location to either I or J
        minInRowOrCol(m, n, cost, rowDone, colDone, I, J, row); //find the lowest element in the highest penalty row or column, stored the location to I and J
        if (I == -1 || J == -1) break; //break if all done;

        Answer();
        cout << "Step " << cnt++ <<":\n";
        ll summer = supply[I]-totalRow[I] < demand[J]-totalCol[J] ? supply[I]-totalRow[I] : demand[J]-totalCol[J];
        x[I][J] = summer;
        totalCost += summer * cost[I][J];
        totalRow[I] += summer;
        totalCol[J] += summer;

        if (totalRow[I] == supply[I]) 
            rowDone[I] = 1;
        if (totalCol[J] == demand[J])
            colDone[J] = 1;
    }
}

void theCost() {
    cout << "cost matrix:"; endl;
    ll w = 5; endl;
    for (ll j = 1; j <= n; j++) 
        cout << "+" << string(w, '-');
    cout << "+\n";
    for (ll i = 1; i <= m; i++) {
        for (ll j = 1; j <= n; j++) 
            cout << "| " << setw(w - 2) << cost[i][j] << " ";
        cout << "|\n";
        for (ll j = 1; j <= n; j++) 
            cout << "+" << string(w, '-');
        cout << "+\n";
    }
    cout << "Total cost = " <<  totalCost; endl; 
    totalCost = 0;
}

vector<vector<ll>> delta;
vector<ll> u, v;
//this function is to calculate u and v 
void calcUV() {
    u = vector<ll>(m+1, her);
    v = vector<ll>(n+1, her);

    // The idea is to find the row or column that contains the most non-zero cells in the matrix x.
    vector<ll> rowCnt(m+1, 0), colCnt(n+1, 0);
    for (ll i = 1; i <= m; ++i) 
        for (ll j = 1; j <= n; ++j) 
            if (x[i][j] != 0) {
                rowCnt[i]++;
                colCnt[j]++;
            }
    ll maxRow = -1, maxCol = -1, I = -1, J = -1;
    for (ll i = 1; i <= m; i++) 
        if (rowCnt[i] > maxRow) {
            maxRow = rowCnt[i];
            I = i;
        }
    for (ll j = 1; j <= n; j++) 
        if (colCnt[j] > maxCol) {
            maxCol = colCnt[j];
            J = j;
        }
    // That row or column will be used as the starting point for calculating the u and v potentials.
    // If a row is selected, set u[i] = 0. If a column is selected, set v[j] = 0.
    if (maxRow >= maxCol) 
        u[I] = 0;
    else 
        v[J] = 0;
    // From there, compute u and v by spreading to other cells
    ll spreading;
    do {
        spreading = 0;
        for (ll i = 1; i <= m; i++) 
            for (ll j = 1; j <= n; j++) 
                if (x[i][j] != 0) {
                    if (u[i] != her && v[j] == her) {
                        v[j] = cost[i][j]-u[i];
                        spreading = 1;
                    }
                    if (v[j] != her && u[i] == her) {
                        u[i] = cost[i][j]-v[j];
                        spreading = 1;
                    }
                }
    } while (spreading);
}

void calcDelta() {// This function calculates delta based on the formula: dij = cij - (ui + vj)
    delta = vector<vector<ll>>(m+1, vector<ll>(n+1, 0));
    for (ll i = 1; i <= m; i++) 
        for (ll j = 1; j <= n; j++) 
            if (x[i][j] == 0) 
                delta[i][j] = cost[i][j] - (u[i]+v[j]);
            else 
                delta[i][j] = 0; 
}

ll badSpot(ll &I, ll &J) {// This function finds the most negative delta value
    ll min = 0, found = 0;
    for (ll i = 1; i <= m; ++i) 
        for (ll j = 1; j <= n; ++j) 
            if (delta[i][j] < min) {
                min = delta[i][j];
                I = i; J = j;
                found = 1;
            }
    return found;
}

bool findCycle(ll Row, ll i, ll j, vector<vector<bool>> &visited, stack<pair<ll,ll>> &mapTrail, pair<ll,ll> start) {
    if (mapTrail.size() > 3 && start == make_pair(i, j)) return true;
    if (visited[i][j]) return false;

    visited[i][j] = true;
    mapTrail.push({i, j});

    if (Row) {
        for (ll J = 1; J <= n; J++)
            if (J != j && (x[i][J] != 0 || (i == start.first && J == start.second)))
                if (findCycle(0, i, J, visited, mapTrail, start))
                    return true;
    } 
    else {
        for (ll I = 1; I <= m; I++) 
            if (I != i && (x[I][j] != 0 || (I == start.first && j == start.second)))
                if (findCycle(1, I, j, visited, mapTrail, start))
                    return true;
    }
    mapTrail.pop();
    return false;
}
void applyTheta(vector<pair<ll,ll>> &turns) {
    ll theta = 1e9;
    for (ll k = 1; k < turns.size(); k += 2) {
        ll i = turns[k].first, j = turns[k].second;
        if (x[i][j] < theta)
            theta = x[i][j];
    }
    for (ll k = 0; k < turns.size(); k++) {
        ll i = turns[k].first, j = turns[k].second;
        if (k % 2 == 0)
            x[i][j] += theta;
        else
            x[i][j] -= theta;
    }
}
void MODImethod() {
    while (1) {
        calcUV();
        calcDelta();
        ll I, J;
        if (!badSpot(I, J))
            break; //all cells were optimized!

        vector<vector<bool>> visited(m+1, vector<bool>(n+1, false)); // this matrix is to note the coordinates which had passed away before
        stack<pair<ll,ll>> mapTrail;
        pair<ll,ll> start = {I, J};

        if (findCycle(1, I, J, visited, mapTrail, start)) {
            vector<pair<ll,ll>> turns;
            while (!mapTrail.empty()) {
                turns.push_back(mapTrail.top());
                mapTrail.pop();
            }
            reverse(turns.begin(), turns.end());
            applyTheta(turns);
        }
        else {
            cout << "Can't find circle!\n";
            break; 
        }
    }
    totalCost = 0;
    for (ll i = 1; i <= m; i++)
        for (ll j = 1; j <= n; j++)
            totalCost += x[i][j]*cost[i][j];
}

int main() {
    readInputFromCSV(); 
    //When creating the input.csv file, type the data based on the following sequence:
    //m, n \n supply[m], \n demand[n], \n cost[m][n] (a matrix with m rows and n columns).

    x.resize(m+1, vector<ll>(n+1, 0));
    cout << "               PBL1 REPORT\n";
    cout << "Input data loaded successfully!\n Select a method to find the Initial Basic Feasible Solution:\n";
    while (1) {
        cout << "\n 1. Northwest Corner Method\n 2. Least Cost Method\n 3. Vogel's Approximation Method (VAM)\n 0. Exit \nYour choice: ";
        char choose; cin >> choose;
        if (choose == '0') 
            return 0;
        else if (choose == '1') {
            northWestMethod();
            cout << "THE RESULT:\n"; 
            Answer();
            theCost();
        }
        else if (choose == '2') {
            leastCostMethod();
            cout << "THE RESULT:\n";
            Answer();
            theCost();
        }    
        else if (choose == '3') {
            vogelMethod();
            cout << "THE RESULT:\n";
            Answer();
            theCost();
        }
        else {
            cout << "\nInvalid character, type again\n";
            continue;
        }
        totalCost = 0;
        cout << "\nWant to continue?\n";
        cout << " 1. Select another Initial Solution Method\n";
        cout << " 2. Proceed to optimaltimal Solution Methods\n";
        cout << " 0. Exit\n";
        cout << "Your choice: ";
        cin >> choose;
        if (choose == '0') 
            return 0;
        else if (choose == '1') continue;
        else if (choose == '2') {
            cout << "\nSelect a method to find the optimaltimal Solution:\n";
            cout << " 1. MODI Method\n";
            cout << " 2. Back to Initial Solution Menu\n";
            cout << " 0. Exit Program\n";
            cout << "Your choice: ";
            while (1) {
                cin >> choose;
                if (choose == '0')
                    return 0;
                else if (choose == '1') {
                    MODImethod();
                    cout << "THE RESULT: after optimized with MODI Method:\n";
                    Answer();
                    theCost();
                }
                else if (choose == '2') 
                    break;
                else {
                    cout << "\nInvalid character, type again\n";
                    continue;
                }
                cout << "\nSelect a method to find the optimaltimal Solution:\n";
                cout << " 1. MODI Method\n";
                cout << " 2. Back to Initial Solution Menu\n";
                cout << " 0. Exit Program\n";
                cout << "Your choice: ";
            }
        }
        else {
            cout << "\nInvalid character, type again\n";
            continue;
        }
    }
}
/*
Author/Director: NGUYEN XUAN KHANG
Right-hand person: Trinh Nhan Tam
Left-hand person: Pham Van Minh
*/
