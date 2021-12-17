#include"../../include/methods.h"

void matchQuery(Query query, String word, DocumentPtr doc){

    //check the matched words array in query
    for(int i=0 ; i<query->matched_words_num ; i++){

        //if this word has already been matched, do nothing
        if(!strcmp(query->matched_words[i], word)) return;

    }

    //if its hasn't been matched yet, then add it
    query->matched_words[query->matched_words_num++] = strdup(word);

    //And if after this all the query wwords have been matched, add this query id to Doc results
    add_query_to_doc_results(query->query_id, doc);    

}

void add_query_to_doc_results(QueryID query_id, DocumentPtr doc){
    addDocumentResult(doc, query_id);
}