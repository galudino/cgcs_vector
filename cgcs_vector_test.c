/*!
    \file       cgcs_vector_test.c
    \brief      Driver source file to show an example of cgcs_vector

    \author     Gemuele Aludino
    \date       05 Sep 2020
    \copyright  Copyright © 2020 Gemuele Aludino

    Copyright © 2020 Gemuele Aludino

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
    DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
    THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#define USE_CGCS_VECTOR              // removes cgcs_ prefix
#include "cgcs_vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
    These comparison functions won't be provided with the library,
    as they are trivial to write -- but they are provided here
    to help demonstrate how cgcs_vector is used.
*/
static int cmp_cstr(const void *c0, const void *c1);
static int cmp_int(const void *c0, const void *c1);
static int cmp_double(const void *c0, const void *c1);

/*
    There are many things you can do with the foreach function,
    but the one you may use most commonly with foreach is
    freeing the memory for each pointer.

    Any function with the prototype
        void *function_name(void *argument)
    can be used with the vforeach function.
*/
static void print_cstr(void *arg);
static void print_int(void *arg);
static void print_double(void *arg);
static void free_cstr(void *arg);

int main(int argc, const char *argv[]) {
    // Note that we've used the #define USE_CGCS_VECTOR directive
    // before #include "cgcs_vector.h", which lets us omit 'cgcs_'
    // from all cgcslib identifiers.

    // Creating a cgcs_vector on the heap.
    // Another way to create a cgcs_vector on the heap is like this: 
    // cgcs_vector *v = malloc(sizeof *v); cgcs_vinit(v, 1);
    vector *v = vnew(1);
    // We set a capacity of 1 to show that the vector will resize as needed.

    // This will be a 'holding' place for the strings
    // we dynamically allocate before we transfer ownership over
    // to cgcs_vector.
    char *s = NULL;

    s = strdup("hello");
    vpushb(v, &s);      // s is shallow-copied.

    s = strdup("how");
    vpushb(v, &s);

    s = strdup("are");
    vpushb(v, &s);

    s = strdup("you");
    vpushb(v, &s);

    s = strdup("doing?");
    vpushb(v, &s);

    char **fr = vfront(v);
    printf("front: %s\n", *fr);
    puts("");

    vforeach(v, print_cstr);
    puts("");

    // Creating a cgcs_vector on the stack.
    cgcs_vector tmp;
    vinit(&tmp, 3);

    const char *strings[] = { "ONE", "TWO", "THREE" };
    for (int i = 0; i < 3; i++) {
        s = strdup(strings[i]);
        vpushb(&tmp, &s);
    }

    // equivalent to vptr_insert(v, vptr_begin(v) + 3);
    vinsert_range(v, vbegin(v) + 3, vbegin(&tmp), vend(&tmp));

    // We no longer need cgcs_vector tmp, so we call the deinit function on it
    // so its internal buffer can be freed.
    vdeinit(&tmp);

    // Pushing an element to the front of v.
    s = strdup("front!");
    vpushf(v, &s);

    vforeach(v, print_cstr);
    puts("");

    vpopf(v);

    vforeach(v, print_cstr);
    puts("");
    
    s = "are";
    printf("element 'are' is at index: %d\n", vsearch(v, cmp_cstr, &s));

    // Freeing all strings in v
    vforeach(v, free_cstr);

    // You can also do
    // vdeinit(v); free(v);
    vdelete(v);

    return 0;
}

static inline int cmp_cstr(const void *c0, const void *c1) {
    return strcmp(*(char **)(c0), *(char **)(c1));
}

static inline int cmp_int(const void *c0, const void *c1) {
    return *(int *)(c0) - *(int *)(c1);
}

static inline int cmp_double(const void *c0, const void *c1) {
    double delta = 0.0;
    return (delta = fabs(*(double *)(c0) - *(double *)(c1)))
    < 0.0001 ? 0 : delta;
}

static inline void print_cstr(void *arg) {
    printf("%s\n", *(char **)(arg));
}

static inline void print_int(void *arg) {
    printf("%d\n", *(int *)(arg));
}

static inline void print_double(void *arg) {
    printf("%f\n", *(double *)(arg));
}

static inline void free_cstr(void *arg) {
    free(*(char **)(arg));
}
