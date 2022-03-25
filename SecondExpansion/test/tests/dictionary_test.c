#include"../../include/dictionary.h"
#include"../../include/acutest.h"
#include"../../include/job_scheduler.h"

JobScheduler job_scheduler=NULL;
// Dictionary needed mutex

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
    Dictionary dictionary=dictionary_create();
    TEST_ASSERT(dictionary->size==0);
    TEST_ASSERT(dictionary->capacity==53);
    TEST_ASSERT(dictionary->hash_function==hash_string);
    TEST_ASSERT(dictionary->array!=NULL);
    for(int i=0;i<dictionary->capacity;i++){
        TEST_ASSERT(dictionary->array[i].entry_list!=NULL);
    }
    dictionary_destroy(dictionary);
}

void test_insert(void){
    srand(time(0));
    job_scheduler=job_scheduler_create(1,1,1);
    Dictionary dictionary=dictionary_create();
    int* ids=malloc(sizeof(*ids)*10000);
    for(int i=0;i<10000;i++){
        ids[i]=rand()%10000;
        String string=create_random_string();
        int size=dictionary->size;   
        bool new_word=dictionary_insert(dictionary,string,ids[i]);     
        if(new_word==true){
            TEST_ASSERT(dictionary->size==size+1);
        }
        else{
            TEST_ASSERT(dictionary->size==size);
        }
        free(string);
    }
    dictionary_destroy(dictionary);
    free(ids);
    job_scheduler_destroy(job_scheduler);
}

void test_find(void){
    srand(time(0));
    job_scheduler=job_scheduler_create(1,1,1);
    Dictionary dictionary=dictionary_create();
    int* ids=malloc(sizeof(*ids)*10000);
    String* strings=malloc(sizeof(*strings)*10000);
    for(int i=0;i<10000;i++){
        ids[i]=rand()%10000;
        strings[i]=create_random_string();
        dictionary_insert(dictionary,strings[i],ids[i]);
    }
    for(int i=0;i<10000;i++){
        Entry entry=dictionary_find(dictionary,strings[i]);
        TEST_ASSERT(entry!=NULL);
        TEST_ASSERT(strcmp(entry->word,strings[i])==0);
        TEST_ASSERT(entry->payload->size!=0);
    }
    for(int i=0;i<10000;i++){
        free(strings[i]);
    }
    free(strings);
    free(ids);
    dictionary_destroy(dictionary);
    job_scheduler_destroy(job_scheduler);
}

void test_remove(void){
    job_scheduler=job_scheduler_create(1,1,1);
    Dictionary dictionary=dictionary_create();
    String* strings=malloc(sizeof(*strings)*10000);
    for(int i=0;i<10000;i++){
        strings[i]=create_random_string();
        dictionary_insert(dictionary,strings[i],i);
        remove_entry_from_list(dictionary,strings[i]);
        bool exists=dictionary_find(dictionary,strings[i]);
        TEST_ASSERT(exists==false);
        free(strings[i]);
    }
    free(strings);
    dictionary_destroy(dictionary);
    job_scheduler_destroy(job_scheduler);
}

TEST_LIST = {
    {"dictionary_create",test_create},
    {"dictionary_insert",test_insert},
    {"dictionary_find",test_find},
    {"dictionary_remove",test_remove},
    {NULL,NULL}
};