#include"../../include/query_map.h"
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
    QueryMap map=query_map_create();
    TEST_ASSERT(map->size==0);
    TEST_ASSERT(map->capacity==53);
    TEST_ASSERT(map->hash_function==hash_string);
    TEST_ASSERT(map->array!=NULL);
    for(int i=0;i<map->capacity;i++){
        TEST_ASSERT(map->array[i].query_list!=NULL);
    }
    query_map_destroy(map);
}

void test_insert(void){
    QueryMap map=query_map_create();
    Query* queries=malloc(sizeof(*queries)*10000);
    String* strings=malloc(sizeof(*strings)*10000);
    for(int i=0;i<10000;i++){
        strings[i]=create_random_string();
        queries[i]=query_create(i,0,0);
        addWord_to_query(queries[i],strings[i]);
        int size=map->size;
        query_map_insert(map,queries[i]);
        TEST_ASSERT(map->size==size+1);
    }
    for(int i=0;i<10000;i++){
        free(strings[i]);
    }
    free(strings);
    free(queries);
    query_map_destroy(map);
}


TEST_LIST = {
    {"query_map_create",test_create},
    {"query_map_insert",test_insert},
    {NULL,NULL}
};