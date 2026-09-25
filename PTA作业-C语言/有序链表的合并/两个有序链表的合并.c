#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int data;
    struct Node* next;
}Node;

// 创建链表的函数 返回头节点
// 尾插法
Node* createList(){
    Node* head = NULL;
    Node* tail = NULL;
    int value;
    while(scanf("%d", &value) == 1 && value != -1){
        // 创建节点
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->data = value;
        newNode->next = NULL;
        // 连接节点
        if(head == NULL){
            head = newNode;
            tail = newNode;
        }else{
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;  // 返回头节点
}

// 合并两个非降序链表
Node* mergeList(Node *s1, Node* s2){
    Node* head = NULL;
    Node* tail = NULL;
    while(s1 != NULL && s2 != NULL){
        Node* temp;
        if(s1->data <= s2->data){
            temp = s1;
            s1 = s1->next;  // 移动更新s1的指针
        }else{
            temp = s2;
            s2 = s2->next;  // 移动更新s2指针
        }
        if(head == NULL){
            head = temp;
            tail = temp;
        }else{
            tail->next = temp;
            tail = temp;
        }
    }
    // s1还有剩余
    if(s1 != NULL){
        if(head == NULL){
            head = s1;
        }else{
            tail->next = s1;
        }
    }
    // s2还有剩余
    if(s2 != NULL){
        if(head == NULL){
            head = s2;
        }else{
            tail->next = s2;
        }
    }
    return head;  // 返回合并之后的新链表的头节点
}

// 输出链表
void printList(Node* head){
    if(head == NULL){
        printf("NULL");
        return;
    }
    Node* p = head;
    while(p != NULL){
        if(p != head){
            printf(" ");
        }
        printf("%d", p->data);
        p = p->next;
    }
}

int main(){
    Node* s1 = createList();
    Node* s2 = createList();
    Node* s3 = mergeList(s1, s2);
    printList(s3);
    return 0;
}