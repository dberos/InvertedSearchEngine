#pragma once

#include"common_types.h"
#include"entry.h"
#include"map.h"
#include"document.h"

struct bst_node{
    String word;
    BstNode left;
    BstNode right;
};

struct bst{
    BstNode root;
    int size;
};

// Creating a Binary Search Tree Node
BstNode bst_node_create(String string);

// Destroying a Binary Search Tree Node
void bst_node_destroy(BstNode node);

// Insert a Node recursively
BstNode bst_node_insert(BstNode node,String string,bool* inserted);

// Inorder traversal, in order to print words
void bst_node_inorder(BstNode node);

// Insert at a new Binary Search Tree everything from on old one
void new_from_old_bst_node(Bst new_bst,BstNode new_bst_node,Bst old_bst,BstNode old_bst_node);

// Creating a Binary Search Tree
Bst bst_create();

// Destroying a Binary Search Tree
void bst_destroy(Bst bst);

// Calling bst_node_insert at the root to make things easier
bool bst_insert(Bst bst,String string);

// Calling bst_node_inorder at the root
void bst_inorder(Bst bst);

// Call new_from_old_bst_node at the root
void new_from_old_bst(Bst new_bst,Bst old_bst);

// Helpful function for map_rehash
void bst_node_insert_at_map(BstNode node,Map map);

// Call node_insert_at_map at the root
void bst_insert_at_map(Bst bst,Map map);

// Find node with given string
BstNode bst_find_node(BstNode node,String string);

// Call find_node at the root
bool bst_find(Bst bst,String string);

// Find min node
BstNode bst_find_min_node(BstNode node);

// Remove node with given string
BstNode bst_remove_node(BstNode node,String string,bool* removed);

// Call node_remove
bool bst_remove(Bst bst,String string);

// Matching a Document
void bst_node_match(BstNode node,Document document);

// Calling node_match at the root
void bst_match(Bst bst,Document document);
