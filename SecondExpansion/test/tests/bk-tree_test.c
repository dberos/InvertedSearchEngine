#include"../../include/bk-tree.h"
#include"../../include/acutest.h"
#include"../../include/job_scheduler.h"

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

// BK-Tree functions needed mutex

void test_create(void){
    job_scheduler=job_scheduler_create(1,1,1);
    Bkt bkt=bkt_create(MT_EDIT_DIST);
    TEST_ASSERT(bkt->distance==EditDistance);
    TEST_ASSERT(bkt->match_type==MT_EDIT_DIST);
    TEST_ASSERT(bkt->root!=NULL);
    TEST_ASSERT(bkt->root->vector!=NULL);
    TEST_ASSERT(bkt->root->entry==NULL);
    bkt_destroy(bkt);
    job_scheduler_destroy(job_scheduler);
}

void test_insert(void){
    job_scheduler=job_scheduler_create(1,1,1);
    Bkt bkt=bkt_create(MT_EDIT_DIST);
    EntryList entry_list=create_entry_list();
    String* strings=malloc(sizeof(*strings)*10000);
    strings[0]=create_random_string();
    Entry entry=create_entry(strings[0]);
    add_entry(entry_list,entry);
    bkt_set_root(bkt,entry);
    for(int i=1;i<10000;i++){
        strings[i]=create_random_string();
        Entry entry=create_entry(strings[i]);
        add_entry(entry_list,entry);
        int distance=bkt->distance
        (bkt->root->entry->word,
            strlen(bkt->root->entry->word),
                entry->word,
                    strlen(entry->word));

        if(bkt->root->vector[distance]==NULL){
            bkt->root->vector[distance]=vector_create();
        }
        int size=bkt->root->vector[distance]->size;
        bkt_insert(bkt,entry);
        TEST_ASSERT(bkt->root->vector[distance]->size==size+1);
    }
    for(int i=0;i<10000;i++){
        free(strings[i]);
    }
    free(strings);
    destroy_entry_list(entry_list);
    bkt_destroy(bkt);
    job_scheduler_destroy(job_scheduler);
}

void test_find(void){
    job_scheduler=job_scheduler_create(1,1,1);
    Bkt bkt=bkt_create(MT_EDIT_DIST);
    EntryList entry_list=create_entry_list();
    String strings[]={"help","fall","felt","fell","small","melt"};
    Entry entry=create_entry("hell");
    bkt_set_root(bkt,entry);
    add_entry(entry_list,entry);
    for(int i=0;i<6;i++){
        Entry entry=create_entry(strings[i]);
        add_entry(entry_list,entry);
        bkt_insert(bkt,entry);
    }

    Vector vector=bkt_find(bkt,"henn",2);
    TEST_ASSERT(strcmp(vector->array[0].entry->word,"hell")==0
        || strcmp(vector->array[1].entry->word,"hell")==0);
    TEST_ASSERT(strcmp(vector->array[1].entry->word,"help")==0
        || strcmp(vector->array[0].entry->word,"help")==0);
    
    TEST_ASSERT(vector->size==2);
    vector_destroy(vector);

    bkt_destroy(bkt);
    destroy_entry_list(entry_list);
    job_scheduler_destroy(job_scheduler);
}

TEST_LIST = {
    {"bk-tree_create",test_create},
    {"bk-tree_insert",test_insert},
    {"bk-tree_find",test_find},
    {NULL,NULL}
};