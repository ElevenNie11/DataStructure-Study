/*
 * 02_union_linklist.c
 * 带头结点的单链表实现集合的并运算：A = A ∪ B
 * 教材：《数据结构》（严蔚敏）第 2 章 线性表 —— 单链表的基本操作
 *
 * 算法思想：
 *   依次取出链表 B 中的每一个元素 e，在链表 A 中顺序查找；
 *   若 A 中不存在 e，则把 e 插入到 A 的表尾；否则跳过。
 * 时间复杂度：
 *   LocateElem_L 需要从头遍历，为 O(n)；Union_L 对 B 的每个元素
 *   都要调用一次查找和一次求表长（ListLength_L 也是 O(n)），
 *   故 Union_L 的时间复杂度为 O(m*n)，m 为 B 的长度。
 */
#include <stdio.h>
#include <stdlib.h>

#define OK      1
#define ERROR   0

typedef int Status;             /* 函数返回状态类型 */
typedef int ElemType;           /* 数据元素类型 */

/* ---------- 单链表的存储结构（带头结点） ---------- */
typedef struct LNode
{
    ElemType      data;         /* 数据域 */
    struct LNode *next;         /* 指针域 */
} LNode, *LinkList;

/* ---------- 基本操作 ---------- */

/* 初始化：生成头结点，头结点的 next 置空 */
Status InitList_L(LinkList *L)
{
    *L = (LinkList)malloc(sizeof(LNode));
    if (*L == NULL)                         /* 存储分配失败 */
        return ERROR;
    (*L)->next = NULL;
    return OK;
}

/* 销毁单链表：从头结点开始逐个释放 */
void DestroyList_L(LinkList *L)
{
    LinkList p, q;
    if (*L == NULL)
        return;
    p = *L;
    while (p != NULL)
    {
        q = p->next;                        /* 先记住后继，再释放当前结点 */
        free(p);
        p = q;
    }
    *L = NULL;
}

/* 求表长（不计头结点），必须从头到尾遍历，O(n) */
int ListLength_L(LinkList L)
{
    int len = 0;
    LinkList p = L->next;                   /* p 指向首元结点 */
    while (p != NULL)
    {
        len++;
        p = p->next;
    }
    return len;
}

/* 取第 i 个元素（1 <= i <= 表长），用 e 带回 */
Status GetElem_L(LinkList L, int i, ElemType *e)
{
    LinkList p = L->next;                   /* 从首元结点开始 */
    int j = 1;
    while (p != NULL && j < i)              /* 顺指针向后查找第 i 个结点 */
    {
        p = p->next;
        j++;
    }
    if (p == NULL || i < 1)
        return ERROR;
    *e = p->data;
    return OK;
}

/*
 * 按值查找：返回第一个数据域等于 e 的结点的指针，找不到返回 NULL。
 * 由于链表不能随机存取，只能从头顺序比较，故为 O(n)。
 */
LinkList LocateElem_L(LinkList L, ElemType e)
{
    LinkList p = L->next;
    while (p != NULL && p->data != e)
        p = p->next;
    return p;                               /* 找到则为结点指针，否则为 NULL */
}

/*
 * 在第 i 个位置之前插入数据元素 e（1 <= i <= 表长+1）。
 * 关键：必须找到第 i-1 个结点（前驱），在它之后插入新结点。
 */
Status ListInsert_L(LinkList L, int i, ElemType e)
{
    LinkList p = L, s;
    int j = 0;
    while (p != NULL && j < i - 1)          /* 寻找第 i-1 个结点 */
    {
        p = p->next;
        j++;
    }
    if (p == NULL || i < 1)                 /* i 非法 */
        return ERROR;
    s = (LinkList)malloc(sizeof(LNode));    /* 生成新结点 */
    if (s == NULL)
        return ERROR;
    s->data = e;
    s->next = p->next;                      /* 新结点指向原来的第 i 个结点 */
    p->next = s;                            /* 前驱结点指向新结点 */
    return OK;
}

/* 输出单链表，形如 {3, 6, 9}；空表输出 { } */
void PrintList_L(LinkList L)
{
    LinkList p = L->next;                   /* 跳过头结点 */
    if (p == NULL)
    {
        printf("{ }");
        return;
    }
    printf("{");
    while (p != NULL)
    {
        printf("%d", p->data);
        if (p->next != NULL)
            printf(", ");
        p = p->next;
    }
    printf("}");
}

/*
 * 集合的并运算：La = La ∪ Lb
 * 对 Lb 中的每个元素 e，若 La 中不存在，则插入到 La 的表尾。
 */
void Union_L(LinkList La, LinkList Lb)
{
    LinkList p = Lb->next;                  /* 从 B 的首元结点开始 */
    ElemType e;
    printf("---- 并集运算过程（依次考察 B 中的每个元素）----\n");
    while (p != NULL)
    {
        e = p->data;
        if (LocateElem_L(La, e) == NULL)     /* A 中不存在 e */
        {
            ListInsert_L(La, ListLength_L(La) + 1, e);  /* 追加到表尾 */
            printf("B中元素 %d 不在A中，已插入。\n", e);
        }
        else
        {
            printf("B中元素 %d 已在A中，跳过。\n", e);
        }
        p = p->next;
    }
}

/* 把数组中的 n 个整数依次插入链表尾部 */
void CreateList_L(LinkList L, int n)
{
    int i, x;
    for (i = 1; i <= n; i++)
    {
        scanf("%d", &x);
        ListInsert_L(L, i, x);
    }
}

int main(void)
{
    LinkList La, Lb;
    int n1 = 0, n2 = 0;                     /* 先给初值：输入异常时不会使用不确定的值 */

    InitList_L(&La);
    InitList_L(&Lb);

    /* ---------- 输入集合 A ---------- */
    printf("请输入集合A的元素个数：");
    scanf("%d", &n1);
    printf("请依次输入集合A的 %d 个整数：\n", n1);
    CreateList_L(La, n1);

    /* ---------- 输入集合 B ---------- */
    printf("请输入集合B的元素个数：");
    scanf("%d", &n2);
    printf("请依次输入集合B的 %d 个整数：\n", n2);
    CreateList_L(Lb, n2);

    /* ---------- 运算前的输出 ---------- */
    printf("\n集合A = ");
    PrintList_L(La);
    printf("\n集合B = ");
    PrintList_L(Lb);
    printf("\n");

    /* ---------- 求并集 La = La ∪ Lb ---------- */
    Union_L(La, Lb);

    /* ---------- 输出结果 ---------- */
    printf("集合A∪B = ");
    PrintList_L(La);
    printf("\n");
    printf("并集元素个数：%d\n", ListLength_L(La));

    /* ---------- 释放所有结点 ---------- */
    DestroyList_L(&La);
    DestroyList_L(&Lb);
    return 0;
}