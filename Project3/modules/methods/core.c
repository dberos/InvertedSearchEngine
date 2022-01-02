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

		//insert in the Match_Type map
		if(match_type==MT_EXACT_MATCH) dictionary_insert(core->exact_queries, word,query_id); 
		else if(match_type==MT_EDIT_DIST) dictionary_insert(core->edit_queries, word,query_id);
		else dictionary_insert(core->hamming_queries, word,query_id);

		//add the word we jst tokenized in the query's info
		addWord_to_query(query, word);

		word=strtok(NULL," ");
	}

	free(str);
	
	
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
    
	query_map_remove(core->query_map,query_id);
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode MatchDocument(DocID doc_id, const char* doc_str){
	// printf("DOCUMENT %u\n", doc_id);
	core->document = map_create();
	DocumentPtr doc = addDocument(core, doc_id);

	//deduplicate text
	String str=strdup((String)doc_str);
	dedup(str, core);
	free(str);

	//build indexes (exact match index is a map  that is already filled and up to date :) - core->document (MAP)
	
	//Create edit distance BK-Tree 
    Index_ptr edit_tree = malloc(sizeof(*edit_tree));
    build_entry_index(core->document->entry_list, MT_EDIT_DIST, edit_tree);

	//Create hamming distance BK-Trees [ one for each possible word length]
	//created an array of index trees hamming_array[num] is for the bk tree of words with num letters
	Index_ptr* hamming_array = malloc(28*sizeof(*hamming_array));
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
}

///////////////////////////////////////////////////////////////////////////////////////////////
int comparator (const void * p1, const void * p2)
{
  return (*(int*)p1 - *(int*)p2);
}

void merge(QueryID queryId[], int left, int mid, int right) {
    int lsubLen = mid - left + 1;
    int rsubLen = right - mid;
  
	/* Create sub arrays where:
		leftSub = queryId[0, mid] 
		rightSub = queryId[mid + 1, right] */
    int leftSub[lsubLen], rightSub[rsubLen];
    for (int i = 0; i < lsubLen; i++) leftSub[i] = queryId[left + i];
    for (int j = 0; j < rsubLen; j++) rightSub[j] = queryId[mid + 1 + j];
  
	// Merge array back until we reach the end of at least one array
    int i = 0;
    int j = 0;
    int k = left;
    while (i < lsubLen && j < rsubLen) {
        if (leftSub[i] <= rightSub[j]) {
            queryId[k] = leftSub[i];
            i++;
        }
        else {
            queryId[k] = rightSub[j];
            j++;
        }
        k++;
    }
  
	// Finally if one of the two arrays has remaining
	// queries add them to the original
    while (i < lsubLen) queryId[k++] = leftSub[i++];
    while (j < rsubLen) queryId[k++] = rightSub[j++];
}
  
// Mergesort breaks each given array into 2 parts (in the middle)
// sorting the children, and then "merging" the sorted arrays back up
void mergeSort(QueryID queryId[], int left, int right) {
    if (left < right) {
        int m = left + (right - left) / 2;
  
        mergeSort(queryId, left, m);
        mergeSort(queryId, m + 1, right);
  
        merge(queryId, left, m, right);
    }
}

ErrorCode GetNextAvailRes(DocID* p_doc_id, unsigned int* p_num_res, QueryID** p_query_ids){
	*p_doc_id=0; *p_num_res=0; *p_query_ids=0;

	if(core->document_number==0) return EC_NO_AVAIL_RES;

	*p_doc_id = core->docs[core->last_result_index]->doc_id;
	*p_num_res = core->docs[core->last_result_index]->num_res;
	mergeSort(core->docs[core->last_result_index]->query_ids, 0, core->docs[core->last_result_index]->num_res-1);
	*p_query_ids = core->docs[core->last_result_index]->query_ids;

	core->last_result_index++;
	
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////
