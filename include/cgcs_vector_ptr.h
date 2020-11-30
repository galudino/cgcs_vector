/*!
    \file       cgcs_vector_ptr.h
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

#ifndef CGCS_VECTOR_PTR_H
#define CGCS_VECTOR_PTR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

// TODO: Fill in all documentation stubs

/*!
    \typedef
    \brief
*/
typedef struct cgcs_vector_ptr cgcs_vptr_t;

/*!
    \typedef
    \brief
*/
typedef void *voidptr;

/*!
    \typedef
    \brief
*/
typedef voidptr *cgcs_vptr_iter_t;

/*!
    \struct
    \brief
*/
struct cgcs_vector_ptr {
    /*!
        \struct
        \brief
    */
    struct cgcs_vector_base_ptr {
        voidptr *m_start;
        voidptr *m_finish;
        voidptr *m_end_of_storage;
    } m_impl;
};

void cgcs_vptr_init(cgcs_vptr_t *self, size_t capacity);
void cgcs_vptr_deinit(cgcs_vptr_t *self);

static voidptr cgcs_vptr_front(cgcs_vptr_t *self);
static voidptr cgcs_vptr_back(cgcs_vptr_t *self);
static voidptr cgcs_vptr_at(cgcs_vptr_t *self, const int index);
static voidptr cgcs_vptr_i(cgcs_vptr_t *self, size_t index);

static bool cgcs_vptr_empty(cgcs_vptr_t *self);
static size_t cgcs_vptr_size(cgcs_vptr_t *self);
static size_t cgcs_vptr_capacity(cgcs_vptr_t *self);

bool cgcs_vptr_resize(cgcs_vptr_t *self, size_t n);
bool cgcs_vptr_shrink_to_fit(cgcs_vptr_t *self);

static cgcs_vptr_iter_t cgcs_vptr_begin(cgcs_vptr_t *self);
static cgcs_vptr_iter_t cgcs_vptr_end(cgcs_vptr_t *self);

cgcs_vptr_iter_t cgcs_vptr_insert(cgcs_vptr_t *self, cgcs_vptr_iter_t it,
                                  const void *valaddr);

cgcs_vptr_iter_t cgcs_vptr_insert_range(cgcs_vptr_t *self,
                                        cgcs_vptr_iter_t it,
                                        cgcs_vptr_iter_t beg,
                                        cgcs_vptr_iter_t end);

cgcs_vptr_iter_t cgcs_vptr_erase(cgcs_vptr_t *self, cgcs_vptr_iter_t it);

cgcs_vptr_iter_t cgcs_vptr_erase_range(cgcs_vptr_t *self, cgcs_vptr_iter_t beg,
                                       cgcs_vptr_iter_t end);

void cgcs_vptr_pushb(cgcs_vptr_t *self, const void *valaddr);
void cgcs_vptr_popb(cgcs_vptr_t *self);
void cgcs_vptr_pushf(cgcs_vptr_t *self, const void *valaddr);
void cgcs_vptr_popf(cgcs_vptr_t *self);

void cgcs_vptr_clear(cgcs_vptr_t *self);

void cgcs_vptr_foreach(cgcs_vptr_t *self, void (*func)(void *));

static void cgcs_vptr_foreach_range(cgcs_vptr_t *self, void (*func)(void *),
                             cgcs_vptr_iter_t beg, cgcs_vptr_iter_t end);

int cgcs_vptr_search(cgcs_vptr_t *self,
                     int (*cmpfn)(const void *, const void *),
                     const void *valaddr);

int cgcs_vptr_search_range(cgcs_vptr_t *self,
                           int (*cmpfn)(const void *, const void *),
                           const void *valaddr, cgcs_vptr_iter_t beg,
                           cgcs_vptr_iter_t end);

cgcs_vptr_iter_t cgcs_vptr_find(cgcs_vptr_t *self,
                                int (*cmpfn)(const void *, const void *),
                                const void *valaddr);

cgcs_vptr_iter_t cgcs_vptr_find_range(cgcs_vptr_t *self,
                                      int (*cmpfn)(const void *, const void *),
                                      const void *valaddr, cgcs_vptr_iter_t beg,
                                      cgcs_vptr_iter_t end);

void cgcs_vptr_qsort(cgcs_vptr_t *self,
                            int (*cmpfn)(const void *, const void *));

void cgcs_vptr_qsort_position(cgcs_vptr_t *self,
                                     int (*cmpfn)(const void *, const void *),
                                     cgcs_vptr_iter_t pos);

static cgcs_vptr_t *cgcs_vptr_new(size_t capacity);
static void cgcs_vptr_delete(cgcs_vptr_t *self);

/*!
    \brief

    \param[in]  self

    \return

    \code
        vptr_t vec = CGCS_VECTOR_PTR_INITIALIZER;
        cgcs_vptr_init(&vec, capacity);
 
        char *str = NULL;
       
        str = strdup("front string"); // str is allocated by the caller
        cgcs_vptr_pushb(&vec, &str);        // &str is a (char **)

        str = strdup("back string");
        cgcs_vptr_vpushb(&vec, &str);

        // Retrieve "front string".
        // cgcs_vptr_front returns a (void *), which makes this assignment possible.
        char **get = cgcs_vptr_front(&vec);

        // Run free_str on all pointers in vec.
        // free_str is a wrapper for free(), that deals with (char *).
        cgcs_vptr_foreach(&vec, free_str);
        
        // Now that the memory addressed by each pointer is freed,
        // we can deinitialize vec.
        cgcs_vptr_deinit(&vec);
    \endcode
*/
static inline voidptr cgcs_vptr_front(cgcs_vptr_t *self) {
    return self->m_impl.m_start;
}

/*!
    \brief

    \param[in]  self

    \return

    \code
     vptr_t vec = CGCS_VECTOR_PTR_INITIALIZER;
     cgcs_vptr_init(&vec, capacity);

     char *str = NULL;

     str = strdup("front string"); // str is allocated by the caller
     cgcs_vptr_pushb(&vec, &str);        // &str is a (char **)

     str = strdup("back string");
     cgcs_vptr_vpushb(&vec, &str);

     // Retrieve "back string".
     // cgcs_vptr_back returns a (void *), which makes this assignment possible.
     char **get = cgcs_vptr_back(&vec);

     // Run free_str on all pointers in vec.
     // free_str is a wrapper for free(), that deals with (char *).
     cgcs_vptr_foreach(&vec, free_str);
     
     // Now that the memory addressed by each pointer is freed,
     // we can deinitialize vec.
     cgcs_vptr_deinit(&vec);
    \endcode
*/
static inline voidptr cgcs_vptr_back(cgcs_vptr_t *self) {
    // m_finish is the address of one-past the last element;
    // we subtract m_finish by 1 to get the element at the back of vptr_t.
    return self->m_impl.m_finish - 1;
}

/*!
    \brief

    \param[in]  self
    \param[in]  index

    \return

    \code
        vptr_t vec = CGCS_VECTOR_PTR_INITIALIZER;
        cgcs_vptr_init(&vec, capacity);

        char *str = NULL;

        str = strdup("front string"); // str is allocated by the caller
        cgcs_vptr_pushb(&vec, &str);        // &str is a (char **)

        str = strdup("middle string");
        cgcs_vptr_pushb(&vec, &str);

        str = strdup("back string");
        cgcs_vptr_pushb(&vec, &str);

        // Retrieve "middle string".
        // cgcs_vptr_at returns a (void *), which makes this assignment possible.
        char **get = cgcs_vptr_at(&vec, 1);

        // Run free_str on all pointers in vec.
        // free_str is a wrapper for free().
        cgcs_vptr_foreach(&vec, free_str);
            
        // Now that the memory addressed by each pointer is freed,
        // we can deinitialize vec.
        cgcs_vptr_deinit(&vec);
    \endcode
*/
static inline voidptr cgcs_vptr_at(cgcs_vptr_t *self, const int index) {
    voidptr *result = self->m_impl.m_start + index;
    return result >= self->m_impl.m_finish ? NULL : result;
}

/*!
    \brief
 
    \param[in]  self
    \param[in]  index
 
    \return
 */
static inline voidptr cgcs_vptr_i(cgcs_vptr_t *self, size_t index) {
    return self->m_impl.m_start + index;
}

/*!
    \brief

    \param[in]  self

    \return
*/
static inline bool cgcs_vptr_empty(cgcs_vptr_t *self) {
    return self->m_impl.m_start == self->m_impl.m_finish;
}

/*!
    \brief

    \param[in]  self

    \return
*/
static inline size_t cgcs_vptr_size(cgcs_vptr_t *self) {
    return self->m_impl.m_finish - self->m_impl.m_start;
}

/*!
    \brief

    \param[in]  self

    \return
*/
static inline size_t cgcs_vptr_capacity(cgcs_vptr_t *self) {
    return self->m_impl.m_end_of_storage - self->m_impl.m_start;
}

/*!
    \brief

    \param[in]  self

    \return
*/
static inline cgcs_vptr_iter_t cgcs_vptr_begin(cgcs_vptr_t *self) {
    // Same as vptr_front.
    return self->m_impl.m_start;
}

/*!
    \brief

    \param[in]  self

    \return
*/
static inline cgcs_vptr_iter_t cgcs_vptr_end(cgcs_vptr_t *self) {
    // Same as vptr_back(self) + 1.
    return self->m_impl.m_finish;
}

/*!
    \brief

    \param[in]  self
    \param[in]  func
    \param[in]  beg
    \param[in]  end
*/
static inline void cgcs_vptr_foreach_range(cgcs_vptr_t *self,
                                           void (*func)(void *),
                                           cgcs_vptr_iter_t beg,
                                           cgcs_vptr_iter_t end) {
    while (beg < end) {
        func(beg++);
    }
}

/*!
    \brief
 
    \param[in]  capacity
 
    \return
 */
static inline cgcs_vptr_t *cgcs_vptr_new(size_t capacity) {
    cgcs_vptr_t *v = NULL;
    
    if ((v = malloc(sizeof *v))) {
        cgcs_vptr_init(v, capacity);
    }
    
    return v;
}

/*!
    \brief
 
    \param[in]  self
 */
static inline void cgcs_vptr_delete(cgcs_vptr_t *self) {
    cgcs_vptr_deinit(self);
    free(self);
}

/*!
    \def    using_namespace_cgcs
    \brief  Macro to omit 'cgcs_' namespace prefix for all cgcs identifiers
    \details  Use '#define using_namespace_cgcs' before #include "cgcs_vector_ptr.h"
 */
#ifdef using_namespace_cgcs
# define using_cgcs_vptr
# endif /* using namespace_cgcs */

/*!
    \def        using_cgcs_vptr
    \brief      Macro to omit 'cgcs_' namespace prefix, for brevity
    \details    Use '#define using_cgcs_vptr' before #include "cgcs_vector_ptr.h"
*/
#ifdef using_cgcs_vptr

typedef cgcs_vptr_t vptr_t;
typedef cgcs_vptr_iter_t vptr_iter_t;

#define vptr_init(self, capacity)               cgcs_vptr_init(self, capacity)
#define vptr_deinit(self)                       cgcs_vptr_deinit(self)

#define vptr_front(self)                        cgcs_vptr_front(self)
#define vptr_back(self)                         cgcs_vptr_back(self)
#define vptr_at(self, index)                    cgcs_vptr_at(self, index)
#define vptr_i(self, index)                     cgcs_vptr_i(self, index)

#define vptr_empty(self)                        cgcs_vptr_empty(self)
#define vptr_size(self)                         cgcs_vptr_size(self)
#define vptr_capacity(self)                     cgcs_vptr_capacity(self)
#define vptr_resize(self, n)                    cgcs_vptr_resize(self, n)
#define vptr_shrink_to_fit(self)                cgcs_vptr_shrink_to_fit(self)

#define vptr_begin(self)                        cgcs_vptr_begin(self)
#define vptr_end(self)                          cgcs_vptr_end(self)

#define vptr_insert(self, it, addr)             cgcs_vptr_insert(self, it, addr)
#define vptr_insert_range(s, i, b, e)         cgcs_vptr_insert_range(s, i, b, e)
#define vptr_erase(self, it)                    cgcs_vptr_erase(self, it)
#define vptr_erase_range(s, b, e)               cgcs_vptr_erase_range(s, b, e)

#define vptr_pushb(self, addr)                  cgcs_vptr_pushb(self, addr)
#define vptr_popb(self)                         cgcs_vptr_popb(self)
#define vptr_pushf(self, addr)                  cgcs_vptr_pushf(self, addr)
#define vptr_popf(self)                         cgcs_vptr_popf(self)
#define vptr_clear(self)                        cgcs_vptr_clear(self)

#define vptr_foreach(self, func)                cgcs_vptr_foreach(self, func)
#define vptr_foreach_range(s, f, b, e)       cgcs_vptr_foreach_range(s, f, b, e)

#define vptr_search(s, c, a)                    cgcs_vptr_search(s, c, a)
#define vptr_search_range(s, c, a, b, e)   cgcs_vptr_search_range(s, c, a, b, e)
#define vptr_find(self, cmpfn, addr)            cgcs_find(self, cmpfn, addr)
#define vptr_find_range(s, c, a, b, e)       cgcs_vptr_find_range(s, c, a, b, e)

#define vptr_qsort(self, cmpfn)                 cgcs_vptr_qsort(self, cmpfn)
#define vptr_qsort_position(s, c, p)           cgcs_vptr_qsort_position(s, c, p)

#define vptr_new(capacity)                      cgcs_vptr_new(capacity)
#define vptr_delete(self)                       cgcs_vptr_delete(self)

#endif /* using_cgcs_vptr */

#ifdef CGCS_VECTOR_PTR_SHORTNAMES

typedef vptr_iter_t viter_t;

#define vinit(self, capacity)                     vptr_init(self, capacity)
#define vdeinit(self)                             vptr_deinit(self)

#define vfront(self)                              vptr_front(self)
#define vback(self)                               vptr_back(self)
#define vat(self, index)                          vptr_at(self, index)

#define vi(self, index)                           vptr_i(self, index)

#define vempty(self)                              vptr_empty(self)
#define vsize(self)                               vptr_size(self)
#define vcapacity(self)                           vptr_capacity(self)
#define vresize(self, n)                          vptr_resize(self, n)

#define vbegin(self)                              vptr_begin(self)
#define vend(self)                                vptr_end(self)

#define vinsert(self, it, addr)                   vptr_insert(self, it, addr)
#define vinsert_range(s, i, b, e)                 vptr_insert_range(s, i, b, e)
#define verase(self, it)                          vptr_erase(self, it)
#define verase_range(s, b, e)                     vptr_erase_range(s, b, e)

#define vpushb(self, addr)                        vptr_pushb(self, addr)
#define vpopb(self)                               vptr_popb(self)
#define vpushf(self, addr)                        vptr_pushf(self, addr)
#define vpopf(self)                               vptr_popf(self)
#define vclear(self)                              vptr_clear(self)

#define vforeach(self, func)                      vptr_foreach(self, func)
#define vforeach_range(self, cmpfn, addr)  vptr_foreach_range(self, cmpfn, addr)

#define vsearch(self, cmpfn, addr)                vptr_search(self, cmpfn, addr)
#define vsearch_range(s, c, a, b, e)            vptr_search_range(s, c, a, b, e)
#define vfind(self, cmpfn, addr)                  vptr_find(self, cmpfn, addr)
#define vfind_range(s, c, a, b, e)                vptr_find_range(s, c, a, b, e)

#define vqsort(self, cmpfn)                       vptr_qsort(self, cmpfn)
#define vqsort_position(self, cmpfn, pos)  vptr_qsort_position(self, cmpfn, pos)

#define vnew(capacity)                            vptr_new(capacity)
#define vdelete(self)                             vptr_delete(self)

#endif /* CGCS_VECTOR_PTR__SHORT_FUNCNAMES */

#endif /* CGCS_VECTOR_PTR_H */
