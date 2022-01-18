#pragma once

#include"common_types.h"

struct vector_node{
    String word;
};

struct vector{
    VectorNode array;
    int size;
    int capacity;
};

// Creating a Vector
Vector vector_create();

// Destroying a Vector
void vector_destroy(Vector vector);

// Inserting at the end of a Vector
void vector_push_back(Vector vector,String word);

// Get the Node of given position
VectorNode vector_at(Vector vector,int pos);

// Check whether word exists
bool vector_find(Vector vector,String word);