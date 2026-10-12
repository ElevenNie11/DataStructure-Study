/*
 * 01_union_seqlist.c
 * 顺序表（动态分配）实现集合的并运算：A = A ∪ B
 * 教材：《数据结构》（严蔚敏）第 2 章 线性表 —— 算法 2.1 的推广
 *
 * 算法思想：
 *   依次取出集合 B 中的每一个元素 e，在 A 中查找是否已经存在；
 *   若不存在，则把 e 插入到 A 的表尾；若已存在则跳过。
 * 时间复杂度：
 *   设 A 的长度为 n，B 的长度为 m，LocateElem_Sq 为 O(n)，
 *   共做 m 次查找，故 Union_Sq 的时间复杂度为 O(m*n)。
 */
#include <stdio.h>
#include <stdlib.h>

#define LIST_INIT_SIZE 100      /* 线性表存储空间的初始分配量 */
#define LISTINCREMENT  10       /* 线性表存储空间的分配增量 */

#define OK      1
#define ERROR   0

typedef int Status;             /* 函数返回状态类型 */
typedef int ElemType;           /* 数据元素类型：本程序处理整数集合 */

/* ---------- 顺序表的存储结构（动态分配） ---------- */
typedef struct
{
    ElemType *elem;             /* 存储空间基址 */
    int       length;           /* 当前长度 */
    int       listsize;         /* 当前分配的存储容量（以 sizeof(ElemType) 为单位） */
} SqList;

/* ---------- 基本操作 ---------- */

/* 初始化顺序表：分配初始空间，表长置 0 */
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

/* 销毁顺序表：释放存储空间，各成员复位 */
void DestroyList_Sq(SqList *L)
{
    free(L->elem);                          /* free(NULL) 是安全的 */
    L->elem = NULL;
    L->length = 0;
    L->listsize = 0;
}

/* 求表长 */
int ListLength_Sq(SqList L)
{
    return L.length;
}

/* 取第 i 个元素（1 <= i <= L.length），用 e 带回 */
Status GetElem_Sq(SqList L, int i, ElemType *e)
{
    if (i < 1 || i > L.length)              /* 下标越界 */
        return ERROR;
    *e = L.elem[i - 1];                     /* 顺序表下标从 0 开始 */
    return OK;
}

/*
 * 顺序查找：在 L 中查找第一个值与 e 相等的数据元素。
 * 返回值：找到时返回该元素的位序（1 起），否则返回 0。
 */
int LocateElem_Sq(SqList L, ElemType e)
{
    int i;
    for (i = 0; i < L.length; i++)          /* 从表头到表尾依次比较 */
    {
        if (L.elem[i] == e)
            return i + 1;                   /* 返回 1 基位序 */
    }
    return 0;                               /* 未找到 */
}

/*
 * 在第 i 个位置之前插入元素 e（1 <= i <= L.length+1）。
 * 空间不足时按 LISTINCREMENT 增量重新分配。
 */
Status ListInsert_Sq(SqList *L, int i, ElemType e)
{
    ElemType *newbase, *p, *q;
    if (i < 1 || i > L->length + 1)         /* 插入位置非法 */
        return ERROR;
    if (L->length >= L->listsize)           /* 当前空间已满，需要扩容 */
    {
        newbase = (ElemType *)realloc(L->elem,
                                      (L->listsize + LISTINCREMENT) * sizeof(ElemType));
        if (newbase == NULL)                /* 分配失败 */
            return ERROR;
        L->elem = newbase;                  /* 新的基址 */
        L->listsize += LISTINCREMENT;
    }
    q = L->elem + i - 1;                    /* q 为插入位置 */
    if (L->length > 0)                      /* 插入位置之后的元素全部后移一位 */
    {
        for (p = L->elem + L->length - 1; p >= q; --p)
            *(p + 1) = *p;
    }
    *q = e;                                 /* 插入 e */
    L->length++;
    return OK;
}

/* 输出顺序表，形如 {3, 6, 9}；空表输出 { } */
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
 * 集合的并运算：A = A ∪ B
 * 保留 A 中元素的原有次序，B 中元素按 B 的次序追加到表尾。
 */
void Union_Sq(SqList *La, SqList Lb)
{
    int n1 = La->length, n2 = Lb.length, i;
    ElemType e;
    printf("---- 并集运算过程（依次考察 B 中的每个元素）----\n");
    for (i = 1; i <= n2; i++)
    {
        GetElem_Sq(Lb, i, &e);              /* 取 B 中第 i 个元素 */
        if (LocateElem_Sq(*La, e) == 0)     /* A 中不存在 e */
        {
            ListInsert_Sq(La, n1 + 1, e);   /* 插入到 A 的表尾 */
            n1++;
            printf("B中元素 %d 不在A中，已插入。\n", e);
        }
        else
        {
            printf("B中元素 %d 已在A中，跳过。\n", e);
        }
    }
}

int main(void)
{
    SqList La, Lb;
    int n1 = 0, n2 = 0, i, x = 0;           /* 先给初值：输入异常时不会使用不确定的值 */

    InitList_Sq(&La);
    InitList_Sq(&Lb);

    /* ---------- 输入集合 A ---------- */
    printf("请输入集合A的元素个数：");
    scanf("%d", &n1);
    printf("请依次输入集合A的 %d 个整数：\n", n1);
    for (i = 1; i <= n1; i++)
    {
        scanf("%d", &x);
        ListInsert_Sq(&La, i, x);
    }

    /* ---------- 输入集合 B ---------- */
    printf("请输入集合B的元素个数：");
    scanf("%d", &n2);
    printf("请依次输入集合B的 %d 个整数：\n", n2);
    for (i = 1; i <= n2; i++)
    {
        scanf("%d", &x);
        ListInsert_Sq(&Lb, i, x);
    }

    /* ---------- 运算前的输出 ---------- */
    printf("\n集合A = ");
    PrintList_Sq(La);
    printf("\n集合B = ");
    PrintList_Sq(Lb);
    printf("\n");

    /* ---------- 求并集 A = A ∪ B ---------- */
    Union_Sq(&La, Lb);

    /* ---------- 输出结果 ---------- */
    printf("集合A∪B = ");
    PrintList_Sq(La);
    printf("\n");
    printf("并集元素个数：%d\n", ListLength_Sq(La));

    /* ---------- 释放所有动态申请的存储空间 ---------- */
    DestroyList_Sq(&La);
    DestroyList_Sq(&Lb);
    return 0;
}