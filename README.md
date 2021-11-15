# InvertedSearchEngine
Κ23α Ανάπτυξη Λογισμικού για Πληροφοριακά Συστήματα, Χειμερινό Εξάμηνο 2021 ~ SIGMOD-2013

## Team

- [Δημήτριος Μπέρος - 1115201600269](https://github.com/dberos)
- [Γιώργος Κοκκίνης - 1115201700050](https://github.com/Kokkinis99)

## Project1

### Map
    Είναι η βασική δομή της άσκησης και αναπαριστά ένα κείμενο
    Τα collisions χειρίζονται με separate chaining
    Το κάθε bucket (MapNode) αναπαριστάται από ένα Binary Search Tree
    Κάθε λέξη, αν είναι μοναδική, γίνεται insert στο Bst του bucket της hash position της
    Το max load factor είναι 0,9 και αν μεγαλώσει σε κάποιο insert τότε συμβαίνει rehash
    
    Στην create του Map δημιουργείται η entry list
    Αν κάποια λέξη μπορέσει να μπει στο bucket που κάνει hash τότε δημιουργείται entry
    Το entry αυτό γίνεται insert στην entry list του Map


#### Tests
    cd tests/map
    make
    make run/make valgrind
    make clean


### Binary Search Tree
    Ένα δυαδικό δέντρο αναζήτησης που χρησιμοποιείται στο Map
    Διάταξη με βάση την strcmp και γίνεται είσοδος μοναδικών strings
    Όλες οι λειτουργίες γίνονται αναδρομικά
    Καλείται πάντα μια βοητική bst_node_ συνάρτηση με όρισμα τη ρίζα στην αντίστοιχη bst συνάρτηση
 

#### Tests
    cd tests/bst
    make
    make run/make valgrind
    make clean

### Dictionary
    Δεν χρησιμοποιείται πουθενά και γι'αυτό δεν υπάρχει κάποιο test
    Είναι ένα ακόμα Hash Table
    Κλειδί του κάθε κόμβου είναι ένα αρχικό γράμμα
    Οι μοναδικές λέξεις από αυτό το αρχικό γίνονται insert στη λίστα του hash position

    Αρχική ιδέα ήταν κάθε κόμβος της entry list να είναι ένα bst με όλα τα entries από το ίδιο αρχικό γράμμα
    Με βάση αυτό γινόταν και το deduplication
    Για μεγάλα αρχεία δεν θα ήταν αποδοτικό
    Μπορεί να χρειαστεί σε επόμενη εργασία τροποποιημένο για το payload


### Entries
    Τα entries έχουν υλοποιηθεί όπως ρητά αυτά ζητούνται στην εκφώνηση.
    Πιο συγκεκριμένα, ένα entry_list είναι ένα σύνολο από entries που χαρακτηρίζονται από
    μια κοινή ιδιότητα. Το entry_list όπως αναφέρεται, αποτελεί την είσοδο ενός ευρετηρίου,
    το οποίο θα χτιστεί πάνω στα entries που αντιστοιχούν σε αυτό.

#### Tests
    cd tests/entry_list
    make
    make run/make valgrind
    make clean
