#include"../../include/document.h"
#include"../../include/methods.h"
#include"../../include/list.h"

extern JobScheduler job_scheduler;

Document document_create(DocID doc_id,String doc_str){
    // Allocating memory for the Document
    Document document=malloc(sizeof(*document));

    // Set Document's ID
    document->doc_id=doc_id;
    // Set Document's whole String
    document->doc_str=strdup(doc_str);

    // Creating the Map to store the Document
    document->map=map_create();
    // Creating the Edit Distance BK-Tree
    document->edit_tree=bkt_create(MT_EDIT_DIST);
    // Allocating memory for the Hamming Distance BK-Trees
	document->hamming_trees=calloc(28,sizeof(*document->hamming_trees));
    // Creating an Open Addressing Hash Table to match Document's queries
    document->hash_table=hash_table_create();
	// Creating the Priority Queue
	document->pqueue=pqueue_create(MIN);

    // Return the Document
    return document;
}

void document_destroy(Document document){
    // Free the whole String
    free(document->doc_str);
    // Destroying the Map
    map_destroy(document->map);
    // Destroying the Edit Distance BK-Tree
    bkt_destroy(document->edit_tree);
    // Destroying the Hamming Distance BK-Trees
    for(int i=0;i<28;i++){
		if(document->hamming_trees[i]!=NULL){
			bkt_destroy(document->hamming_trees[i]);
		}
    }
    free(document->hamming_trees);
    // Destroying the Hash Table
    hash_table_destroy(document->hash_table);
	// Destroying the Priority Queue
	pqueue_destroy(document->pqueue);
    // Free the document
    free(document);
}

void document_deduplication(Document document){
	// Get each word
	String token;
	// Get whole document string
	String res=document->doc_str;
	// Thread safe strtok_r
	while((token=strtok_r(res," ",&res))){
		map_insert(document->map,token);
	}
}

void document_create_edit(Core core,Document document){
	// From all available MT_EDIT_DIST Entries
	for(int i=0;i<core->edit_queries->capacity;i++){
		// Get each Node
		DictionaryNode node=&core->edit_queries->array[i];
		// Check whether Node's list isn't empty
		if(node->entry_list->size>0){
			// For all Entries of the list
			for(Entry entry=node->entry_list->head;entry!=NULL;entry=entry->next){
				if(document->edit_tree->root->entry==NULL){
					// Set the root
					bkt_set_root(document->edit_tree,entry);
				}
				else{
					// Or insert, if root exists
					bkt_insert(document->edit_tree,entry);
				}
			}
		}
	}
}

void document_create_hamming(Core core,Document document){
    // From all available MT_HAMMING_DIST Entries
	for(int i=0;i<core->hamming_queries->capacity;i++){
		// Get each Node
		DictionaryNode node=&core->hamming_queries->array[i];
		// Check whether node's list isn't empty
		if(node->entry_list->size>0){
			// From all Entries of the list
			for(Entry entry=node->entry_list->head;entry!=NULL;entry=entry->next){
				// Creating the BK-Tree if it doesn't exist
				if(document->hamming_trees[strlen(entry->word)]==NULL){
					document->hamming_trees[strlen(entry->word)]=bkt_create(MT_HAMMING_DIST);
				}
				if(document->hamming_trees[strlen(entry->word)]->root->entry==NULL){
					// Set the root of the correct BK-Tree
					bkt_set_root(document->hamming_trees[strlen(entry->word)],entry);
				}
				else{
					// Or insert, if root exists
					bkt_insert(document->hamming_trees[strlen(entry->word)],entry);
				}
			}
		}
	}
}

void document_match(Core core,Document document){
	for(int i=0;i<document->map->capacity;i++){
		// Get the Node's Bst
		Bst bst=document->map->array[i].bst;
		if(bst->size>0){
			bst_match(bst,document);
		}
	}
}
