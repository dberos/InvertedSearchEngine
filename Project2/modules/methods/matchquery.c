#include"../../include/methods.h"

void matchQuery(Query query, String word, DocumentPtr doc){
    
    //check the matched words array in query
    for(int i=0 ; i<query->matched_words_num ; i++){

        //if this word has already been matched, do nothing
        if(!strcmp(query->matched_words[i], word)) return;

    }

    //if its hasn't been matched yet, then add it
    query->matched_words[query->matched_words_num++] = strdup(word);

    //And if after this all the query words have been matched, add this query id to Doc results
    if(query->matched_words_num==query->query_words_num){
        add_query_to_doc_results(query->query_id, doc);    
        //lock query so if we meet it again, we dont bother with this one because its alread matched
        query->lock=true;
    }
}

void add_query_to_doc_results(QueryID query_id, DocumentPtr doc){
    // printf("DOC %u result %u\n", doc->doc_id, query_id);
    addDocumentResult(doc, query_id);
}