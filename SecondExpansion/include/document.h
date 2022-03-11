#pragma once

#include"common_types.h"
#include"methods.h"
#include"core.h"
#include"map.h"
#include"bk-tree.h"
#include"hash_table.h"


///////////////////////////////////////////////////////////////////////////////////////////////

// Keeps all query ID results associated with a document
struct Document{
	DocID doc_id;
	String doc_str;
	Map map;
	Bkt edit_tree;
	Bkt* hamming_trees;
	HashTable hash_table;
	QueryID query_ids[2000];
	int num_res;
};


///////////////////////////////////////////////////////////////////////////////////////////////

// Creating a Document
Document document_create(DocID doc_id,String doc_str);

// Destroying a Document
void document_destroy(Document document);

// Deduplicating a Document
void document_deduplication(Document document);

// Creating the Edit Distance BK-Tree of the Document
void document_create_edit(Core core,Document document);

// Creating the Hamming Distance BK-Trees of the Document
void document_create_hamming(Core core,Document document);

// Matching a Document
void document_match(Core core,Document document);
