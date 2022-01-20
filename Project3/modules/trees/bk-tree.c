#include"../../include/bk-tree.h"

BktNode bkt_node_create(){
    // Allocating memory for the Node
    BktNode node=malloc(sizeof(*node));
    // Set the root's entry to NULL
    // Root will get the entry word randomly
    node->entry=NULL;
    // Allocate memory for the Vector array
    node->vector=malloc(sizeof(*node->vector)*28);
    // Creating the Vectors
    for(int i=0;i<28;i++){
        node->vector[i]=vector_create();
    }
    // Return the node
    return node;
}

void bkt_node_destroy(BktNode node){
    // Destroy the Vectors
    for(int i=0;i<28;i++){
        vector_destroy(node->vector[i]);
    }
    // Free the array
    free(node->vector);
    // Free the word if it isn't NULL
    // Free the Node
    free(node);
}

Bkt bkt_create(MatchType match_type){
    // Allocating memory for the BK-Tree
    Bkt bkt=malloc(sizeof(*bkt));
    // Set the Match Type of the BK-Tree
    bkt->match_type=match_type;
    // Create the root
    bkt->root=bkt_node_create();

    // Set Distance Function
    if(bkt->match_type==MT_EDIT_DIST){
        bkt->distance=EditDistance;
    }
    else{
        bkt->distance=HammingDistance;
    }
    // Return the BK-Tree
    return bkt;
}

void bkt_destroy(Bkt bkt){
    // Destroy the root
    bkt_node_destroy(bkt->root);
    // Free the BK-Tree
    free(bkt);
}

void bkt_set_root(Bkt bkt,Entry entry){
    bkt->root->entry=entry;
}

void bkt_insert(Bkt bkt,Entry entry){
    // Assert bkt_set_root has already been called
    // Or stop the program to avoid segs
    assert(bkt->root->entry!=NULL);

    // Find the distance from the root
    int distance=bkt->distance
        (bkt->root->entry->word,
            strlen(bkt->root->entry->word),
                entry->word,
                    strlen(entry->word));
    
    // Insert at the correct Vector
    vector_push_back(bkt->root->vector[distance],entry);
}

Vector bkt_find(Bkt bkt,String word,int threshold){
    // Create a Vector to store the similar entry results
    Vector results=vector_create();
    // Find the distance from the root
    int distance=bkt->distance
        (bkt->root->entry->word,
            strlen(bkt->root->entry->word),
                word,
                    strlen(word));
    
    // Check whether root entry can be inserted
    if(distance<=threshold){
        vector_push_back(results,bkt->root->entry);
    }
    
    // Iterate over [d-n,d+n] asserting starting position is positive
    for(int d=((d=distance-threshold)<0) ? 1 : (distance-threshold);
        (d<=distance+threshold);
            d++){
                // Find the vector of this position
                Vector vector=bkt->root->vector[d];
                for(int i=0;i<vector->size;i++){
                    // Find distance
                    int dist=bkt->distance
                        (vector->array[i].entry->word,
                            strlen(vector->array[i].entry->word),
                                word,
                                    strlen(word));
                                    
                    // Assert distance is inside given threshold
                    if(dist<=threshold){
                        // Insert at the results
                        vector_push_back(results,vector->array[i].entry);
                    }
                }
            }
    return results;
}
