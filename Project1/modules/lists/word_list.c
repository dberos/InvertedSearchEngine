#include"../../include/word_list.h"

WordList word_list_create(){
    // Allocate memory for the List
    WordList list=malloc(sizeof(*list));
    // Set head to NULL
    list->head=NULL;
    // Set size to 0
    list->size=0;
    // Return the List
    return list;
}

void word_list_destroy(WordList list){
    WordListNode prev;
    WordListNode curr;
    curr=list->head;
    while(curr!=NULL){
        prev=curr;
        curr=curr->next;
        word_destroy(prev->word);
        free(prev);
        list->size--;
    }
    free(list);
}

void word_list_insert(WordList list,String word){
    WordListNode node=malloc(sizeof(*node));
    node->word=word_create(word);
    node->next=list->head;
    list->head=node;
    list->size++;
}

bool word_list_insert_end(WordList list,String word){
    WordListNode node=malloc(sizeof(*node));
    WordListNode curr;
    node->word=word_create(word);
    // First value found would get inserted twice
    if(list->head!=NULL && strcmp(list->head->word->word,word)==0){
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
            if(strcmp(curr->next->word->word,word)!=0){
                curr=curr->next;
            }
            else{
                word_destroy(node->word);
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
