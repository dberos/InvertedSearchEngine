#include"../../include/hash_table.h"

#define MAX_HASH_LOAD_FACTOR 0.5

int hash_primes[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
	786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};


HashTable hash_table_create(){
    // Allocating memory for the Hash Table
    HashTable hash_table=malloc(sizeof(*hash_table));

    // Set starting size
    hash_table->size=0;
    // Set starting capacity
    hash_table->capacity=hash_primes[3];
    // Set Hash Function
    hash_table->hash_function=hash_int;

    // Allocating memory for the array
    hash_table->array=malloc(sizeof(*hash_table->array)*hash_table->capacity);
    // On each Node
    for(int i=0;i<hash_table->capacity;i++){
        // Get the Node
        HashNode node=&hash_table->array[i];
        // Set key to NULL
        node->key=NULL;
        // Create a List
        node->value=linked_list_create();
    }

    // Return the Hash Table
    return hash_table;
}

void hash_table_destroy(HashTable hash_table){
    // On each Node
    for(int i=0;i<hash_table->capacity;i++){
        // Get the Node
        HashNode node=&hash_table->array[i];
        // Destroy the Node's List
        linked_list_destroy(node->value);
    }
    // Free the array
    free(hash_table->array);
    // Free the Hash Table
    free(hash_table);
}

bool hash_table_insert(HashTable hash_table,Pointer query_id,String word,int num_words){
    // Hash Position
    ulong pos;
    for(pos=hash_table->hash_function(query_id)%hash_table->capacity;
            hash_table->array[pos].key!=NULL;
                pos=(pos+1)%hash_table->capacity){
                    if(hash_table->array[pos].key==query_id){
                        break;
                    }
                }
    
    // Find Hash Position when traverse stopped
    HashNode node=&hash_table->array[pos];

    // Check whether to set the key
    if(node->key==NULL){
        // Set key
        node->key=query_id;
        // Increase the size
        hash_table->size++;
    }

    // Check whether word can be inserted at the List
    bool inserted=linked_list_insert_end(node->value,word);
    // Get the size of the List
    int size=node->value->size;
    // Check whether Query can match
    bool match=inserted==true && size==num_words ? true : false;

    // Find Load Factor
    float load_factor=(float)hash_table->size/hash_table->capacity;
    // If it got increased
    if(load_factor>MAX_HASH_LOAD_FACTOR){
        // Then rehash
        hash_table_rehash(hash_table);
    }
    // Return whether Query matched
    return match;
}

void hash_table_rehash(HashTable hash_table){
    // Find old array
    HashNode old_array=hash_table->array;
    // Find old capacity
    int old_capacity=hash_table->capacity;

    int primes=sizeof(hash_primes)/sizeof(int);

    // Find the next prime, if it exists
    for(int i=0;i<primes;i++){
        if(hash_primes[i]>old_capacity){
            hash_table->capacity=hash_primes[i];
            break;
        }
    }
    // Or continue by doubling up the capacity, if all primes are used
    if(hash_table->capacity==old_capacity){
        hash_table->capacity*=2;
    }

    // Allocate memory for the array
    hash_table->array=malloc(sizeof(*hash_table->array)*hash_table->capacity);
    // Set size to 0
    hash_table->size=0;

    // On each Node
    for(int i=0;i<hash_table->capacity;i++){
        // Get the Node
        HashNode node=&hash_table->array[i];
        // Set key
        node->key=0;
        // Create a List
        node->value=linked_list_create();
    }

    // Insert everything at the new Hash Table
    for(int i=0;i<old_capacity;i++){
        // Get the Node
        HashNode node=&old_array[i];
        if(node->key!=0){
            // Insert the words
            for(LinkedListNode lnode=node->value->head;lnode!=NULL;lnode=lnode->next){
                // Don't mind for number of words here
                hash_table_insert(hash_table,node->key,lnode->string,0);
            }
        }
        linked_list_destroy(node->value);
    }
    // Free the old array
    free(old_array);
}

bool hash_table_find(HashTable hash_table,Pointer query_id,int num_words){
    // Hash Position
    ulong pos;
    for(pos=hash_table->hash_function(query_id)%hash_table->capacity;
            hash_table->array[pos].key!=0;
                pos=(pos+1)%hash_table->capacity){
                    if(hash_table->array[pos].key==query_id){
                        break;
                    }
                }

    // Find Hash Position when traverse stopped
    HashNode node=&hash_table->array[pos];
    // Return whether QueryID has matched
    return node->value->size==num_words ? true : false;
}
