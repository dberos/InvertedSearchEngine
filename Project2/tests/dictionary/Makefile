# paths
MODULES = ../../modules
INCLUDE = ../../include

# compiler
CC = gcc

# Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
CFLAGS = -Wall -Werror -g -I$(INCLUDE)
LDFLAGS = -lm

# Αρχεία .o για δημιουργία της βιβλιοθήκης libcore.so
OBJS = dictionary_test.o \
$(MODULES)/entries/word.o $(MODULES)/entries/entry.o \
$(MODULES)/trees/bst.o $(MODULES)/trees/bkt.o \
$(MODULES)/lists/word_list.o $(MODULES)/lists/list.o $(MODULES)/lists/query_list.o \
$(MODULES)/hash/hash_functions.o $(MODULES)/hash/dictionary.o \
$(MODULES)/hash/map.o $(MODULES)/hash/query_map.o \
$(MODULES)/methods/deduplication.o $(MODULES)/methods/exact_matching.o \
$(MODULES)/methods/core.o $(MODULES)/methods/global_core.o $(MODULES)/methods/query.o \
$(MODULES)/methods/matchquery.o $(MODULES)/methods/documents.o


# Το εκτελέσιμο πρόγραμμα
EXEC = dictionary_test

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

valgrind: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC) $(ARGS)

gdb: $(EXEC)
	gdb --args ./$(EXEC) $(ARGS)
