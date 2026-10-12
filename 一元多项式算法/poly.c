/*
 * 05_polynomial.c
 * 一元多项式的加、减运算（带头结点的有序单链表）
 *
 * 存储结构设计：
 *   每个结点存放一项：系数 coef（double）与指数 expn（int）；
 *   链表中的结点按指数 expn 递增有序排列；
 *   表中不允许出现系数为 0 的结点（即"同类项"已经合并、零项已经删除）。
 *   这样的组织方式使两个多项式相加/相减可以像归并两个有序表一样，
 *   用一趟扫描完成，时间复杂度为 O(n+m)。
 *
 * 消项处理：
 *   合并同类项后若系数变为 0，必须把该结点从链表中摘除并 free，
 *   否则多项式中会残留 0 系数项，破坏"有序、无零项"的约定。
 *
 * 本程序不使用 math.h，求幂由自定义的 Pow() 完成，因此链接时无需 -lm。
 */
#include <stdio.h>
#include <stdlib.h>

#define OK      1
#define ERROR   0

typedef int Status;

/* ---------- 一元多项式的存储结构 ---------- */
typedef struct PNode
{
    double        coef;         /* 系数 */
    int           expn;         /* 指数 */
    struct PNode *next;         /* 指针域 */
} PNode, *Polynomial;

/* ---------- 内部辅助函数 ---------- */

/* 自己实现整数次幂，避免依赖 math.h 与 -lm */
static double Pow(double x, int e)
{
    double r = 1.0;
    int i;
    for (i = 0; i < e; i++)
        r *= x;
    return r;
}

/* 在 tail 之后追加一个新结点，并让 tail 指向新的尾结点 */
static void AppendTerm(Polynomial *tail, double coef, int expn)
{
    PNode *s = (PNode *)malloc(sizeof(PNode));
    if (s == NULL)
    {
        printf("内存分配失败！\n");
        exit(1);
    }
    s->coef = coef;
    s->expn = expn;
    s->next = NULL;
    (*tail)->next = s;
    *tail = s;
}

/* ---------- 多项式的基本操作 ---------- */

/* 初始化：生成头结点（头结点不存放有效项） */
void InitPolyn(Polynomial *P)
{
    *P = (PNode *)malloc(sizeof(PNode));
    if (*P == NULL)
    {
        printf("内存分配失败！\n");
        exit(1);
    }
    (*P)->coef = 0.0;
    (*P)->expn = -1;                        /* 头结点的指数无意义，取 -1 */
    (*P)->next = NULL;
}

/* 销毁多项式：释放包括头结点在内的全部结点 */
void DestroyPolyn(Polynomial *P)
{
    PNode *p, *q;
    if (*P == NULL)
        return;
    p = *P;
    while (p != NULL)
    {
        q = p->next;
        free(p);
        p = q;
    }
    *P = NULL;
}

/* 求多项式的项数（不计头结点） */
int PolynLength(Polynomial P)
{
    int len = 0;
    PNode *p = P->next;
    while (p != NULL)
    {
        len++;
        p = p->next;
    }
    return len;
}

/*
 * 有序插入一项（coef, expn）：
 *   1) 若 coef 为 0，直接忽略（不产生零项）；
 *   2) 沿链表找到第一个指数 >= expn 的结点 p，pre 为其前驱；
 *   3) 若 p 的指数恰好等于 expn，说明是同类项，系数相加：
 *        相加后系数为 0 —— 消项，把 p 从链表中摘除并释放；
 *        相加后系数不为 0 —— 只改系数，不新建结点；
 *   4) 否则在 pre 之后新建结点插入，保持指数递增有序。
 */
void InsertTerm(Polynomial P, double coef, int expn)
{
    PNode *pre = P;
    PNode *p = P->next;
    PNode *s;

    if (coef == 0.0)                        /* 系数为 0 的输入直接忽略 */
        return;

    while (p != NULL && p->expn < expn)     /* 找第一个指数不小于 expn 的结点 */
    {
        pre = p;
        p = p->next;
    }

    if (p != NULL && p->expn == expn)       /* 同类项：合并系数 */
    {
        p->coef += coef;
        if (p->coef == 0.0)                 /* 消项：系数为 0 则删除该结点 */
        {
            pre->next = p->next;
            free(p);
        }
    }
    else                                    /* 新指数：新建结点插入 */
    {
        s = (PNode *)malloc(sizeof(PNode));
        if (s == NULL)
        {
            printf("内存分配失败！\n");
            exit(1);
        }
        s->coef = coef;
        s->expn = expn;
        s->next = p;                        /* 接在后继之前 */
        pre->next = s;                      /* 前驱指向新结点 */
    }
}

/* 按 系数,指数 的格式读入 n 项，建立多项式 */
void CreatePolyn(Polynomial P, int n)
{
    int i, e = 0;                           /* 先给初值：输入格式异常时不会使用不确定的值 */
    double c = 0.0;
    for (i = 1; i <= n; i++)
    {
        printf("请输入第 %d 项（系数,指数）：", i);
        scanf("%lf,%d", &c, &e);
        InsertTerm(P, c, e);
    }
}

/*
 * 输出多项式，形如 2x^2+6x^5。
 * 格式约定：
 *   系数为 1 或 -1 时省略数字 1（但负号保留）；
 *   指数为 0 时不打印 x^0，只打印系数；
 *   指数为 1 时打印 x 而不是 x^1；
 *   第一项之前不打印 '+'；空多项式打印 0。
 */
void PrintPolyn(Polynomial P)
{
    PNode *p = P->next;
    double c, ac;
    int neg, first = 1;

    if (p == NULL)                          /* 空多项式 */
    {
        printf("0");
        return;
    }

    while (p != NULL)
    {
        c = p->coef;
        neg = (c < 0);                      /* 系数符号 */
        ac = neg ? -c : c;                  /* 系数的绝对值 */

        if (first)                          /* 首项：只在为负时打印负号 */
        {
            if (neg)
                printf("-");
            first = 0;
        }
        else                                /* 非首项：打印 '+' 或 '-' */
        {
            if (neg)
                printf("-");
            else
                printf("+");
        }

        if (p->expn == 0)                   /* 常数项：只打印系数 */
        {
            printf("%g", ac);
        }
        else
        {
            if (ac != 1.0)                  /* 系数 1 / -1 省略数字 1 */
                printf("%g", ac);
            if (p->expn == 1)               /* 一次项打印 x */
                printf("x");
            else
                printf("x^%d", p->expn);
        }
        p = p->next;
    }
}

/*
 * 加减运算的公共内核：Pc = Pa + sign * Pb（sign = +1 为加，-1 为减）。
 * 算法思想与有序表归并完全一致：
 *   两个指针 pa、pb 分别游走于 Pa、Pb（都从首元结点开始）：
 *     指数不等时，把指数较小的一项复制到 Pc；
 *     指数相等时，系数相加/相减，结果为 0 则"消项"（不产生结点）；
 *   最后把未扫描完的表的剩余项依次复制到 Pc。
 * 整个过程不修改 Pa、Pb，结果放在新链表 Pc 中。
 */
static void PolyAddSub(Polynomial Pa, Polynomial Pb, Polynomial *Pc, int sign)
{
    PNode *pa = Pa->next;
    PNode *pb = Pb->next;
    Polynomial tail;
    double c;

    InitPolyn(Pc);                          /* 为结果新建头结点 */
    tail = *Pc;                             /* tail 始终指向结果表的尾结点 */

    while (pa != NULL && pb != NULL)
    {
        if (pa->expn < pb->expn)            /* a 的指数小，先复制 a 的项 */
        {
            AppendTerm(&tail, pa->coef, pa->expn);
            pa = pa->next;
        }
        else if (pa->expn > pb->expn)       /* b 的指数小，先复制 b 的项 */
        {
            AppendTerm(&tail, sign * pb->coef, pb->expn);
            pb = pb->next;
        }
        else                                /* 指数相同：合并系数 */
        {
            c = pa->coef + sign * pb->coef;
            if (c != 0.0)                   /* 不为 0 才生成结点（消项处理） */
                AppendTerm(&tail, c, pa->expn);
            pa = pa->next;
            pb = pb->next;
        }
    }
    while (pa != NULL)                      /* a 的剩余项 */
    {
        AppendTerm(&tail, pa->coef, pa->expn);
        pa = pa->next;
    }
    while (pb != NULL)                      /* b 的剩余项 */
    {
        AppendTerm(&tail, sign * pb->coef, pb->expn);
        pb = pb->next;
    }
    tail->next = NULL;
}

/* 多项式加法：Pc = Pa + Pb（不修改 Pa、Pb） */
void AddPolyn(Polynomial Pa, Polynomial Pb, Polynomial *Pc)
{
    PolyAddSub(Pa, Pb, Pc, 1);              /* sign = +1 表示相加 */
}

/* 多项式减法：Pc = Pa - Pb（不修改 Pa、Pb） */
void SubtractPolyn(Polynomial Pa, Polynomial Pb, Polynomial *Pc)
{
    PolyAddSub(Pa, Pb, Pc, -1);             /* sign = -1 表示相减 */
}

/* 求多项式在 x 处的值（秦九韶思想不适用，此处按项直接累加） */
double EvaluatePolyn(Polynomial P, double x)
{
    double sum = 0.0;
    PNode *p = P->next;
    while (p != NULL)
    {
        sum += p->coef * Pow(x, p->expn);
        p = p->next;
    }
    return sum;
}

/* ---------- 主函数：菜单驱动程序 ---------- */
int main(void)
{
    Polynomial A = NULL, B = NULL, R = NULL;
    int hasA = 0, hasB = 0, hasR = 0;
    int op = -1, n = 0, which = 0;          /* 先给初值：输入异常时不会使用不确定的值 */
    double x = 0.0;

    while (op != 0)
    {
        printf("\n============ 一元多项式运算 ============\n");
        printf(" 1. 输入多项式 A\n");
        printf(" 2. 输入多项式 B\n");
        printf(" 3. 显示多项式 A 与 B\n");
        printf(" 4. 计算 A + B\n");
        printf(" 5. 计算 A - B\n");
        printf(" 6. 求多项式在 x 处的值\n");
        printf(" 0. 退出程序\n");
        printf("========================================\n");
        printf("请选择操作(0-6)：");

        if (scanf("%d", &op) != 1)          /* 输入异常，结束循环 */
            break;
        if (op == 0)
        {
            printf("程序结束，再见！\n");
            break;
        }

        switch (op)
        {
        case 1:                             /* 输入多项式 A */
            if (A != NULL)
                DestroyPolyn(&A);           /* 释放旧的多项式 */
            InitPolyn(&A);
            printf("请输入多项式A的项数n：");
            scanf("%d", &n);
            printf("请依次输入 %d 行，每行格式为 系数,指数 （例如 2,2）：\n", n);
            CreatePolyn(A, n);
            hasA = 1;
            break;

        case 2:                             /* 输入多项式 B */
            if (B != NULL)
                DestroyPolyn(&B);
            InitPolyn(&B);
            printf("请输入多项式B的项数n：");
            scanf("%d", &n);
            printf("请依次输入 %d 行，每行格式为 系数,指数 （例如 2,2）：\n", n);
            CreatePolyn(B, n);
            hasB = 1;
            break;

        case 3:                             /* 显示 A 与 B */
            if (hasA == 0 || hasB == 0)
            {
                printf("请先输入多项式A和B！\n");
                break;
            }
            printf("A(x) = ");
            PrintPolyn(A);
            printf("   （项数：%d）\n", PolynLength(A));
            printf("B(x) = ");
            PrintPolyn(B);
            printf("   （项数：%d）\n", PolynLength(B));
            break;

        case 4:                             /* A + B */
        case 5:                             /* A - B */
        {
            const char *expr;
            if (hasA == 0 || hasB == 0)
            {
                printf("请先输入多项式A和B！\n");
                break;
            }
            if (R != NULL)                  /* 释放上一次的运算结果 */
                DestroyPolyn(&R);
            if (op == 4)
            {
                AddPolyn(A, B, &R);
                expr = "A(x)+B(x)";
            }
            else
            {
                SubtractPolyn(A, B, &R);
                expr = "A(x)-B(x)";
            }
            hasR = 1;

            printf("\nA(x) = ");
            PrintPolyn(A);
            printf("\nB(x) = ");
            PrintPolyn(B);
            printf("\n%s = R(x) = ", expr);
            PrintPolyn(R);
            printf("\n");
            printf("运算结果的项数：%d\n", PolynLength(R));
            if (PolynLength(R) < PolynLength(A) + PolynLength(B))
                printf("提示：结果项数 %d 少于 |A|+|B| = %d，说明运算中发生了同类项合并，"
                       "系数相消为 0 的项已被消去（消项）。\n",
                       PolynLength(R), PolynLength(A) + PolynLength(B));
            printf("运算后原多项式保持不变：\n");
            printf("A(x) = ");
            PrintPolyn(A);
            printf("\nB(x) = ");
            PrintPolyn(B);
            printf("\n");
            break;
        }

        case 6:                             /* 求值 */
            printf("请选择要求值的多项式：1-A(x)  2-B(x)  3-上次运算结果R(x)：");
            scanf("%d", &which);
            printf("请输入x的值：");
            scanf("%lf", &x);
            if (which == 1)
            {
                if (hasA == 0)
                    printf("多项式A尚未输入！\n");
                else
                    printf("A(%.4f) = %.4f\n", x, EvaluatePolyn(A, x));
            }
            else if (which == 2)
            {
                if (hasB == 0)
                    printf("多项式B尚未输入！\n");
                else
                    printf("B(%.4f) = %.4f\n", x, EvaluatePolyn(B, x));
            }
            else if (which == 3)
            {
                if (hasR == 0)
                    printf("尚无可用的运算结果R(x)，请先执行 4 或 5！\n");
                else
                    printf("R(%.4f) = %.4f\n", x, EvaluatePolyn(R, x));
            }
            else
            {
                printf("选择无效！\n");
            }
            break;

        default:
            printf("输入的操作码无效，请重新选择(0-6)！\n");
            break;
        }
    }

    /* ---------- 退出前释放全部动态存储空间 ---------- */
    DestroyPolyn(&A);
    DestroyPolyn(&B);
    DestroyPolyn(&R);
    return 0;
}