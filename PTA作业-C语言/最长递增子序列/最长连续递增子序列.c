#include <stdio.h>
#define MAXSIZE 100000
int main(){
    int n;
    scanf("%d", &n);
    int arr[MAXSIZE];
    for(int i = 0; i < n; i++){
        scanf("%d", &arr[i]);
    }
    // 当前递增子序列的起始位置和长度
    int currStart = 0;
    int currLen = 1;
    // 最长递增子序列的起始位置和长度
    int maxStart = 0;
    int maxLen = 1;
    // 遍历数组
    // 条件必须是 i < n - 1
    // 而不是 i < n 否则arr[i + 1]，当 i = n - 1 时，arr[n]数组会越界
    for(int i = 0; i < n - 1; i++){
        if(arr[i] < arr[i + 1]){
            currLen++;
        }else{
            // 递增中断，从当前位置重新开始
            currLen = 1;
            currStart = i + 1;
        }
        // 如果找到比当前递增子序列还要长的子序列
        if(currLen > maxLen){
            maxLen = currLen;
            maxStart = currStart;
        }
    }

    // 输出最长子序列
    // 不要写成：i < maxLen （i = 5, maxLen = 4）
    // 比如 for(int i = 5; i < 4; i++) 不可能对
    for(int i = maxStart; i < maxStart + maxLen; i++){
        if(i > maxStart){
            printf(" ");     // 先打印空格后打印数字，第一个数字之前不打印空格
        }
        printf("%d", arr[i]);
    }
    return 0;
}