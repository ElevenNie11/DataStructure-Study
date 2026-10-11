#include <stdio.h>

int main(){
    int m, n, k;
    scanf("%d %d %d", &m, &n, &k);
    while(k--){
        int seq[1000], stack[1000];
        for(int i = 0; i < n; i++){
            scanf("%d", &seq[i]);
        }
        int top = -1;   // 栈顶指针
        int pos = 0;    // 目标序列当前匹配位置
        int flag = 1;
        
        for(int i = 1; i <= n; i++){
            if(top + 1 >= m){       // 栈满，无法入栈
                flag = 0;
                break;       
            }
            stack[++top] = i;    // 题目要求：按1..n入栈
            while(top >= 0 && stack[top] == seq[pos]){
                top--;      // 出栈
                pos++;      // 匹配下一个目标序列元素
            }
        }
        if(flag && pos == n){
            printf("YES\n");
        } else {
            printf("NO\n");
        }
    }
    return 0;
}