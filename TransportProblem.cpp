/*
***************************************************************************************************************************************************
*   Lead Author: NGUYEN XUAN KHANG                                                                                                                *
*   Co-authors: Trinh Nhan Tam - Pham Van Minh                                                                                                    *
*   Supervisor: Assoc. Prof. Dr. Nguyen Tan Khoi                                                                                                  *
*   Completion Date: June 08th 2025                                                                                                               *
*   Description: Implementation of the Transport Problem                                                                                          *
***************************************************************************************************************************************************
*/
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <fstream>
#include <algorithm>

#define ll long long
#define her 1608 // her is a tmp variable
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

    ifstream file("input.csv"); // input.csv is file name, changeably
    if (!file.is_open()) {
    cerr << "Error: Cannot optimalen file!";
    exit(1);
    }
    vector<float> data; 
    data.push_back(her); 
    // Push 'her' into the vector to eliminate the first element, so that the first inserted data will be stored at index 1
    // The purpose is to simplify the way we handle an array or a matrix in code, using indices from 1 to n instead of 0 to n-1
    string line;

    // The idea of reading file is to push all the data from the file into a vector, 
    // then, based on the determined sequence, many numbers will be separated into variables, arrays, and matrices 
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
    // The vector "wall" and those steps are used to eliminate the first row of the cost matrix 
    // so that the first row will be the row with index 1

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
        if (totalrow[i] == supply[i]) i++; // Move down to the cell below if totalrow[i] is full
        if (totalcol[j] == demand[j]) j++; // Move to right cell if totalcol[j] is full
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
                    // If there are multiple cells with the same minimum cost, choose the one with the best distribution potential:
                    // the largest possible min(supply[I] - totalrow[I], demand[J] - totalcol[J])
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
// This function is to calculate the penalty value of each row and column
float Penalty(ll Row, ll iORj, ll restSize, vector<vector<float>> c, vector<ll>& done) {
    vector<vector<float>> copy = c; 
    float m1 = 1e9, m2 = 1e9;
    
    // I decided to use only 1 function to calculate the penalty value instead of 2 function for either row or column
    // so here, the variable "Row", "Row" indicates whether the array being calculated is a row or a column (1 is row, 0 is column) 
    // the "done" array checks whether the row or column had been done yet
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
    else { // else if Row == 0, this function will calc in column, "restSize" will be m (the number of rows)
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

// This function is to find the lowest element in the row or column, the result is location of that element, stored in I and J
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
// This function is to find the row or the column with the highest penalty value, the result is location of the row or columnn, stored in I or J
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
// This function is the main function of VAM
void vogelMethod() {
    x = vector<vector<ll>>(m+1, vector<ll>(n+1, 0));
    ll cnt = 1;
    vector<ll> rowDone(m+1, 0); vector<ll> colDone(n+1, 0);
    vector<ll> totalRow(m+1, 0); vector<ll> totalCol(n+1, 0);
    
    while (1) {
        vector<float> penRow(m+1, 0); vector<float> penCol(n+1, 0);

        for (ll i = 1; i <= m; i++) 
            penRow[i] = rowDone[i]? -1 : Penalty(1, i, n, cost, colDone); // Calc all penalty values in each rows
        for (ll j = 1; j <= n; j++) 
            penCol[j] = colDone[j]? -1 : Penalty(0, j, m, cost, rowDone); // Calc all penalty values in each columns
    
        ll I = -1, J = -1;
        ll row = 1;
        findCR7(m, n, penRow, penCol, rowDone, colDone, I, J, row); // Find the highest penalty value row or column, stored the location to either I or J
        minInRowOrCol(m, n, cost, rowDone, colDone, I, J, row); // Find the lowest element in the highest penalty row or column, stored the location to I and J
        if (I == -1 || J == -1) break; // Break if all done;

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

vector<vector<ll>> delta;
vector<ll> u, v;
// This function is to calculate u and v 
void calcUV() {
    u = vector<ll>(m+1, her);
    v = vector<ll>(n+1, her);

    // The idea is to find the row or column that contains the most non-zero cells in the matrix x.
    vector<ll> rowCnt(m+1, 0), colCnt(n+1, 0);
    for (ll i = 1; i <= m; i++) 
        for (ll j = 1; j <= n; j++) 
            if (x[i][j] != 0) {
                rowCnt[i]++; colCnt[j]++;
            }
    ll maxRow = -1, maxCol = -1, I = -1, J = -1;
    for (ll i = 1; i <= m; i++) 
        if (rowCnt[i] > maxRow) {
            maxRow = rowCnt[i]; I = i;
        }
    for (ll j = 1; j <= n; j++) 
        if (colCnt[j] > maxCol) {
            maxCol = colCnt[j]; J = j;
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
// This function is to find a closed cycle, starting from position (i, j)
bool findCycle(ll Row, ll i, ll j, vector<vector<bool>>& visited, stack<pair<ll,ll>>& mapTrail, pair<ll,ll> start) {
    // "Row" indicates the current direction: 1 means moving along the row, 0 means moving along the column.
    // "mapTrail" is a stack used to store the coordinates of the x[i][j] cells involved in the current cycle.
    // Coordinates are stored in a pair, where '.first' represents i (row) and '.second' represents j (column)

    if (mapTrail.size() > 3 && start == make_pair(i, j)) return true;
    // If mapTrail contains more than 3 elements and the current position (i, j) equals the start position, return true to indicate a closed cycle is found
    if (visited[i][j]) return false;
    // "visited" tracks visited positions to avoid revisiting them in the same search path.

    visited[i][j] = true;
    mapTrail.push({i, j}); // push coordinate to mapTrail

    if (Row) { // If Row == 1, move along the row: keep i fixed, iterate over J.
        for (ll J = 1; J <= n; J++)
            if (J != j && (x[i][J] != 0 || (i == start.first && J == start.second)))
                if (findCycle(0, i, J, visited, mapTrail, start)) 
                    return true;
    } 
    else { // Else (Row == 0), move along the column: keep j fixed, iterate over I.
        for (ll I = 1; I <= m; I++) 
            if (I != i && (x[I][j] != 0 || (I == start.first && j == start.second)))
                if (findCycle(1, I, j, visited, mapTrail, start))
                    return true;
    }
    // Recursive call to extend the closed loop path by alternating row/column directions,
    // only visiting cells that are already allocated (x != 0) or the starting cell.

    mapTrail.pop(); // Backtrack: remove the current cell from the cycle path if no valid continuation is found from this point.
    return false; // No valid cycle found from this cell, backtrack to try other paths
}
// This function finds the minimum theta and adds it to (+) cells while subtracting it from (-) cells
void applyTheta(vector<pair<ll,ll>> &turns) {
    ll theta = 1e9;
    for (ll k = 1; k < turns.size(); k += 2) {
    // k = 1 means the second element (index 1), since the first element (index 0) has a (+) sign;
    // k += 2 because the signs alternate, so elements with (-) are every other element starting from index 1
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
    ll cnt = 0;
    while (1) {
        calcUV();
        calcDelta();
        ll I, J;
        if (!badSpot(I, J)) { // All cells were optimized!
            cout << "All cells were optimized!\n";
            break; 
        }
            

        vector<vector<bool>> visited(m+1, vector<bool>(n+1, false)); 
        stack<pair<ll,ll>> mapTrail;
        pair<ll,ll> start = {I, J};

        // Coordinates are stored in a pair, where '.first' represents i (row) and '.second' represents j (column)
        
        if (findCycle(1, I, J, visited, mapTrail, start)) {
            vector<pair<ll,ll>> turns;
            while (!mapTrail.empty()) {
            // We transfer data from stack mapTrail to vector turns to perform the update of adding/subtracting theta values.
                turns.push_back(mapTrail.top());
                mapTrail.pop();
            }
            reverse(turns.begin(), turns.end()); // This is a built-in STL function that reverses the elements of an array or vector<> in place.
            applyTheta(turns);
        }
        else {
            cout << "All cells were optimized!!\n";
            break;
        }
        cnt++;
        if (cnt == 3000) break;
    }
    totalCost = 0;
    for (ll i = 1; i <= m; i++)
        for (ll j = 1; j <= n; j++)
            totalCost += x[i][j]*cost[i][j];
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
}

int main() {
    readInputFromCSV(); 
    //When creating the input.csv file, type the data based on the following sequence:
    // m, n \n supply[m], \n demand[n], \n cost[m][n] (a matrix with m rows and n columns).

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
            cout << "Total cost = " <<  totalCost; endl; totalCost = 0;
            theCost();
        }
        else if (choose == '2') {
            leastCostMethod();
            cout << "THE RESULT:\n";
            Answer();
            cout << "Total cost = " <<  totalCost; endl; totalCost = 0;
            theCost();
        }    
        else if (choose == '3') {
            vogelMethod();
            cout << "THE RESULT:\n";
            Answer();
            cout << "Total cost = " <<  totalCost; endl; totalCost = 0;
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
                    cout << "Total cost = " <<  totalCost; endl; totalCost = 0;
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
    return 0;
}
/*
Lead Author: NGUYEN XUAN KHANG
Right-hand person: Trinh Nhan Tam
Left-hand person: Pham Van Minh
*/

/*
e.g
8,8
126,125,124,110,115,130,143,127
119,129,135,130,133,118,125,111
7,9,3,7,5,4,4,11
5,10,11,11,6,7,4,9
9,5,9,14,12,9,11,6
2,4,3,3,10,2,1,6
7,6,3,5,2,9,13,4
8,7,9,13,5,6,10,4
3,9,8,4,4,6,6,7
4,8,9,12,12,5,6,10
*/
