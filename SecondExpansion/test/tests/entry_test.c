#include"../../include/acutest.h"
#include"../../include/entry.h"

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
    String word=create_random_string();
    Entry entry=create_entry(word);
    TEST_ASSERT(entry!=NULL);
    TEST_ASSERT(entry->next==NULL);
    TEST_ASSERT(entry->payload!=NULL);
    list_destroy(entry->payload);
    free(entry->word);
    free(entry);
    free(word);
}

void test_insert(void){
    EntryList entry_list=create_entry_list();
    String* strings=malloc(sizeof(*strings)*10000);
    for(int i=0;i<10000;i++){
        strings[i]=create_random_string();
        int size=entry_list->size;
        Entry entry=create_entry(strings[i]);
        add_entry(entry_list,entry);
        TEST_ASSERT(entry_list->size==size+1);
    }
    for(int i=0;i<10000;i++){
        free(strings[i]);
    }
    free(strings);
    destroy_entry_list(entry_list);
}

void test_iterate(void){
    EntryList entry_list=create_entry_list();
    String* strings=malloc(sizeof(*strings)*10000);
    for(int i=0;i<10000;i++){
        strings[i]=create_random_string();
        Entry entry=create_entry(strings[i]);
        add_entry(entry_list,entry);
    }
    int count=0;
    for(Entry entry=entry_list->head;entry!=NULL;entry=entry->next){
        TEST_ASSERT(strcmp(entry->word,strings[count])==0);
        count++;
    }
    for(int i=0;i<10000;i++){
        free(strings[i]);
    }
    free(strings);
    destroy_entry_list(entry_list);
}

TEST_LIST = {
    {"entry_create",test_create},
    {"entry_insert",test_insert},
    {"entry_iterate",test_iterate},
    {NULL,NULL}
};