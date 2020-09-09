/*!
    \file       cgcs_vector_ptr_main.c
    \brief      Driver source file to show an example of cgcs_vector_ptr

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

#define using_cgcs_vptr               // removes cgcs_ prefix
#define CGCS_VECTOR_PTR_SHORTNAMES    // reduces vptr_ prefix to v
#include "cgcs_vector_ptr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
    These comparison functions won't be provided with the library,
    as they are trivial to write -- but they are provided here
    to help demonstrate how vptr_t is used.
*/
int cmp_cstr(const void *c0, const void *c1);
int cmp_int(const void *c0, const void *c1);
int cmp_double(const void *c0, const void *c1);

/*
    There are many things you can do with the foreach function,
    but the one you may use most commonly with foreach is
    freeing the memory for each pointer.
*/
void *print_cstr(void *arg);
void *print_int(void *arg);
void *print_double(void *arg);
void *free_cstr(void *arg);

int main(int argc, const char *argv[]) {
    // Creating a vptr_t on the heap.
    // You can also do vptr_t *v = vptr_init(malloc(sizeof *v), size_value);
    vptr_t *v = vptr_new(1);
    // We set a capacity of 1 to show that the vector will resize as needed.

    // This will be a 'holding' place for the strings
    // we dynamically allocate before we transfer ownership over
    // to vptr_t.
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

    // Creating a vptr_t on the stack.
    vptr_t tmp;
    vinit(&tmp, 3);

    const char *strings[] = { "ONE", "TWO", "THREE" };
    for (int i = 0; i < 3; i++) {
        s = strdup(strings[i]);
        vpushb(&tmp, &s);
    }

    // equivalent to vptr_insert(v, vptr_begin(v) + 3);
    vinsert_range(v, vbegin(v) + 3, vbegin(&tmp), vend(&tmp));

    // We no longer need vptr_t tmp, so we call the deinit function on it
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

    // You can use the delete macro on v,
    // or do free(vptr_deinit(v));
    vptr_delete(v);

    return 0;
}

int cmp_cstr(const void *c0, const void *c1) {
    char *a = *(char **)(c0);
    char *b = *(char **)(c1);

    return strcmp(a, b);
}

int cmp_int(const void *c0, const void *c1) {
    int a = *(int *)(c0);
    int b = *(int *)(c1);

    return a - b;
}

int cmp_double(const void *c0, const void *c1) {
    double a = *(double *)(c0);
    double b = *(double *)(c1);

    double delta = fabs(a - b);

    if (delta < 0.0001) {
        return 0;
    } else {
        return (int)(delta);
    }
}

void *print_cstr(void *arg) {
    char *cstr = *(char **)(arg);
    printf("%s\n", cstr);

    return NULL;
}

void *print_int(void *arg) {
    int i = *(int *)(arg);
    printf("%d\n", i);

    return NULL;
}

void *print_double(void *arg) {
    double d = *(double *)(arg);
    printf("%f\n", d);

    return NULL;
}

void *free_cstr(void *arg) {
    char **cstr = (char **)(arg);
    free(*cstr);

    return NULL;
}
