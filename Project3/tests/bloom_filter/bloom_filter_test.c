#include"../../include/bloom_filter.h"
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
    BloomFilter bloom=bloom_create();
    TEST_ASSERT(bloom->size==0);
    TEST_ASSERT(bloom->bytes==BLOOMBYTES);
    TEST_ASSERT(bloom->bits==BLOOMBITS);
    TEST_ASSERT(bloom->k==BLOOMK);
    TEST_ASSERT(bloom->hash_function==hash_i);
    TEST_ASSERT(bloom->array!=NULL);
    for(uint i=0;i<bloom->bytes;i++){
        TEST_ASSERT(bloom->array[i]==0);
    }
    bloom_destroy(bloom);
}

// Trivial test_insert
void test_insert(void){
    BloomFilter bloom=bloom_create();
    for(int i=0;i<10000;i++){
        int size=bloom->size;
        bloom_insert(bloom,&i);
        TEST_ASSERT(bloom->size==size+1);
    }
    TEST_ASSERT(bloom->size==10000);
    bloom_destroy(bloom);

    BloomFilter bloom1=bloom_create();
    String* strings=malloc(sizeof(*strings)*10000);
    for(int i=0;i<10000;i++){
        strings[i]=create_random_string();
        int size=bloom1->size;
        bloom_insert(bloom1,strings[i]);
        TEST_ASSERT(bloom1->size==size+1);
        free(strings[i]);
    }
    TEST_ASSERT(bloom1->size==10000);
    bloom_destroy(bloom1);
    free(strings);
}

void test_check(void){
    BloomFilter bloom=bloom_create();
    for(int i=0;i<10000;i++){
        if(i%2==0){
            bloom_insert(bloom,&i);
        }
    }
    for(int i=0;i<10000;i++){
        if(i%2!=0){
            // Big numbers can return false positive(size of Bloom Filter is 100000)
            bool exists=bloom_check(bloom,&i);
            TEST_ASSERT(exists==false);
        }
        // else{
        //     // checked with 1e7 bytes and 8e7 bits and works but pc will explode when doing testdriver
        // }
    }
    bloom_destroy(bloom);

    // Smaller Bloom Filter
    BloomFilter bloom1=bloom_create();
    for(int i=0;i<100;i++){
        if(i%2==0){
            bloom_insert(bloom1,&i);
        }
    }
    for(int i=0;i<100;i++){
        if(i%2==0){
            bool exists=bloom_check(bloom1,&i);
            TEST_ASSERT(exists==true);
        }
        else{
            bool exists=bloom_check(bloom1,&i);
            TEST_ASSERT(exists==false);
        }
    }
    bloom_destroy(bloom1);

    // Can't really test with random strings because they can reappear and some can be inserted at !mod2
    String strings[4]={"this","is","a","test"};
    BloomFilter bloom2=bloom_create();
    for(int i=0;i<4;i++){
        bloom_insert(bloom2,strings[i]);
    }
    for(int i=0;i<4;i++){
        bool exists=bloom_check(bloom2,strings[i]);
        TEST_ASSERT(exists==true);
    }
    bloom_destroy(bloom2);
}


TEST_LIST = {
    {"bloom_filter_create",test_create},
    {"bloom_filter_insert",test_insert},
    {"bloom_filter_check",test_check},
    {NULL,NULL}
};