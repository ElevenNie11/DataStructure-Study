#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int data;
    struct Node* next;
}Node;

Node* createList(){
    Node* head = NULL;
    Node* tail = NULL;
    int value;
    // 本题目有一个PTA边界测试（分值为1分）：value没有说必须等于-1才结束测试，只是说输入为负整数的时候就结束测试
    // 所以value >= 0
    while(scanf("%d", &value) == 1 && value >= 0){
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->data = value;
        newNode->next = NULL;
        if(head == NULL){
            head = newNode;
            tail = newNode;
        }else{
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}


// 快慢指针
Node* findNode(Node* head, int k){
    Node* fast = head;
    Node* slow = head;
    // 快指针先往前走k个位置
    for(int i = 0; i < k; i++){
        if(fast == NULL){
            return NULL;
        }
        fast = fast->next;
    }
    // 这时候慢指针也开始走，快慢指针同时走
    // 快慢指针之间永远相差k
    while(fast != NULL){
        fast = fast->next;
        slow = slow->next;
    }
    return slow;
}

int main(){
    int k;
    scanf("%d", &k);
    Node* list = createList();
    Node* result = findNode(list, k);
    if(result == NULL){
        printf("NULL");
    }else{
        printf("%d", result->data);
    }
    return 0;
}