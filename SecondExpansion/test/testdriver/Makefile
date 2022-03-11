# paths
MODULES = ../../modules
INCLUDE = ../../include
LIB = ../../lib

MAKE += --silent

# compiler
CC = gcc

# Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
CFLAGS = -O3 -fPIC -Wall -Werror -g -I$(INCLUDE)
LDFLAGS = -lpthread -lm

# Η βιβλιοθήκη
LIBRARY = libcore.so

# Αρχείο .o και βιβλιοθήκη
OBJS = test.o $(LIB)/$(LIBRARY)

# Το εκτελέσιμο πρόγραμμα
EXEC = testdriver

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

# Για να φτιάξουμε το libcore.so, τρέχουμε το make στο lib directory.
$(LIB)/$(LIBRARY):
	$(MAKE) -C $(LIB) $(LIBRARY)

clean:
	rm -f $(OBJS) $(EXEC) && cd $(LIB) && $(MAKE) clean

run: $(EXEC)
	./$(EXEC)

valgrind: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC)

gdb: $(EXEC)
	gdb --args ./$(EXEC)
