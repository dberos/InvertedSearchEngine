#pragma once

#include"common_types.h"

struct entry{
    String word;
    String payload;
};

// Creating an Entry
Entry entry_create(String word);

// Destroying an Entry
void entry_destroy(Entry entry);
