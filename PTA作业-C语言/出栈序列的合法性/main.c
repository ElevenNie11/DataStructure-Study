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
            while(top >= 0 && stack[top] == seq[pos]){  // 【重点】：压完这个栈以后，能弹多少弹多少
                top--;      // 出栈
                pos++;      // 匹配下一个目标序列元素
            }
        }
        if(flag && pos == n){
            printf("YES\n");
        }else{
            printf("NO\n");
        }
    }
    return 0;
}


// 目标序列：按1..n入栈
// seq: 5 6 4 3 7 2 1 
// seq[0] = 5, seq[1] = 6, seq[2] = 4, seq[3] = 3, seq[4] = 7, seq[5] = 2, seq[6] = 1
// seq[n] 的值是永远不变的

// top = 0 ：stack[top] = 1  seq[0] = 5
// top = 1 ：stack[top] = 2
// top = 2 ：stack[top] = 3
// top = 3 ：stack[top] = 4
// top = 4 ：stack[top] = 5  stack[top] == seq[0]  => top = 3, pos = 1

// top = 3 ：stack[top] = 4  stack[top] != seq[1]  => （继续压栈）top = 4, stck[top] = 6
// top = 4 ：stack[top] = 6  stack[top] == seq[1]  => top = 3, pos = 2
// top = 3 ：stack[top] = 4  stack[top] == seq[2]  => top = 2, pos = 3
// top = 2 ：stack[top] = 3  stack[top] == seq[3]  => top = 1, pos = 4
// top = 1 ：stack[top] = 2  stack[top] != seq[4]  => （继续压栈）top = 2, stck[top] = 7
// top = 2 ：stack[top] = 7  stack[top] == seq[4]  => top = 1, pos = 5
// top = 1 ：stack[top] = 2  stack[top] == seq[5]  => top = 0, pos = 6
// top = 0 ：stack[top] = 1  stack[top] == seq[6]  => top = -1（栈空）, pos = 7
// YES