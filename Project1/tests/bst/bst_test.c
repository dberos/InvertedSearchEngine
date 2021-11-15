#include"../../include/acutest.h"
#include"../../include/bst.h"
#include"../../include/map.h"
#include"../../include/word_list.h"

static String create_random_string(){
    // Create alphabet
    char chars[]="abcdefghijklmnopqrstuvwxyz";
    // Lenght of alphabet
    int len=sizeof(chars)-1;
    // Find a random lenght for the string
    int random_string_len=rand()%len+1;
    // Allocate memory for the string
    String string=malloc(random_string_len+1);
    for(int i=0;i<random_string_len;i++){
        // Find random letter
        int random=rand()%len;
        // Insert at position i the random letter
        string[i]=chars[random];
    }
    // NULL terminated
    string[random_string_len]='\0';
    // Return the string
    return string;
}

void test_create(void){
    // Create a Bst
    Bst bst=bst_create();
    // TEST_ASSERT everything from bst_create
    TEST_ASSERT(bst!=NULL);
    TEST_ASSERT(bst->root==NULL);
    TEST_ASSERT(bst->size==0);
    // Destroy the Bst
    bst_destroy(bst);
}

void test_insert(void){
    srand(time(0));
    Bst bst=bst_create();
    for(int i=0;i<10000;i++){
        // Create a random string
        String string=create_random_string();
        // Bst size before insertion
        int size=bst->size;
        bool inserted=bst_insert(bst,string);
        if(inserted==true){
            TEST_ASSERT(bst->size==size+1);
        }
        else{
            TEST_ASSERT(bst->size==size);
        }
        free(string);
    }
    bst_destroy(bst);
}

void test_find(void){
    srand(time(0));
    Bst bst=bst_create();
    for(int i=0;i<10000;i++){
        // Create a random string
        String string=create_random_string();
        // Bst size before insertion
        int size=bst->size;
        bool inserted=bst_insert(bst,string);
        if(inserted==true){
            bool exists=bst_find(bst,string);
            TEST_ASSERT(exists==true);
        }
        else{
            bool exists=bst_find(bst,string);
            // Some strings may already be present
            // So they can't be inserted but still exist on the Bst 
            if(exists==true){
                TEST_ASSERT(bst->size==size);
            }
            else{
                TEST_ASSERT(bst->size==size+1);
            }

        }
        free(string);
    }
    bst_destroy(bst);
}

void test_remove(void){
    srand(time(0));
    Bst bst=bst_create();
    WordList list=dictionary_word_list_create();
    for(int i=0;i<10000;i++){
        String string=create_random_string();
        word_list_insert_end(list,string);
        bst_insert(bst,string);
        free(string);
    }
    for(WordListNode node=list->head;node!=NULL;node=node->next){
        bool removed=bst_remove(bst,node->word->word);
        TEST_ASSERT(removed==true);
    }
    bst_destroy(bst);
    dictionary_word_list_destroy(list);
}

TEST_LIST = {
    {"bst_create",test_create},
    {"bst_insert",test_insert},
    {"bst_find",test_find},
    {"bst_remove",test_remove},
    {NULL,NULL}
};
