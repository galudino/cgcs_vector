/*!
    \file       cgcs_vector.c
    \brief      Source file for a vector ADT (elements of any pointer type)

    \author     Gemuele Aludino
    \date       05 Sep 2020
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
    base->m_start = allocfn(sizeof base->m_start * capacity);
    assert(base->m_start);
    memset(base->m_start, 0, sizeof *base->m_start * capacity);

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
    voidptr *old_start = base->m_start;

    base->m_start = allocfn(sizeof *base->m_start * capacity);
    assert(base->m_start);
    memcpy(base->m_start, old_start, sizeof *old_start * size);

    freefn(old_start);

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
void vector_init(vector_t *self, size_t capacity) {
    cgcs_vector_base_initialize(&(self->m_impl));
    cgcs_vector_base_new_block(&(self->m_impl), capacity);
}

/*!
    \brief

    \param[in]     self
    \param[in]     capacity
*/
void vector_init_alloc_fn(vector_t *self, size_t capacity, void *(*allocfn)(size_t)) {
    cgcs_vector_base_initialize(&(self->m_impl));
    cgcs_vector_base_new_block_allocfn(&(self->m_impl), capacity, allocfn);
}

/*!
    \brief

    \param[in]      self

    \return
*/
void vector_deinit(vector_t *self) {
    // Pointees are not managed by deinit.
    //
    // If you want to free the memory addressed by each pointer
    // in vptr's buffer, run a "destroy" function on each element
    // using vector_foreach -- or iterate over all elements manually
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
void vector_deinit_free_fn(vector_t *self, void (*freefn)(void *)) {
    freefn(self->m_impl.m_start);
    cgcs_vector_base_initialize(&(self->m_impl));
}

/*!
    \brief

    \param[in]  self
    \param[in]  n

    \return
*/
bool vector_resize(vector_t *self, size_t n) {
    if (n <= vector_capacity(self)) {
        return false;
    } else {
        cgcs_vector_base_resize_block(&(self->m_impl), vector_size(self), n);
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
bool vector_resize_alloc_free_fn(vector_t *self, size_t n, 
                              void *(*allocfn)(size_t), void (*freefn)(void *)) {
    if (n <= vector_capacity(self)) {
        return false;
    } else {
        cgcs_vector_base_resize_block_allocfreefn(&(self->m_impl), vector_size(self), n, 
                                                  allocfn, freefn);
        return true;
    }
}

/*!
    \brief

    \param[in]  self

    \return
*/
bool vector_shrink_to_fit(vector_t *self) {
    const size_t capacity = vector_capacity(self);
    const size_t size = vector_size(self);

    return (capacity > size) ? vector_resize(self, size) : false;
}

/*!
    \brief

    \param[in]  self
    \param[in]  allocfn
    \param[in]  freefn

    \return
*/
bool vector_shrink_to_fit_alloc_free_fn(vector_t *self, 
                                     void *(*allocfn)(size_t), void (*freefn)(void *)) {
    const size_t capacity = vector_capacity(self);
    const size_t size = vector_size(self);

    return (capacity > size) ? vector_resize_alloc_free_fn(self, size, allocfn, freefn) : false;
}

/*!
    \brief

    \param[in]  self
    \param[in]  it
    \param[in]  valaddr

    \return
*/
vector_iterator_t vector_insert(vector_t *self, vector_iterator_t it, const void *valaddr) {
    if (cgcs_vector_base_full_capacity(&(self->m_impl))) {
        size_t position = it - self->m_impl.m_start;
        vector_resize(self, vector_capacity(self) * 2);

        // it must be updated if this vector is resized,
        // since we use it's address in memmove.
        //
        // Without this reassignment, memmove will not work properly,
        // because it is assumed that it points to some address within
        // [ vector_begin(self), vector_end(self) )
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
vector_iterator_t vector_insert_alloc_free_fn(vector_t *self, vector_iterator_t it,
                                          const void *valaddr, 
                                          void *(*allocfn)(size_t), void (*freefn)(void *)) {    
    if (cgcs_vector_base_full_capacity(&(self->m_impl))) {
        size_t position = it - self->m_impl.m_start;
        vector_resize_alloc_free_fn(self, vector_capacity(self) * 2, allocfn, freefn);

        // it must be updated if this vector is resized,
        // since we use it's address in memmove.
        //
        // Without this reassignment, memmove will not work properly,
        // because it is assumed that it points to some address within
        // [ vector_begin(self), vector_end(self) )
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
vector_iterator_t vector_insert_range(vector_t *self, vector_iterator_t it, vector_iterator_t beg,
                              vector_iterator_t end) {
    const size_t count = end - beg;
    size_t curr_capacity = vector_capacity(self);

    if (vector_size(self) + count > curr_capacity) {
        size_t position = it - self->m_impl.m_start;
        vector_resize(self, curr_capacity * 2);

        // See vector_insert on why we update it
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
vector_iterator_t vector_insert_range_alloc_free_fn(vector_t *self,
                                        vector_iterator_t it,
                                        vector_iterator_t beg,
                                        vector_iterator_t end,
                                        void *(*allocfn)(size_t),
                                        void (*freefn)(void *)) {
    const size_t count = end - beg;
    size_t curr_capacity = vector_capacity(self);

    if (vector_size(self) + count > curr_capacity) {
        size_t position = it - self->m_impl.m_start;
        vector_resize_alloc_free_fn(self, curr_capacity * 2, allocfn, freefn);

        // See vector_insert on why we update it
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
vector_iterator_t vector_erase(vector_t *self, vector_iterator_t it) {
    if (vector_empty(self) == false) {
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
vector_iterator_t vector_erase_range(vector_t *self, vector_iterator_t beg, vector_iterator_t end) {
    if (vector_empty(self) == false) {
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
void vector_push_back(vector_t *self, const void *valaddr) {
    if (cgcs_vector_base_full_capacity(&(self->m_impl))) {
        vector_resize(self, vector_capacity(self) * 2);
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
void vector_push_back_alloc_free_fn(vector_t *self, const void *valaddr, 
                             void *(*allocfn)(size_t), void (*freefn)(void *)) {
    if (cgcs_vector_base_full_capacity(&(self->m_impl))) {
        vector_resize_alloc_free_fn(self, vector_capacity(self) * 2, allocfn, freefn);
    }

    *(self->m_impl.m_finish++) = *(void **)(valaddr);
}

/*!
    \brief

    \param[in]  self
*/
void vector_pop_back(vector_t *self) {
    if (vector_empty(self) == false) {
        // We simply move m_finish one block to the left.
        --self->m_impl.m_finish;
    }
}

/*!
    \brief

    \param[in]  self
*/
void vector_clear(vector_t *self) {
    memset(self->m_impl.m_start, '\0',
           vector_size(self) * sizeof *self->m_impl.m_start);
    self->m_impl.m_finish = self->m_impl.m_start;
}

/*!
    \brief

    \param[in]  self
    \param[in]  func
*/
void vector_foreach(vector_t *self, void (*func)(void *)) {
    vector_iterator_t it = vector_begin(self);
    vector_iterator_t end = vector_end(self);

    for (; it < end; it++) {
        func(it);
    }
}

void vector_foreach_b(vector_t *self, void (^block)(void *)) {
    vector_iterator_t it = vector_begin(self);
    vector_iterator_t end = vector_end(self);

    for (; it < end; it++) {
        block(it);
    }
}

/*!
    \brief

    \param[in]  self
    \param[in]  cmpfn
    \param[in]  valaddr

    \return
*/
int vector_search(vector_t *self, int (*cmpfn)(const void *, const void *),
                const void *valaddr) {
    vector_iterator_t it = vector_begin(self);
    vector_iterator_t end = vector_end(self);

    for (; it < end; it++) {
        if (cmpfn(it, valaddr) == 0) {
            break;
        }
    }

    return it == end ? (-1) : (int)(end - it);
}

int vector_search_b(vector_t *self, int (^cmp_b)(const void *, const void *), const void *valaddr) {
    vector_iterator_t it = vector_begin(self);
    vector_iterator_t end = vector_end(self);

    for (; it < end; it++) {
        if (cmp_b(it, valaddr) == 0) {
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
int vector_search_range(vector_t *self, int (*cmpfn)(const void *, const void *),
                      const void *valaddr, vector_iterator_t beg, vector_iterator_t end) {
    vector_iterator_t self_end = vector_end(self);

    for (; beg < end; beg++) {
        if (cmpfn(beg++, valaddr) == 0) {
            break;
        }
    }

    return (beg == end) ? -1 : (int)(self_end - beg);
}

int vector_search_range_b(vector_t *self,
                       int (^cmp_b)(const void *, const void *),
                       const void *valaddr, vector_iterator_t beg,
                       vector_iterator_t end) {
    vector_iterator_t self_end = vector_end(self);

    for (; beg < end; beg++) {
        if (cmp_b(beg++, valaddr) == 0) {
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
vector_iterator_t vector_find(vector_t *self, int (*cmpfn)(const void *, const void *),
                      const void *valaddr) {
    vector_iterator_t it = vector_begin(self);
    vector_iterator_t end = vector_end(self);

    for (; it < end; it++) {
        if (cmpfn(it, valaddr) == 0) {
            break;
        }
    }

    return (it == end) ? NULL : it;
}

vector_iterator_t vector_find_b(vector_t *self,
                                int (^cmp_b)(const void *, const void *),
                                const void *valaddr) {
    vector_iterator_t it = vector_begin(self);
    vector_iterator_t end = vector_end(self);

    for (; it < end; it++) {
        if (cmp_b(it, valaddr) == 0) {
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
vector_iterator_t vector_find_range(vector_t *self,
                            int (*cmpfn)(const void *, const void *),
                            const void *valaddr, vector_iterator_t beg,
                            vector_iterator_t end) {
    for (; beg < end; beg++) {
        if (cmpfn(beg, valaddr) == 0) {
            break;
        }
    }

    return (beg == end) ? NULL : beg;
}

vector_iterator_t vector_find_range_b(vector_t *self,
                                      int (^cmp_b)(const void *, const void *),
                                      const void *valaddr, vector_iterator_t beg,
                                      vector_iterator_t end) {
    for (; beg < end; beg++) {
        if (cmp_b(beg, valaddr) == 0) {
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
void vector_qsort(vector_t *self, int (*cmpfn)(const void *, const void *)) {
    qsort(self->m_impl.m_start,
          vector_size(self),
          sizeof *self->m_impl.m_start,
          cmpfn);
}

void vector_qsort_b(vector_t *self,
                 int (^cmp_b)(const void *, const void *)) {
    qsort_b(self->m_impl.m_start,
          vector_size(self),
          sizeof *self->m_impl.m_start,
          cmp_b);
}

/*!
    \brief

    \param[in]  self
    \param[in]  cmpfn
    \param[in]  pos
*/
void vector_qsort_position(vector_t *self, 
                          int (*cmpfn)(const void *, const void *),
                          vector_iterator_t pos) {
    qsort(pos,
          self->m_impl.m_finish - pos,
          sizeof *self->m_impl.m_start,
          cmpfn);
}

void vector_qsort_position_b(vector_t *self,
                          int (^cmp_b)(const void *, const void *),
                          vector_iterator_t pos) {
    qsort_b(pos,
          self->m_impl.m_finish - pos,
          sizeof *self->m_impl.m_start,
          cmp_b);
}

void vector_mergesort(vector_t *self, int (*cmpfn)(const void *, const void *)) {
    mergesort(self->m_impl.m_start, vector_size(self), sizeof *self->m_impl.m_start, cmpfn);
}

void vector_mergesort_b(vector_t *self, int (^cmp_b)(const void *, const void *)) {
    mergesort_b(self->m_impl.m_start, vector_size(self), sizeof *self->m_impl.m_start, cmp_b);
}

void vector_mergesort_position(vector_t *self, int (*cmpfn)(const void *, const void *), vector_iterator_t pos) {
    mergesort(pos,
          self->m_impl.m_finish - pos,
          sizeof *self->m_impl.m_start,
          cmpfn); 
}

void vector_mergesort_position_b(vector_t *self, int (^cmp_b)(const void *, const void *), vector_iterator_t pos) {
    mergesort_b(pos,
          self->m_impl.m_finish - pos,
          sizeof *self->m_impl.m_start,
          cmp_b);
}

void vector_heapsort(vector_t *self, int (*cmpfn)(const void *, const void *)) {
    heapsort(self->m_impl.m_start, vector_size(self), sizeof *self->m_impl.m_start, cmpfn);    
}

void vector_heapsort_b(vector_t *self, int (^cmp_b)(const void *, const void *)) {
    heapsort_b(self->m_impl.m_start, vector_size(self), sizeof *self->m_impl.m_start, cmp_b);
}

void vector_heapsort_position(vector_t *self, int (*cmpfn)(const void *, const void *), vector_iterator_t pos) {
    heapsort(pos,
          self->m_impl.m_finish - pos,
          sizeof *self->m_impl.m_start,
          cmpfn); 
}

void vector_heapsort_position_b(vector_t *self, int (^cmp_b)(const void *, const void *), vector_iterator_t pos) {
    heapsort_b(pos,
          self->m_impl.m_finish - pos,
          sizeof *self->m_impl.m_start,
          cmp_b);
}
