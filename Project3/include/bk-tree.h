#pragma once

#include"common_types.h"
#include"vector.h"
#include"core.h"
#include"bkt.h"

struct bkt_node{
    String word;
    Vector* vector;
};

struct bkt{
    BktNode root;
    MatchType match_type;
    DistanceFunction distance;
};

// Creating a BK-Tree Node
BktNode bkt_node_create();

// Destroying a BK-Tree Node
void bkt_node_destroy(BktNode node);

// Creating a BK-Tree
Bkt bkt_create(MatchType match_type);

// Destroying a BK-Tree
void bkt_destroy(Bkt bkt);

// Destroying a BK-Tree
void bkt_destroy(Bkt bkt);

// Set the word of the root
void bkt_set_root(Bkt bkt,String word);

// Insert a word at a BK-Tree
void bkt_insert(Bkt bkt,String word);

// Find the similar words of given word given the threshold
Vector bkt_find(Bkt bkt,String word,uint threshold);
