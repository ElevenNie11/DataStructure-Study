// 上三角矩阵就是主对角线以下的元素全部为 0
/*

        j
       0  1  2
i  0   1  2  3
   1   0  5  6
   2   0  0  9

*/
#include <stdio.h>

int main(){
    int n;
    scanf("%d", &n);
    int a[n][n];
    // 输入矩阵
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            scanf("%d", &a[i][j]);
        }
    }

    // 转换并且输出上三角矩阵
    printf("上三角矩阵：\n");
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(j >= i){
                printf("%d ", a[i][j]);
            }else{
                printf("0 ");
            }
        }
        printf("\n");
    }

    // 转换并且输出下三角矩阵
    printf("下三角矩阵：\n");
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(i >= j){
                printf("%d ", a[i][j]);
            }else{
                printf("0 ");
            }
        }
        printf("\n");
    }
    
    return 0;
}