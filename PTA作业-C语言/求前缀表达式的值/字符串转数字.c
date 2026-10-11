#include <stdio.h>

int stringToint(char str[]){
    int num = 0;
    int i = 0;
    while(str[i] != '\0'){
        num = num * 10 + (str[i] - '0');
        i++;
    }
    return num;
}

int main(){
    char str[100];
    printf("请输入数字字符串：");
    scanf("%s", str);
    int result = stringToint(str);
    printf("转换后的数字：%d", result);
    return 0;
}