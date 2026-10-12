/*
 * 03_merge_seqlist.c
 * 顺序表实现两个有序表的归并（非递减有序 -> 非递减有序）
 * 教材：《数据结构》（严蔚敏）算法 2.7 —— MergeList_Sq
 *
 * 算法思想：
 *   设 La、Lb 均为非递减有序表，用三个指针 pa、pb、pc 分别指向
 *   La、Lb、Lc 中当前待处理的元素。循环比较 *pa 与 *pb，把较小者
 *   "摘"到 Lc 的末尾；当其中一个表先到达表尾时，把另一个表的剩余
 *   元素直接复制到 Lc 的末尾。
 * 时间复杂度：O(La.length + Lb.length)（线性时间）
 * 空间复杂度：O(La.length + Lb.length)（为 Lc 新申请空间）
 *
 * 注意：La、Lb 采用"值传递"，函数中只读取它们的内容，
 *       因此调用后原表保持不变。
 */
#include <stdio.h>
#include <stdlib.h>

#define LIST_INIT_SIZE 100      /* 线性表存储空间的初始分配量 */
#define LISTINCREMENT  10       /* 线性表存储空间的分配增量 */

#define OK      1
#define ERROR   0

typedef int Status;
typedef int ElemType;

/* ---------- 顺序表的存储结构（动态分配） ---------- */
typedef struct
{
    ElemType *elem;             /* 存储空间基址 */
    int       length;           /* 当前长度 */
    int       listsize;         /* 当前分配的存储容量 */
} SqList;

/* 初始化顺序表 */
Status InitList_Sq(SqList *L)
{
    L->elem = (ElemType *)malloc(LIST_INIT_SIZE * sizeof(ElemType));
    L->length = 0;
    L->listsize = 0;                        /* 分配失败时仍是合法的"空表"，可安全销毁或重试 */
    if (L->elem == NULL)                    /* 存储分配失败 */
        return ERROR;
    L->listsize = LIST_INIT_SIZE;           /* 分配成功，记录实际容量 */
    return OK;
}

/* 销毁顺序表 */
void DestroyList_Sq(SqList *L)
{
    free(L->elem);
    L->elem = NULL;
    L->length = 0;
    L->listsize = 0;
}

/* 求表长 */
int ListLength_Sq(SqList L)
{
    return L.length;
}

/* 在第 i 个位置之前插入元素 e（此处仅用于建表，直接插在表尾） */
Status ListInsert_Sq(SqList *L, int i, ElemType e)
{
    ElemType *newbase, *p, *q;
    if (i < 1 || i > L->length + 1)
        return ERROR;
    if (L->length >= L->listsize)           /* 空间已满，扩容 */
    {
        newbase = (ElemType *)realloc(L->elem,
                                      (L->listsize + LISTINCREMENT) * sizeof(ElemType));
        if (newbase == NULL)
            return ERROR;
        L->elem = newbase;
        L->listsize += LISTINCREMENT;
    }
    q = L->elem + i - 1;
    if (L->length > 0)
    {
        for (p = L->elem + L->length - 1; p >= q; --p)
            *(p + 1) = *p;
    }
    *q = e;
    L->length++;
    return OK;
}

/* 输出顺序表 */
void PrintList_Sq(SqList L)
{
    int i;
    if (L.length == 0)
    {
        printf("{ }");
        return;
    }
    printf("{");
    for (i = 0; i < L.length; i++)
    {
        if (i > 0)
            printf(", ");
        printf("%d", L.elem[i]);
    }
    printf("}");
}

/*
 * 归并两个非递减有序顺序表 La、Lb，结果放在 Lc 中。
 * 说明：这里 pa_last / pb_last 取"表尾元素的下一个位置"（尾后指针），
 *       与教材中"指向最后一个元素"的写法等价，但对空表也安全。
 */
void MergeList_Sq(SqList La, SqList Lb, SqList *Lc)
{
    ElemType *pa, *pb, *pc;                 /* 三个工作指针 */
    ElemType *pa_last, *pb_last;            /* 两个表的尾后指针 */
    int total = La.length + Lb.length;

    /* 结果表的总长度 = 两个表长度之和，一次性分配空间 */
    Lc->listsize = total;
    Lc->length = total;
    if (total == 0)                         /* 两个表都是空表 */
    {
        Lc->elem = NULL;
        return;
    }
    Lc->elem = (ElemType *)malloc(total * sizeof(ElemType));
    if (Lc->elem == NULL)                   /* 存储分配失败 */
    {
        printf("内存分配失败！\n");
        exit(1);
    }

    pa = La.elem;
    pb = Lb.elem;
    pc = Lc->elem;                          /* pc 指向 Lc 中当前的空闲位置 */
    pa_last = La.elem + La.length;
    pb_last = Lb.elem + Lb.length;

    /* 主循环：两个表都未取完时，取较小者送入 Lc */
    while (pa < pa_last && pb < pb_last)
    {
        if (*pa <= *pb)                     /* 相等时先取 La 中的元素，保证稳定性 */
            *pc++ = *pa++;
        else
            *pc++ = *pb++;
    }
    /* 收尾：把尚未取完的那个表的剩余元素顺序复制过去 */
    while (pa < pa_last)
        *pc++ = *pa++;
    while (pb < pb_last)
        *pc++ = *pb++;
}

int main(void)
{
    SqList La, Lb, Lc;
    int n = 0, i, x = 0;                    /* 先给初值：输入异常时不会使用不确定的值 */

    InitList_Sq(&La);
    InitList_Sq(&Lb);

    /* ---------- 输入有序表 La ---------- */
    printf("请输入有序表La的元素个数：");
    scanf("%d", &n);
    printf("请依次输入有序表La的 %d 个整数（非递减）：\n", n);
    for (i = 1; i <= n; i++)
    {
        scanf("%d", &x);
        ListInsert_Sq(&La, i, x);
    }

    /* ---------- 输入有序表 Lb ---------- */
    printf("请输入有序表Lb的元素个数：");
    scanf("%d", &n);
    printf("请依次输入有序表Lb的 %d 个整数（非递减）：\n", n);
    for (i = 1; i <= n; i++)
    {
        scanf("%d", &x);
        ListInsert_Sq(&Lb, i, x);
    }

    /* ---------- 归并 ---------- */
    MergeList_Sq(La, Lb, &Lc);

    /* ---------- 输出 ---------- */
    printf("\n有序表La = ");
    PrintList_Sq(La);
    printf("\n有序表Lb = ");
    PrintList_Sq(Lb);
    printf("\n归并结果Lc = ");
    PrintList_Sq(Lc);
    printf("\n");
    printf("归并后元素个数：%d\n", ListLength_Sq(Lc));

    /* ---------- 验证原表未被修改 ---------- */
    printf("\n原表La、Lb保持不变的验证：\n");
    printf("有序表La = ");
    PrintList_Sq(La);
    printf("\n有序表Lb = ");
    PrintList_Sq(Lb);
    printf("\n");

    /* ---------- 释放存储空间 ---------- */
    DestroyList_Sq(&La);
    DestroyList_Sq(&Lb);
    DestroyList_Sq(&Lc);
    return 0;
}