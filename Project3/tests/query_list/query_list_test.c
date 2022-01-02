#include"../../include/query_list.h"
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
    QueryList list=query_list_create();
    TEST_ASSERT(list->size==0);
    TEST_ASSERT(list->head==NULL);
    TEST_ASSERT(list->tail==NULL);
    query_list_destroy(list);
}

void test_insert(void){
    srand(time(0));
    QueryList list=query_list_create();
    Query* queries=malloc(sizeof(*queries)*10000);
    String* strings=malloc(sizeof(*strings)*10000);
    for(int i=0;i<10000;i++){
        queries[i]=query_create(i,0,0);
        strings[i]=create_random_string();
        addWord_to_query(queries[i],strings[i]);
        int size=list->size;
        query_list_insert_head(list,queries[i]);
        TEST_ASSERT(list->size==size+1);
        TEST_ASSERT(list->head->query==queries[i]);
    }
    int count=9999;
    for(QueryListNode node=list->head;node!=NULL;node=node->next){
        TEST_ASSERT(node->query==queries[count]);
        count--;
    }
    query_list_destroy(list);
    for(int i=0;i<10000;i++){
        free(strings[i]);
    }
    free(queries);
    free(strings);

    QueryList list2=query_list_create();
    Query* queries2=malloc(sizeof(*queries2)*10000);
    String* strings2=malloc(sizeof(*strings2)*10000);
    for(int i=0;i<10000;i++){
        queries2[i]=query_create(i,0,0);
        strings2[i]=create_random_string();
        addWord_to_query(queries2[i],strings2[i]);
        int size2=list2->size;
        query_list_insert_tail(list2,queries2[i]);
        TEST_ASSERT(list2->size==size2+1);
        TEST_ASSERT(list2->tail->query==queries2[i]);
    }
    int count2=0;
    for(QueryListNode node=list2->head;node!=NULL;node=node->next){
        TEST_ASSERT(node->query==queries2[count2]);
        count2++;
    }
    query_list_destroy(list2);
    for(int i=0;i<10000;i++){
        free(strings2[i]);
    }
    free(queries2);
    free(strings2);
}

void test_find(void){
    QueryList list=query_list_create();
    Query* queries=malloc(sizeof(*queries)*10000);
    String* strings=malloc(sizeof(*strings)*10000);
    for(int i=0;i<10000;i++){
        strings[i]=create_random_string();
        queries[i]=query_create(i,0,0);
        addWord_to_query(queries[i],strings[i]);
        query_list_insert_tail(list,queries[i]);
    }
    for(int i=0;i<10000;i++){
        Query query=query_list_find(list,i);
        TEST_ASSERT(query!=NULL);
        TEST_ASSERT(strcmp(query->words[0],strings[i])==0);
    }
    for(int i=0;i<10000;i++){
        free(strings[i]);
    }
    free(queries);
    free(strings);
    query_list_destroy(list);
}

void test_remove(void){
    QueryList list=query_list_create();
    Query* queries=malloc(sizeof(*queries)*10000);
    String* strings=malloc(sizeof(*strings)*10000);
    for(int i=0;i<10000;i++){
        strings[i]=create_random_string();
        queries[i]=query_create(i,0,0);
        addWord_to_query(queries[i],strings[i]);
        query_list_insert_tail(list,queries[i]);
    }
    for(int i=0;i<10000;i++){
        int size=list->size;
        bool removed=query_list_remove(list,i);
        TEST_ASSERT(removed==true);
        TEST_ASSERT(list->size==size-1);
    }
    for(int i=0;i<10000;i++){
        free(strings[i]);
    }
    free(queries);
    free(strings);
    query_list_destroy(list);
}

void test_iterate(void){
    srand(time(0));
    QueryList list=query_list_create();
    Query* queries=malloc(sizeof(*queries)*10000);
    String* strings=malloc(sizeof(*strings)*10000);
    for(int i=0;i<10000;i++){
        strings[i]=create_random_string();
        queries[i]=query_create(i,0,0);
        addWord_to_query(queries[i],strings[i]);
        query_list_insert_tail(list,queries[i]);
    }
    TEST_ASSERT(list->tail->query==queries[9999]);
    TEST_ASSERT(list->head->query==queries[0]);

    int i=0;
    for(QueryListNode node=list->head;node!=NULL;node=node->next){
        TEST_ASSERT(node->query==queries[i]);
        i++;
    }
    for(int i=0;i<10000;i++){
        free(strings[i]);
    }
    free(queries);
    free(strings);
    query_list_destroy(list);
}

TEST_LIST = {
    {"query_list_create",test_create},
    {"query_list_insert",test_insert},
    {"query_list_find",test_find},
    {"query_list_remove",test_remove},
    {"query_list_iterate",test_iterate},
    {NULL,NULL}
};