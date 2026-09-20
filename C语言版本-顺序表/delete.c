// 删除第 pos 个元素相当于把它后面的元素整体向前移动一位，那么就有：length--
// 继续使用memmove()
#include <stdio.h>
#include <string.h>
#define MAXSIZE 100

// 顺序表结构体
typedef struct{
    int data[MAXSIZE];
    int length;
} SeqList;

void delete(SeqList* list, int pos){
    // 判断位置是否合法
    // 注意不能是：pos > list->length + 1
    if(pos < 1 || pos > list->length){
        printf("插入位置不合法！");
        return ;
    }
    // 1. 移动：把 pos 后面的所有元素向前移动一个位置
    // 下标：0  1  2  3  4
    // pos： 1  2  3  4  5
    // 例如：10 20 30 40 50
    //             ↑
    //           删除30（pos = 3）
    // 2. 删除之后：10 20 40 50
    // 所以需要把：40 50移动到30 40所在的位置
    memmove(&list->data[pos - 1], &list->data[pos], (list->length - pos) * sizeof(int));
    // 表长-1
    list->length--;
}

int main(){
    SeqList list = {
        {10, 20, 30, 40, 50},
        5
    };
    // 删除第3个元素（pos = 3）
    delete(&list, 3);
    for(int i = 0; i < list.length; i++){
        printf("%d ", list.data[i]);
    }
    return 0;
}