#include"../../include/dictionary.h"


#define MAX_DICTIONARY_LOAD_FACTOR 0.9

int prime_table_dictionary[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
	786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};

Dictionary dictionary_create(){
    // Allocate memory for the Dictionary
    Dictionary dictionary=malloc(sizeof(*dictionary));

    // Set starting size to 0
    dictionary->size=0;
    // Set starting capacity as the first prime from the array
    dictionary->capacity=prime_table_dictionary[0];
    // Set Hash Function
    dictionary->hash_function=hash_id;

    // Allocate memory for the List array
    dictionary->array=malloc(sizeof(*dictionary->array)*dictionary->capacity);
    // On each Node
    for(int i=0;i<dictionary->capacity;i++){
        // Create A List
        dictionary->array[i].value=list_create();
    }

    // Return the dictionary
    return dictionary;
}

void dictionary_destroy(Dictionary dictionary){
    // On each Node
    for(int i=0;i<dictionary->capacity;i++){
        // Destroy the List
        list_destroy(dictionary->array[i].value);
    }

    // Free the array
    free(dictionary->array);
    // Free the Dictionary
    free(dictionary);
}

void dictionary_insert(Dictionary dictionary,Pointer key,Pointer value){
    // Find the Hash Position
    uint pos=dictionary->hash_function(key)%dictionary->capacity;
    // Find the Node
    DictionaryNode node=&dictionary->array[pos];
    // List of the Node
    List list=dictionary->array[pos].value;

    // If the list is empty
    if(list->size==0){
        // Increase the size of the dictionary
        dictionary->size++;
        // Set key of the node
        node->key=key;
    }

    // Insert at the List without traverse
    list_insert_tail(list,value);

    // Find Load Factor
    float load_factor=(float)dictionary->size/dictionary->capacity;
    if(load_factor>MAX_DICTIONARY_LOAD_FACTOR){
        // If it got increased then rehash
        dictionary_rehash(dictionary);
    }
}

void dictionary_rehash(Dictionary dictionary){
    // Find old array
    DictionaryNode old_array=dictionary->array;
    // Find old capacity
    int old_capacity=dictionary->capacity;

    int primes=sizeof(prime_table_dictionary)/sizeof(int);

    // Find the next prime, if it exists
    for(int i=0;i<primes;i++){
        if(prime_table_dictionary[i]>old_capacity){
            dictionary->capacity=prime_table_dictionary[i];
            break;
        }
    }
    // Or continue by doubling up the capacity, if all primes are used
    if(dictionary->capacity==old_capacity){
        dictionary->capacity*=2;
    }

    // Allocate memory for the array
    dictionary->array=malloc(sizeof(*dictionary->array)*dictionary->capacity);
    // Set size to 0
    dictionary->size=0;

    // On each Node
    for(int i=0;i<dictionary->capacity;i++){
        // Create a New List
        dictionary->array[i].value=list_create();
    }

    // Insert everything to the new Dicitionary
    for(int i=0;i<old_capacity;i++){
        if(old_array[i].value->head!=NULL){
            for(ListNode node=old_array[i].value->head;node!=NULL;node=node->next){
                dictionary_insert(dictionary,old_array[i].key,node->value);
            }
        }
        list_destroy(old_array[i].value);
    }
    // Free the old array
    free(old_array);
}

List dictionary_find(Dictionary dictionary,Pointer key){
    // Hash Position
    uint pos=dictionary->hash_function(key)%dictionary->capacity;
    // List of the Hash Position
    List list=dictionary->array[pos].value;

    // Return it if it isn't empty
    return list->size>0 ? list : NULL;
}
