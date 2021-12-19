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

void list_insert_head(List list,uint value){
    ListNode node=malloc(sizeof(*node));
    node->value=value;
    node->next=list->head;
    list->head=node;
    list->size++;
}

void list_insert_tail(List list,uint value){
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


bool check_list_existence(List list, uint value){
    for(ListNode lnode=list->head;lnode!=NULL;lnode=lnode->next){
        if(lnode->value==value){
            return true;
        }
    }
    return false;
}


bool list_remove(List list,uint value){

    ListNode node;
    ListNode curr=list->head;
    ListNode prev=NULL;
    bool exists=false;
    for(ListNode lnode=list->head;lnode!=NULL;lnode=lnode->next){
        if(lnode->value==value){
            node=lnode;
            exists=true;
            break;
        }
    }

    if(exists==false){
        return false;
    }
    if(list->head==node){
        list->head=list->head->next;
        free(node);

    }
    else{
        while(curr!=node){
            prev=curr;
            curr=curr->next;
        }
        prev->next=curr->next;
        free(node);
    }
    list->size--;
    return true;
}

