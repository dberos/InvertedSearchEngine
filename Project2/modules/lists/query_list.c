#include"../../include/query_list.h"

QueryList query_list_create(){
    // Allocating memory for the list
    QueryList query_list=malloc(sizeof(*query_list));

    // Set size to 0
    query_list->size=0;
    // Set head to NULL
    query_list->head=NULL;
    // Set tail to NULL;
    query_list->tail=NULL;

    // Return the list
    return query_list;
}

void query_list_destroy(QueryList query_list){
    QueryListNode prev;
    QueryListNode curr;
    curr=query_list->head;
    while(curr!=NULL){
        prev=curr;
        curr=curr->next;
        query_destroy(prev->query);
        free(prev);
        query_list->size--;
    }
    free(query_list);
}

void query_list_insert_head(QueryList query_list,Query query){
    QueryListNode node=malloc(sizeof(*node));
    node->query=query;
    node->next=query_list->head;
    query_list->head=node;
    query_list->size++;
}

void query_list_insert_tail(QueryList list,Query query){
    QueryListNode node=malloc(sizeof(*node));
    node->next=NULL;
    node->query=query;
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

bool query_list_remove(QueryList query_list,uint id){
    QueryListNode node;
    QueryListNode curr=query_list->head;
    QueryListNode prev=NULL;
    bool exists=false;
    for(QueryListNode lnode=query_list->head;lnode!=NULL;lnode=lnode->next){
        if(lnode->query->query_id==id){
            node=lnode;
            exists=true;
            break;
        }
    }
    if(exists==false){
        return false;
    }
    if(query_list->head==node){
        query_list->head=query_list->head->next;
        query_destroy(node->query);
        free(node);
    }else
    if(query_list->tail==node){
        QueryListNode previous=NULL;
        for(QueryListNode lnode=query_list->head;lnode!=NULL;lnode=lnode->next){
            if(lnode==query_list->tail){
                query_list->tail=previous;
                previous->next=NULL;
                query_destroy(lnode->query);
                free(lnode);
                break;
            }
            previous=lnode;

        }
    }
    else{
        while(curr!=node){
            prev=curr;
            curr=curr->next;
        }
        prev->next=curr->next;
        query_destroy(node->query);
        free(node);
    }
    query_list->size--;
    return true;
}

bool query_list_detach(QueryList query_list,uint id){
    QueryListNode node;
    QueryListNode curr=query_list->head;
    QueryListNode prev=NULL;
    bool exists=false;
    for(QueryListNode lnode=query_list->head;lnode!=NULL;lnode=lnode->next){
        if(lnode->query->query_id==id){
            node=lnode;
            exists=true;
            break;
        }
    }
    if(exists==false){
        return false;
    }
    if(query_list->head==node){
        query_list->head=query_list->head->next;
        query_destroy(node->query);
        free(node);
    }else
    if(query_list->tail==node){
        QueryListNode previous=NULL;
        for(QueryListNode lnode=query_list->head;lnode!=NULL;lnode=lnode->next){
            if(lnode==query_list->tail){
                query_list->tail=previous;
                previous->next=NULL;
                query_destroy(lnode->query);
                free(lnode);
                break;
            }
            previous=lnode;

        }
    }
    else{
        while(curr!=node){
            prev=curr;
            curr=curr->next;
        }
        prev->next=curr->next;
        query_destroy(node->query);
        free(node);
    }
    query_list->size--;
    return true;
}

Query query_list_find(QueryList query_list,uint id){
    for(QueryListNode node=query_list->head;node!=NULL;node=node->next){
        if(node->query->query_id==id){
            return node->query;
        }
    }
    return NULL;
}
