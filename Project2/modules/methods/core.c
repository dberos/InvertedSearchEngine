/*
 * core.cpp version 1.0
 * Copyright (c) 2013 KAUST - InfoCloud Group (All Rights Reserved)
 * Author: Amin Allam
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include"../../include/common_types.h"
#include"../../include/core.h"
#include"../../include/methods.h"
#include"../include/bkt.h"


Core core=NULL;

///////////////////////////////////////////////////////////////////////////////////////////////

// Computes edit distance between a null-terminated string "a" with length "na"
//  and a null-terminated string "b" with length "nb" 
// int EditDistance(char* a, int na, char* b, int nb)
// {
// 	return EC_SUCCESS;
// }

///////////////////////////////////////////////////////////////////////////////////////////////

// Computes Hamming distance between a null-terminated string "a" with length "na"
//  and a null-terminated string "b" with length "nb" 
// unsigned int HammingDistance(char* a, int na, char* b, int nb)
// {
// 	return EC_SUCCESS;
// }

// ///////////////////////////////////////////////////////////////////////////////////////////////

// // Keeps all information related to an active query
// struct Query
// {
// 	QueryID query_id;
// 	char str[MAX_QUERY_LENGTH];
// 	MatchType match_type;
// 	unsigned int match_dist;
// };

///////////////////////////////////////////////////////////////////////////////////////////////

// Keeps all query ID results associated with a dcoument
// struct Document
// {
// 	DocID doc_id;
// 	unsigned int num_res;
// 	QueryID* query_ids;
// };


///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode InitializeIndex(){
	core=core_create();
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode DestroyIndex(){
	core_destroy(core);
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode StartQuery(QueryID query_id, const char* query_str, MatchType match_type, unsigned int match_dist){

	// Create a Query
    Query query=query_create(query_id, match_type,match_dist);



	//Tokenize query words and add them to the Match_Type map or update the payload (if the word already exists in that map)
    String str=strdup(query_str);
	
	String word=strtok(str," ");
	while(word!=NULL){
		remove_special_characters_decapitalize(word);

		//check methods.h for the core struct

		//insert in the Match_Type map
		if(match_type==MT_EXACT_MATCH) dictionary_insert(core->exact_queries, word,query_id); 
		else if(match_type==MT_EDIT_DIST) dictionary_insert(core->edit_queries, word,query_id);
		else dictionary_insert(core->hamming_queries, word,query_id);

		//add the word we jst tokenized in the query's info
		addWord_to_query(query, word);

		word=strtok(NULL," ");
	}

	free(str);
	

	// Need cast to String to not disqualify const expression
	query_map_insert(core->query_map, query);

	// Insert this query in core->th_box in the right shell ( check methods.h in core struct for explanation )
	if(core->th_boxes[match_dist]->size==0){
		query_list_insert_tail(core->th_boxes[match_dist], query);
	}else{
		query_list_insert_tail(core->th_boxes[match_dist], query);
	}	

	return EC_SUCCESS;

}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode EndQuery(QueryID query_id){
	// Find Query from active query set
	Query query=query_map_find(core->query_map,query_id);
	if(query==NULL){
		return EC_FAIL;
	}
	// Find MATCH TYPE to remove from the correct dictionary
	// from each Query's words's payload, only this query_id
	if(query->match_type==MT_EXACT_MATCH){
		dictionary_remove(core->exact_queries,core->query_map,query_id);
	}
	else if(query->match_type==MT_EDIT_DIST){
		dictionary_remove(core->edit_queries,core->query_map,query_id);
	}
	else{
		dictionary_remove(core->hamming_queries,core->query_map,query_id);
	}

	// Remove this query from core->th_box[] ( check methods.h in core struct for explanation )
	query_list_detach(core->th_boxes[query->match_dist], query_id);
    
	query_map_remove(core->query_map,query_id);
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode MatchDocument(DocID doc_id, const char* doc_str){
	printf("DOCUMENT %u\n", doc_id);
	core->document = map_create();
	DocumentPtr doc = addDocument(core, doc_id);

	//deduplicate text
	dedup(doc_str, core);

	//build indexes (exact match index is a map  that is already filled and up to date :)
	
	//Create edit distance BK-Tree 
    Index_ptr edit_tree = (Index_ptr)malloc(sizeof(bkindex));
    build_entry_index_from_dictionary(core->edit_queries, MT_EDIT_DIST, edit_tree);

	//Create hamming distance BK-Trees [ one for each possible word length]
	//created an array of index trees hamming_array[num] is for the bk tree of words with num letters
	Index_ptr* hamming_array = (Index_ptr*)malloc(28*sizeof(Index_ptr));
	fill_hamming_ix_array(hamming_array, core->hamming_queries, MT_HAMMING_DIST);
	



	//For every word of this -deduplicated- document
    for(Entry entry=core->document->entry_list->head;entry!=NULL;entry=entry->next){
		// printf("----------------------------------\Word %s\n--------------------------------------\n", entry->word);
        
		//we will lookup this word for every possible threshold ( th_box of core struct ) check methods.h for definition & explanation
		for (int threshold=0 ; threshold<4 ; threshold++){
			EntryList exact_result = create_entry_list();
			EntryList edit_result = create_entry_list();
			EntryList hamming_result = create_entry_list();

			//do the lookups for every matcing type
			lookup_entry_index(entry->word, edit_tree, threshold, edit_result, MT_EDIT_DIST);
			lookup_entry_index(entry->word, hamming_array[strlen(entry->word)-4], threshold, hamming_result, MT_HAMMING_DIST);
			Entry exact_entry = dictionary_find(core->exact_queries, entry->word);
			if(exact_entry!=NULL){ //If there was such word in exact dictionary, there can be only one by the way
				add_entry(exact_result, create_entry_with_payload(exact_entry->word, exact_entry->payload));
			}


			//For every query having this threshold
			for(QueryListNode th_query=core->th_boxes[threshold]->head ; th_query!=NULL ; th_query=th_query->next){

				QueryID qid = th_query->query->query_id;
				
				//if we have already matched this query, just move on to the next one
				if(th_query->query->lock==true){
					continue;
				}
				//now depending on the match type of this query, check the results of this index
				EntryList results;
				if(th_query->query->match_type == MT_EXACT_MATCH)	results = exact_result;
				else if(th_query->query->match_type == MT_EDIT_DIST)	results = edit_result;
				else results = hamming_result;
				 
				//for every word in our look up results
				for(Entry resultNode=results->head; resultNode!=NULL ; resultNode=resultNode->next){
					//check if this result word has th_query's id in its payload
					if( check_list_existence(resultNode->payload, qid)==false ){
						continue;
					}else{
						// (check modules/methods/matchquery.c)
						matchQuery(th_query->query, resultNode->word, doc);
					}

				}

			}



			//destroy results from lookups for this document word and threshold move on to the next one

			destroy_entry_list(exact_result);
			destroy_entry_list(edit_result);
			destroy_entry_list(hamming_result);

		}



    }

	
	//Now that we are done with this document, we should clear all matched_words info from all queries so we do nto affect the next document
	clear_matchedInfo(core);

	destroy_entry_index(edit_tree);
	destroy_hamming_array(hamming_array);

	map_destroy(core->document);
	
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////
int comparator (const void * p1, const void * p2)
{
  return (*(int*)p1 - *(int*)p2);
}


ErrorCode GetNextAvailRes(DocID* p_doc_id, unsigned int* p_num_res, QueryID** p_query_ids){
	printf("GETNEXTAVAILABLE____________________________________________\n");
	*p_doc_id=0; *p_num_res=0; *p_query_ids=0;

	if(core->document_number==0) return EC_NO_AVAIL_RES;

	*p_doc_id = core->docs[core->last_result_index]->doc_id;
	*p_num_res = core->docs[core->last_result_index]->num_res;
	qsort(core->docs[core->last_result_index]->query_ids, core->docs[core->last_result_index]->num_res, sizeof(uint), comparator);

	*p_query_ids = core->docs[core->last_result_index]->query_ids;

	core->last_result_index++;
	printf("PHRA NEXT__________________________________________________\n");
	
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////
