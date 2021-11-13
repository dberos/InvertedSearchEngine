#include "../../include/bkt.h"

//Distance Metrics 
unsigned int HammingDistance(String a, String b){

    int i, min, max, result;

    result=0;

    (strlen(a) < strlen(b)) ? (min=a, max=b) : (min=b, max=a);

    for(i=0 ; i<min ; i++){
        if(a[i]!=b[i]) result++;
    }

    return result+max-min;

}

unsigned int EditDistance(String a, String b){
    return;
}



index_node_ptr create_index_node(const String word){

    index_node_ptr newnode = (index_node_ptr)malloc(sizeof(index_node));
    
    newnode->word = strdup(word);
    newnode->parent_distance = 0;
    newnode->children_number = 0;
    newnode->children = NULL;

    return newnode;

}


void build_entry_index(const EntryList el, Metric type, Index_ptr ix){

    if(get_first(el)==NULL){
        perror("Entry list was empty!\nNo index was created!\n");
        return NULL;
    }

    ix = (Index_ptr)malloc(sizeof(index));


    //Create the root of the BK tree
    Entry temp = get_first(el);

    ix->root = create_index_node(temp->word);
    


    //Now we should traverse through the entry list and build the entry index
    Entry next_entry = NULL;
        
    
    while((next_entry = get_next(el, temp))!=NULL){
        add_index_node(ix->root, create_index_node(next_entry->word), type);
    }

    return;
}


void add_index_node(index_node_ptr parent, index_node_ptr newnode, Metric type){
    
    unsigned int distance;

    if(type==0) distance = HammingDistance(parent->word, newnode->word); // Hamming Distance
    else if(type==1) distance = HammingDistance(parent->word, newnode->word);  // Edit Distance

    //Iterate through children o this bk tree node and check whether there exists a kid with the same metric distance 
    //with the word we want to insert
    int i;

    for(i=0 ; i<parent->children_number ; i++){

        //if there is a children node with such distance 
        if(parent->children[i]->parent_distance == distance){
            //recursive function call for this node
            add_index_node(parent->children[i], newnode, type);
            return;
        }
    }
    
    //if no node with such metric distance was found:
    //add new node to parent

    //create a new slot for the new node
    parent->children = (index_node_ptr*)realloc(parent->children, (parent->children_number + 1)*sizeof(index_node_ptr));

    //update children numebr of parent
    parent->children_number++;
    
    //update the parent distance of the new node we are adding
    newnode->parent_distance = distance;
    
    
}

void destroy_index_nodes(index_node_ptr node){
    int i;

    for( i=0 ; i<node->children_number ; i++ ){
        destroy_index_nodes(node->children[i]);
    }

    if(node->children != NULL) free(node->children);
    
    free(node->word);
    free(node);

}

void destroy_entry_index(Index_ptr ix){
    destroy_entry_index(ix->root);
    free(ix);
}

void lookup_entry_index(const String w, Index_ptr ix, int threshold, EntryList result){
    
}