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
    srand(time(0));
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

void test_remove(void){
    srand(time(0));
    QueryMap map=query_map_create();
    Query* queries=malloc(sizeof(*queries)*10000);
    String* strings=malloc(sizeof(*strings)*10000);
    for(int i=0;i<10000;i++){
        strings[i]=create_random_string();
        queries[i]=query_create(i,0,0);
        addWord_to_query(queries[i],strings[i]);
        query_map_insert(map,queries[i]);
    }
    for(int i=0;i<10000;i++){
        int size=map->size;
        query_map_remove(map,i);
        TEST_ASSERT(map->size==size-1);
        Query query=query_map_find(map,i);
        TEST_ASSERT(query==NULL);
    }
    for(int i=0;i<10000;i++){
        free(strings[i]);
    }
    free(strings);
    free(queries);
    query_map_destroy(map);
}

void test_find(void){
    srand(time(0));
    QueryMap map=query_map_create();
    Query* queries=malloc(sizeof(*queries)*10000);
    // Queries will be destroyed on rehash so i need another to check 
    Query* queries2=malloc(sizeof(*queries)*10000);
    String* strings=malloc(sizeof(*strings)*10000);
    for(int i=0;i<10000;i++){
        strings[i]=create_random_string();
        queries[i]=query_create(i,0,0); 
        queries2[i]=query_create(i,0,0); 
        addWord_to_query(queries[i],strings[i]);
        addWord_to_query(queries2[i],strings[i]);
        query_map_insert(map,queries[i]);
    }
    for(int i=0;i<10000;i++){
        Query query=query_map_find(map,i);
        TEST_ASSERT(query!=NULL);
        TEST_ASSERT(strcmp(queries2[i]->words[0],query->words[0])==0);
        TEST_ASSERT(query->match_dist==queries2[i]->match_dist);
        TEST_ASSERT(query->match_type==queries2[i]->match_type);
        TEST_ASSERT(query->matched_words_num==queries2[i]->matched_words_num);
        TEST_ASSERT(query->query_id==queries2[i]->query_id);
        TEST_ASSERT(query->query_words_num==queries2[i]->query_words_num);
    }
    for(int i=0;i<10000;i++){
        free(strings[i]);
        query_destroy(queries2[i]);
    }
    free(strings);
    free(queries);
    free(queries2);
    query_map_destroy(map);
    
}


TEST_LIST = {
    {"query_map_create",test_create},
    {"query_map_insert",test_insert},
    {"query_map_remove",test_remove},
    {"query_map_find",test_find},
    {NULL,NULL}
};