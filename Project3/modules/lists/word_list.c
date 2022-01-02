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
        list->size++;

        return;
    }
    
    //if it's not the first word
    list->end->next = new;

    //update end
    list->end = new;
    
    //update list size
    list->size++;
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
