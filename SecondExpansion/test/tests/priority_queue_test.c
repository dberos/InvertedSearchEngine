#include"../../include/priority_queue.h"
#include"../../include/acutest.h"

void test_create(void){
    PriorityQueue max=pqueue_create(MAX);
    TEST_ASSERT(max->priority==MAX);
    TEST_ASSERT(max->array!=NULL);
    TEST_ASSERT(max->size==0);
    TEST_ASSERT(max->capacity==10);
    pqueue_destroy(max);
}

void test_insert(void){
    PriorityQueue max=pqueue_create(MAX);
    for(int i=0;i<10000;i++){
        int size=max->size;
        pqueue_insert(max,i);
        TEST_ASSERT(max->size==size+1);
    }
    pqueue_destroy(max);
}

void test_find(void){
    PriorityQueue max=pqueue_create(MAX);
    for(int i=0;i<10000;i++){
        pqueue_insert(max,i);
    }
    for(int i=9999;i<=0;i++){
        uint id=pqueue_find(max);
        TEST_ASSERT(id==i);
        pqueue_remove(max);
    }
    pqueue_destroy(max);

    PriorityQueue min=pqueue_create(MIN);
    for(int i=0;i<10000;i++){
        pqueue_insert(min,i);
    }
    for(int i=0;i<10000;i++){
        uint id=pqueue_find(min);
        TEST_ASSERT(id==i);
        pqueue_remove(min);
    }
    pqueue_destroy(min);
}

TEST_LIST = {
    {"pqueue_create",test_create},
    {"pqueue_insert",test_insert},
    {"pqueue_find",test_find},
    {NULL,NULL}
};
