#include"../../include/acutest.h"
#include"../../include/entry.h"
#include"../../include/word_list.h"


void test_create_word_node(void) {

    word_node_ptr wnode = create_word_node("dberos97");
    TEST_ASSERT(wnode->word != NULL);
    TEST_ASSERT(wnode->next == NULL);

    destroy_word_list_node(wnode);
}

void test_create_list(void) {
    words_list_ptr wl = word_list_create();
    TEST_ASSERT(wl!=NULL);
    TEST_ASSERT(wl->head==NULL);
    TEST_ASSERT(wl->end==NULL);
    TEST_ASSERT(!wl->size);
    
    word_list_destroy(wl);
}

void test_add_word_to_list(void) {
    words_list_ptr wl = word_list_create();
    add_word_to_list("dberos97", wl);
    add_word_to_list("bderos97", wl);

    TEST_ASSERT(wl->size==2);

    word_list_destroy(wl);

    
}

void test_pop_word(void) {
    words_list_ptr wl = word_list_create();
    add_word_to_list("dberos97", wl);
    add_word_to_list("bderos97", wl);
    add_word_to_list("dbezos97", wl);

    String popstr = NULL;
    popstr = pop_word_list(wl);

    TEST_ASSERT(wl->size==2);
    TEST_ASSERT(popstr!=NULL);
    
    free(popstr);
    word_list_destroy(wl);
    

}


TEST_LIST = {
    {"create_word_node", test_create_word_node},
    {"word_list_create", test_create_list},
    {"add_word_to_list", test_add_word_to_list},
    {"pop_word_list", test_pop_word},
    {NULL, NULL}
};