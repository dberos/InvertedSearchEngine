#include"../../include/acutest.h"
#include"../../include/entry.h"

void test_create_entry(void) {
    Entry entry = create_entry("dberos97");
    TEST_ASSERT(entry->word != NULL);
    TEST_ASSERT(entry->payload != NULL);
    TEST_ASSERT(entry->next == NULL);

    EntryList entry_list=create_entry_list();
    TEST_ASSERT(entry_list!=NULL);
    destroy_entry(entry,entry_list);
    destroy_entry_list(entry_list);
}

void test_create_list(void) {
    EntryList el = create_entry_list();
    TEST_ASSERT(el!=NULL);
    TEST_ASSERT(el->head==NULL);
    TEST_ASSERT(el->end==NULL);

    destroy_entry_list(el);
}

void test_insert(void) {
    EntryList el = create_entry_list();
    Entry entry1 = create_entry("dberos97");
    Entry entry2 = create_entry("bderos79");
    
    add_entry(el, entry1);
    TEST_ASSERT(el->head == entry1);
    TEST_ASSERT(el->end == entry1);

    add_entry(el, entry2);
    TEST_ASSERT(el->end == entry2);

    destroy_entry_list(el);
}

void test_find_first(void) {
    EntryList el = create_entry_list();
    Entry entry1 = create_entry("dberos97");
    Entry entry2 = create_entry("bderos79");
    
    add_entry(el, entry1);
    add_entry(el, entry2);

    Entry firstEntry = get_first(el);
    TEST_ASSERT(firstEntry == entry1);

    destroy_entry_list(el);
}

void test_find_next(void) {
    EntryList el = create_entry_list();
    Entry entry1 = create_entry("dberos97");
    Entry entry2 = create_entry("bderos79");
    
    add_entry(el, entry1);
    add_entry(el, entry2);

    Entry secondEntry = get_next(el, entry1);
    TEST_ASSERT(secondEntry == entry2);

    destroy_entry_list(el);
}

TEST_LIST = {
    {"create_entry", test_create_entry},
    {"create_entry_list", test_create_list},
    {"add_entry", test_insert},
    {"get_first", test_find_first},
    {"get_next", test_find_next},
    {NULL, NULL}
};