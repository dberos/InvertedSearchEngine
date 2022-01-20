#include"../../include/vector.h"

Vector vector_create(){
    // Allocating memory for the Vector
    Vector vector=malloc(sizeof(*vector));

    // Set starting size to 0
    vector->size=0;
    // Set starting capacity to 10
    vector->capacity=10;

    // Allocate memory for the array
    vector->array=malloc(sizeof(*vector->array)*vector->capacity);

    // Return vector
    return vector;
}

void vector_destroy(Vector vector){
    // Free the array
    free(vector->array);
    // Free the vector
    free(vector);
}

void vector_push_back(Vector vector,Entry entry){
    // Increase the size of vector
    vector->size++;

    // If it needs realloc
    if(vector->capacity<vector->size){
        // Double up the capacity
        vector->capacity*=2;
        // Realloc
        vector->array=realloc(vector->array,sizeof(*vector->array)*vector->capacity);
    }

    // Position for the insertion since size is increased
    int pos=vector->size-1;
    // Node for the insertion
    VectorNode node=&vector->array[pos];
    // Set the Entry of the Node
    node->entry=entry;
}

VectorNode vector_at(Vector vector,int pos){
    // Return the node if it exists
    return pos<vector->size && pos>=0 ? &vector->array[pos] : NULL;
}

bool vector_find(Vector vector,String word){
    // Iterate over the array and check for word existance
    for(int i=0;i<vector->size;i++){
        VectorNode node=vector_at(vector,i);
        if(node->entry!=NULL){
            if(strcmp(node->entry->word,word)==0){
                return true;
            }
        }
    }
    return false;
}
