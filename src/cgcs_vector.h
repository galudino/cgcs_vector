/*!
    \file       cgcs_vector.h
    \brief      Header file for a vector ADT (elements of any pointer type)

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

#ifndef CGCS_VECTOR_H
#define CGCS_VECTOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

// TODO: Fill in all documentation stubs

/*!
    \typedef
    \brief
*/
typedef struct cgcs_vector cgcs_vector;

/*!
    \typedef
    \brief
*/
typedef void *voidptr;

/*!
    \typedef
    \brief
*/
typedef voidptr *cgcs_vector_iterator;

/*!
    \struct
    \brief
*/
struct cgcs_vector {
    /*!
        \struct
        \brief
    */
    struct cgcs_vector_base {
        voidptr *m_start;
        voidptr *m_finish;
        voidptr *m_end_of_storage;
    } m_impl;
};

void cgcs_vinit(cgcs_vector *self, size_t capacity);

void cgcs_vinit_allocfn(cgcs_vector *self, size_t capacity, 
                        void *(*allocfn)(size_t));

void cgcs_vdeinit(cgcs_vector *self);
void cgcs_vdeinit_freefn(cgcs_vector *self, void (*freefn)(void *));

static voidptr cgcs_vfront(cgcs_vector *self);
static voidptr cgcs_vback(cgcs_vector *self);
static voidptr cgcs_vat(cgcs_vector *self, const int index);
static voidptr cgcs_vi(cgcs_vector *self, size_t index);

static bool cgcs_vempty(cgcs_vector *self);
static size_t cgcs_vsize(cgcs_vector *self);
static size_t cgcs_vcapacity(cgcs_vector *self);

bool cgcs_vresize(cgcs_vector *self, size_t n);
bool cgcs_vresize_allocfreefn(cgcs_vector *self, size_t n, 
                              void *(*allocfn)(size_t), void (*freefn)(void *));

bool cgcs_vshrink_to_fit(cgcs_vector *self);
bool cgcs_vshrink_to_fit_allocfreefn(cgcs_vector *self, 
                                     void *(*allocfn)(size_t), void (*freefn)(void *));

static cgcs_vector_iterator cgcs_vbegin(cgcs_vector *self);
static cgcs_vector_iterator cgcs_vend(cgcs_vector *self);

cgcs_vector_iterator cgcs_vinsert(cgcs_vector *self, cgcs_vector_iterator it,
                                  const void *valaddr);
cgcs_vector_iterator cgcs_vinsert_allocfreefn(cgcs_vector *self, cgcs_vector_iterator it,
                                          const void *valaddr, 
                                          void *(*allocfn)(size_t), void (*freefn)(void *));
cgcs_vector_iterator cgcs_vinsert_range(cgcs_vector *self,
                                        cgcs_vector_iterator it,
                                        cgcs_vector_iterator beg,
                                        cgcs_vector_iterator end);
cgcs_vector_iterator cgcs_vinsert_range_allocfreefn(cgcs_vector *self,
                                        cgcs_vector_iterator it,
                                        cgcs_vector_iterator beg,
                                        cgcs_vector_iterator end,
                                        void *(*allocfn)(size_t),
                                        void (*freefn)(void *));

cgcs_vector_iterator cgcs_verase(cgcs_vector *self, cgcs_vector_iterator it);
cgcs_vector_iterator cgcs_verase_range(cgcs_vector *self, cgcs_vector_iterator beg,
                                       cgcs_vector_iterator end);

void cgcs_vpushb(cgcs_vector *self, const void *valaddr);
void cgcs_vpushb_allocfreefn(cgcs_vector *self, const void *valaddr, 
                             void *(*allocfn)(size_t), void (*freefn)(void *));

void cgcs_vpopb(cgcs_vector *self);

void cgcs_vpushf(cgcs_vector *self, const void *valaddr);
void cgcs_vpushf_allocfreefn(cgcs_vector *self, const void *valaddr,
                             void *(*allocfn)(size_t), void (*freefn)(void *));

void cgcs_vpopf(cgcs_vector *self);

void cgcs_vclear(cgcs_vector *self);

void cgcs_vforeach(cgcs_vector *self, void (*func)(void *));
void cgcs_vforeach_b(cgcs_vector *self, void (^block)(void *));

static void cgcs_vforeach_range(cgcs_vector *self, void (*func)(void *),
                                cgcs_vector_iterator beg, cgcs_vector_iterator end);
static void cgcs_vforeach_range_b(cgcs_vector *self, void (^block)(void *),
                                  cgcs_vector_iterator beg, cgcs_vector_iterator end);

int cgcs_vsearch(cgcs_vector *self,
                 int (*cmpfn)(const void *, const void *),
                 const void *valaddr);
int cgcs_vsearch_b(cgcs_vector *self, int (^cmp_b)(const void *, const void *), const void *valaddr);

int cgcs_vsearch_range(cgcs_vector *self,
                       int (*cmpfn)(const void *, const void *),
                       const void *valaddr, cgcs_vector_iterator beg,
                       cgcs_vector_iterator end);
int cgcs_vsearch_range_b(cgcs_vector *self,
                       int (^cmp_b)(const void *, const void *),
                       const void *valaddr, cgcs_vector_iterator beg,
                       cgcs_vector_iterator end);

cgcs_vector_iterator cgcs_vfind(cgcs_vector *self,
                                int (*cmp_b)(const void *, const void *),
                                const void *valaddr);
cgcs_vector_iterator cgcs_vfind_b(cgcs_vector *self,
                                int (^cmp_b)(const void *, const void *),
                                const void *valaddr);

cgcs_vector_iterator cgcs_vfind_range(cgcs_vector *self,
                                      int (*cmpfn)(const void *, const void *),
                                      const void *valaddr, cgcs_vector_iterator beg,
                                      cgcs_vector_iterator end);
cgcs_vector_iterator cgcs_vfind_range_b(cgcs_vector *self,
                                      int (^cmp_b)(const void *, const void *),
                                      const void *valaddr, cgcs_vector_iterator beg,
                                      cgcs_vector_iterator end);

void cgcs_vqsort(cgcs_vector *self,
                 int (*cmpfn)(const void *, const void *));
void cgcs_vqsort_b(cgcs_vector *self,
                 int (^cmp_b)(const void *, const void *));

void cgcs_vqsort_position(cgcs_vector *self,
                          int (*cmpfn)(const void *, const void *),
                          cgcs_vector_iterator pos);
void cgcs_vqsort_position_b(cgcs_vector *self,
                          int (^cmp_b)(const void *, const void *),
                          cgcs_vector_iterator pos);

void cgcs_vmergesort(cgcs_vector *self, int (*cmpfn)(const void *, const void *));
void cgcs_vmergesort_b(cgcs_vector *self, int (^cmp_b)(const void *, const void *));

void cgcs_vmergesort_position(cgcs_vector *self, int (*cmpfn)(const void *, const void *), cgcs_vector_iterator pos);
void cgcs_vmergesort_position_b(cgcs_vector *self, int (^cmp_b)(const void *, const void *), cgcs_vector_iterator pos);

void cgcs_vheapsort(cgcs_vector *self, int (*cmpfn)(const void *, const void *));
void cgcs_vheapsort_b(cgcs_vector *self, int (^cmp_b)(const void *, const void *));

void cgcs_vheapsort_position(cgcs_vector *self, int (*cmpfn)(const void *, const void *), cgcs_vector_iterator pos);
void cgcs_vheapsort_position_b(cgcs_vector *self, int (^cmp_b)(const void *, const void *), cgcs_vector_iterator pos);

static cgcs_vector *cgcs_vnew(size_t capacity);
static cgcs_vector *cgcs_vnew_allocfn(size_t capacity, void *(*allocfn)(size_t));

static void cgcs_vdelete(cgcs_vector *self);
static void cgcs_vdelete_freefn(cgcs_vector *self, void (*freefn)(void *));

/*!
    \brief

    \param[in]  self

    \return

    \code
        cgcs_vector vec = CGCS_VECTOR_INITIALIZER;
        cgcs_vinit(&vec, capacity);
 
        char *str = NULL;
       
        str = strdup("front string"); // str is allocated by the caller
        cgcs_vpushb(&vec, &str);      // &str is a (char **)

        str = strdup("back string");
        cgcs_vpushb(&vec, &str);

        // Retrieve "front string".
        // cgcs_vfront returns a (void *), which makes this assignment possible.
        char **get = cgcs_vfront(&vec);

        // Run free_str on all pointers in vec.
        // free_str is a wrapper for free(), that deals with (char *).
        cgcs_vforeach(&vec, free_str);
        
        // Now that the memory addressed by each pointer is freed,
        // we can deinitialize vec.
        cgcs_vdeinit(&vec);
    \endcode
*/
static inline voidptr cgcs_vfront(cgcs_vector *self) {
    return self->m_impl.m_start;
}

/*!
    \brief

    \param[in]  self

    \return

    \code
        cgcs_vector vec = CGCS_VECTOR_INITIALIZER;
        cgcs_vinit(&vec, capacity);
 
        char *str = NULL;
       
        str = strdup("front string"); // str is allocated by the caller
        cgcs_vpushb(&vec, &str);      // &str is a (char **)

        str = strdup("back string");
        cgcs_vpushb(&vec, &str);

        // Retrieve "back string".
        // cgcs_vback returns a (void *), which makes this assignment possible.
        char **get = cgcs_vback(&vec);

        // Run free_str on all pointers in vec.
        // free_str is a wrapper for free(), that deals with (char *).
        cgcs_vforeach(&vec, free_str);
        
        // Now that the memory addressed by each pointer is freed,
        // we can deinitialize vec.
        cgcs_vdeinit(&vec);
    \endcode
*/
static inline voidptr cgcs_vback(cgcs_vector *self) {
    // m_finish is the address of one-past the last element;
    // we subtract m_finish by 1 to get the element at the back of cgcs_vector
    return self->m_impl.m_finish - 1;
}

/*!
    \brief

    \param[in]  self
    \param[in]  index

    \return

    \code
        cgcs_vector vec = CGCS_VECTOR_INITIALIZER;
        cgcs_vinit(&vec, capacity);
 
        char *str = NULL;
       
        str = strdup("front string"); // str is allocated by the caller
        cgcs_vpushb(&vec, &str);      // &str is a (char **)

        str = strdup("middle string");
        cgcs_vpushb(&vec, &str);

        str = strdup("back string");
        cgcs_vpushb(&vec, &str);

        // Retrieve "middle string"
        // cgcs_vat returns a (void *), which makes this assignment possible.
        char **get = cgcs_vat(&vec, 1);

        // Run free_str on all pointers in vec.
        // free_str is a wrapper for free(), that deals with (char *).
        cgcs_vforeach(&vec, free_str);
        
        // Now that the memory addressed by each pointer is freed,
        // we can deinitialize vec.
        cgcs_vdeinit(&vec);
    \endcode
*/
static inline voidptr cgcs_vat(cgcs_vector *self, const int index) {
    voidptr *result = self->m_impl.m_start + index;
    return result >= self->m_impl.m_finish ? NULL : result;
}

/*!
    \brief
 
    \param[in]  self
    \param[in]  index
 
    \return
 */
static inline voidptr cgcs_vi(cgcs_vector *self, size_t index) {
    return self->m_impl.m_start + index;
}

/*!
    \brief

    \param[in]  self

    \return
*/
static inline bool cgcs_vempty(cgcs_vector *self) {
    return self->m_impl.m_start == self->m_impl.m_finish;
}

/*!
    \brief

    \param[in]  self

    \return
*/
static inline size_t cgcs_vsize(cgcs_vector *self) {
    return self->m_impl.m_finish - self->m_impl.m_start;
}

/*!
    \brief

    \param[in]  self

    \return
*/
static inline size_t cgcs_vcapacity(cgcs_vector *self) {
    return self->m_impl.m_end_of_storage - self->m_impl.m_start;
}

/*!
    \brief

    \param[in]  self

    \return
*/
static inline cgcs_vector_iterator cgcs_vbegin(cgcs_vector *self) {
    // Same as vfront.
    return self->m_impl.m_start;
}

/*!
    \brief

    \param[in]  self

    \return
*/
static inline cgcs_vector_iterator cgcs_vend(cgcs_vector *self) {
    // Same as vback(self) + 1.
    return self->m_impl.m_finish;
}

/*!
    \brief

    \param[in]  self
    \param[in]  func
    \param[in]  beg
    \param[in]  end
*/
static inline void cgcs_vforeach_range(cgcs_vector *self,
                                           void (*func)(void *),
                                           cgcs_vector_iterator beg,
                                           cgcs_vector_iterator end) {
    while (beg < end) {
        func(beg++);
    }
}

static inline void cgcs_vforeach_range_b(cgcs_vector *self, void (^block)(void *),
                                  cgcs_vector_iterator beg, cgcs_vector_iterator end) {
    while (beg < end) {
        block(beg++);
    }
}

/*!
    \brief
 
    \param[in]  capacity
 
    \return
 */
static inline cgcs_vector *cgcs_vnew(size_t capacity) {
    cgcs_vector *v = NULL;
    
    if ((v = malloc(sizeof *v))) {
        cgcs_vinit(v, capacity);
    }
    
    return v;
}

/*!
    \brief
 
    \param[in]  capacity
    \param[in]  allocfn
 
    \return
 */
static inline cgcs_vector *cgcs_vnew_allocfn(size_t capacity, void *(*allocfn)(size_t)) {
    cgcs_vector *v = NULL;
    
    if ((v = allocfn(sizeof *v))) {
        cgcs_vinit_allocfn(v, capacity, allocfn);
    }
    
    return v;
}

/*!
    \brief
 
    \param[in]  self
 */
static inline void cgcs_vdelete(cgcs_vector *self) {
    cgcs_vdeinit(self);
    free(self);
}

/*!
    \brief
 
    \param[in]  self
    \param[in]  freefn
 */
static inline void cgcs_vdelete_freefn(cgcs_vector *self, void (*freefn)(void *)) {
    cgcs_vdeinit_freefn(self, freefn);
    freefn(self);
}

/*!
    \def    USE_CGCS_VECTOR
    \brief  Macro to omit 'cgcs_' namespace prefix for all cgcs_vector identifiers
    \details  Use '#define USE_CGCS_VECTOR' before #include "cgcs_vector.h"
 */
#ifdef USE_CGCS_VECTOR
# define USE_CGCS_VECTOR

typedef cgcs_vector vector;
typedef cgcs_vector_iterator vector_iterator;

#define vinit(self, capacity)               cgcs_vinit(self, capacity)
#define vinit_allocfn(self, capacity, allocfn)  cgcs_vinit_allocfn(self, capacity, allocfn)
#define vinit_alloc_b(self, capacity, alloc_b) cgcs_vinit_alloc_b(self, capacity, alloc_b)
#define vdeinit(self)                       cgcs_vdeinit(self)
#define vdeinit_freefn(self, freefn)        cgcs_vdeinit_freefn(self, freefn)
#define vdeinit_free_b(self, free_b)        cgcs_vdeinit_free_b(self, free_b)

#define vfront(self)                        cgcs_vfront(self)
#define vback(self)                         cgcs_vback(self)
#define vat(self, index)                    cgcs_vat(self, index)
#define vi(self, index)                     cgcs_vi(self, index)

#define vempty(self)                        cgcs_vempty(self)
#define vsize(self)                         cgcs_vsize(self)
#define vcapacity(self)                     cgcs_vcapacity(self)
#define vresize(self, n)                    cgcs_vresize(self, n)
#define vresize_allocfreefn(self, n, allocfn, freefn) cgcs_vresize_allocfreefn(self, n, allocfn, freefn)
#define vresize_allocfree_b(self, n, alloc_b, free_b) cgcs_vresize_allocfree_b(self, n, alloc_b, free_b)
#define vshrink_to_fit(self)                cgcs_vshrink_to_fit(self)
#define vshrink_to_fit_allocfreefn(self, allocfn, freefn) cgcs_vshrink_to_fit_allocfreefn(self, allocfn, freefn)
#define vshrink_to_fit_allocfree_b(self, alloc_b, free_b) cgcs_vshrink_to_fit_allocfree_b(self, alloc_b, free_b)

#define vbegin(self)                        cgcs_vbegin(self)
#define vend(self)                          cgcs_vend(self)

#define vinsert(self, it, addr)             cgcs_vinsert(self, it, addr)
#define vinsert_allocfreefn(self, it, addr, allocfn, freefn) cgcs_vinsert_allocfreefn(self, it, addr, allocfn, freefn)
#define vinsert_allocfree_b(self, it, addr, alloc_b, free_b) cgcs_vinsert_allocfree_b(self, it, addr, alloc_b, free_b)
#define vinsert_range(s, i, b, e)           cgcs_vinsert_range(s, i, b, e)
#define vinsert_range_allocfreefn(s, i, b, e, a, f) cgcs_vinsert_range_allocfreefn(s, i, b, e, a, f)
#define vinsert_range_allocfree_b(s, i, b, e, a, f) cgcs_vinsert_range_allocfree_b(s, i, b, e, a, f)
#define verase(self, it)                    cgcs_verase(self, it)
#define verase_range(s, b, e)               cgcs_verase_range(s, b, e)

#define vpushb(self, addr)                  cgcs_vpushb(self, addr)
#define vpushb_allocfreefn(self, addr, allocfn, freefn) cgcs_vpushb_allocfreefn(self, addr, allocfn, freefn)
#define vpushb_allocfree_b(self, addr, alloc_b, free_b) cgcs_vpushb_allocfree_b(self, addr, alloc_b, free_b)
#define vpopb(self)                         cgcs_vpopb(self)
#define vpushf(self, addr)                  cgcs_vpushf(self, addr)
#define vpushf_allocfreefn(self, addr, allocfn, freefn) cgcs_vpushf_allocfreefn(self, addr, allocfn, freefn)
#define vpushf_allocfree_b(self, addr, alloc_b, free_b) cgcs_vpushf_allocfree_b(self, addr, alloc_b, free_b)
#define vpopf(self)                         cgcs_vpopf(self)
#define vclear(self)                        cgcs_vclear(self)

#define vforeach(self, func)                cgcs_vforeach(self, func)
#define vforeach_b(self, block)             cgcs_vforeach_b(self, block)
#define vforeach_range(s, f, b, e)          cgcs_vforeach_range(s, f, b, e)
#define vforeach_range_b(self, block, beg, end) cgcs_foreach_range_b(self, block, beg, end)

#define vsearch(s, c, a)                    cgcs_vsearch(s, c, a)
#define vsearch_b(s, c, a)                  cgcs_vsearch(s, c, a)
#define vsearch_range(s, c, a, b, e)        cgcs_vsearch_range(s, c, a, b, e)
#define vsearch_range_b(s, c, a, b, e)      cgcs_vsearch_range_b(s, c, a, b, e)
#define vfind(self, cmpfn, addr)            cgcs_vfind(self, cmpfn, addr)
#define vfind_b(self, cmp_b, addr)          cgcs_vfind_b(self, cmp_b, addr)
#define vfind_range(s, c, a, b, e)          cgcs_vfind_range(s, c, a, b, e)
#define vfind_range_b(s, c, a, b, e)        cgcs_vfind_range_b(s, c, a, b, e)

#define vqsort(self, cmpfn)                 cgcs_vqsort(self, cmpfn)
#define vqsort_b(self, cmp_b)               cgcs_vqsort_b(self, cmp_b)
#define vqsort_position(s, c, p)            cgcs_vqsort_position(s, c, p)
#define vqsort_position_b(s, c, p)          cgcs_vqsort_position_b(s, c, p)

#define vmergesort(self, cmpfn)                 cgcs_vmergesort(self, cmpfn)
#define vmergesort_b(self, cmp_b)               cgcs_vmergesort_b(self, cmp_b)
#define vmergesort_position(s, c, p)            cgcs_vmergesort_position(s, c, p)
#define vmergesort_position_b(s, c, p)          cgcs_vmergesort_position_b(s, c, p)

#define vheapsort(self, cmpfn)                 cgcs_vheapsort(self, cmpfn)
#define vheapsort_b(self, cmp_b)               cgcs_vheapsort_b(self, cmp_b)
#define vheapsort_position(s, c, p)            cgcs_vheapsort_position(s, c, p)
#define vheapsort_position_b(s, c, p)          cgcs_vheapsort_position_b(s, c, p)

#define vnew(capacity)                      cgcs_vnew(capacity)
#define vnew_allocfn(capacity, allocfn)     cgcs_vnew_allocfn(capacity, allocfn)
#define vnew_alloc_b(capacity, alloc_b)     cgcs_vnew_alloc_b(capacity, alloc_b)
#define vdelete(self)                       cgcs_vdelete(self)
#define vdelete_freefn(self, freefn)        cgcs_vdelete_freefn(self, freefn)
#define vdelete_free_b(self, free_b)        cgcs_vdelete_free_b(self, free_b)

#endif /* USE_CGCS_VECTOR */

#endif /* CGCS_VECTOR_H */
