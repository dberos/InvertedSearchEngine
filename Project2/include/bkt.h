#pragma once

#include"common_types.h"
#include"entry.h"
#include"word_list.h"
#include"dictionary.h"




typedef enum{
    hamming,edit
} Metric;

typedef struct index_node{

    String word;
    List payload;
    int parent_distance;

    int children_number;
    struct index_node** children;

}index_node;

typedef index_node* index_node_ptr;

typedef struct bkindex{
    index_node_ptr root;
}bkindex;


typedef bkindex* Index_ptr;


//Distance Metrics & tools
unsigned int minOfTwo(int x, int y);
unsigned int minOfThree(int x, int y, int z);
unsigned int HammingDistance(String str1, String str2);
unsigned int EditDistance(String str1, String str2);

index_node_ptr create_index_node(const String word, List payload);
void build_entry_index(const EntryList el, MatchType type, Index_ptr ix);
void build_entry_index_from_dictionary(Dictionary dictionary, MatchType type, Index_ptr ix);
void fill_hamming_ix_array(Index_ptr* array, Dictionary dictionary, MatchType type);


void add_index_node(index_node_ptr parent, index_node_ptr newnode, MatchType type);
void destroy_index_nodes(index_node_ptr node);
void destroy_entry_index(Index_ptr ix);
void destroy_hamming_array(Index_ptr* array);
void lookup_entry_index(const String w, Index_ptr ix, int threshold, EntryList result, MatchType type);
void recursive_search(const String w, index_node_ptr node, int threshold, EntryList result, MatchType type);
