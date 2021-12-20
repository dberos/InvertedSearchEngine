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
	
	
	Query query_copy = query_create(query->query_id,query->match_type, query->match_dist);
	passWords_to_query(query_copy, query);
	// Insert this query in core->th_box in the right shell ( check methods.h in core struct for explanation )
	if(core->th_boxes[match_dist]->size==0){
		query_list_insert_tail(core->th_boxes[match_dist], query_copy);
	}else{
		query_list_insert_tail(core->th_boxes[match_dist], query_copy);
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
	// query_list_detach(core->th_boxes[query->match_dist], query_id);
    
	query_map_remove(core->query_map,query_id);
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode MatchDocument(DocID doc_id, const char* doc_str){
	// printf("DOCUMENT %u\n", doc_id);
	core->document = map_create();
	DocumentPtr doc = addDocument(core, doc_id);

	//deduplicate text
	dedup(doc_str, core);

	//build indexes (exact match index is a map  that is already filled and up to date :) - core->document (MAP)
	
	//Create edit distance BK-Tree 
    Index_ptr edit_tree = (Index_ptr)malloc(sizeof(bkindex));
    build_entry_index(core->document->entry_list, MT_EDIT_DIST, edit_tree);

	//Create hamming distance BK-Trees [ one for each possible word length]
	//created an array of index trees hamming_array[num] is for the bk tree of words with num letters
	Index_ptr* hamming_array = (Index_ptr*)malloc(28*sizeof(Index_ptr));
	fill_hamming_ix_array(hamming_array, core->document->entry_list, MT_HAMMING_DIST);
	

	//for every possible threshold (match_dist)
	for(int threshold=0 ; threshold<4 ; threshold++){
		
		//for every word of the edit dictionary
		for(int i=0 ; i<core->edit_queries->capacity ; i++){
            for(Entry edit_entry=core->edit_queries->array[i].entry_list->head;edit_entry!=NULL;edit_entry=edit_entry->next){
				
				EntryList results = create_entry_list();

				lookup_entry_index(edit_entry->word, edit_tree, threshold, results, MT_EDIT_DIST);
				//so now we have the results | the words of the document that matched with this threshold
				if(results->size!=0){
					
					// if it matched with at least one word
					//then we should add this word to the matched words of a query if:
					//  1) a query belongs to the th_box of the current threshold ) th_box[threshold]
					//  2) has match type edit
					//  3) belongs to the payload of the word wwe just checked
					
					//so, for every query of this threshold
					for(QueryListNode th_query=core->th_boxes[threshold]->head ; th_query!=NULL ; th_query=th_query->next){
						//if it has edit distance as MatchType and belongs to the payload of the word we just checked
						if( (th_query->query->match_type==MT_EDIT_DIST) && check_list_existence(edit_entry->payload, th_query->query->query_id) ){
							// (check modules/methods/matchquery.c)
							matchQuery(th_query->query, edit_entry->word, doc);
						}

					}
				
				}

				destroy_entry_list(results);

			}
    	} 


		//Now the same for every word of the hammign dictionary
		//for every word of the hamming dictionary
		for(int i=0 ; i<core->hamming_queries->capacity ; i++){
            for(Entry hamming_entry=core->hamming_queries->array[i].entry_list->head;hamming_entry!=NULL;hamming_entry=hamming_entry->next){
				
				EntryList results = create_entry_list();

				lookup_entry_index(hamming_entry->word, hamming_array[strlen(hamming_entry->word)-4], threshold, results, MT_HAMMING_DIST);
				//so now we have the results | the words of the document that matched with this threshold
				if(results->size!=0){
					// if it matched with at least one word
					//then we should add this word to the matched words of a query if:
					//  1) a query belongs to the th_box of the current threshold ) th_box[threshold]
					//  2) has match type hamming
					//  3) belongs to the payload of the word wwe just checked
					
					//so, for every query of this threshold
					for(QueryListNode th_query=core->th_boxes[threshold]->head ; th_query!=NULL ; th_query=th_query->next){

						//if it has edit distance as MatchType and belongs to the payload of the word we just checked
						if( (th_query->query->match_type==MT_HAMMING_DIST) && check_list_existence(hamming_entry->payload, th_query->query->query_id) ){
							
							// (check modules/methods/matchquery.c)
							matchQuery(th_query->query, hamming_entry->word, doc);
						}

					}
				
				}

				destroy_entry_list(results);

			}
    	} 

		//And last but not least, take care of the exact queries
		//which appear only on threshold==0
		if(threshold==0){
			for(int i=0 ; i<core->exact_queries->capacity ; i++){
				for(Entry exact_entry=core->exact_queries->array[i].entry_list->head;exact_entry!=NULL;exact_entry=exact_entry->next){
					
					EntryList results = create_entry_list();

					bool result = map_find(core->document, exact_entry->word);
					if(result!=false){ //If there was such word in exact dictionary, there can be only one by the way
						add_entry(results, create_entry("exactresult"));
					}
					//so now we have the results | the words of the document that matched with this threshold
					if(results->size!=0){
						// if it matched with at least one word
						//then we should add this word to the matched words of a query if:
						//  1) a query belongs to the th_box of the current threshold ) th_box[threshold]
						//  2) has match type exact
						//  3) belongs to the payload of the word wwe just checked
						
						//so, for every query of this threshold
						for(QueryListNode th_query=core->th_boxes[0]->head ; th_query!=NULL ; th_query=th_query->next){

							//if it has edit distance as MatchType and belongs to the payload of the word we just checked
							if( (th_query->query->match_type==MT_EXACT_MATCH) && check_list_existence(exact_entry->payload, th_query->query->query_id) ){
								// (check modules/methods/matchquery.c)
								matchQuery(th_query->query, exact_entry->word, doc);
							}

						}
					
					}

					destroy_entry_list(results);

				}
			}
		}











	}

	// //Now that we are done with this document, we should clear all matched_words info from all queries so we do nto affect the next document
	clear_matchedInfo(core);

	destroy_entry_index(edit_tree);
	destroy_hamming_array(hamming_array);

	map_destroy(core->document);

	return EC_SUCCESS;



//--------------------------------WAY 2--------------------------------------------

	//for every active query
    // for(int i=0;i<core->query_map->capacity;i++){

	// 	for(QueryListNode lnode=core->query_map->array[i].query_list->head; lnode!=NULL ; lnode=lnode->next){


	// 		bool matched_query = true;

	// 		//for every query word
	// 		for(int j=0 ; j<lnode->query->query_words_num  ; j++){

	// 			EntryList results = create_entry_list();

	// 			if(lnode->query->match_type==MT_EDIT_DIST){
	// 				lookup_entry_index(lnode->query->words[j], edit_tree, lnode->query->match_dist, results, MT_EDIT_DIST);
	// 			}else if(lnode->query->match_type==MT_HAMMING_DIST){
	// 				lookup_entry_index(lnode->query->words[j], hamming_array[ strlen(lnode->query->words[j])-4 ], lnode->query->match_dist, results, MT_HAMMING_DIST);
	// 			}else{
	// 				bool result = map_find(core->document, lnode->query->words[j]);
	// 				if(result!=false){ //If there was such word in exact dictionary, there can be only one by the way
	// 					add_entry(results, create_entry("exactresult"));
	// 				}

	// 			}

	// 			//if for a query word there was no result:
	// 			if(results->size==0){
	// 				matched_query = false;
	// 			} 

	// 			destroy_entry_list(results);
	// 		}

	// 		//if matched query is still true, then all query words had a match
	// 		if(matched_query==true){
    // 			addDocumentResult(doc, lnode->query->query_id);
	// 		}
			

	// 	}
	// }

	
	// //Now that we are done with this document, we should clear all matched_words info from all queries so we do nto affect the next document
	// //clear_matchedInfo(core);

	// destroy_entry_index(edit_tree);
	// destroy_hamming_array(hamming_array);

	// map_destroy(core->document);

	// return EC_SUCCESS;

//--------------------------------WAY 3--------------------------------------------


	// //For every word of this -deduplicated- document
    // for(Entry entry=core->document->entry_list->head;entry!=NULL;entry=entry->next){
	// 	// printf("----------------------------------\Word %s\n--------------------------------------\n", entry->word);
        
	// 	//we will lookup this word for every possible threshold ( th_box of core struct ) check methods.h for definition & explanation
	// 	for (int threshold=0 ; threshold<4 ; threshold++){
	// 		EntryList exact_result = create_entry_list();
	// 		EntryList edit_result = create_entry_list();
	// 		EntryList hamming_result = create_entry_list();

	// 		//do the lookups for every matcing type
	// 		lookup_entry_index(entry->word, edit_tree, threshold, edit_result, MT_EDIT_DIST);
	// 		lookup_entry_index(entry->word, hamming_array[strlen(entry->word)-4], threshold, hamming_result, MT_HAMMING_DIST);
	// 		Entry exact_entry = dictionary_find(core->exact_queries, entry->word);
	// 		if(exact_entry!=NULL){ //If there was such word in exact dictionary, there can be only one by the way
	// 			add_entry(exact_result, create_entry_with_payload(exact_entry->word, exact_entry->payload));
	// 		}


	// 		//For every query having this threshold
	// 		for(QueryListNode th_query=core->th_boxes[threshold]->head ; th_query!=NULL ; th_query=th_query->next){

	// 			QueryID qid = th_query->query->query_id;
				
	// 			//if we have already matched this query, just move on to the next one
	// 			if(th_query->query->lock==true){
	// 				continue;
	// 			}
	// 			//now depending on the match type of this query, check the results of this index
	// 			EntryList results;
	// 			if(th_query->query->match_type == MT_EXACT_MATCH)	results = exact_result;
	// 			else if(th_query->query->match_type == MT_EDIT_DIST)	results = edit_result;
	// 			else results = hamming_result;
				 
	// 			//for every word in our look up results
	// 			for(Entry resultNode=results->head; resultNode!=NULL ; resultNode=resultNode->next){
	// 				//check if this result word has th_query's id in its payload
	// 				if( check_list_existence(resultNode->payload, qid)==false ){
	// 					continue;
	// 				}else{
	// 					// (check modules/methods/matchquery.c)
	// 					matchQuery(th_query->query, resultNode->word, doc);
	// 				}

	// 			}

	// 		}



	// 		//destroy results from lookups for this document word and threshold move on to the next one

	// 		destroy_entry_list(exact_result);
	// 		destroy_entry_list(edit_result);
	// 		destroy_entry_list(hamming_result);

	// 	}



    // }

	
	// //Now that we are done with this document, we should clear all matched_words info from all queries so we do nto affect the next document
	// clear_matchedInfo(core);

	// destroy_entry_index(edit_tree);
	// destroy_hamming_array(hamming_array);

	// map_destroy(core->document);
	
	// return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////
int comparator (const void * p1, const void * p2)
{
  return (*(int*)p1 - *(int*)p2);
}

void merge(QueryID arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
  
    /* create temp arrays */
    int L[n1], R[n2];
  
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
  
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
  
    /* Copy the remaining elements of L[], if there
    are any */
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
  
    /* Copy the remaining elements of R[], if there
    are any */
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}
  
/* l is for left index and r is right index of the
sub-array of arr to be sorted */
void mergeSort(QueryID arr[], int l, int r)
{
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;
  
        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
  
        merge(arr, l, m, r);
    }
}

ErrorCode GetNextAvailRes(DocID* p_doc_id, unsigned int* p_num_res, QueryID** p_query_ids){
	// printf("GETNEXTAVAILABLE____________________________________________\n");
	*p_doc_id=0; *p_num_res=0; *p_query_ids=0;

	if(core->document_number==0) return EC_NO_AVAIL_RES;

	*p_doc_id = core->docs[core->last_result_index]->doc_id;
	*p_num_res = core->docs[core->last_result_index]->num_res;
	// qsort(core->docs[core->last_result_index]->query_ids, core->docs[core->last_result_index]->num_res, sizeof(uint), comparator);
	mergeSort(core->docs[core->last_result_index]->query_ids, 0, core->docs[core->last_result_index]->num_res-1);
	*p_query_ids = core->docs[core->last_result_index]->query_ids;

	core->last_result_index++;
	// printf("GOT NEXT__________________________________________________\n");
	
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////
