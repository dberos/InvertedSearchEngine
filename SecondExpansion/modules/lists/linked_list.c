#include"../../include/linked_list.h"

LinkedList linked_list_create(){
    LinkedList list=malloc(sizeof(*list));
    list->head=NULL;
    list->size=0;
    return list;
}


void linked_list_destroy(LinkedList list){
    LinkedListNode prev;
    LinkedListNode curr;
    curr=list->head;
    while(curr!=NULL){
        prev=curr;
        curr=curr->next;
        free(prev->string);
        free(prev);
        list->size--;
    }
    free(list);
}


bool linked_list_insert_end(LinkedList list,String string){
    LinkedListNode node=malloc(sizeof(*node));
    LinkedListNode curr;
    node->string=strdup(string);
    // First value found would get inserted twice
    if(list->head!=NULL && strcmp(list->head->string,string)==0){
        free(node->string);
        free(node);
        return false;
    }
    if(list->head==NULL){
        node->next=list->head;
        list->head=node;
    }
    else{
        curr=list->head;
        while(curr->next!=NULL){
            if(strcmp(curr->next->string,string)!=0){
                curr=curr->next;
            }
            else{
                free(node->string);
                free(node);
                return false;
            }
        }
        node->next=curr->next;
        curr->next=node;
    }
    list->size++;
    return true;
}
