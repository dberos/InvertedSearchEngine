#pragma once

#include"common_types.h"
#include"word.h"
#include"bst.h"
#include"map.h"
#include"hash_functions.h"

// Parse a .txt file
// Create a struct word for each word and insert its first letter at a MapNode
// And the whole word at its Binary Search Tree
// No duplicate words or special characters allowed
int deduplication(FILE* document,Map map);