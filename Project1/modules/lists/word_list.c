#include"../../include/word_list.h"
#include"../../include/common_types.h"

words_list_ptr word_list_create(){
    // Allocate memory for the List
    words_list_ptr list = (words_list_ptr)malloc(sizeof(words_list));
    // Set head to NULL
    list->head = NULL;
    // Set end to NULL
    list->end = NULL;
    // Set size to 0
    list->size = 0;
    // Return the List
    return list;
}

void destroy_word_list_node(word_node_ptr node){
    
    if(node == NULL) return;

    destroy_word_list_node(node->next);

    free(node->word);
    free(node);

}

void word_list_destroy(words_list_ptr list){
    destroy_word_list_node(list->head);
    free(list);
}


word_node_ptr create_word_node(const String word){

    word_node_ptr temp = (word_node_ptr)malloc(sizeof(word_node));

    temp->word = strdup(word);
    temp->next = NULL;

    return temp;

}

void add_word_to_list(const String word, words_list_ptr list){

    word_node_ptr new = create_word_node(word);

    //In case this is the first word
    if(list->head==NULL){
        list->head = new;
        list->end = new;
        return;
    }
    
    //if it's not the first word
    list->end->next = new;

    //update end
    list->end = new;
    
}

String pop_word_list(words_list_ptr list){

    if(list->head==NULL) return NULL;
    
    String word = strdup(list->head->word);

    word_node_ptr temp = list->head;



    if(list->size > 1){
     
        list->head = list->head->next;
    
    }else{ //if this was the last word

        list->head = NULL;
        list->end = NULL;
    } 
    
    //update wordlist size
    list->size--;

    free(temp->word);
    free(temp);

    return word;

}


/////////////////////////   Only used at Dictionary     /////////////////////////


WordList dictionary_word_list_create(){
    // Allocate memory for the List
    WordList list=malloc(sizeof(*list));
    // Set head to NULL
    list->head=NULL;
    // Set size to 0
    list->size=0;
    // Return the List
    return list;
}

void dictionary_word_list_destroy(WordList list){
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

//////////////////////////////////////////////////////////////////////////////////
