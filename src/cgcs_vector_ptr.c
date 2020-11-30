/*!
    \file       cgcs_vector_ptr.c
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

#define using_cgcs_vptr
#include "cgcs_vector_ptr.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static void
cgcs_vector_base_ptr_initialize(struct cgcs_vector_base_ptr *base);
static void
cgcs_vector_base_ptr_new_block(struct cgcs_vector_base_ptr *base,
                               size_t capacity);
static void
cgcs_vector_base_ptr_resize_block(struct cgcs_vector_base_ptr *base,
                                  size_t size, size_t new_capacity);
static bool
cgcs_vector_base_ptr_full_capacity(struct cgcs_vector_base_ptr *base);

/*!
    \brief

    \param[in]     self
    \param[in]     capacity
*/
void vptr_init(vptr_t *self, size_t capacity) {
    cgcs_vector_base_ptr_initialize(&(self->m_impl));
    cgcs_vector_base_ptr_new_block(&(self->m_impl), capacity);
}

/*!
    \brief

    \param[in]      self

    \return
*/
void vptr_deinit(vptr_t *self) {
    // Pointees are not managed by deinit.
    //
    // If you want to free the memory addressed by each pointer
    // in vptr's buffer, run a "destroy" function on each element
    // using vptr_foreach -- or iterate over all elements manually
    // and free each pointer as needed.
    free(self->m_impl.m_start);
    cgcs_vector_base_ptr_initialize(&(self->m_impl));
}

/*!
    \brief

    \param[in]  self
    \param[in]  n

    \return
*/
bool vptr_resize(vptr_t *self, size_t n) {
    if (n <= vptr_capacity(self)) {
        return false;
    } else {
        cgcs_vector_base_ptr_resize_block(&(self->m_impl), vptr_size(self), n);
        return true;
    }
}

/*!
    \brief

    \param[in]  self

    \return
*/
bool vptr_shrink_to_fit(vptr_t *self) {
    const size_t capacity = vptr_capacity(self);
    const size_t size = vptr_size(self);

    return (capacity > size) ? vptr_resize(self, size) : false;
}

/*!
    \brief

    \param[in]  self
    \param[in]  it
    \param[in]  valaddr

    \return
*/
vptr_iter_t vptr_insert(vptr_t *self, vptr_iter_t it, const void *valaddr) {
    if (cgcs_vector_base_ptr_full_capacity(&(self->m_impl))) {
        size_t position = it - self->m_impl.m_start;
        vptr_resize(self, vptr_capacity(self) * 2);

        // it must be updated if this vector is resized,
        // since we use it's address in memmove.
        //
        // Without this reassignment, memmove will not work properly,
        // because it is assumed that it points to some address within
        // [ vptr_begin(self), vptr_end(self) )
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
vptr_iter_t vptr_insert_range(vptr_t *self, vptr_iter_t it, vptr_iter_t beg,
                              vptr_iter_t end) {
    const size_t count = end - beg;
    size_t curr_capacity = vptr_capacity(self);

    if (vptr_size(self) + count > curr_capacity) {
        size_t position = it - self->m_impl.m_start;
        vptr_resize(self, curr_capacity * 2);

        // See vptr_insert on why we update it
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

    \return
*/
vptr_iter_t vptr_erase(vptr_t *self, vptr_iter_t it) {
    if (vptr_empty(self) == false) {
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
vptr_iter_t vptr_erase_range(vptr_t *self, vptr_iter_t beg, vptr_iter_t end) {
    if (vptr_empty(self) == false) {
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
void vptr_pushb(vptr_t *self, const void *valaddr) {
    if (cgcs_vector_base_ptr_full_capacity(&(self->m_impl))) {
        vptr_resize(self, vptr_capacity(self) * 2);
    }

    *(self->m_impl.m_finish++) = *(void **)(valaddr);
}

/*!
    \brief

    \param[in]  self
*/
void vptr_popb(vptr_t *self) {
    if (vptr_empty(self) == false) {
        // We simply move m_finish one block to the left.
        --self->m_impl.m_finish;
    }
}

/*!
    \brief

    \param[in]  self
    \param[in]  valaddr
*/
void vptr_pushf(vptr_t *self, const void *valaddr) {
    if (cgcs_vector_base_ptr_full_capacity(&(self->m_impl))) {
        vptr_resize(self, vptr_capacity(self) * 2);
    }

    // memmove(dst, src, block size)    
    memmove(self->m_impl.m_start + 1, 
            self->m_impl.m_start, 
            sizeof *self->m_impl.m_start * vptr_size(self));

    *(self->m_impl.m_start) = *(void **)(valaddr);
    ++self->m_impl.m_finish;
}

/*!
    \brief

    \param[in]  self
*/
void vptr_popf(vptr_t *self) {
    if (vptr_empty(self) == false) {
        // memmove(dst, src, block size)
        // We move everything from [m_start + 1, m_finish) one block over to the left.
        memmove(self->m_impl.m_start, 
                self->m_impl.m_start + 1, 
                sizeof *self->m_impl.m_start * vptr_size(self) - 1);

        // Finally, we decrement the m_finish address one block.
        --self->m_impl.m_finish;
    }
}

/*!
    \brief

    \param[in]  self
*/
void vptr_clear(vptr_t *self) {
    memset(self->m_impl.m_start, '\0',
           vptr_size(self) * sizeof *self->m_impl.m_start);
    self->m_impl.m_finish = self->m_impl.m_start;
}

/*!
    \brief

    \param[in]  self
    \param[in]  func
*/
void vptr_foreach(vptr_t *self, void (*func)(void *)) {
    vptr_iter_t it = vptr_begin(self);
    vptr_iter_t end = vptr_end(self);

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
int vptr_search(vptr_t *self, int (*cmpfn)(const void *, const void *),
                const void *valaddr) {
    vptr_iter_t it = vptr_begin(self);
    vptr_iter_t end = vptr_end(self);

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
int vptr_search_range(vptr_t *self, int (*cmpfn)(const void *, const void *),
                      const void *valaddr, vptr_iter_t beg, vptr_iter_t end) {
    vptr_iter_t self_end = vptr_end(self);

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
vptr_iter_t vptr_find(vptr_t *self, int (*cmpfn)(const void *, const void *),
                      const void *valaddr) {
    vptr_iter_t it = vptr_begin(self);
    vptr_iter_t end = vptr_end(self);

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
vptr_iter_t vptr_find_range(vptr_t *self,
                            int (*cmpfn)(const void *, const void *),
                            const void *valaddr, vptr_iter_t beg,
                            vptr_iter_t end) {
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
void vptr_qsort(vptr_t *self, int (*cmpfn)(const void *, const void *)) {
    qsort(self->m_impl.m_start,
          vptr_size(self),
          sizeof *self->m_impl.m_start,
          cmpfn);
}

/*!
    \brief

    \param[in]  self
    \param[in]  cmpfn
    \param[in]  pos
*/
void vptr_qsort_position(vptr_t *self, int (*cmpfn)(const void *, const void *),
                         vptr_iter_t pos) {
    qsort(pos,
          self->m_impl.m_finish - pos,
          sizeof *self->m_impl.m_start,
          cmpfn);
}


/*!
    \brief

    \param[in]  base
*/
static inline void
cgcs_vector_base_ptr_initialize(struct cgcs_vector_base_ptr *base) {
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
cgcs_vector_base_ptr_new_block(struct cgcs_vector_base_ptr *base,
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
    \param[in]  size
    \param[in]  capacity
*/
static inline void
cgcs_vector_base_ptr_resize_block(struct cgcs_vector_base_ptr *base,
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

    \return
*/
static inline bool
cgcs_vector_base_ptr_full_capacity(struct cgcs_vector_base_ptr *base) {
    return base->m_finish == base->m_end_of_storage;
}
