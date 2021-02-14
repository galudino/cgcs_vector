/*!
    \file       cgcs_vector.c
    \brief      Source file for a vector ADT (elements of any pointer type)

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

// TODO: Fill in all documentation stubs

#include "cgcs_vector.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/*!
    \brief

    \param[in]  base
*/
static inline void
cgcs_vector_base_initialize(struct cgcs_vector_base *base) {
    base->m_start = NULL;
    base->m_finish = NULL;
    base->m_end_of_storage = NULL;
}

/*!
    \brief

    \param[in]  base
    \param[in]  capacity
*/
static inline void
cgcs_vector_base_new_block(struct cgcs_vector_base *base,
                               size_t capacity) {
    voidptr *start = calloc(capacity, sizeof *start);
    assert(start);

    base->m_start = start;
    base->m_finish = base->m_start;
    base->m_end_of_storage = base->m_start + capacity;
}

/*!
    \brief

    \param[in]  base
    \param[in]  capacity
    \param[in]  allocfn
 */
static inline void
cgcs_vector_base_new_block_allocfn(struct cgcs_vector_base *base,
                                    size_t capacity,
                                    void *(*allocfn)(size_t)) {
    voidptr *start = allocfn(sizeof *start * capacity);
    assert(start);
    memset(start, 0, sizeof *start * capacity);

    base->m_start = start;
    base->m_finish = base->m_start;
    base->m_end_of_storage = base->m_start + capacity;
}

/*!
    \brief

    \param[in]  base
    \param[in]  size
    \param[in]  capacity
*/
static inline void
cgcs_vector_base_resize_block(struct cgcs_vector_base *base,
                                  size_t size, size_t capacity) {
    voidptr *start = realloc(base->m_start, sizeof *start * capacity);
    assert(start);

    base->m_start = start;
    base->m_finish = base->m_start + size;
    base->m_end_of_storage = base->m_start + capacity;
}

/*!
    \brief

    \param[in]  base
    \param[in]  size
    \param[in]  capacity
    \param[in]  allocfn
    \param[in]  freefn
*/
static inline void
cgcs_vector_base_resize_block_allocfreefn(struct cgcs_vector_base *base,
                                  size_t size, size_t capacity,
                                  void *(*allocfn)(size_t), void (*freefn)(void *)) {
    voidptr *start = malloc(sizeof *start * capacity);
    assert(start);
    memcpy(start, base->m_start, sizeof *base->m_start * size);
    freefn(base->m_start);

    base->m_start = start;
    base->m_finish = base->m_start + size;
    base->m_end_of_storage = base->m_start + capacity;
}

/*!
    \brief

    \param[in]  base

    \return
*/
static inline bool
cgcs_vector_base_full_capacity(struct cgcs_vector_base *base) {
    return base->m_finish == base->m_end_of_storage;
}

/*!
    \brief

    \param[in]     self
    \param[in]     capacity
*/
void cgcs_vinit(cgcs_vector *self, size_t capacity) {
    cgcs_vector_base_initialize(&(self->m_impl));
    cgcs_vector_base_new_block(&(self->m_impl), capacity);
}

/*!
    \brief

    \param[in]     self
    \param[in]     capacity
*/
void cgcs_vinit_allocfn(cgcs_vector *self, size_t capacity, void *(*allocfn)(size_t)) {
    cgcs_vector_base_initialize(&(self->m_impl));
    cgcs_vector_base_new_block_allocfn(&(self->m_impl), capacity, allocfn);
}

/*!
    \brief

    \param[in]      self

    \return
*/
void cgcs_vdeinit(cgcs_vector *self) {
    // Pointees are not managed by deinit.
    //
    // If you want to free the memory addressed by each pointer
    // in vptr's buffer, run a "destroy" function on each element
    // using cgcs_vforeach -- or iterate over all elements manually
    // and free each pointer as needed.
    free(self->m_impl.m_start);
    cgcs_vector_base_initialize(&(self->m_impl));
}

/*!
    \brief

    \param[in]      self
    \param[in]      freefn

    \return
*/
void cgcs_vdeinit_freefn(cgcs_vector *self, void (*freefn)(void *)) {
    freefn(self->m_impl.m_start);
    cgcs_vector_base_initialize(&(self->m_impl));
}

/*!
    \brief

    \param[in]  self
    \param[in]  n

    \return
*/
bool cgcs_vresize(cgcs_vector *self, size_t n) {
    if (n <= cgcs_vcapacity(self)) {
        return false;
    } else {
        cgcs_vector_base_resize_block(&(self->m_impl), cgcs_vsize(self), n);
        return true;
    }
}

/*!
    \brief

    \param[in]  self
    \param[in]  n
    \param[in]  allocfn
    \param[in]  freefn

    \return
*/
bool cgcs_vresize_allocfreefn(cgcs_vector *self, size_t n, 
                              void *(*allocfn)(size_t), void (*freefn)(void *)) {
    if (n <= cgcs_vcapacity(self)) {
        return false;
    } else {
        cgcs_vector_base_resize_block_allocfreefn(&(self->m_impl), cgcs_vsize(self), n, 
                                                  allocfn, freefn);
        return true;
    }
}

/*!
    \brief

    \param[in]  self

    \return
*/
bool cgcs_vshrink_to_fit(cgcs_vector *self) {
    const size_t capacity = cgcs_vcapacity(self);
    const size_t size = cgcs_vsize(self);

    return (capacity > size) ? cgcs_vresize(self, size) : false;
}

/*!
    \brief

    \param[in]  self
    \param[in]  allocfn
    \param[in]  freefn

    \return
*/
bool cgcs_vshrink_to_fit_allocfreefn(cgcs_vector *self, 
                                     void *(*allocfn)(size_t), void (*freefn)(void *)) {
    const size_t capacity = cgcs_vcapacity(self);
    const size_t size = cgcs_vsize(self);

    return (capacity > size) ? cgcs_vresize_allocfreefn(self, size, allocfn, freefn) : false;
}

/*!
    \brief

    \param[in]  self
    \param[in]  it
    \param[in]  valaddr

    \return
*/
cgcs_vector_iterator cgcs_vinsert(cgcs_vector *self, cgcs_vector_iterator it, const void *valaddr) {
    if (cgcs_vector_base_full_capacity(&(self->m_impl))) {
        size_t position = it - self->m_impl.m_start;
        cgcs_vresize(self, cgcs_vcapacity(self) * 2);

        // it must be updated if this vector is resized,
        // since we use it's address in memmove.
        //
        // Without this reassignment, memmove will not work properly,
        // because it is assumed that it points to some address within
        // [ cgcs_vbegin(self), cgcs_vend(self) )
        it = self->m_impl.m_start + position;
    }

    // memmove(dst, src, block size)
    // We move everything from [it, m_finish) one block over right.
    memmove(it + 1, it, sizeof *it * (self->m_impl.m_finish - it));

    // We've made room for the new element, so we make the assignment now.
    *(it) = *(void **)(valaddr);

    // Finally, we advance the m_finish address one block.
    ++self->m_impl.m_finish;

    return it;
}

/*!
    \brief

    \param[in]  self
    \param[in]  it
    \param[in]  valaddr
    \param[in]  allocfn
    \param[in]  freefn

    \return
*/
cgcs_vector_iterator cgcs_vinsert_allocfreefn(cgcs_vector *self, cgcs_vector_iterator it,
                                          const void *valaddr, 
                                          void *(*allocfn)(size_t), void (*freefn)(void *)) {    
    if (cgcs_vector_base_full_capacity(&(self->m_impl))) {
        size_t position = it - self->m_impl.m_start;
        cgcs_vresize_allocfreefn(self, cgcs_vcapacity(self) * 2, allocfn, freefn);

        // it must be updated if this vector is resized,
        // since we use it's address in memmove.
        //
        // Without this reassignment, memmove will not work properly,
        // because it is assumed that it points to some address within
        // [ cgcs_vbegin(self), cgcs_vend(self) )
        it = self->m_impl.m_start + position;
    }

    // memmove(dst, src, block size)
    // We move everything from [it, m_finish) one block over right.
    memmove(it + 1, it, sizeof *it * (self->m_impl.m_finish - it));

    // We've made room for the new element, so we make the assignment now.
    *(it) = *(void **)(valaddr);

    // Finally, we advance the m_finish address one block.
    ++self->m_impl.m_finish;

    return it;
}

/*!
    \brief

    \param[in]  self
    \param[in]  it
    \param[in]  beg
    \param[in]  end

    \return
*/
cgcs_vector_iterator cgcs_vinsert_range(cgcs_vector *self, cgcs_vector_iterator it, cgcs_vector_iterator beg,
                              cgcs_vector_iterator end) {
    const size_t count = end - beg;
    size_t curr_capacity = cgcs_vcapacity(self);

    if (cgcs_vsize(self) + count > curr_capacity) {
        size_t position = it - self->m_impl.m_start;
        cgcs_vresize(self, curr_capacity * 2);

        // See cgcs_vinsert on why we update it
        // if we resize the buffer.
        it = self->m_impl.m_start + position;
    }

    // memmove(dst, src, block size)
    // We move everything from [it, m_finish) (m_finish - it) blocks over right.
    memmove(it + count, it, sizeof *it * (self->m_impl.m_finish - it));

    // Now we copy the contents in range [beg, end) at position it.
    memcpy(it, beg, sizeof *it * count);

    // Finally, we advance the m_finish address count blocks.
    self->m_impl.m_finish += count;

    return it;
}

/*!
    \brief

    \param[in]  self
    \param[in]  it
    \param[in]  beg
    \param[in]  end
    \param[in]  allocfn
    \param[in]  freefn

    \return
*/
cgcs_vector_iterator cgcs_vinsert_range_allocfreefn(cgcs_vector *self,
                                        cgcs_vector_iterator it,
                                        cgcs_vector_iterator beg,
                                        cgcs_vector_iterator end,
                                        void *(*allocfn)(size_t),
                                        void (*freefn)(void *)) {
    const size_t count = end - beg;
    size_t curr_capacity = cgcs_vcapacity(self);

    if (cgcs_vsize(self) + count > curr_capacity) {
        size_t position = it - self->m_impl.m_start;
        cgcs_vresize_allocfreefn(self, curr_capacity * 2, allocfn, freefn);

        // See cgcs_vinsert on why we update it
        // if we resize the buffer.
        it = self->m_impl.m_start + position;
    }

    // memmove(dst, src, block size)
    // We move everything from [it, m_finish) (m_finish - it) blocks over right.
    memmove(it + count, it, sizeof *it * (self->m_impl.m_finish - it));

    // Now we copy the contents in range [beg, end) at position it.
    memcpy(it, beg, sizeof *it * count);

    // Finally, we advance the m_finish address count blocks.
    self->m_impl.m_finish += count;

    return it;}

/*!
    \brief

    \param[in]  self
    \param[in]  it

    \return
*/
cgcs_vector_iterator cgcs_verase(cgcs_vector *self, cgcs_vector_iterator it) {
    if (cgcs_vempty(self) == false) {
        const size_t move_element_count = self->m_impl.m_finish - it;
        // memmove(dst, src, block size)
        // We move everything from [it + 1, m_finish) one block over to the left.
        memmove(it, it + 1, sizeof *it * move_element_count);

        // Finally, we decrement the m_finish address one block.
        --self->m_impl.m_finish;
    }

    return it;
}

/*!
    \brief

    \param[in]  self
    \param[in]  beg
    \param[in]  end

    \return
*/
cgcs_vector_iterator cgcs_verase_range(cgcs_vector *self, cgcs_vector_iterator beg, cgcs_vector_iterator end) {
    if (cgcs_vempty(self) == false) {
        const size_t move_element_count = self->m_impl.m_finish - beg;
        const size_t count = end - beg;
        // memmove(dst, src, block size)
        // We move everything from [end, m_finish) one block over to the left.
        memmove(beg, end, sizeof *beg * move_element_count);

        // Finally, we decrement the m_finish address count blocks.
        self->m_impl.m_finish -= count;
    }

    return beg;
}

/*!
    \brief

    \param[in]  self
    \param[in]  valaddr
*/
void cgcs_vpushb(cgcs_vector *self, const void *valaddr) {
    if (cgcs_vector_base_full_capacity(&(self->m_impl))) {
        cgcs_vresize(self, cgcs_vcapacity(self) * 2);
    }

    *(self->m_impl.m_finish++) = *(void **)(valaddr);
}

/*!
    \brief

    \param[in]  self
    \param[in]  valaddr
    \param[in]  allocfn
    \param[in]  freefn
*/
void cgcs_vpushb_allocfreefn(cgcs_vector *self, const void *valaddr, 
                             void *(*allocfn)(size_t), void (*freefn)(void *)) {
    if (cgcs_vector_base_full_capacity(&(self->m_impl))) {
        cgcs_vresize_allocfreefn(self, cgcs_vcapacity(self) * 2, allocfn, freefn);
    }

    *(self->m_impl.m_finish++) = *(void **)(valaddr);
}

/*!
    \brief

    \param[in]  self
*/
void cgcs_vpopb(cgcs_vector *self) {
    if (cgcs_vempty(self) == false) {
        // We simply move m_finish one block to the left.
        --self->m_impl.m_finish;
    }
}

/*!
    \brief

    \param[in]  self
    \param[in]  valaddr
*/
void cgcs_vpushf(cgcs_vector *self, const void *valaddr) {
    if (cgcs_vector_base_full_capacity(&(self->m_impl))) {
        cgcs_vresize(self, cgcs_vcapacity(self) * 2);
    }

    // memmove(dst, src, block size)    
    memmove(self->m_impl.m_start + 1, 
            self->m_impl.m_start, 
            sizeof *self->m_impl.m_start * cgcs_vsize(self));

    *(self->m_impl.m_start) = *(void **)(valaddr);
    ++self->m_impl.m_finish;
}

/*!
    \brief

    \param[in]  self
    \param[in]  valaddr
    \param[in]  allocfn
    \param[in]  freefn
*/
void cgcs_vpushf_allocfreefn(cgcs_vector *self, const void *valaddr,
                             void *(*allocfn)(size_t), void (*freefn)(void *)) {
    if (cgcs_vector_base_full_capacity(&(self->m_impl))) {
        cgcs_vresize_allocfreefn(self, cgcs_vcapacity(self) * 2, allocfn, freefn);
    }

    // memmove(dst, src, block size)    
    memmove(self->m_impl.m_start + 1, 
            self->m_impl.m_start, 
            sizeof *self->m_impl.m_start * cgcs_vsize(self));

    *(self->m_impl.m_start) = *(void **)(valaddr);
    ++self->m_impl.m_finish;
}

/*!
    \brief

    \param[in]  self
*/
void cgcs_vpopf(cgcs_vector *self) {
    if (cgcs_vempty(self) == false) {
        // memmove(dst, src, block size)
        // We move everything from [m_start + 1, m_finish) one block over to the left.
        memmove(self->m_impl.m_start, 
                self->m_impl.m_start + 1, 
                sizeof *self->m_impl.m_start * cgcs_vsize(self) - 1);

        // Finally, we decrement the m_finish address one block.
        --self->m_impl.m_finish;
    }
}

/*!
    \brief

    \param[in]  self
*/
void cgcs_vclear(cgcs_vector *self) {
    memset(self->m_impl.m_start, '\0',
           cgcs_vsize(self) * sizeof *self->m_impl.m_start);
    self->m_impl.m_finish = self->m_impl.m_start;
}

/*!
    \brief

    \param[in]  self
    \param[in]  func
*/
void cgcs_vforeach(cgcs_vector *self, void (*func)(void *)) {
    cgcs_vector_iterator it = cgcs_vbegin(self);
    cgcs_vector_iterator end = cgcs_vend(self);

    while (it < end) {
        func(it++);
    }
}

/*!
    \brief

    \param[in]  self
    \param[in]  cmpfn
    \param[in]  valaddr

    \return
*/
int cgcs_vsearch(cgcs_vector *self, int (*cmpfn)(const void *, const void *),
                const void *valaddr) {
    cgcs_vector_iterator it = cgcs_vbegin(self);
    cgcs_vector_iterator end = cgcs_vend(self);

    while (it < end) {
        if (cmpfn(it++, valaddr) == 0) {
            break;
        }
    }

    return it == end ? (-1) : (int)(end - it);
}

/*!
    \brief

    \param[in]  self
    \param[in]  cmpfn
    \param[in]  valaddr
    \param[in]  beg
    \param[in]  end

    \return
*/
int cgcs_vsearch_range(cgcs_vector *self, int (*cmpfn)(const void *, const void *),
                      const void *valaddr, cgcs_vector_iterator beg, cgcs_vector_iterator end) {
    cgcs_vector_iterator self_end = cgcs_vend(self);

    while (beg < end) {
        if (cmpfn(beg++, valaddr) == 0) {
            break;
        }
    }

    return (beg == end) ? -1 : (int)(self_end - beg);
}

/*!
    \brief

    \param[in]  self
    \param[in]  cmpfn
    \param[in]  valaddr

    \return
*/
cgcs_vector_iterator cgcs_vfind(cgcs_vector *self, int (*cmpfn)(const void *, const void *),
                      const void *valaddr) {
    cgcs_vector_iterator it = cgcs_vbegin(self);
    cgcs_vector_iterator end = cgcs_vend(self);

    while (it < end) {
        if (cmpfn(it++, valaddr) == 0) {
            break;
        }
    }

    return (it == end) ? NULL : it;
}

/*!
    \brief

    \param[in]  self
    \param[in]  cmpfn
    \param[in]  valaddr
    \param[in]  beg
    \param[in]  end

    \return
*/
cgcs_vector_iterator cgcs_vfind_range(cgcs_vector *self,
                            int (*cmpfn)(const void *, const void *),
                            const void *valaddr, cgcs_vector_iterator beg,
                            cgcs_vector_iterator end) {
    while (beg < end) {
        if (cmpfn(beg++, valaddr) == 0) {
            break;
        }
    }

    return (beg == end) ? NULL : beg;
}


/*!
    \brief

    \param[in]  self
    \param[in]  cmpfn
*/
void cgcs_vqsort(cgcs_vector *self, int (*cmpfn)(const void *, const void *)) {
    qsort(self->m_impl.m_start,
          cgcs_vsize(self),
          sizeof *self->m_impl.m_start,
          cmpfn);
}

/*!
    \brief

    \param[in]  self
    \param[in]  cmpfn
    \param[in]  pos
*/
void cgcs_vqsort_position(cgcs_vector *self, int (*cmpfn)(const void *, const void *),
                         cgcs_vector_iterator pos) {
    qsort(pos,
          self->m_impl.m_finish - pos,
          sizeof *self->m_impl.m_start,
          cmpfn);
}
