# paths
MODULES = ../modules
INCLUDE = ../include

# compiler
CC = gcc

# Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
CFLAGS = -O3 -fPIC -Wall -Werror -g -I$(INCLUDE)
LDFLAGS = -lm

# Αρχεία .o για δημιουργία της βιβλιοθήκης libcore.so
OBJS = $(MODULES)/entries/word.o $(MODULES)/entries/entry.o \
$(MODULES)/trees/bst.o $(MODULES)/trees/bkt.o $(MODULES)/lists/word_list.o $(MODULES)/lists/list.o \
$(MODULES)/hash/hash_functions.o $(MODULES)/hash/dictionary.o $(MODULES)/hash/map.o \
$(MODULES)/methods/deduplication.o $(MODULES)/methods/exact_matching.o $(MODULES)/methods/core.o


# Η βιβλιοθήκη
LIBRARY = libcore.so

$(LIBRARY): $(OBJS)
	$(CC) $(OBJS) -shared -o $(LIBRARY) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(LIBRARY)
