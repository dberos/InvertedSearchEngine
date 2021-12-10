#include"../../include/methods.h"

Core core_create(){
    Core core=malloc(sizeof(*core));
    core->document=map_create();
    core->queries=dictionary_create();
    return core;
}

void core_destroy(Core core){
    map_destroy(core->document);
    dictionary_destroy(core->queries);
    free(core);
}
