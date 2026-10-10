#include <stdio.h>
#include <string.h>
#define MAXSIZE 1000

char stack[MAXSIZE]; // 把遇到的左括号(、[、{、C压入栈中，等后面遇到对的右括号)、]、}、D时，弹出栈顶元素判断是否匹配
int top = -1;        // 栈顶下标，-1表示栈空

// 三个辅助函数
// 辅助01：检查左右括号是否匹配
int check(char left, char right){
    if(left == '(' && right == ')') return 1;
    if(left == '[' && right == ']') return 1;
    if(left == '{' && right == '}') return 1;
    if(left == 'C' && right == 'D') return 1;
    return 0;
}
// 辅助02：打印缺少右半部分的左符号
void printLeft(char c){
    if(c == 'C'){
        printf("/*-?\n");
    }else{
        printf("%c-?\n", c);
    }
}
// 辅助03：打印缺少左半部分的右符号
void printRight(char c){
    if(c == 'D'){
        printf("?-*/\n");
    }else{
        printf("?-%c\n", c);
    }
}

// 主程序逐行读取输入，处理每一行，遇到单独的一行 . 就结束输入
int main(){
    char line[5000];
    while(fgets(line, sizeof(line), stdin) != NULL){
        int start = 0;
        // 去掉行首空白符
        while(line[start] == ' ' || line[start] == '\t' || line[start] == '\r' || line[start] == '\n'){
            start++;
        }
        int end = strlen(line) - 1;
        // 去掉行尾空白符
        while(end >= start && (line[end] == ' ' || line[end] == '\t' || line[end] == '\r' || line[end] == '\n')){
            end--;
        }
        // 如果整行只有一个 . 就结束输入
        if(end == start && line[start] == '.'){
            break;
        }
        // 遍历每一行的字符(逐字符扫描，处理各种符号)
        for(int i = 0; line[i] != '\0'; i++){
            char c = line[i];
            char right = '\0'; // 右半部分符号，初始化为空字符

            // 压栈
            // 如果遇到左半部分符号(、[、{、C，就压入栈中
            if(c == '/' && line[i + 1] == '*'){
                if(top < MAXSIZE - 1){
                    stack[++top] = 'C'; // 压入栈中
                }
                i++; // 跳过下一个字符 * 避免 * 被重复处理
                continue; // 继续处理下一个字符(不需要走配对逻辑，左符号只入栈，等后面匹配)
            }

            // 出栈
            // 如果遇到右半部分符号)、]、}、D，就弹出栈顶元素判断是否匹配
            if(c == '*' && line[i + 1] == '/'){
                right = 'D'; // 右半部分符号为 D
                i++;         // 跳过下一个字符 / 避免 / 被重复处理
            }else if(c == ')' || c == ']' || c == '}'){
                right = c;   // 右半部分符号为 )、]、}
            }else if(c == '(' || c == '[' || c == '{'){
                if(top < MAXSIZE - 1){
                    stack[++top] = c; // 压入栈中
                }
                continue; // 继续处理下一个字符(不需要走配对逻辑，左符号只入栈，等后面匹配)
            }else{
                continue; // 其他字符不处理，继续处理下一个字符
            }

            if(top == -1){
                printf("NO\n");    // 栈空，说明没有匹配的左半部分符号
                printRight(right); // 打印缺少左半部分的右符号
                return 0;          // 结束程序
            }

            if(!check(stack[top], right)){
                printf("NO\n");         // 栈顶元素和当前右半部分符号不匹配
                printLeft(stack[top]);  // 打印缺少右半部分的左符号
                return 0;               // 结束程序
            }
            top--; // 匹配成功，弹出栈顶元素
        }
    }

    if(top == -1){
        printf("YES\n"); // 栈空，说明所有符号匹配成功
    }else{
        printf("NO\n"); // 栈不空，说明还有未匹配的左半部分符号
        printLeft(stack[top]); // 打印缺少右半部分的左符号
    }
    return 0;
}