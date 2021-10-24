#pragma once

#include"common_types.h"
#include"word.h"

struct bst_node{
    Word word;
    BstNode left;
    BstNode right;
};

struct bst{
    BstNode root;
};

// Creating a Binary Search Tree Node
BstNode bst_node_create(String string);

// Destroying a Binary Search Tree Node
void bst_node_destroy(BstNode node);

// Insert a Node recursively
BstNode bst_node_insert(BstNode node,String string);

// Inorder traversal, in order to print words
void bst_node_inorder(BstNode node);

// Insert at a new Binary Search Tree everything from on old one
void new_from_old_bst_node(Bst new_bst,BstNode new_bst_node,Bst old_bst,BstNode old_bst_node);

// Creating a Binary Search Tree
Bst bst_create();

// Destroying a Binary Search Tree
void bst_destroy(Bst bst);

// Calling bst_node_insert at the root to make things easier
void bst_insert(Bst bst,String string);

// Calling bst_node_inorder at the root
void bst_inorder(Bst bst);

// Call new_from_old_bst_node at the root
void new_from_old_bst(Bst new_bst,Bst old_bst);