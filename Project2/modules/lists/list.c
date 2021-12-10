#include"../../include/list.h"

List list_create(){
    // Allocating memory for the list
    List list=malloc(sizeof(*list));

    // Set size to 0
    list->size=0;
    // Set head to NULL
    list->head=NULL;
    // Set tail to NULL;
    list->tail=NULL;

    // Return the list
    return list;
}

void list_destroy(List list){
    ListNode prev;
    ListNode curr;
    curr=list->head;
    while(curr!=NULL){
        prev=curr;
        curr=curr->next;
        free(prev);
        list->size--;
    }
    free(list);
}

void list_insert_head(List list,Pointer value){
    ListNode node=malloc(sizeof(*node));
    node->value=value;
    node->next=list->head;
    list->head=node;
    list->size++;
}

void list_insert_tail(List list,Pointer value){
    ListNode node=malloc(sizeof(*node));
    node->next=NULL;
    node->value=value;
    if(list->head==NULL){
        list->head=node;
        list->tail=node;
        list->size++;
        return;
    }
    list->tail->next=node;
    list->tail=node;
    list->size++;
}
