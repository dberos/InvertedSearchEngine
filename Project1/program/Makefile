# paths
MODULES = ../modules
INCLUDE = ../include

# Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
CFLAGS = -Wall -Werror -g -I$(INCLUDE)

# Αρχεία .o
OBJS = main.o \
$(MODULES)/words/word.o $(MODULES)/trees/bst.o \
$(MODULES)/hash/hash_functions.o $(MODULES)/hash/map.o \
$(MODULES)/methods/deduplication.o

# Το εκτελέσιμο πρόγραμμα
EXEC = main

ARGS = none

$(EXEC): $(OBJS)
	gcc $(OBJS) -o $(EXEC)

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

valgrind: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC) $(ARGS)

gdb: $(EXEC)
	gdb --args ./$(EXEC) $(ARGS)