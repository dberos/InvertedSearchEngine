#include"../../include/hash_table.h"
#include"../../include/acutest.h"
#include"../../include/time.h"

void test_create(void){
    HashTable hash_table=hash_table_create();
    TEST_ASSERT(hash_table->size==0);
    TEST_ASSERT(hash_table->capacity==53);
    TEST_ASSERT(hash_table->hash_function==hash_string);
    TEST_ASSERT(hash_table->array!=NULL);
    for(int i=0;i<hash_table->capacity;i++){
        TEST_ASSERT(hash_table->array[i].key==NULL);
        TEST_ASSERT(hash_table->array[i].value!=NULL);
    }
    hash_table_destroy(hash_table);
}

void test_insert(void){
    HashTable hash_table=hash_table_create();
    for(int i=1;i<10000;i++){
        int random=rand()%2+1;
        if(random==1){

            TEST_ASSERT(hash_table_insert(hash_table,i,"matching",1)==true);
        }
        else{
            TEST_ASSERT(hash_table_insert(hash_table,i,"matching",2)==false);
        }
    }
    hash_table_destroy(hash_table);
}

TEST_LIST = {
    {"hash_table_create",test_create},
    {"hash_table_insert",test_insert},
    {NULL,NULL}
};
