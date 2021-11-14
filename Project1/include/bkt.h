#pragma once

#include"common_types.h"
#include"entry.h"
#include"word_list.h"



typedef enum{
    hamming,edit
} Metric;

typedef struct index_node{

    String word;
    int parent_distance;

    int children_number;
    struct index_node** children;

}index_node;

typedef index_node* index_node_ptr;

typedef struct index{
    index_node_ptr root;
}index;


typedef index* Index_ptr;




unsigned int minOfTwo(int x, int y);
unsigned int minOfThree(int x, int y, int z);
unsigned int HammingDistance(String str1, String str2);
unsigned int EditDistance(String str1, String str2);

index_node_ptr create_index_node(const String word);
void build_entry_index(const EntryList el, Metric type, Index_ptr ix);
void add_index_node(index_node_ptr parent, index_node_ptr newnode, Metric type);
void destroy_index_nodes(index_node_ptr node);
void destroy_entry_index(Index_ptr ix);
void lookup_entry_index(const String w, Index_ptr ix, int threshold, EntryList result, Metric type);
void recursive_search(const String w, index_node_ptr node, int threshold, EntryList result, words_list_ptr candidates, Metric type);
