#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE  100

// 全局变量
double stack[MAXSIZE];
int top = -1;

// 入栈函数
void push(double x){
    stack[++top] = x;
}

// 出栈函数
double pop(){
    return stack[top--];
}

// 判断是否为运算符的函数
int isOperator(char c){
    return c == '+' || c == '-' || c == '*' || c == '/';
}
int main(){
    char str[1000];
    fgets(str, sizeof(str), stdin);
    char* token[100]; // 字符指针数组(保存每一个token)
    int count = 0;
    // strtok(原字符串, 分隔符);
    // 按照分隔符分割原字符串
    char* p = strtok(str, " \n");  // strtok()是C语言里面的字符串切割函数:按照空格和回车切割字符串
    while(p != NULL){
        token[count] = p;
        count++;
        p = strtok(NULL, " \n");   // 继续刚才那个字符串，找下一个
    }

    // 从右往左处理
    int ok = 1;   // 处理除0问题和只有一个操作数的问题
    for(int i = count - 1; i >= 0; i--){
        if(!(strlen(token[i]) == 1 && isOperator(*token[i]))){  // 整个token只有一个字符才是运算符（比如：-是减号，但是-2是负数）
            push(atof(token[i]));  // 不可以写成 atof(*token[i]) 因为*token[i]取出的是字符，不是字符串，不能直接传给atof()
        }else{ // 如果是运算符
            // 弹栈之前检查操作数是否足够
            if(top < 1){
                ok = 0;
                break;
            }
            double a = pop();
            double b = pop();
            double result;
            switch(*token[i]){
                case '+':
                    result = a + b;
                    break;
                case '-':
                    result = a - b;
                    break;
                case '*':
                    result = a * b;
                    break;
                case '/':
                    if(b == 0){  // 除0
                        ok = 0;
                        break;
                    }
                    result = a / b;
                    break;
            }
            if(!ok) break;
            push(result);
        }
    }
    if(ok && top == 0){
        printf("%.1lf", stack[top]);
    }else{
        printf("ERROR");
    }
    return 0;
}