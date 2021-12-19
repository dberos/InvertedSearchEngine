#include"../../include/documents.h"


DocumentPtr addDocument(Core core, DocID doc_id){
    
    core->docs = (DocumentPtr*) realloc(core->docs, (core->document_number + 1) * sizeof(DocumentPtr));
    
    core->docs[core->document_number] = createDoc(doc_id);
    

    return core->docs[core->document_number++];

}

DocumentPtr createDoc(DocID doc_id){
    DocumentPtr doc = (DocumentPtr)malloc(sizeof(struct Document));
    doc->doc_id = doc_id;
    doc->num_res = 0;
    doc->query_ids = NULL;

    return doc;
}


void addDocumentResult(DocumentPtr doc, QueryID id){
    doc->query_ids = (QueryID*) realloc(doc->query_ids, (doc->num_res + 1) * sizeof(QueryID));
    doc->query_ids[doc->num_res++] = id;
    return;

}

void destroyDocument(DocumentPtr doc){
    free(doc->query_ids);
    free(doc);
}


void destroyDocumentsArray(DocumentPtr* array, uint doc_number){
    for(int i=0 ; i<doc_number ; i++){
        destroyDocument(array[i]);
    }

    free(array);
}

void clear_matchedInfo(Core core){
    for(int i=0 ; i<4 ; i++){
        for(QueryListNode node=core->th_boxes[i]->head ; node!=NULL ; node=node->next){
            
            for(int j=0 ; j<node->query->matched_words_num ; j++){
                free(node->query->matched_words[j]);
            }

            node->query->matched_words_num = 0;
            node->query->lock = false;
        }
    }
}