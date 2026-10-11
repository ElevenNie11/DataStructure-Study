// 前缀表达式求值（波兰表达式）
// 三种 ERROR 情况
// 1. 除数为 0
// 2. 遇到运算符时栈里不足两个数（如 + 1）
// 3. 最终栈里不是恰好剩 1 个数（如 + 1 2 3多出操作数）

// 前缀表达式不能像后缀表达式一样直接从左扫描，需要：
// 1. 从右往左扫描
// 2. 遇到数字就【入栈】
// 3. 遇到运算符就【弹出】两个数字计算
// 4. 【结果重新入栈】
// 5. 最后【栈顶即答案】

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
// 返回的为什么不是char类型呢？
// 因为char类型是字符类型，返回值只能是一个字符，而我们需要判断一个字符是否是运算符，返回值应该是布尔类型（0或1），所以使用int类型更合适
// 返回c=='+' || c=='-' || c=='*' || c=='/'，如果c是运算符则返回1，否则返回0
int isOperator(char c){
    return c == '+' || c == '-' || c == '*' || c == '/';
}
// 补充（提交时可以删除isOperator2函数）
// 或者把这个函数写成：
int isOperator2(char c){
    if(c == '+' || c == '-' || c == '*' || c == '/'){
        return 1;
    }else{
        return 0;
    }
}


int main(){
    char str[1000];
    fgets(str, sizeof(str), stdin);
    // 把输入的一张行字符串，按照空格切割成一个一个的 token，存入一个数组中
    // 因为我们输入的是：+ + 2 * 3 - 7 4 / 8 4
    // 实际上读进C里面是一个完整的字符串："+ + 2 * 3 - 7 4 / 8 4\n"
    // 但是计算时我们需要一个一个拿：+ + 2 * 3... 所以需要进行【字符串分割，分割成token】
    char* token[100]; // 字符指针数组(保存每一个token)
    int count = 0;
/*
    token
    |
    |---- token[0] ---> "+"
    |
    |---- token[1] ---> "+"
    |
    |---- token[2] ---> "2"
    |
    |---- token[3] ---> "*"
    .
    .
    .
*/
    // strtok(原字符串, 分隔符);
    // 按照分隔符分割原字符串
    char* p = strtok(str, " ");  // strtok()是C语言里面的字符串切割函数:按照空格切割字符串
    while(p != NULL){
        token[count] = p;
        count++;
        p = strtok(NULL, " ");   // 继续刚才那个字符串，找下一个
    }

    // 从右往左处理
    for(int i = count - 1; i >= 0; i--){
        // 如果是数字
        // 【注意】：*token[i] 取出来是字符char类型，而不是double类型，所以还必须把它们转换成double类型
        // 【易错】：*token[i] 写法没问题但是token[i]代表的是字符串（比如"123"），而*token[i]就代表的'1'
        // 【提示】：*token[i]等价于token[i][0]

        if(!isOperator(*token[i])){
            push(atof(token[i]));  // 不可以写成 atof(*token[i]) 因为*token[i]取出的是字符，不是字符串，不能直接传给atof()
        }else{ // 如果是运算符
            double a = pop();
            double b = pop();
            double result;
            switch(*token[i]){   // *token[i]等价于token[i][0]
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
                    result = a / b;
                    break;
            }
        push(result);
        }
    }
    if(top == 0){
        printf("%.1lf", stack[top]);
    }else{
        printf("ERROR");
    }
    return 0;
}


// 易错：字符转数字以及字符【串】转数字
// *token[i]         → 取第一个字符
// (double)*token[i] → 字符ASCII转double（但是只会把第一个字符转成double）
// 比如"123"，只会把"1"转换成double类型的1
// atof(token[i])    → 整个字符串转double