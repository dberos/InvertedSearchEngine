#pragma once

#include"common_types.h"
#include"hash_functions.h"

struct bloom_filter{
    uchar* array;               // Bit array
    uint size;                  // Total inserted values, for testing purposes
    uint bytes;                 // Size of Bloom Filter in bytes
    uint bits;                  // Size of Bloom Filter in bits
    uint k;                     // K hash functions
    HashBloom hash_function;    // Hash Function
};

// Creating a Bloom Filter
BloomFilter bloom_create(uint bytes);

// Destroying a Bloom Filter
void bloom_destroy(BloomFilter bloom);

// Inserting an element at a Bloom Filter
void bloom_insert(BloomFilter bloom,Pointer value);

// Check if an element exists at a Bloom Filter
bool bloom_check(BloomFilter bloom,Pointer value);
