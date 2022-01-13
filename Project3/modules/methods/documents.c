#include"../../include/documents.h"
#include"../../include/methods.h"


DocumentPtr addDocument(Core core, DocID doc_id){
    
    core->docs = realloc(core->docs, (core->document_number + 1) * sizeof(*core->docs));
    
    core->docs[core->document_number] = createDoc(doc_id);
    

    return core->docs[core->document_number++];

}

DocumentPtr createDoc(DocID doc_id){
    DocumentPtr doc = malloc(sizeof(*doc));
    doc->doc_id = doc_id;
    doc->num_res = 0;

    return doc;
}


void addDocumentResult(DocumentPtr doc, QueryID id){
    memcpy(&doc->query_ids[doc->num_res],&id,sizeof(id));
    doc->num_res++;
    return;

}

void destroyDocument(DocumentPtr doc){
    free(doc);
}


void destroyDocumentsArray(DocumentPtr* array, uint doc_number){
    for(int i=0 ; i<doc_number ; i++){
        destroyDocument(array[i]);
    }

    free(array);
}

void clear_matchedInfo(Core core){
    // Start with the only Exact QueryMap
    for(int j=0;j<core->query_exact_map->capacity;j++){
        // Find the Node
        QueryMapNode node=&core->query_exact_map->array[j];
        // If its list isn't empty
        if(node->query_list->size>0){
            // Traverse it
            for(QueryListNode lnode=node->query_list->head;lnode!=NULL;lnode=lnode->next){
                // And clean matched info
                for(int m=0;m<lnode->query->matched_words_num;m++){
                    free(lnode->query->matched_words[m]);
                }
                lnode->query->matched_words_num=0;
                lnode->query->lock=false;
            }
        }
    }
    // Continue with the Edit and Hamming QueryMaps
    for(int i=0;i<4;i++){
        // Edit QueryMap[i]
        for(int j=0;j<core->query_edit_map[i]->capacity;j++){
            // Find the Node
            QueryMapNode node=&core->query_edit_map[i]->array[j];
            // If its list isn't empty
            if(node->query_list->size>0){
                // Traverse it
                for(QueryListNode lnode=node->query_list->head;lnode!=NULL;lnode=lnode->next){
                    // And clean matched info
                    for(int m=0;m<lnode->query->matched_words_num;m++){
                        free(lnode->query->matched_words[m]);
                    }
                    lnode->query->matched_words_num=0;
                    lnode->query->lock=false;
                }
            }
        }
        // Hamming QueryMap[i]
        for(int j=0;j<core->query_hamming_map[i]->capacity;j++){
            // Find the Node
            QueryMapNode node=&core->query_hamming_map[i]->array[j];
            // If its list isn't empty
            if(node->query_list->size>0){
                // Traverse it
                for(QueryListNode lnode=node->query_list->head;lnode!=NULL;lnode=lnode->next){
                    // And clean matched info
                    for(int m=0;m<lnode->query->matched_words_num;m++){
                        free(lnode->query->matched_words[m]);
                    }
                    lnode->query->matched_words_num=0;
                    lnode->query->lock=false;
                }
            }
        }
    }
}

ErrorCode SpecificMatchDocument(Core core,MatchType match_type,int threshold){
    if(match_type==MT_EDIT_DIST){
        //for every word of the edit dictionary
		for(int i=0 ; i<core->edit_queries->capacity ; i++){
            for(Entry edit_entry=core->edit_queries->array[i].entry_list->head;edit_entry!=NULL;edit_entry=edit_entry->next){
				
				EntryList results = create_entry_list();

				lookup_entry_index(edit_entry->word, core->current_edit_tree, threshold, results, MT_EDIT_DIST);
				//so now we have the results | the words of the document that matched with this threshold
				if(results->size!=0){
					
					// if it matched with at least one word
					//then we should add this word to the matched words of a query if:
					//  1) a query belongs to the th_box of the current threshold ) th_box[threshold]
					//  2) has match type edit
					//  3) belongs to the payload of the word wwe just checked
					
					for(int j=0;j<core->query_edit_map[threshold]->capacity;j++){
						QueryMapNode node=&core->query_edit_map[threshold]->array[j];
						if(node->query_list->size>0){
							for(QueryListNode lnode=node->query_list->head;lnode!=NULL;lnode=lnode->next){
								if(check_list_existence(edit_entry->payload,lnode->query->query_id)){
									matchQuery(lnode->query,edit_entry->word,core->current_doc);
								}
							}
						}
					}
				}
				destroy_entry_list(results);
			}
    	}
        return EC_SUCCESS; 
    }
    else if(match_type==MT_HAMMING_DIST){
        //Now the same for every word of the hammign dictionary
		//for every word of the hamming dictionary
		for(int i=0 ; i<core->hamming_queries->capacity ; i++){
            for(Entry hamming_entry=core->hamming_queries->array[i].entry_list->head;hamming_entry!=NULL;hamming_entry=hamming_entry->next){
				
				EntryList results = create_entry_list();

				lookup_entry_index(hamming_entry->word, core->current_hamming_array[strlen(hamming_entry->word)-4], threshold, results, MT_HAMMING_DIST);
				//so now we have the results | the words of the document that matched with this threshold
				if(results->size!=0){
					// if it matched with at least one word
					//then we should add this word to the matched words of a query if:
					//  1) a query belongs to the th_box of the current threshold ) th_box[threshold]
					//  2) has match type hamming
					//  3) belongs to the payload of the word wwe just checked

					for(int j=0;j<core->query_hamming_map[threshold]->capacity;j++){
						QueryMapNode node=&core->query_hamming_map[threshold]->array[j];
						if(node->query_list->size>0){
							for(QueryListNode lnode=node->query_list->head;lnode!=NULL;lnode=lnode->next){
								if(check_list_existence(hamming_entry->payload,lnode->query->query_id)){
									matchQuery(lnode->query,hamming_entry->word,core->current_doc);
								}
							}
						}
					}
				}
				destroy_entry_list(results);
			}
    	}
        return EC_SUCCESS; 
    }
    else{
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

                    for(int j=0;j<core->query_exact_map->capacity;j++){
                        QueryMapNode node=&core->query_exact_map->array[j];
                        if(node->query_list->size>0){
                            for(QueryListNode lnode=node->query_list->head;lnode!=NULL;lnode=lnode->next){
                                if(check_list_existence(exact_entry->payload,lnode->query->query_id)){
                                    matchQuery(lnode->query,exact_entry->word,core->current_doc);
                                }
                            }
                        }
                    }
                }
                destroy_entry_list(results);
            }
		}
        return EC_SUCCESS;
    }
    return EC_FAIL;
}