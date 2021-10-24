#include"../../include/map.h"

#define MAX_LOAD_FACTOR 0.5

int prime_table[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
	786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};

Map map_create(){
    // Allocate memory for the Map
    Map map=malloc(sizeof(*map));
    // Set starting size to 0
    map->size=0;
    // Set starting capacity as the first number from the array
    map->capacity=prime_table[0];
    // Set Hash Function
    map->hash_function=hash_string;

    // Allocate memotry for the array
    map->array=malloc(sizeof(*map->array)*map->capacity);
    // On each Node
    for(int i=0;i<map->capacity;i++){
        // Set state to empty
        map->array[i].state=empty;
        // And create a Binary Search Tree
        map->array[i].bst=bst_create();
    }
    // Return the Map
    return map;
}

void map_destroy(Map map){
    for(int i=0;i<map->capacity;i++){
        // Destroy the Node's Binary Search Tree
        bst_destroy(map->array[i].bst);
        if(map->array[i].state==occupied){
            // If Node is occupied, key was strduped
            free(map->array[i].key);
        }
    }
    // Free the array
    free(map->array);
    // Free the Map
    free(map);
}

void map_insert(Map map,Word word){
    // Position for the insertion
    ulong pos;
    // Whether key already exists
    bool already_in_map=false;
    for(pos=map->hash_function(word->first_letter)%map->capacity;
            map->array[pos].state==occupied;
            pos=(pos+1)%map->capacity){
                if(strcmp(map->array[pos].key,word->first_letter)==0){
                    already_in_map=true;
                    break;
                }
            }

    MapNode node=&map->array[pos];
    if(already_in_map==false){
        map->size++;
        node->state=occupied;
        node->key=strdup(word->first_letter);
    }
    // Insert at the Node's Binary Search Tree
    bst_insert(node->bst,word->word);

    float load_factor=(float)map->size/map->capacity;
    if(load_factor>MAX_LOAD_FACTOR){
        map_rehash(map);
    }
}

void map_rehash(Map map){
    // Find old_capacity
    int old_capacity=map->capacity;
    // Find old array
    MapNode old_array=map->array;

    int primes=sizeof(prime_table)/sizeof(int);

    // Find the next prime, if it exists
    for(int i=0;i<primes;i++){
        if(prime_table[i]>old_capacity){
            map->capacity=prime_table[i];
            break;
        }
    }

    // Or continue by doubling up the capacity, if all primes are used
    if(map->capacity>old_capacity){
        map->capacity*=2;
    }

    // Allocate memory for the array
    map->array=malloc(sizeof(*map->array)*map->capacity);
    // Set size to 0
    map->size=0;
    // Set each state to empty
    for(int i=0;i<map->capacity;i++){
        map->array[i].state=empty;
    }

    for(int i=0;i<old_capacity;i++){
        if(old_array[i].state==occupied){
            ulong pos;
            bool already_in_map=false;
            // Need to scan and stop at empty or deleted because collisions can happen
            // And not just get the first hash position
            for(pos=map->hash_function(old_array[i].key)%map->capacity;
                    map->array[pos].state==occupied;
                    pos=(pos+1)%map->capacity){
                        if(strcmp(map->array[i].key,old_array[i].key)==0){
                            already_in_map=true;
                            break;
                        }
                    }
            
            MapNode node=&map->array[pos];
            if(already_in_map==false){
                map->size++;
                node->state=occupied;
                node->key=strdup(old_array[i].key);
            }
            // Create a new Binary Search Tree
            node->bst=bst_create();
            // Insert everything from the old one to it
            new_from_old_bst(node->bst,old_array[i].bst);
            // Destroy the old one
            bst_destroy(old_array[i].bst);
            // Free old key
            free(old_array[i].key);
        }
        else{
            // State was empty, but Bst still exists since each Node contains one
            bst_destroy(old_array[i].bst);
        }
    }
    for(int i=0;i<map->capacity;i++){
        if(map->array[i].state==empty){
            // Create everywhere else a Bst
            map->array[i].bst=bst_create();
        }
    }
    // Free the old array
    free(old_array);
}

bool map_check_key(Map map,Word word){
    int count=0;
    ulong pos;
    // Scan the Map and find whether key is same as given word's first letter
    for(pos=map->hash_function(word->first_letter)%map->capacity;
            map->array[pos].state==occupied;
            pos=(pos+1)%map->capacity){
                if(map->array[pos].state==occupied && strcmp(map->array[pos].key,word->first_letter)==0){
                    return true;
                }
                count++;
                if(count==map->capacity){
                    return false;
                }
            }
    return false;
}

String map_return_key(Map map,Word word){
    int count=0;
    ulong pos;
    for(pos=map->hash_function(word->first_letter)%map->capacity;
            map->array[pos].state==occupied;
            pos=(pos+1)%map->capacity){
                if(map->array[pos].state==occupied && strcmp(map->array[pos].key,word->first_letter)==0){
                    return map->array[pos].bst->root->word->word;
                }
                count++;
                if(count==map->capacity){
                    return NULL;
                }
            }
    return NULL;
}
