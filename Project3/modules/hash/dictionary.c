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
    dictionary->hash_function=hash_string;

    // Allocate memory for the List array
    dictionary->array=malloc(sizeof(*dictionary->array)*dictionary->capacity);
    // On each Node
    for(int i=0;i<dictionary->capacity;i++){
        // Create an Entry List
        dictionary->array[i].entry_list=create_entry_list();
    }

    // Return the dictionary
    return dictionary;
}

void dictionary_destroy(Dictionary dictionary){
    // On each Node
    for(int i=0;i<dictionary->capacity;i++){
        // Destroy the Entry List
        destroy_entry_list(dictionary->array[i].entry_list);
    }

    // Free the array
    free(dictionary->array);
    // Free the Dictionary
    free(dictionary);
}

bool dictionary_insert(Dictionary dictionary,String word,uint id){
    // Find the Hash Position
    ulong pos=dictionary->hash_function(word)%dictionary->capacity;
    // Find the Node
    DictionaryNode node=&dictionary->array[pos];

    // Check whether word can be inserted in the Node's Entry List
    bool inserted=insert_entry(node->entry_list,word,id);
    if(inserted==true){
        // Increase the size if so
        dictionary->size++;
    }

    // Find Load Factor
    float load_factor=(float)dictionary->size/dictionary->capacity;
    if(load_factor>MAX_DICTIONARY_LOAD_FACTOR){
        // Rehash if it got increased
        dictionary_rehash(dictionary);
    }
    return inserted;
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
        // Create a new Entry List
        dictionary->array[i].entry_list=create_entry_list();
    }

    // Insert everything to the new Dicitionary
    for(int i=0;i<old_capacity;i++){
        if(old_array[i].entry_list->head!=NULL){
            for(Entry entry=old_array[i].entry_list->head;entry!=NULL;entry=entry->next){
                uint ptr=entry->payload->head->value;
                QueryID id=ptr;
                dictionary_insert(dictionary,entry->word,ptr);
                // Fix payload
                Entry en=dictionary_find(dictionary,entry->word);
                for(ListNode node=entry->payload->head;node!=NULL;node=node->next){
                    if(node->value!=id){
                        list_insert_tail(en->payload,node->value);
                    }
                }
            }
        }
        destroy_entry_list(old_array[i].entry_list);
    }
    // Free the old array
    free(old_array);
}

Entry dictionary_find(Dictionary dictionary,String word){
    // Find Hash Position
    ulong pos=dictionary->hash_function(word)%dictionary->capacity;
    // Node of the position
    DictionaryNode node=&dictionary->array[pos];

    for(Entry entry=node->entry_list->head;entry!=NULL;entry=entry->next){
        if(strcmp(entry->word,word)==0){
            // Return the entry if its word is the given word
            return entry;
        }
    }
    return NULL;
}


void dictionary_remove(Dictionary dictionary,QueryMap query_map,uint id){
    // Find the query from the query map with this id so we can find its words and remove them
    Query query=query_map_find(query_map,id);

    // If there is such query
    if(query!=NULL){
        // For each of querys's words
        for(int i=0; i<query->query_words_num ; i++){
            // Find each Entry
            Entry entry=dictionary_find(dictionary, query->words[i]);
            // Remove the query id from its payload
            list_remove(entry->payload,id);

            // If this was the last id of this payload!
            // We need to remove the Entry from this EntryList 
            if(entry->payload->size==0){
                remove_entry_from_list(dictionary, query->words[i]);
            }  
        }
    }
}

void remove_entry_from_list(Dictionary dictionary, String word){
    // Find Hash Position
    ulong pos=dictionary->hash_function(word)%dictionary->capacity;
    // Node of the position
    DictionaryNode node=&dictionary->array[pos];
    // Previous Entry
    Entry previous=NULL;

    // For each Entry
    for(Entry entry=node->entry_list->head;entry!=NULL;entry=entry->next){
        if(strcmp(entry->word, word)==0){
            // If it is also the head of the entry list
            if(entry==node->entry_list->head){
                node->entry_list->head = entry->next;
                node->entry_list->size--;
                destroy_entry(entry, node->entry_list);
                return;
            }
            // If it's not the head (also means that previous is != NULL)
            previous->next=entry->next;
            node->entry_list->size--;
            destroy_entry(entry, node->entry_list);
            return;
        }
        previous=entry;
    }
}
