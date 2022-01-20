#pragma once

#include"common_types.h"
#include"vector.h"
#include"core.h"
#include"bkt.h"
#include"entry.h"

struct bkt_node{
    Entry entry;
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

// Set the Entry of the root
void bkt_set_root(Bkt bkt,Entry entry);

// Insert an entry at a BK-Tree
void bkt_insert(Bkt bkt,Entry entry);

// Find the similar words of given word given the threshold
Vector bkt_find(Bkt bkt,String word,int threshold);
