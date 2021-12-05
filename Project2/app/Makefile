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

# Αρχείο .o για δημιουργία της βιβλιοθήκης
LIB = $(MODULES)/methods/core.o

# Το εκτελέσιμο πρόγραμμα
EXEC = testdriver

# Η βιβλιοθήκη
LIBRARY = libcore.so

$(EXEC): $(OBJS)
	gcc $(OBJS) -o $(EXEC)

clean:
	rm -f $(OBJS) $(EXEC) $(LIBRARY)

run: $(EXEC)
	./$(EXEC)

valgrind: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC)

gdb: $(EXEC)
	gdb --args ./$(EXEC)

# make lib για δημιουργία βιβλιοθήκης από το core μας
lib: $(LIB)
	gcc -shared -o $(LIBRARY) $(LIB)
