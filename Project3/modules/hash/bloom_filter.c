#include"../../include/bloom_filter.h"

BloomFilter bloom_create(){
    // Allocate memory for the Bloom Filter
    BloomFilter bloom=malloc(sizeof(*bloom));

    // Set starting total inserted values
    bloom->size=0;
    // Set size in bytes
    bloom->bytes=BLOOMBYTES;
    // Set size in bits
    bloom->bits=BLOOMBITS;
    // Set k number of hash functions
    bloom->k=BLOOMK;
    // Set hash function
    bloom->hash_function=hash_i;
    
    // Allocate memory for the bit array
    bloom->array=malloc(bloom->bytes);
    for(uint i=0;i<bloom->bytes;i++){
        // Initialize the array with 0 in each position
        bloom->array[i]=0;
    }
    return bloom;
}

void bloom_destroy(BloomFilter bloom){
    // Free array
    free(bloom->array);
    // Free the Bloom Filter
    free(bloom);
}

void bloom_insert(BloomFilter bloom,Pointer value){
    // hash_i Hash Function passes through 2 other Hash Functions
    ulong pos=bloom->hash_function(value,bloom->k)%(bloom->bits);
    // Set 1 and left shift
    bloom->array[pos/8] |= 1 << pos%8;
    // Increase the number of inserted values
    bloom->size++;
}

bool bloom_check(BloomFilter bloom,Pointer value){
    // Find Hash Position
    ulong pos=bloom->hash_function(value,bloom->k)%(bloom->bits);
    // If bit is 0 then value is for sure absent
    return bloom->array[pos/8] & 1 << pos%8 ? true : false;
}
