#include"../../include/acutest.h"
#include"../../include/bkt.h"
#include"../../include/entry.h"
#include"../../include/map.h"
#include"../../include/methods.h"

void test_hamming(void) {
    int dist = HammingDistance("abc", "ab");
    TEST_ASSERT(dist == 1);

    // and now for something harder
    dist = HammingDistance("dberos97", "dbezros97");
    TEST_ASSERT(dist == 6);
}

void test_edit(void) {
    int dist = EditDistance("abc", "ab");
    TEST_ASSERT(dist == 1);

    // and now for something harder
    dist = EditDistance("dberos97", "dbezros97");
    TEST_ASSERT(dist == 1);
}

void test_build_entry_index(void) {
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

void test_create_index_node(void) {
    index_node_ptr testNode = create_index_node("dberos97");
    TEST_ASSERT(!strcmp(testNode->word,"dberos97"));
    TEST_ASSERT(testNode->parent_distance == 0);
    TEST_ASSERT(testNode->children_number == 0);
    TEST_ASSERT(testNode->children == NULL);

    destroy_index_nodes(testNode);
}


TEST_LIST = {
    {"HammingDistance", test_hamming},
    {"EditDistance", test_edit},
    {"index_bkt_create", test_build_entry_index},
    {"create_index_node", test_create_index_node},
    {NULL,NULL}
};