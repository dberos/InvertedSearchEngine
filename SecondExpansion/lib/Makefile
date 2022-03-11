# paths
MODULES = ../modules
INCLUDE = ../include

# compiler
CC = gcc

# Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
CFLAGS = -O3 -fPIC -Wall -Werror -g -I$(INCLUDE)
LDFLAGS = -lpthread -lm

# Αρχεία .o για δημιουργία της βιβλιοθήκης libcore.so
OBJS = \
$(MODULES)/entries/entry.o \
$(MODULES)/trees/bst.o \
$(MODULES)/trees/distances.o \
$(MODULES)/lists/list.o \
$(MODULES)/hash/hash_functions.o \
$(MODULES)/hash/dictionary.o \
$(MODULES)/hash/map.o \
$(MODULES)/methods/core.o \
$(MODULES)/methods/global_core.o \
$(MODULES)/methods/query.o \
$(MODULES)/methods/document.o \
$(MODULES)/vectors/vector.o \
$(MODULES)/trees/bk-tree.o \
$(MODULES)/methods/job.o \
$(MODULES)/lists/queue.o \
$(MODULES)/methods/job_scheduler.o \
$(MODULES)/methods/start_query.o \
$(MODULES)/methods/end_query.o \
$(MODULES)/methods/match_document.o \
$(MODULES)/methods/get_next_avail_res.o \
$(MODULES)/lists/linked_list.o \
$(MODULES)/hash/hash_table.o \
$(MODULES)/lists/document_queue.o \
$(MODULES)/vectors/query_vector.o

# Η βιβλιοθήκη
LIBRARY = libcore.so

$(LIBRARY): $(OBJS)
	$(CC) $(OBJS) -shared -o $(LIBRARY) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(LIBRARY)
