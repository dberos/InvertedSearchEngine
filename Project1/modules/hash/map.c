#include"../../include/map.h"

#define MAX_LOAD_FACTOR 0.9

int prime_table[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
	786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};


Map map_create(){
    // Allocate memory for the Map
    Map map=malloc(sizeof(*map));

    // Set starting size to 0;
    map->size=0;
    // Set starting capacity as the first prime from the array
    map->capacity=prime_table[0];
    // Set Hash Function
    map->hash_function=hash_string;

    // Allocate memory for the array
    map->array=malloc(sizeof(*map->array)*map->capacity);
    // On each Node
    for(int i=0;i<map->capacity;i++){
        // Create a Binary Search Tree
        map->array[i].bst=bst_create();
    }

    // Create the Entry List
    map->entry_list=create_entry_list();

    // Return the Map
    return map;
}

void map_destroy(Map map){
    // On each Node
    for(int i=0;i<map->capacity;i++){
        // Destroy the Binary Search Tree
        bst_destroy(map->array[i].bst);
    }

    // Destroy the Map's Entry List
    destroy_entry_list(map->entry_list);
    // Free the array
    free(map->array);
    // Free the Map
    free(map);
}

bool map_insert(Map map,String word){
    // Find the Hash Position
    ulong pos=map->hash_function(word)%map->capacity;
    // Find the Node 
    MapNode node=&map->array[pos];

    // Check whether the word can be inserted on the Node's Bst
    bool inserted=bst_insert(node->bst,word);
    if(inserted==true){
        // Increase the size of the Map if so
        map->size++;
        // Create an Entry
        Entry entry=create_entry(word);
        // Insert at the Map's Entry List
        add_entry(map->entry_list,entry);
    }

    // Find Load Factor
    float load_factor=(float)map->size/map->capacity;
    if(load_factor>MAX_LOAD_FACTOR){
        // If it got increased then rehash
        map_rehash(map);
    }

    return inserted;
}

void map_rehash(Map map){
    // Find the old array
    MapNode old_array=map->array;
    // Find old capacity
    int old_capacity=map->capacity;

    int primes=sizeof(prime_table)/sizeof(int);
    // Find the next prime if it exists
    for(int i=0;i<primes;i++){
        if(prime_table[i]>old_capacity){
            map->capacity=prime_table[i];
            break;
        }
    }
    // Or continue by doublind up the capacity
    if(map->capacity==old_capacity){
        map->capacity*=2;
    }

    // Destroy previous Entry List
    destroy_entry_list(map->entry_list);
    // Create a new one
    map->entry_list=create_entry_list();

    // Allocate memory for the array
    map->array=malloc(sizeof(*map->array)*map->capacity);
    // Set size to 0
    map->size=0;

    // On each Node
    for(int i=0;i<map->capacity;i++){
        // Create a Binary Search Tree
        map->array[i].bst=bst_create();
    }

    // Insert everything from the old Map
    for(int i=0;i<old_capacity;i++){
        if(old_array[i].bst->root!=NULL){
            bst_insert_at_map(old_array[i].bst,map);
        }
        // Destroy the old Bst
        bst_destroy(old_array[i].bst);
    }

    // Free the old array
    free(old_array);
}

bool map_find(Map map,String word){
    // Find the Hash Position
    ulong pos=map->hash_function(word)%map->capacity;
    // Find the Node 
    MapNode node=&map->array[pos];

    // Check whether word exists at the Bst
    bool exists=bst_find(node->bst,word);
    return exists;
}

bool map_remove(Map map,String word){
    // Find the Hash Position
    ulong pos=map->hash_function(word)%map->capacity;
    // Find the Node 
    MapNode node=&map->array[pos];

    // Check whether word can be removed, if it exists, to change Map's size
    bool removed=bst_remove(node->bst,word);
    if(removed==true){
        map->size--;
    }
    return removed;
}
