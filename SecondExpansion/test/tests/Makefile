# paths
MODULES = ../../modules
INCLUDE = ../../include
LIB = ../../lib

MAKE += --silent

# compiler
CC = gcc

# Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
CFLAGS = -O3 -fPIC -Wall -Werror -MMD -g -I$(INCLUDE)
LDFLAGS = -lpthread -lm

# Η βιβλιοθήκη
LIBRARY = libcore.so

# Αρχεία .o και βιβλιοθήκη
BK-Tree_Test_OBJS = bk-tree_test.o $(LIB)/$(LIBRARY)
Bst_Test_OBJS = bst_test.o $(LIB)/$(LIBRARY)
Dictionary_Test_OBJS = dictionary_test.o $(LIB)/$(LIBRARY)
Entry_Test_OBJS = entry_test.o $(LIB)/$(LIBRARY)
Hash_Table_Test_OBJS = hash_table_test.o $(LIB)/$(LIBRARY)
Linked_List_Test_OBJS = linked_list_test.o $(LIB)/$(LIBRARY)
List_Test_OBJS = list_test.o $(LIB)/$(LIBRARY)
Map_Test_OBJS = map_test.o $(LIB)/$(LIBRARY)
Vector_Test_OBJS = vector_test.o $(LIB)/$(LIBRARY)

# Λίστα με όλα τα εκτελέσιμα
_OBJS := $(subst _OBJS,,$(filter %_OBJS,$(.VARIABLES)))
TESTS := $(filter-out %.a,$(_OBJS))

# Μαζεύουμε όλα τα objects σε μία μεταβλητή
OBJS := $(foreach target, $(_OBJS), $($(target)_OBJS))
DEPS := $(patsubst %.o, %.d, $(filter %.o, $(OBJS)))
-include $(DEPS)

RUN_TARGETS ?= $(addprefix run-, $(TESTS))
VAL_TARGETS ?= $(addprefix valgrind-, $(TESTS))


.PHONY: all clean run valgrind

all: $(TESTS)

.SECONDEXPANSION:
$(TESTS): $$($$@_OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

# Για να φτιάξουμε το libcore.so, τρέχουμε το make στο lib directory.
$(LIB)/$(LIBRARY):
	$(MAKE) -C $(LIB) $(LIBRARY)

clean:
	rm -f $(OBJS) $(TESTS) $(DEPS) && cd $(LIB) && $(MAKE) clean

run-%: %
	./$*

# Το make run εκτελεί όλα τα run targets
run: $(RUN_TARGETS)

valgrind-%: %
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all ./$*

valgrind: $(VAL_TARGETS)
