#include"../../include/bst.h"

extern Core core;

BstNode bst_node_create(String string){
    // Allocate memory for Node
    BstNode node=malloc(sizeof(*node));
    // Create the value of the node
    node->word=string;
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
    bst_node_destroy(node->right);
    free(node);
}

BstNode bst_node_insert(BstNode node,String string,bool* inserted){
    if(node==NULL){
        *inserted=true;
        return bst_node_create(string);
    }
    // Recursively find where to insert
    if(strcmp(string,node->word)<0){
        node->left=bst_node_insert(node->left,string,inserted);
    }
    else if(strcmp(string,node->word)>0){
        node->right=bst_node_insert(node->right,string,inserted);
    }
    return node;
}

void bst_node_inorder(BstNode node){
    if(node!=NULL){
        bst_node_inorder(node->left);
        printf("WORD: %s \n",node->word);
        bst_node_inorder(node->right);
    }
}

void new_from_old_bst_node(Bst new_bst,BstNode new_bst_node,Bst old_bst,BstNode old_bst_node){
    if(old_bst_node!=NULL){
        bool inserted=true;
        // Inorder insert
        new_from_old_bst_node(new_bst,new_bst_node,old_bst,old_bst_node->left);
        new_bst->root=bst_node_insert(new_bst->root,old_bst_node->word,&inserted);
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
        map_insert(map,node->word);
        bst_node_insert_at_map(node->right,map);
    }
}

void bst_insert_at_map(Bst bst,Map map){
    bst_node_insert_at_map(bst->root,map);
}

BstNode bst_find_node(BstNode node,String string){
    if(node==NULL){
        return NULL;
    }
    if(strcmp(string,node->word)==0){
        return node;
    }
    else if(strcmp(string,node->word)<0){
        return bst_find_node(node->left,string);
    }
    else{
        return bst_find_node(node->right,string);
    }
}

bool bst_find(Bst bst,String string){
    BstNode node=bst_find_node(bst->root,string);
    if(node!=NULL){
        return true;
    }
    return false;
}

BstNode bst_find_min_node(BstNode node){
    BstNode temp=node;
    while(temp->left!=NULL){
        temp=temp->left;
    }
    if(temp!=NULL){
        return temp;
    }
    return NULL;
}

BstNode bst_remove_node(BstNode node,String string,bool* removed){
    if(node==NULL){
        return NULL;
    }
    // Recursively find node
    if(strcmp(string,node->word)<0){
        node->left=bst_remove_node(node->left,string,removed);
        *removed=true;
        return node;
    }
    else if(strcmp(string,node->word)>0){
        node->right=bst_remove_node(node->right,string,removed);
        *removed=true;
        return node;
    }
    // If left doesn't exist
    if(node->left==NULL){
        BstNode right=node->right;
        free(node);
        *removed=true;
        return right;
    }
    // If right doesn't exist
    else if(node->right==NULL){
        BstNode left=node->left;
        free(node);
        *removed=true;
        return left;
    }
    // Both exist
    else{
        // Root is the given root
        BstNode root=node;
        // And call temp its right
        BstNode temp=node->right;
        // Fix pointers while left exists
        while(temp->left!=NULL){
            root=temp;
            temp=temp->left;
        }
        if(root!=node){
            root->left=temp->right;
        }
        else{
            root->right=temp->right;
        }
        // Set new one since tree is fixed
        node->word=temp->word;
        free(temp);
        *removed=true;
    }
    return node;
}

bool bst_remove(Bst bst,String string){
    bool removed=false;
    bst->root=bst_remove_node(bst->root,string,&removed);
    if(removed==true){
        bst->size--;
    }
    return removed;
}

void bst_node_match(BstNode node,Document document){
    if(node!=NULL){
        bst_node_match(node->left,document);
        String word=node->word;
        // Check for MT_EXACT_MATCH
		Entry entry=dictionary_find(core->exact_queries,word);
		if(entry!=NULL){
			for(ListNode node=entry->payload->head;node!=NULL;node=node->next){
				// Find the Query
				Query query=query_vector_at(core->vector,node->value);
				if(query!=NULL && query->match_type==MT_EXACT_MATCH){
					// Match it
					bool match=hash_table_insert(document->hash_table,
															query->ptr,
																word,
																	query->list->size);
					if(match==true){
						// Insert at the Priority Queue
						pqueue_insert(document->pqueue,query->query_id);
					}
				}
			}
		}
		// For every possible Match Distance
        for(int threshold=1;threshold<4;threshold++){
			// Get the results from MT_EDIT_DISTANCE
			Vector edit_result=bkt_find(document->edit_tree,
											word,
												threshold);
            if(edit_result!=NULL){
				// For all words that matched
				for(int i=0;i<edit_result->size;i++){
					// For all QUERYIDs in matched entry
					List list=edit_result->array[i].entry->payload;
					for(ListNode node=list->head;node!=NULL;node=node->next){
						// Find the Query
						Query query=query_vector_at(core->vector,node->value);

						if(query!=NULL && query->match_type==MT_EDIT_DIST && query->match_dist==threshold){
							// Match it
							bool match=hash_table_insert(document->hash_table,
															query->ptr,
																edit_result->array[i].entry->word,
																	query->list->size);
							if(match==true){
								// Insert at the Priority Queue
								pqueue_insert(document->pqueue,query->query_id);
							}
						}
					}
				}
				vector_destroy(edit_result);
			}

			if(document->hamming_trees[strlen(word)]!=NULL){
				// Get the results from MT_HAMMING_DISTANCE
				Vector hamming_result=bkt_find(document->hamming_trees[strlen(word)],
													word,
														threshold);
				if(hamming_result!=NULL){
					// For all words that matched
					for(int i=0;i<hamming_result->size;i++){
						// For all QUERYIDs in matched entry
						List list=hamming_result->array[i].entry->payload;
						for(ListNode node=list->head;node!=NULL;node=node->next){
							// Find the Query
							Query query=query_vector_at(core->vector,node->value);
							if(query!=NULL && query->match_type==MT_HAMMING_DIST && query->match_dist==threshold){
								// Match it							
								bool match=hash_table_insert(document->hash_table,
																query->ptr,
																	hamming_result->array[i].entry->word,
																		query->list->size);
								if(match==true){
									// Insert at the Priority Queue
									pqueue_insert(document->pqueue,query->query_id);
								}
							}
						}
					}
					vector_destroy(hamming_result);
				}
			}
		}
        bst_node_match(node->right,document);
    }
}

void bst_match(Bst bst,Document document){
    bst_node_match(bst->root,document);
}
