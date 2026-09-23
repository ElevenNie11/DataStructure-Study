# 数据结构 Data Structure 学习笔记
📒[FeShu Note]:https://wonderfulworld.feishu.cn/docx/PDSadKfT6oq2HCxLA4Ec17Csndb?from=from_copylink

📒[飞书 笔记]:https://wonderfulworld.feishu.cn/docx/PDSadKfT6oq2HCxLA4Ec17Csndb?from=from_copylink

---

# #01顺序表（线性表的顺序表示）
线性表：线性表是由n个数据元素组成的有限序列

顺序表：顺序表是线性表的顺序存储结构，用一段**地址连续**的存储单元依次存储线性表中的数据元素（*把线性表中的元素，一个挨着一个放在连续的内存空间里*）

顺序表种的元素在存储空间中是按照线性表的逻辑顺序依次排列的：
```
逻辑上的顺序：10 → 20 → 30 → 40 → 50
在内存种也是：10 → 20 → 30 → 40 → 50（它们的存储地址是连续的）
内存地址假设：1000 → 1004 → 1008 → 1012 → 1016（这就是顺序存储）
10 的地址：1000
20 的地址：1004
30 的地址：1008
40 的地址：1012
50 的地址：1016
```

## 顺序表的核心：数组
在实际编程中，顺序表通常使用**数组**实现，比如：`int[] data = new int[100];` java创建了一个容量为100个int类型的数组
**顺序表的底层存储通常就是数组**，⚠️但要注意的是：“数组”是存储数据的工具，而“顺序表”是对这个数组进行封装后形成的一种数据结构，所以我们经常用**struct**⚠️
```
// 相当于把数组 + 长度捆绑在了一起
typedef struct {
    int data[100];  // 负责存数据
    int length;     // 负责记录当前有多少个**有效**数据（数组容量和有效数据个数是两个不同的概念！！）
} SeqList;

// 结构体把实现顺序表所需要的各种信息组织到一起

SeqList
┌──────────────────────────────┐
│                              │
│  data                        │
│  ┌────┬────┬────┬────┬────┐  │
│  │ 10 │ 20 │ 30 │ 40 │ 50 │  │
│  └────┴────┴────┴────┴────┘  │
│                              │
│  length = 5                  │
│                              │
└──────────────────────────────┘

```

### 📚顺序表的核心操作：查找/插入/删除/修改
- 查找：顺序表按**下标**随机访问的时间复杂度是**O(1)**。如果是按**值**查找的话时间复杂度是**O(n)**
- 插入：要在中间插入一个元素，因为顺序表必须**保持连续**，所以后面的元素都需要往后移动。顺序表插入操作的最坏时间复杂度是**O(n)**
  ````
  // 如果数组容量不够了怎么办？
            ↓
    1. 创建一个更大的数组
            ↓
    2. 把旧数组的数据复制过去
            ↓
    3. 再插入新元素
  ````
- 删除：删除和插入一样，也需要移动大量元素（*向左移动*）。顺序表删除操作的最坏时间复杂度也是**O(n)**
- 修改：利用下标即可。顺序表按**下标**修改元素的时间复杂度是**O(1)**

### 插入：memmove
`memmove(目标位置, 源位置, 移动的字节数);`

`源地址    = &data[pos - 1]`

`目标地址  = &data[pos]`

```
memmove(
    &list->data[pos],
    &list->data[pos - 1],
    (list->length - pos + 1) * sizeof(int)
);
```

```
原来的顺序表：
下标：  0   1   2   3   4
       10  20  30  40  50
在第3个位置插入 25        （第3个位置的下标是2）
为了给25腾位置，需要把30 40 50都往后移动：
10 20 30 40 50
      ↓  ↓  ↓
10 20 30 30 40 50
然后，list->data[pos - 1] = value;
于是：list->data[2] = 25;
最终：10 20 25 30 40 50
```

---

## Q：为什么这里用 memmove()，而不是 memcpy()？
## A:因为这里源内存和目标内存发生了重叠
```
源：[30][40][50]
          ↓
目标：   [30][40][50]
```

### 🚀删除也用memmove
```
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
```

## 📕总结memmove

### Ⅰ.插入 :把后面的元素向后移动
```
memmove(
    &list->data[pos],
    &list->data[pos - 1],
    (list->length - pos + 1) * sizeof(int)
);
```

### Ⅱ.删除：把后面的元素向前移动
```
memmove(
    &list->data[pos - 1],
    &list->data[pos],
    (list->length - pos) * sizeof(int)
);
```

---

# 链表（线性表的链式表示）
