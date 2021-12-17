#include"../../include/list.h"
#include"../../include/acutest.h"
#include"../../include/time.h"

void test_create(void){
    List list=list_create();
    TEST_ASSERT(list->size==0);
    TEST_ASSERT(list->head==NULL);
    TEST_ASSERT(list->tail==NULL);
    list_destroy(list);
}

void test_insert(void){
    srand(time(0));
    List list=list_create();
    int* head=malloc(sizeof(*head)*10000);
    for(int i=0;i<10000;i++){
        // Duplicates allowed since there is no iteration to insert
        head[i]=rand()%10000;
        int size=list->size;
        list_insert_head(list,&head[i]);
        TEST_ASSERT(list->size==size+1);
        TEST_ASSERT(list->head->value==&head[i]);
    }
    
    int count=9999;
    for(ListNode node=list->head;node!=NULL;node=node->next){
        TEST_ASSERT(node->value==&head[count]);
        count--;
    }
    list_destroy(list);
    free(head);

    List list1=list_create();
    int* tail=malloc(sizeof(*tail)*10000);
    for(int i=0;i<10000;i++){
        // Duplicates allowed since there is no iteration to insert
        tail[i]=rand()%10000;
        int size=list1->size;
        list_insert_tail(list1,&tail[i]);
        TEST_ASSERT(list1->size==size+1);
        TEST_ASSERT(list1->tail->value==&tail[i]);
    }
    
    count=0;
    for(ListNode node=list1->head;node!=NULL;node=node->next){
        TEST_ASSERT(node->value==&tail[count]);
        count++;
    }
    list_destroy(list1);
    free(tail);
}

void test_find(void){
    List list=list_create();
    for(int i=0;i<10000;i++){
        list_insert_tail(list,&i);
    }
    for(int i=0;i<10000;i++){
        bool exists=check_list_existence(list,&i);
        TEST_ASSERT(exists==true);
    }
    list_destroy(list);
}

void test_remove(void){
    List list=list_create();
    for(int i=0;i<10000;i++){
        list_insert_tail(list,&i);
    }
    for(int i=0;i<10000;i++){
        int size=list->size;
        bool removed=list_remove(list,&i);
        TEST_ASSERT(removed==true);
        TEST_ASSERT(list->size==size-1);
    }
    list_destroy(list);
}

void test_iterate(void){
    srand(time(0));
    List list=list_create();
    int* values=malloc(sizeof(*values)*10000);
    for(int i=0;i<10000;i++){
        values[i]=rand()%10000;
        list_insert_tail(list,&values[i]);
    }
    TEST_ASSERT(list->tail->value==&values[9999]);
    TEST_ASSERT(list->head->value==&values[0]);

    int i=0;
    for(ListNode node=list->head;node!=NULL;node=node->next){
        TEST_ASSERT(node->value==&values[i]);
        i++;
    }
    free(values);
    list_destroy(list);
}

TEST_LIST = {
    {"list_create",test_create},
    {"list_insert",test_insert},
    {"list_find",test_find},
    {"list_remove",test_remove},
    {"list_iterate",test_iterate},
    {NULL,NULL}
};