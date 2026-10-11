#include <stdio.h>
#include <stdlib.h>
// PTA所给程序
#define ERROR -1
typedef int ElementType;
typedef enum { addq, delq, end } Operation;
//typedef enum { false, true } bool;       (为了vscode不报错把它暂时注释掉，PTA题目本身含这条语句)
typedef int Position;
typedef struct QNode *PtrToQNode;
struct QNode {
    ElementType *Data;  /* 存储元素的数组   */
    Position Front;     /* 队列的头、尾指针 */
    int Count;          /* 队列中元素个数   */
    int MaxSize;        /* 队列最大容量     */
};
typedef PtrToQNode Queue; 

Queue CreateQueue( int MaxSize )
{
    Queue Q = (Queue)malloc(sizeof(struct QNode));
    Q->Data = (ElementType *)malloc(MaxSize * sizeof(ElementType));
    Q->Front = 0;
    Q->Count = 0;
    Q->MaxSize = MaxSize;
    return Q;
}

bool AddQ( Queue Q, ElementType X );
ElementType DeleteQ( Queue Q );

Operation GetOp();  /* 裁判实现，细节不表 */

int main()
{
    ElementType X;
    Queue Q;
    int N, done = 0;

    scanf("%d", &N);
    Q = CreateQueue(N);
    while ( !done ) {
        switch( GetOp() ) {
        case addq: 
            scanf("%d", &X);
            AddQ(Q, X);
            break;
        case delq:
            X = DeleteQ(Q);
            if ( X!=ERROR ) printf("%d is out\n", X);
            break;
        case end:
            while (Q->Count) printf("%d ", DeleteQ(Q));
            done = 1;
            break;
        }
    }
    return 0;
}

/* 你的代码将被嵌在这里 */

// 入队函数AddQ
bool AddQ(Queue Q, ElementType X){
    if(Q->Count == Q->MaxSize){
        printf("Queue Full\n");
        return false;
    }

    // 因为我们没有rear指针，所以：当前队尾位置 = 队头位置 + 当前元素数量
    // 因为循环，所以再除以MaxSize即可
    int rear = (Q->Front + Q->Count) % Q->MaxSize;  // 计算新的队尾位置
    Q->Data[rear] = X;
    Q->Count++;

    return true;
}

// 出队函数 DeleteQ
ElementType DeleteQ(Queue Q){
    if(Q->Count == 0){
        printf("Queue Empty\n");
        return ERROR;
    }

    ElementType item =Q->Data[Q->Front];     // 保存队头元素
    Q->Front = (Q->Front + 1) % Q->MaxSize;  // 移动Front（【重点】当Front移到数组末尾（如Front=4），再+1就是5，会越界。取模让它绕回0）
    Q->Count--;

    return item;
}

// 初始入队 A B C：   [A][B][C][ ][ ]   Front=0 Count=3
// 出队 A、B：        [ ][ ][C][ ][ ]   Front=2 Count=1
// 入队 D：           [ ][ ][C][D][ ]   rear=(2+1)%5=3
// 入队 E：           [ ][ ][C][D][E]   rear=(2+2)%5=4
// 入队 F：           [F][ ][C][D][E]   rear=(2+3)%5=0  ← 从头部重新利用