#include"../../include/acutest.h"
#include"../../include/bkt.h"
#include"../../include/entry.h"
#include"../../include/map.h"
#include"../../include/methods.h"

void test_hamming(void) {
    int dist = HammingDistance("abc", strlen("abc"), "ab", strlen("ab"));
    TEST_ASSERT(dist == 1);

    // and now for something harder
    dist = HammingDistance("dberos97", strlen("dberos97"), "dbezros97", strlen("dbezros97"));
    TEST_ASSERT(dist == 6);
}

void test_edit(void) {
    int dist = EditDistance("abc", strlen("abc"), "ab", strlen("ab"));
    TEST_ASSERT(dist == 1);

    // and now for something harder
    dist = EditDistance("dberos97", strlen("dberos97"), "dbezros97", strlen("dbezros97"));
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
    List payload=list_create();
    list_insert_tail(payload, 1);
    list_insert_tail(payload, 2);
    list_insert_tail(payload, 3);
    index_node_ptr testNode = create_index_node("dberos97", payload);
    TEST_ASSERT(!strcmp(testNode->word,"dberos97"));
    TEST_ASSERT(testNode->parent_distance == 0);
    TEST_ASSERT(testNode->children_number == 0);
    TEST_ASSERT(testNode->children == NULL);
    
    destroy_index_nodes(testNode);
    list_destroy(payload);
}


void test_lookup(void){
    Map map=map_create();


    FILE* doc=fopen("../../data/queryekf.txt","r");
    if(deduplication(doc,map)==1){
        map_destroy(map);
    }

    Index_ptr test_tree = (Index_ptr)malloc(sizeof(bkindex));
    build_entry_index(map->entry_list, hamming, test_tree);

    EntryList results=create_entry_list();

    lookup_entry_index("henn", test_tree, 2, results, MT_HAMMING_DIST);

    TEST_ASSERT(results->size==2);
    TEST_ASSERT(!strcmp(results->head->word,"hell"));
    // Entry entry=results->head->next;
    // TEST_ASSERT(strcmp(entry->word,"help"));


    map_destroy(map);
    destroy_entry_index(test_tree);
    destroy_entry_list(results);
    fclose(doc);
}

TEST_LIST = {
    // {"HammingDistance", test_hamming},
    {"EditDistance", test_edit},
    {"index_bkt_create", test_build_entry_index},
    {"create_index_node", test_create_index_node},
    {"lookup", test_lookup},
    {NULL,NULL}
};