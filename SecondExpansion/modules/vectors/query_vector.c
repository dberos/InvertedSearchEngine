#include"../../include/query_vector.h"

extern JobScheduler job_scheduler;

QueryVector query_vector_create(){
    // Allocating memory for the Vector
    QueryVector vector=malloc(sizeof(*vector));

    // Set starting size to 0
    vector->size=0;
    // Set starting capacity
    vector->capacity=10;

    // Allocating memory for the array
    vector->array=malloc(sizeof(*vector->array)*vector->capacity);

    // Return Vector
    return vector;
}

void query_vector_destroy(QueryVector vector){
    // On each Node
    for(int i=0;i<vector->size;i++){
        // Destroy the Query if it exists
        if(vector->array[i].query!=NULL){
            query_destroy(vector->array[i].query);
        }
    }
    // Free the array
    free(vector->array);
    // Free the Vector
    free(vector);
}

void query_vector_insert(QueryVector vector,QueryID query_id,String query_str,MatchType match_type,uint match_dist){
    // Lock the mutex
    pthread_mutex_lock(&job_scheduler->query_vector_mutex);
    // Increase the size of vector
    vector->size++;

    // If it needs realloc
    if(vector->capacity<query_id){
        // Double up the capacity
        vector->capacity*=2;
        // Realloc
        vector->array=realloc(vector->array,sizeof(*vector->array)*vector->capacity);
    }
    
    // Get position
    int pos=query_id-1;
    // Node for the insertion
    QueryVectorNode node=&vector->array[pos];


    // Creating a Query
    Query query=query_create(query_id,match_type,match_dist);
    // Set query's words
    query_cleanup(query,query_str);
    
    // Set Node's Query
    node->query=query;
    // Unlock the mutex
    pthread_mutex_unlock(&job_scheduler->query_vector_mutex);
}

Query query_vector_at(QueryVector vector,uint pos){
    // Get the Query
    const Query query=vector->array[pos-1].query;
    // Return it
    return query;
}

void query_vector_set(QueryVector vector,uint pos){
    // Lock the mutex
    pthread_mutex_lock(&job_scheduler->query_vector_mutex);
    // Get the Node
    QueryVectorNode node=&vector->array[pos-1];
    // Destroy the Query
    query_destroy(node->query);
    // Set it to NULL
    node->query=NULL;
    // Don't decrease the size
    // Push Backs depend on size and query isn't for sure at last position
    // Unlock the mutex
    pthread_mutex_unlock(&job_scheduler->query_vector_mutex);
}