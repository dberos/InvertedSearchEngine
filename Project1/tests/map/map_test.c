#include"../../include/acutest.h"
#include"../../include/bst.h"
#include"../../include/map.h"
#include"../../include/methods.h"

void test_create(void){
    Map map=map_create();
    TEST_ASSERT(map!=NULL);
    TEST_ASSERT(map->array!=NULL);
    TEST_ASSERT(map->capacity==53);
    TEST_ASSERT(map->hash_function==hash_string);
    TEST_ASSERT(map->size==0);
    TEST_ASSERT(map->entry_list!=NULL);
    map_destroy(map);
}

void test_insert(void){
    Map map=map_create();

    FILE* doc=fopen("../../data/deduplication.txt","r");
    if(deduplication(doc,map)==1){
        map_destroy(map);
    }

    fclose(doc);
    int size=map->size;
    FILE* doc1=fopen("../../data/deduplication.txt","r");
    if(deduplication(doc1,map)==1){
        map_destroy(map);
    }

    fclose(doc1);

    TEST_ASSERT(map->size==size);
    map_destroy(map);
}


TEST_LIST = {
    {"map_create",test_create},
    {"map_insert",test_insert},
    {NULL,NULL}
};