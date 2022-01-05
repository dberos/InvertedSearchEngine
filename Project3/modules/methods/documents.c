#include"../../include/documents.h"


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