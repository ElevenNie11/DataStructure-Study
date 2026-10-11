#include <stdio.h>
#include <stdlib.h>

#define ERROR 1e8
typedef int ElementType;
typedef enum { push, pop, end } Operation;
// typedef enum { false, true } bool;     (为了vscode不报错把它暂时注释掉，PTA题目本身含这条语句)
typedef int Position;
struct SNode {
    ElementType *Data;
    Position Top1, Top2;
    int MaxSize;
};
typedef struct SNode *Stack;

Stack CreateStack( int MaxSize );
bool Push( Stack S, ElementType X, int Tag );
ElementType Pop( Stack S, int Tag );

Operation GetOp();
void PrintStack( Stack S, int Tag );

int main()
{
    int N, Tag, X;
    Stack S;
    int done = 0;

    scanf("%d", &N);
    S = CreateStack(N);
    while ( !done ) {
        switch( GetOp() ) {
        case push: 
            scanf("%d %d", &Tag, &X);
            if (!Push(S, X, Tag)) printf("Stack %d is Full!\n", Tag);
            break;
        case pop:
            scanf("%d", &Tag);
            X = Pop(S, Tag);
            if ( X==ERROR ) printf("Stack %d is Empty!\n", Tag);
            break;
        case end:
            PrintStack(S, 1);
            PrintStack(S, 2);
            done = 1;
            break;
        }
    }
    return 0;
}

/* 你的代码将被嵌在这里 */
// 两个栈不要分别开两个数组，而是让它们共享同一个数组空间，一个从左往右增长，一个从右往左增长
Stack CreateStack( int MaxSize ){
    Stack s;
    s = (Stack)malloc(sizeof(struct SNode));
    s->Data = (ElementType*)malloc(MaxSize* sizeof(ElementType));
    s->Top1 = -1;
    s->Top2 = MaxSize;
    s->MaxSize = MaxSize;
    return s;
}

bool Push(Stack s, ElementType x, int Tag){
    if(s->Top1 + 1 == s->Top2){
        printf("Stack Full\n");
        return false;
    }
    if(Tag == 1){
        s->Data[++s->Top1] = x;
    }else if(Tag == 2){
        s->Data[--s->Top2] = x;
    }
    return true;
}

ElementType Pop(Stack s, int Tag){
    if(Tag == 1){
        if(s->Top1 == -1){
            printf("Stack 1 Empty\n");
            return ERROR;
        }
        return s->Data[s->Top1--];
    }else if(Tag == 2){
        if(s->Top2 == s->MaxSize){
            printf("Stack 2 Empty\n");
            return ERROR;
        }
        return s->Data[s->Top2++];
    }
    return ERROR;
}