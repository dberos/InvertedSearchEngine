#include"../../include/acutest.h"
#include"../../include/bkt.h"
#include"../../include/entry.h"
#include"../../include/map.h"
#include"../../include/methods.h"


void test_build(void){
    // Create a Bst
    Map map=map_create();

    FILE* doc=fopen("../../data/map_testing_acutest.txt","r");
    if(deduplication(doc,map)==1){
        map_destroy(map);
    }

    Index_ptr test_tree = (Index_ptr)malloc(sizeof(bkindex));


    build_entry_index(map->entry_list, hamming, test_tree);
    // TEST_ASSERT everything from bst_create
    TEST_ASSERT(test_tree->root!=NULL);

    // Destroy the Bst
    map_destroy(map);
    destroy_entry_index(test_tree);
    fclose(doc);
}



TEST_LIST = {
    {"index_bkt_create",test_build},
    {NULL,NULL}
};