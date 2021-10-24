#include"../../include/bst.h"

BstNode bst_node_create(String string){
    // Allocate memory for Node
    BstNode node=malloc(sizeof(*node));
    // Create a word as value of the Node 
    node->word=word_create(string[0],string);
    // Set left NULL
    node->left=NULL;
    // Set right NULL
    node->right=NULL;
    // Return the Node
    return node;
}

void bst_node_destroy(BstNode node){
    if(node==NULL){
        return;
    }
    // Inorder destroy
    bst_node_destroy(node->left);
    // Destroy the word
    word_destroy(node->word);
    bst_node_destroy(node->right);
    free(node);
}

BstNode bst_node_insert(BstNode node,String string){
    if(node==NULL){
        return bst_node_create(string);
    }
    // Recursively find where to insert
    if(strcmp(string,node->word->word)<0){
        node->left=bst_node_insert(node->left,string);
    }
    else if(strcmp(string,node->word->word)>0){
        node->right=bst_node_insert(node->right,string);
    }
    return node;
}

void bst_node_inorder(BstNode node){
    if(node!=NULL){
        bst_node_inorder(node->left);
        printf("WORD: %s \n",node->word->word);
        bst_node_inorder(node->right);
    }
}

void new_from_old_bst_node(Bst new_bst,BstNode new_bst_node,Bst old_bst,BstNode old_bst_node){
    if(old_bst_node!=NULL){
        // Inorder insert
        new_from_old_bst_node(new_bst,new_bst_node,old_bst,old_bst_node->left);
        new_bst->root=bst_node_insert(new_bst->root,old_bst_node->word->word);
        new_from_old_bst_node(new_bst,new_bst_node,old_bst,old_bst_node->right);
    }
}

Bst bst_create(){
    // Allocate memory for the Binary Search Tree
    Bst bst=malloc(sizeof(*bst));
    // Set root NULL
    bst->root=NULL;
    // Return the Binary Search Tree
    return bst;
}

void bst_destroy(Bst bst){
    bst_node_destroy(bst->root);
    free(bst);
}

void bst_insert(Bst bst,String string){
    // Call node insert just to make things easier
    bst->root=bst_node_insert(bst->root,string);
}

void bst_inorder(Bst bst){
    bst_node_inorder(bst->root);
}

void new_from_old_bst(Bst new_bst,Bst old_bst){
    new_from_old_bst_node(new_bst,new_bst->root,old_bst,old_bst->root);
}

