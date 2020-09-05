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

cgcs_vptr_t *cgcs_vptr_init(cgcs_vptr_t *self, size_t capacity);
cgcs_vptr_t *cgcs_vptr_deinit(cgcs_vptr_t *self);

voidptr cgcs_vptr_front(cgcs_vptr_t *self);
voidptr cgcs_vptr_back(cgcs_vptr_t *self);
voidptr cgcs_vptr_at(cgcs_vptr_t *self, const int index);

#define cgcs_vptr_i(self, index) (((self)->(m_impl.m_start)) + (index))

bool cgcs_vptr_empty(cgcs_vptr_t *self);
size_t cgcs_vptr_size(cgcs_vptr_t *self);
size_t cgcs_vptr_capacity(cgcs_vptr_t *self);

bool cgcs_vptr_resize(cgcs_vptr_t *self, size_t n);
bool cgcs_vptr_shrink_to_fit(cgcs_vptr_t *self);

cgcs_vptr_iter_t cgcs_vptr_begin(cgcs_vptr_t *self);
cgcs_vptr_iter_t cgcs_vptr_end(cgcs_vptr_t *self);

cgcs_vptr_iter_t cgcs_vptr_insert(cgcs_vptr_t *self, cgcs_vptr_iter_t it,
                                  const void *valaddr);

cgcs_vptr_iter_t cgcs_vptr_insert_range(cgcs_vptr_t *self, cgcs_vptr_iter_t it,
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

void cgcs_vptr_foreach(cgcs_vptr_t *self, void *(*func)(void *));

void cgcs_vptr_foreach_range(cgcs_vptr_t *self, void *(*func)(void *),
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

/*!
    \define     vptr_new
    \brief      Macro to allocate and initialize a vptr_t on the heap
 */
#define cgcs_vptr_new(capacity) vptr_init(malloc(sizeof(vptr_t)), capacity)

/*!
    \define     vptr_delete
    \brief      Macro to destroy and deallocate a vptr_t from the heap
 */
#define cgcs_vptr_delete(self) free(vptr_deinit(self))

/*!
    \brief      Macro to omit 'cgcs_' namespace prefix, for brevity
    \details    Use '#define using_cgcs_vptr' before #include "vector_ptr.h"
*/
#ifdef using_cgcs_vptr

typedef cgcs_vptr_t vptr_t;
typedef cgcs_vptr_iter_t vptr_iter_t;

#define vptr_init cgcs_vptr_init
#define vptr_deinit cgcs_vptr_deinit

#define vptr_front cgcs_vptr_front
#define vptr_back cgcs_vptr_back
#define vptr_at cgcs_vptr_at
#define vptr_i cgcs_vptr_i

#define vptr_empty cgcs_vptr_empty
#define vptr_size cgcs_vptr_size
#define vptr_capacity cgcs_vptr_capacity
#define vptr_resize cgcs_vptr_resize

#define vptr_begin cgcs_vptr_begin
#define vptr_end cgcs_vptr_end

#define vptr_insert cgcs_vptr_insert
#define vptr_insert_range cgcs_vptr_insert_range
#define vptr_erase cgcs_vptr_erase
#define vptr_erase_range cgcs_vptr_erase_range

#define vptr_pushb cgcs_vptr_pushb
#define vptr_popb cgcs_vptr_popb
#define vptr_pushf cgcs_vptr_pushf
#define vptr_popf cgcs_vptr_popf
#define vptr_clear cgcs_vptr_clear

#define vptr_foreach cgcs_vptr_foreach
#define vptr_foreach_range cgcs_vptr_foreach_range

#define vptr_search cgcs_vptr_search
#define vptr_search_range cgcs_vptr_search_range
#define vptr_find cgcs_vptr_find
#define vptr_find_range cgcs_vptr_find_range

#define vptr_qsort cgcs_vptr_qsort
#define vptr_qsort_position cgcs_vptr_qsort_position

#define vptr_new cgcs_vptr_new
#define vptr_delete cgcs_vptr_delete

#endif /* using_cgcs_vptr */

#endif /* CGCS_VECTOR_PTR_H */
