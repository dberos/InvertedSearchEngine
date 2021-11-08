#include"../../include/dictionary.h"

// Load Factor should be 0.5 since i only want to store in one node a list with the words of the same initial letter
// So the key of the node is an initial letter and all words from that initial will be placed at its list
// Rehash will never happen here or collisions with different letters hashing on the same position


#define MAX_DICTIONARY_LOAD_FACTOR 0.5

int prime_table_dictionary[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
	786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};

Dictionary dictionary_create(){
    // Allocate memory for the Dictionary
    Dictionary dictionary=malloc(sizeof(*dictionary));

    // Set starting sizeto 0
    dictionary->size=0;
    // Set starting capacity as the first prime from the array
    dictionary->capacity=prime_table_dictionary[0];
    // Set Hash Function
    dictionary->hash_function=hash_string;

    // Allocate memory for the List array
    dictionary->array=malloc(sizeof(*dictionary->array)*dictionary->capacity);
    // On each Node
    for(int i=0;i<dictionary->capacity;i++){
        // Create A List
        dictionary->array[i]=word_list_create();
    }

    // Return the dictionary
    return dictionary;
}

void dictionary_destroy(Dictionary dictionary){
    // On each Node
    for(int i=0;i<dictionary->capacity;i++){
        // Destroy the List
        word_list_destroy(dictionary->array[i]);
    }

    // Free the array
    free(dictionary->array);
    // Free the Dictionary
    free(dictionary);
}

void dictionary_insert(Dictionary dictionary,Word word){
    // Find the Hash Position
    ulong pos=dictionary->hash_function(word->first_letter)%dictionary->capacity;
    // Find the Node
    WordList list=dictionary->array[pos];

    // If this letter is first time seen
    if(list->head==NULL){
        // Increase the size
        dictionary->size++;
    }
    // Try to insert at the List
    word_list_insert_end(list,word->word);

    // Find Load Factor
    float load_factor=(float)dictionary->size/dictionary->capacity;
    if(load_factor>MAX_DICTIONARY_LOAD_FACTOR){
        // If it got increased then rehash
        dictionary_rehash(dictionary);
    }
}

void dictionary_rehash(Dictionary dictionary){
    // Find old array
    WordList* old_array=dictionary->array;
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
        dictionary->array[i]=word_list_create();
    }

    // Insert everything to the new Dicitionary
    for(int i=0;i<old_capacity;i++){
        if(old_array[i]->head!=NULL){
            for(WordListNode node=old_array[i]->head;node!=NULL;node=node->next){
                dictionary_insert(dictionary,node->word);
            }
        }
        word_list_destroy(old_array[i]);
    }
    // Free the old array
    free(old_array);
}

void dictionary_init(Dictionary dictionary){
    // Create an alphabet to insert at the dictionary so i can convert capital letters
    String words[]={"Aa","Bb","Cc","Dd","Ee","Ff","Gg","Hh","Ii","Jj",
                        "Kk","Ll","Mm","Nn","Oo","Pp","Qq","Rr","Ss","Tt",
                            "Uu","Vv","Ww","Xx","Yy","Zz"};


    // Create the Strings to Words
    Word* capitals=malloc(sizeof(*capitals)*26);
    for(int i=0;i<26;i++){
        capitals[i]=word_create(words[i]);
    }

    for(int i=0;i<26;i++){
        dictionary_insert(dictionary,capitals[i]);
    }

    for(int i=0;i<26;i++){
        word_destroy(capitals[i]);
    }
    free(capitals);
}

bool dictionary_check_key(Dictionary dictionary,Word word){
    // Find the Hash Position
    ulong pos=dictionary->hash_function(word->first_letter)%dictionary->capacity;
    // Find the List
    WordList list=dictionary->array[pos];

    if(list->head!=NULL && strcmp(list->head->word->first_letter,word->first_letter)==0){
        return true;
    }
    return false;
}

String dictionary_return_key(Dictionary dictionary,Word word){
    // Find the Hash Position
    ulong pos=dictionary->hash_function(word->first_letter)%dictionary->capacity;
    // Find the List
    WordList list=dictionary->array[pos];

    if(list->head!=NULL && strcmp(list->head->word->first_letter,word->first_letter)==0){
        return list->head->word->word;
    }
    return NULL;
}
