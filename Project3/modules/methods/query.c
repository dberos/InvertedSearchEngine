#include"../../include/query.h"
#include"../../include/methods.h"

Query query_create(QueryID id, MatchType match_type, uint match_dist){
    // Allocate memory for the Query
    Query query=malloc(sizeof(*query));
    // Set ID
    query->query_id=id;
    //  Set MatchType
    query->match_type=match_type;
    // Set MatchDistance (the threshold)
    query->match_dist=match_dist;
    // Set Lock
    query->lock=false;
    // Set starting matching words number
    query->matched_words_num=0;
    // Set starting Querys's words number
    query->query_words_num=0;
    // Allocate memory for the word array
    query->words=malloc(sizeof(*query->words)*MAX_QUERY_WORDS);
    // Allocate memory for the matched words array
    query->matched_words=malloc(sizeof(*query->matched_words)*MAX_QUERY_WORDS);

    // Return the Query
    return query;
}

void addWord_to_query(Query query, String word){
    query->words[query->query_words_num] = strdup(word);
    query->query_words_num++;
}

void passWords_to_query(Query new_query, Query old_query){
    for(int i=0 ; i<old_query->query_words_num ; i++){
        new_query->words[i] = strdup(old_query->words[i]);
        new_query->query_words_num++;
    }
}

void query_destroy(Query query){
    // Free each of Querys's words
    for(int i=0;i<query->query_words_num;i++){
        free(query->words[i]);
    }
    free(query->words);

    // Free each of Querys's matched words
    for(int i=0;i<query->matched_words_num;i++){
        free(query->matched_words[i]);
    }
    free(query->matched_words);
    
    // Free the Query
    free(query);
}

void query_cleanup(Query query,String query_str){
    // Get the whole Query String
    String str=strdup(query_str);
    // Each word of the Query
    String word=strtok(str," ");
    // Get each word
    while(word!=NULL){
        // Clean each word
        remove_special_characters_decapitalize(word);
        // Insert each word at Querys's word array
        addWord_to_query(query,word);
        word=strtok(NULL," ");
    }
    free(str);
}

void matchQuery(Core core, Query query, String word, DocumentPtr doc){
	
    for(int i=0 ; i<query->matched_words_num ; i++){
        //if this word has already been matched, do nothing
        if(!strcmp(query->matched_words[i], word)) return;

    }

    //if its hasn't been matched yet, then add it
    query->matched_words[query->matched_words_num++] = strdup(word);

    //And if after this all the query words have been matched, add this query id to Doc results
    if(query->matched_words_num==query->query_words_num){
        // printf("docid: %u   queryid: %u\n", doc->doc_id, query->query_id);
        add_query_to_doc_results(core, query->query_id, doc);    
        //lock query so if we meet it again, we dont bother with this one because its alread matched
        query->lock=true;
    }
    // pthread_mutex_unlock(&core->job_scheduler->addto_documentresults_mutex);

}

void add_query_to_doc_results(Core core, QueryID query_id, DocumentPtr doc){
    
    addDocumentResult(core, doc, query_id);
}
