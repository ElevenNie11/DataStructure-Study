// 顺序表是一种逻辑上的线性表，而数组是实现这种线性表的底层存储方式之一
// 结构体只是我们在C语言中把“数组 + 长度等管理信息”组织起来的一种实现手段
#include <stdio.h>
#include <string.h>
#define MAXSIZE 100

// 顺序表
typedef struct{
    int data[MAXSIZE];
    int length;
} SeqList;


// 在第 pos 个位置插入 value
// SeqList* list首地址，传地址进去就可以同时修改data[]和length了
void insert(SeqList* list, int pos, int value){
    // 判断位置是否合法
    // 顺序表容量固定，不是变长数组
    if(pos < 1 || pos > list->length + 1){
        printf("插入位置不合法！");
        return;
    }
    // 1. 移动
    // 用memmove函数将 pos 位置及其以后的元素都像后移动一个位置
    // memmove(目标位置, 源位置, 移动的字节数); 位置都是“地址”
    // 1 2 3 4 5
    // 如果要在第3个位置插入新元素6，把第3个元素开始的所有元素往后移动，也就是在2和3之间插入6，那么3和4和5就要往后移动，那么一共移动3个int位置的字节
    // 原来：length = 5
    // 现在：length1 = 6
    // 移动：length + 1 - 3 = length1 - pos = 6 - 3 = 3
    memmove(&list->data[pos], &list->data[pos - 1], (list->length + 1 - pos)*sizeof(int));
    for(int i = 0; i < list->length; i++){
        printf("%d ", list->data[i]);
    }
    printf("\n");
    //2. 插入新元素：修改下标为 pos-1 的元素
    list->data[pos - 1] = value;
    //3. 表长+1
    list->length++;
}


int main(){
    SeqList list = {
        {10, 20, 30, 40, 50},
        5
    };
    // 在第3个位置插入25
    insert(&list, 3, 25);
    for(int i = 0; i < list.length; i++){
        printf("%d ", list.data[i]);
    }
    return 0;
}