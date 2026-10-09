
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAXSIZE 100

char stack[MAXSIZE];  // 用于存放尚未匹配的左符号
int top = -1;         // 栈顶元素的下标，top为-1表示栈为空  

int inComment = 0;    // 是否处于/* ... */注释内部
char quote = 0;       // 是否处于字符串或字符常量内部
// printf("hello ( [ {");
// 这里的括号都在字符串中 不应该参与括号匹配
int escape = 0;       // 是否刚刚读取到转义符号 \

char errorLeft = 0;   // 记录不匹配的左符号
char errorRight = 0;  // 记录不匹配的右符号
int errorFound = 0;   // 是否已经找到第一个错误

// isLeft函数的作用是：判断一个字符是否为左符号
int isLeft(char c) {
    return c == '(' || c == '[' || c == '{' || c == 'C';
}

// getMatch函数的作用是：给定一个左符号，返回它对应的右符号
char getMatch(char c){
    if(c == '('){
        return ')';
    }
    if(c == '['){
        return ']';
    }
    if(c == '{'){
        return '}';
    }
    return '\0';
}

// setError函数的作用是：记录（保存）第一个不匹配的符号
void setError(char left, char right){
    if(!errorFound){
        errorLeft = left;
        errorRight = right;
        errorFound = 1;
    }
}

int main(){
    char line[5000];  // 用于存放每一行输入的字符串

    // 读取输入的每一行 直到遇到单独的"."为止
    // 题目规定，当某一行只有一个句点 . 时才输入结束 因此程序需要判断这一行是否为结束标志 而不是遇到程序中任意一个句点就结束
    while(fgets(line, sizeof(line), stdin) != NULL){
        // 逐个扫描每个字符
        int start = 0;
        // isspace函数的作用是：判断一个字符是否为空白字符（空格、制表符、换行符等）
        // while循环的作用是：跳过行首的空白字符，找到这一行第一个有意义的字符
        while(isspace(line[start]) && line[start] != '\n' && line[start] != '\r'){
            start++;
        }

        int end = start + 1;
        // 它负责跳过句点后面的空格、制表符、回车符和换行符
        while(line[end] == ' ' || line[end] == '\t' || line[end] == '\r' || line[end] == '\n'){
            end++;
        }

        if(line[start] == '.' && line[end] == '\0'){
            break;
        }

        // 开始逐个字符扫描源程序
        for (int i = 0; line[i] != '\0'; i++) {
            char c = line[i];
            char next = line[i + 1];

            if (inComment) {
                if (c == '*' && next == '/') {
                    inComment = 0;
                    i++;
                }
                continue;
            }

            if (quote != 0) {
                if (escape) {
                    escape = 0;
                } else if (c == '\\') {
                    escape = 1;
                } else if (c == quote) {
                    quote = 0;
                }
                continue;
            }

            if (c == '/' && next == '/') {
                break;
            }

            if (c == '/' && next == '*') {
                inComment = 1;
                i++;
                continue;
            }

            if (c == '*' && next == '/') {
                setError(0, 'D');
                break;
            }

            if (c == '"' || c == '\'') {
                quote = c;
                continue;
            }

            if (c == '(' || c == '[' || c == '{') {
                if (top < MAXSIZE - 1) {
                    stack[++top] = c;
                }
            } else if (c == ')' || c == ']' || c == '}') {
                if (top == -1) {
                    setError(0, c);
                } else if (getMatch(stack[top]) == c) {
                    top--;
                } else {
                    setError(stack[top], 0);
                }
            }

            if (errorFound) {
                break;
            }
        }

        if (errorFound) {
            break;
        }
    }

    if (!errorFound) {
        if (inComment) {
            setError('C', 0);
        } else if (top != -1) {
            setError(stack[0], 0);
        }
    }

    if (errorFound) {
        printf("NO\n");

        if (errorLeft == 'C') {
            printf("/*-?\n");
        } else if (errorRight == 'D') {
            printf("?-*/\n");
        } else if (errorLeft == 0) {
            printf(" ?-%c\n", errorRight);
        } else {
            printf("%c-?\n", errorLeft);
        }
    } else {
        printf("YES\n");
    }

    return 0;
}