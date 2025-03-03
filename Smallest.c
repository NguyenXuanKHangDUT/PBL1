#include <stdio.h>
#include "Smallest.h"
int m, n, source[10000], receive[10000];
float cost[10000][10000];
int input_so_hang_cung_(int m, int a[10000]) {
    int k; printf("nhap lan luot so hang chua trong tung kho: "); 
    for (int j = 0; j < m; j++) {scanf("%d", &a[j]);}
    for (int j = 0; j < m; j++) {
        k += a[j];
    } return k;
}
int input_so_hang_cau_(int n, int b[10000]) {
    int k = 0; printf("nhap lan luot so hang yeu cau cua tung cua hang: "); 
    for (int i = 0; i < n; i++) {scanf("%d", &b[i]);}
    for (int i = 0; i < n; i++) {
        k += b[i];
    } return k;
}

void input_transport_cost(int m, int n, float a[10000][10000]) {
    printf("ma tran %dx%d lan luoc cuoc phi tu kho i den cua hang j tuong ung:\n", n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {scanf("%f", &a[i][j]);}
    }
}

int main() {
    printf("nhap so kho hang : ");
    scanf("%d", &m);
    int k1 = input_so_hang_cung_(m, source);

    printf("nhap so cua hang : ");
    scanf("%d", &n);
    int k2 = input_so_hang_cau_(n, receive);

    if (k1 < k2) {printf("cung < cau, khong can bang"); return 0;}
    else if (k1 > k2) {printf("cung > cau, khong can bang" ); return 0;}
    input_transport_cost(m, n, cost);
    printf("\n");
    Smallest_Cost(m, source, n, receive, cost, k1);
}