#include"../../include/bst.h"

BstNode bst_node_create(String string){
    // Allocate memory for Node
    BstNode node=malloc(sizeof(*node));
    // Create an entry as value of the node
    node->entry=entry_create(string);
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
    // Destroy the entry
    entry_destroy(node->entry);
    bst_node_destroy(node->right);
    free(node);
}

BstNode bst_node_insert(BstNode node,String string,bool* inserted){
    if(node==NULL){
        *inserted=true;
        return bst_node_create(string);
    }
    // Recursively find where to insert
    if(strcmp(string,node->entry->word)<0){
        node->left=bst_node_insert(node->left,string,inserted);
    }
    else if(strcmp(string,node->entry->word)>0){
        node->right=bst_node_insert(node->right,string,inserted);
    }
    return node;
}

void bst_node_inorder(BstNode node){
    if(node!=NULL){
        bst_node_inorder(node->left);
        printf("WORD: %s \n",node->entry->word);
        bst_node_inorder(node->right);
    }
}

void new_from_old_bst_node(Bst new_bst,BstNode new_bst_node,Bst old_bst,BstNode old_bst_node){
    if(old_bst_node!=NULL){
        bool inserted=true;
        // Inorder insert
        new_from_old_bst_node(new_bst,new_bst_node,old_bst,old_bst_node->left);
        new_bst->root=bst_node_insert(new_bst->root,old_bst_node->entry->word,&inserted);
        new_from_old_bst_node(new_bst,new_bst_node,old_bst,old_bst_node->right);
        new_bst->size=old_bst->size;
    }
}

Bst bst_create(){
    // Allocate memory for the Binary Search Tree
    Bst bst=malloc(sizeof(*bst));
    // Set root NULL
    bst->root=NULL;
    // Set starting size to 0
    bst->size=0;
    // Return the Binary Search Tree
    return bst;
}

void bst_destroy(Bst bst){
    bst_node_destroy(bst->root);
    free(bst);
}

bool bst_insert(Bst bst,String string){
    // Whether the word can be inserted on the Bst
    bool inserted=false;
    // Call node insert just to make things easier
    bst->root=bst_node_insert(bst->root,string,&inserted);
    // Check if it got inserted
    if(inserted==true){
        // To increase the size if so
        bst->size++;
    }
    return inserted;
}

void bst_inorder(Bst bst){
    bst_node_inorder(bst->root);
}

void new_from_old_bst(Bst new_bst,Bst old_bst){
    new_from_old_bst_node(new_bst,new_bst->root,old_bst,old_bst->root);
}

void bst_node_insert_at_map(BstNode node,Map map){
    if(node!=NULL){
        bst_node_insert_at_map(node->left,map);
        map_insert(map,node->entry->word);
        bst_node_insert_at_map(node->right,map);
    }
}

void bst_insert_at_map(Bst bst,Map map){
    bst_node_insert_at_map(bst->root,map);
}
