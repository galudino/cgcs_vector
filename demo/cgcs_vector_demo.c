/*!
    \file       cgcs_vector_demo.c
    \brief      Client source file for cgcs_vector

    \author     Gemuele Aludino
    \date       14 Feb 2021
 */

#include "cgcs_vector.h"

#include <stdio.h>
#include <string.h>

int charptr_compare(const void *c0, const void *c1);
void charptr_free(void *arg);
void charptr_print(void *arg);

int main(int argc, const char *argv[]) {
    const char *items[] = {
        "Beta", "Foxtrot", "Delta", "Alpha",
        "Golf", "Kilo", "Charlie", "Echo", "Igloo",
        "Hotel"
    };

    const size_t length_items = sizeof(items) / sizeof *items;

    // Initialize vector with capacity of 2
    vector_t v;
    vector_init(&v, 2);

    // Use vpushb, aka vector_push_back to append to vector
    printf("Appending elements to vector...\n");
    for (int i = 0; i < length_items; i++) {
        char *str = strdup(items[i]);
        vector_push_back(&v, &str);
    }

    // Use vforeach to run a void (*)(void *) on each element
    printf("{ ");
    //vforeach(&v, charptr_print);
    // Instead of using pointers-to-functions, use blocks
    // (if your compiler/version of C supports it)
    vector_foreach_b(&v, ^(void *arg) { printf(" %s ", *(char **)(arg)); });
    printf("}\n\n");

    // Memory for an element is allocated by the caller,
    // and ownership of that element's memory 
    // will be turned over to the vector.
    // The caller is responsible for freeing all memory.
    char *appended = strdup("Jack");
    printf("Appending new element, '%s', to vector...\n", appended);
    vector_push_back(&v, &appended);

    printf("{ ");
    vector_foreach(&v, charptr_print);
    printf("}\n\n");

    printf("Sorting vector...\n");
    //vqsort(&v, charptr_compare);
    vector_qsort_b(&v,
        ^(const void *a, const void *b) {
            return strcmp(*(char **)(a), *(char **)(b));
        });

    printf("{ ");
    //vforeach(&v, charptr_print);
    vector_foreach_b(&v, ^(void *arg) { printf(" %s ", *(char **)(arg)); });
    printf("}\n\n");

    printf("Using iterator to traverse vector, printing elements one-by-one...\n");
    vector_iterator_t it = vector_begin(&v);

    printf("{ ");    
    while (it < vector_end(&v)) {
        printf("%s ", *(char **)(it++));
    }
    printf("}\n");

    const char *echo = "Echo";
    printf("Searching for key '%s'...\n", echo);
    it = vector_find(&v, charptr_compare, &echo);

    if (it != vector_end(&v)) {
        printf("Found key: '%s'\n", *(char **)(it));
        printf("Now erasing '%s' from vector...\n", *(char **)(it));
        vector_erase(&v, it);
    }

    printf("{ ");
    vector_foreach(&v, charptr_print);
    printf("}\n\n");

    // If elements are pointers to memory on the heap
    // (or another resource), iterate over each element
    // and free each resource.
    // You can use vector_foreach with a void (*)(void *),
    // or use iterators to access each address directly.
    vector_foreach(&v, charptr_free);

    // Always run vdeinit before the end of a vector's scope,
    // or when you are finished with it.
    vector_deinit(&v);

    return EXIT_SUCCESS;
}

int charptr_compare(const void *c0, const void *c1) {
    return strcmp(*(char **)(c0), *(char **)(c1));
}

void charptr_free(void *arg) {
    free(*(char **)(arg));
}

void charptr_print(void *arg) {
    printf("%s ", *(char **)(arg));
}
