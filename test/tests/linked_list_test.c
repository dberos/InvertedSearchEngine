#include"../../include/linked_list.h"
#include"../../include/acutest.h"
#include"../../include/time.h"

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
    LinkedList list=linked_list_create();
    TEST_ASSERT(list->size==0);
    TEST_ASSERT(list->head==NULL);
    linked_list_destroy(list);
}

void test_insert(void){
    LinkedList list=linked_list_create();
    String* strings=malloc(sizeof(*strings)*10000);
    for(int i=0;i<10000;i++){
        strings[i]=create_random_string();
        int size=list->size;
        bool inserted=linked_list_insert_end(list,strings[i]);
        if(inserted==true){
            TEST_ASSERT(list->size==size+1);
        }
        else{
            TEST_ASSERT(list->size==size);
        }
    }
    TEST_ASSERT(strcmp(list->head->string,strings[0])==0);
    for(int i=0;i<10000;i++){
        free(strings[i]);
    }
    free(strings);
    linked_list_destroy(list);
}

void test_iterate(void){
    LinkedList list=linked_list_create();
    String* strings=malloc(sizeof(*strings)*10000);
    String* strings_inserted=malloc(sizeof(*strings_inserted)*10000);
    int c=0;
    for(int i=0;i<10000;i++){
        strings[i]=create_random_string();
        bool inserted=linked_list_insert_end(list,strings[i]);
        if(inserted==true){
            strings_inserted[c]=strings[i];
            c++;
        }
    }
    int i=0;
    for(LinkedListNode node=list->head;node!=NULL;node=node->next){
        TEST_ASSERT(strcmp(node->string,strings_inserted[i])==0);
        i++;
    }
    for(int i=0;i<10000;i++){
        free(strings[i]);
    }
    free(strings);
    free(strings_inserted);
    linked_list_destroy(list);
}

TEST_LIST = {
    {"linked_list_create",test_create},
    {"linked_list_insert",test_insert},
    {"linked_list_iterate",test_iterate},
    {NULL,NULL}
};
