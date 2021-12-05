# paths
MODULES = ../modules
INCLUDE = ../include

# Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
CFLAGS = -O3 -fPIC -Wall -Werror -g -I$(INCLUDE)

# Αρχεία .o
OBJS = test.o \
$(MODULES)/entries/word.o $(MODULES)/entries/entry.o \
$(MODULES)/trees/bst.o $(MODULES)/trees/bkt.o $(MODULES)/lists/word_list.o \
$(MODULES)/hash/hash_functions.o $(MODULES)/hash/dictionary.o $(MODULES)/hash/map.o \
$(MODULES)/methods/deduplication.o $(MODULES)/methods/exact_matching.o $(MODULES)/methods/core.o

# Το εκτελέσιμο πρόγραμμα
EXEC = testdriver

# Το όνομα της βιβλιοθήκης για δημιουργία
LIB = core

# Η βιβλιοθήκη
LIBRARY = $(LIB).so

$(EXEC): $(OBJS)
	gcc $(OBJS) -o $(LIBRARY)

clean:
	rm -f $(OBJS) $(EXEC) $(LIBRARY)

run: $(EXEC)
	./$(LIBRARY)

valgrind: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(LIBRARY)

gdb: $(EXEC)
	gdb --args ./$(LIBRARY)