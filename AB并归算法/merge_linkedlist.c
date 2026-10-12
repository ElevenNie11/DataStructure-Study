/*
 * 04_merge_linklist.c
 * 单链表实现两个有序表的归并，给出两种实现方式：
 *   方式一 MergeList_L_New   —— 新建结点，不破坏原表；
 *   方式二 MergeList_L       —— 教材算法 2.12，复用 La 的头结点，破坏原表。
 * 教材：《数据结构》（严蔚敏）算法 2.12
 *
 * 时间复杂度：两种方式都只需一趟扫描，O(La.length + Lb.length)。
 * 空间复杂度：方式一为 O(n+m)（新建 n+m 个结点）；
 *             方式二为 O(1)（只改变指针，不申请新结点）。
 */
#include <stdio.h>
#include <stdlib.h>

#define OK      1
#define ERROR   0

typedef int Status;
typedef int ElemType;

/* ---------- 单链表的存储结构（带头结点） ---------- */
typedef struct LNode
{
    ElemType      data;
    struct LNode *next;
} LNode, *LinkList;

/* 初始化：生成头结点 */
Status InitList_L(LinkList *L)
{
    *L = (LinkList)malloc(sizeof(LNode));
    if (*L == NULL)
        return ERROR;
    (*L)->next = NULL;
    return OK;
}

/* 销毁单链表（含头结点） */
void DestroyList_L(LinkList *L)
{
    LinkList p, q;
    if (*L == NULL)
        return;
    p = *L;
    while (p != NULL)
    {
        q = p->next;
        free(p);
        p = q;
    }
    *L = NULL;
}

/* 求表长（不计头结点） */
int ListLength_L(LinkList L)
{
    int len = 0;
    LinkList p = L->next;
    while (p != NULL)
    {
        len++;
        p = p->next;
    }
    return len;
}

/* 在第 i 个位置之前插入元素 e */
Status ListInsert_L(LinkList L, int i, ElemType e)
{
    LinkList p = L, s;
    int j = 0;
    while (p != NULL && j < i - 1)          /* 定位到第 i-1 个结点 */
    {
        p = p->next;
        j++;
    }
    if (p == NULL || i < 1)
        return ERROR;
    s = (LinkList)malloc(sizeof(LNode));    /* 生成新结点 */
    if (s == NULL)
        return ERROR;
    s->data = e;
    s->next = p->next;
    p->next = s;
    return OK;
}

/* 输出单链表 */
void PrintList_L(LinkList L)
{
    LinkList p = L->next;
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

/* 读入 n 个整数，依次尾插建表 */
void CreateList_L(LinkList L, int n)
{
    int i, x;
    for (i = 1; i <= n; i++)
    {
        scanf("%d", &x);
        ListInsert_L(L, i, x);
    }
}

/*
 * 方式一：新建结点实现归并，La、Lb 的内容保持不变。
 * 算法思想：pa、pb 分别游走于 La、Lb，每次取较小者，
 *           申请一个新结点接到 Lc 的尾部。
 */
void MergeList_L_New(LinkList La, LinkList Lb, LinkList *Lc)
{
    LinkList pa = La->next;
    LinkList pb = Lb->next;
    LinkList pc, s;

    InitList_L(Lc);                         /* Lc 也带头结点 */
    pc = *Lc;                               /* pc 始终指向 Lc 的尾结点 */

    while (pa != NULL && pb != NULL)
    {
        s = (LinkList)malloc(sizeof(LNode));/* 申请新结点，与原表无关 */
        if (s == NULL)
        {
            printf("内存分配失败！\n");
            exit(1);
        }
        if (pa->data <= pb->data)
        {
            s->data = pa->data;
            pa = pa->next;
        }
        else
        {
            s->data = pb->data;
            pb = pb->next;
        }
        s->next = NULL;
        pc->next = s;                       /* 尾插到 Lc */
        pc = s;
    }
    /* 把剩余部分继续复制过来 */
    while (pa != NULL)
    {
        s = (LinkList)malloc(sizeof(LNode));
        if (s == NULL)
        {
            printf("内存分配失败！\n");
            exit(1);
        }
        s->data = pa->data;
        s->next = NULL;
        pc->next = s;
        pc = s;
        pa = pa->next;
    }
    while (pb != NULL)
    {
        s = (LinkList)malloc(sizeof(LNode));
        if (s == NULL)
        {
            printf("内存分配失败！\n");
            exit(1);
        }
        s->data = pb->data;
        s->next = NULL;
        pc->next = s;
        pc = s;
        pb = pb->next;
    }
}

/*
 * 方式二（教材算法 2.12）：利用原表的结点空间，不申请新结点。
 * 关键步骤：
 *   1) 用 La 的头结点作为 Lc 的头结点；
 *   2) pa、pb 分别指向两个表的首元结点，pc 指向 Lc 的尾结点；
 *   3) 循环把较小的结点"摘"下来接到 pc 之后；
 *   4) 把未取完的链直接接到 pc 之后；
 *   5) 释放 Lb 的头结点（La 的头结点已经变成 Lc 的头结点，不能释放）。
 * 调用后 La、Lb 均被破坏，不应再使用。
 */
void MergeList_L(LinkList La, LinkList Lb, LinkList *Lc)
{
    LinkList pa = La->next;
    LinkList pb = Lb->next;
    LinkList pc;

    *Lc = pc = La;                          /* 用 La 的头结点作为 Lc 的头结点 */
    while (pa != NULL && pb != NULL)
    {
        if (pa->data <= pb->data)
        {
            pc->next = pa;                  /* 摘下 pa 接到 Lc 尾部 */
            pc = pa;
            pa = pa->next;
        }
        else
        {
            pc->next = pb;                  /* 摘下 pb 接到 Lc 尾部 */
            pc = pb;
            pb = pb->next;
        }
    }
    pc->next = (pa != NULL) ? pa : pb;      /* 插入剩余段 */

    free(Lb);                               /* 释放 Lb 的头结点 */
}

int main(void)
{
    LinkList La, Lb, Lc;
    int n = 0;                              /* 先给初值：输入异常时不会使用不确定的值 */

    /* ---------- 建立有序表 La、Lb ---------- */
    InitList_L(&La);
    InitList_L(&Lb);

    printf("请输入有序表La的元素个数：");
    scanf("%d", &n);
    printf("请依次输入有序表La的 %d 个整数（非递减）：\n", n);
    CreateList_L(La, n);

    printf("请输入有序表Lb的元素个数：");
    scanf("%d", &n);
    printf("请依次输入有序表Lb的 %d 个整数（非递减）：\n", n);
    CreateList_L(Lb, n);

    printf("\n有序表La = ");
    PrintList_L(La);
    printf("\n有序表Lb = ");
    PrintList_L(Lb);
    printf("\n");

    /* ================= 方式一 ================= */
    printf("\n===== 方式一：新建结点，不破坏原表 =====\n");
    MergeList_L_New(La, Lb, &Lc);
    printf("有序表La = ");
    PrintList_L(La);
    printf("\n有序表Lb = ");
    PrintList_L(Lb);
    printf("\n归并结果Lc = ");
    PrintList_L(Lc);
    printf("\n归并后元素个数：%d\n", ListLength_L(Lc));
    printf("验证：La 的长度 = %d，Lb 的长度 = %d，原表内容未被修改。\n",
           ListLength_L(La), ListLength_L(Lb));
    DestroyList_L(&Lc);                     /* 释放方式一产生的链表 */

    /* ================= 方式二 ================= */
    printf("\n===== 方式二：教材算法2.12，复用La头结点 =====\n");
    MergeList_L(La, Lb, &Lc);
    La = NULL;                              /* La、Lb 已归并进 Lc，置空防止误用 */
    Lb = NULL;
    printf("归并结果Lc = ");
    PrintList_L(Lc);
    printf("\n归并后元素个数：%d\n", ListLength_L(Lc));
    printf("说明：算法2.12 复用了 La 的头结点并重排原有结点，\n");
    printf("      同时释放了 Lb 的头结点，故原表 La、Lb 已被破坏、不再可用。\n");

    /* ---------- 释放存储空间 ---------- */
    DestroyList_L(&Lc);
    DestroyList_L(&La);                     /* 此时为 NULL，调用是安全的 */
    DestroyList_L(&Lb);
    return 0;
}