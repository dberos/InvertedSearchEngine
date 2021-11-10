#include"../../include/acutest.h"
#include"../../include/bst.h"
#include"../../include/map.h"

void test_create(void){
    Bst bst=bst_create();
    TEST_ASSERT(bst!=NULL);
    TEST_ASSERT(bst->root==NULL);
    TEST_ASSERT(bst->size==0);
    bst_destroy(bst);
}

void test_insert(void){
    Bst bst=bst_create();

    String strings[]={"at","at","the","the","or","or"};

    for(int i=0;i<6;i++){
        int size=bst->size;
        bst_insert(bst,strings[i]);
        if(i%2==0){
            TEST_ASSERT(bst->size==size+1);
        }
        else{
            TEST_ASSERT(bst->size==size);
        }
    }
    bst_destroy(bst);
}

TEST_LIST = {
    {"bst_create",test_create},
    {"bst_insert",test_insert},
    {NULL,NULL}
};
