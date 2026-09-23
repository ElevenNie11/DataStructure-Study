#include <stdio.h>
#define MAXSIZE 100000
// “双指针”法
// int main(){
//     int n;
//     scanf("%d", &n);
//     int arr1[MAXSIZE];
//     int arr2[MAXSIZE];
//     for(int i = 0; i < n; i++){
//         scanf("%d", &arr1[i]);
//     }
//     for(int i = 0; i < n; i++){
//         scanf("%d", &arr2[i]);
//     }

//     int i = 0;
//     int j = 0;
//     int mid = 0;
//     // 找两个序列合起来后的第 n 个数
//     for(int k = 0; k < n; k++){
//         if(arr1[i] <= arr2[j]){
//             mid = arr1[i];
//             i++;
//         }else{
//             mid = arr2[j];
//             j++;
//         }
//     }
//     printf("%d", mid);
//     return 0;
// }


// 合并数组法
int main(){
    int n;
    scanf("%d", &n);
    int arr1[MAXSIZE];
    int arr2[MAXSIZE];
    int arr3[MAXSIZE];
    for(int i = 0; i < n; i++){
        scanf("%d", &arr1[i]);
    }
    for(int i = 0; i < n; i++){
        scanf("%d", &arr2[i]);
    }

    // 合并两个数组
    int i = 0;
    int j = 0;
    int k = 0;
    while(i < n && j < n){
        if(arr1[i] <= arr2[j]){
            arr3[k++] = arr1[i++];
        }else{
            arr3[k++] = arr2[j++];
        }
    }
    // 若arr1还有剩余元素
    while(i < n){
        arr3[k++] = arr1[i++];
    }
    // 若arr2还有剩余元素
    while(j < n){
        arr3[k++] = arr2[j++];
    }
    
    // 中位数
    printf("%d", arr3[n - 1]);
    return 0;
}

// 注意：使用合并数组法的时候，PTA可能会提示“段错误”
// 这是因为数组MAXSIZE太大，导致了栈溢出。而arr1、2、3有放在main函数里面，属于局部变量
// 局部变量是放在**栈**空间里的
// 解决方案：把这三个数组从main里拿出来，就成了全局遍历，就不会占main的栈空间

// // 合并数组法
// // 全局变量
// int arr1[MAXSIZE];
// int arr2[MAXSIZE];
// int arr3[MAXSIZE];

// int main(){
//     int n;
//     scanf("%d", &n);
//     for(int i = 0; i < n; i++){
//         scanf("%d", &arr1[i]);
//     }
//     for(int i = 0; i < n; i++){
//         scanf("%d", &arr2[i]);
//     }

//     // 合并两个数组
//     int i = 0;
//     int j = 0;
//     int k = 0;
//     while(i < n && j < n){
//         if(arr1[i] <= arr2[j]){
//             arr3[k++] = arr1[i++];
//         }else{
//             arr3[k++] = arr2[j++];
//         }
//     }
//     // 若arr1还有剩余元素
//     while(i < n){
//         arr3[k++] = arr1[i++];
//     }
//     // 若arr2还有剩余元素
//     while(j < n){
//         arr3[k++] = arr2[j++];
//     }
    
//     // 中位数
//     printf("%d", arr3[n - 1]);
//     return 0;
// }