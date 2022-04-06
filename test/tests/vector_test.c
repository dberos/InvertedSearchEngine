#include"../../include/vector.h"
#include"../../include/entry.h"
#include"../../include/acutest.h"

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
    Vector vector=vector_create();
    TEST_ASSERT(vector->size==0);
    TEST_ASSERT(vector->capacity==10);
    TEST_ASSERT(vector->array!=NULL);
    vector_destroy(vector);
}

void test_insert(void){
    Vector vector=vector_create();
    String* strings=malloc(sizeof(*strings)*10000);
    for(int i=0;i<10000;i++){
        strings[i]=create_random_string();
        Entry entry=create_entry(strings[i]);
        int size=vector->size;
        vector_push_back(vector,entry);
        TEST_ASSERT(vector->size==size+1);
    }
    for(int i=0;i<10000;i++){
        VectorNode node=vector_at(vector,i);
        Entry entry=node->entry;
        list_destroy(entry->payload);
        free(entry->word);
        free(entry);
        free(strings[i]);
    }
    free(strings);
    vector_destroy(vector);
}

void test_find(void){
    Vector vector=vector_create();
    String* strings=malloc(sizeof(*strings)*10000);
    for(int i=0;i<10000;i++){
        strings[i]=create_random_string();
        Entry entry=create_entry(strings[i]);
        vector_push_back(vector,entry);
    }
    for(int i=0;i<10000;i++){
        VectorNode node=vector_at(vector,i);
        TEST_ASSERT(strcmp(node->entry->word,strings[i])==0);
    }
    for(int i=0;i<10000;i++){
        VectorNode node=vector_at(vector,i);
        Entry entry=node->entry;
        list_destroy(entry->payload);
        free(entry->word);
        free(entry);
        free(strings[i]);
    }
    free(strings);
    vector_destroy(vector);
}

TEST_LIST = {
    {"vector_create",test_create},
    {"vector_insert",test_insert},
    {"vector_find",test_find},
    {NULL,NULL}
};