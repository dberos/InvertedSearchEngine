#include"../../include/query_vector.h"
#include"../../include/acutest.h"

JobScheduler job_scheduler=NULL;

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
    QueryVector vector=query_vector_create();
    TEST_ASSERT(vector->size==0);
    TEST_ASSERT(vector->capacity==10);
    TEST_ASSERT(vector->array!=NULL);
    query_vector_destroy(vector);
}

void test_insert(void){
    job_scheduler=job_scheduler_create(1,1,1);
    QueryVector vector=query_vector_create();
    String* strings=malloc(sizeof(*strings)*10000);
    for(int i=1;i<10000;i++){
        strings[i]=create_random_string();
        int size=vector->size;
        query_vector_insert(vector,i,strings[i],MT_EXACT_MATCH,0);
        TEST_ASSERT(vector->size==size+1);
    }
    for(int i=1;i<10000;i++){
        free(strings[i]);
    }
    free(strings);
    query_vector_destroy(vector);
    job_scheduler_destroy(job_scheduler);
}

void test_remove(void){
    job_scheduler=job_scheduler_create(1,1,1);
    QueryVector vector=query_vector_create();
    String* strings=malloc(sizeof(*strings)*10000);
    for(int i=1;i<10000;i++){
        strings[i]=create_random_string();
        query_vector_insert(vector,i,strings[i],MT_EXACT_MATCH,0);
    }
    for(int i=1;i<10000;i++){
        int size=vector->size;
        query_vector_set(vector,i);
        // Size isn't decreased
        TEST_ASSERT(vector->size==size);
        // Query is destroyed
        Query query=query_vector_at(vector,i);
        TEST_ASSERT(query==NULL);
    }
    for(int i=1;i<10000;i++){
        free(strings[i]);
    }
    free(strings);
    query_vector_destroy(vector);
    job_scheduler_destroy(job_scheduler);
}

void test_iterate(void){
    job_scheduler=job_scheduler_create(1,1,1);
    QueryVector vector=query_vector_create();
    String* strings=malloc(sizeof(*strings)*10000);
    Query* queries=malloc(sizeof(*queries)*10000);
    for(int i=1;i<10000;i++){
        strings[i]=create_random_string();
        query_vector_insert(vector,i,strings[i],MT_EXACT_MATCH,0);
        queries[i]=query_create(i,MT_EXACT_MATCH,0);
        query_cleanup(queries[i],strings[i]);
    }
    for(int i=1;i<10000;i++){
        Query query=query_vector_at(vector,i);
        TEST_ASSERT(query->match_dist==queries[i]->match_dist);
        TEST_ASSERT(query->match_type==queries[i]->match_type);
        TEST_ASSERT(strcmp(query->list->head->string,queries[i]->list->head->string)==0);
    }
    for(int i=1;i<10000;i++){
        free(strings[i]);
        query_destroy(queries[i]);
    }
    free(strings);
    free(queries);
    query_vector_destroy(vector);
    job_scheduler_destroy(job_scheduler);
}

TEST_LIST = {
    {"query_vector_create",test_create},
    {"query_vector_insert",test_insert},
    {"query_vector_remove",test_remove},
    {"query_vector_iterate",test_iterate},
    {NULL,NULL}
};
