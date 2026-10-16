// 矩阵转置的核心就是：把原矩阵的第 i 行第 j 列元素，放到转置矩阵的第 j 行第 i 列
#include <stdio.h>

int main(){
    int a[100][100];
    int b[100][100];
    int m,n;
    scanf("%d %d", &m, &n);
    // 输入矩阵
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            scanf("%d", &a[i][j]);
        }
    }
    // 矩阵转置
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            b[j][i] = a[i][j];
        }
    }
    // 输出转置矩阵
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            printf("%d ", b[i][j]);
        }
        printf("\n");
    }
    return 0;
}