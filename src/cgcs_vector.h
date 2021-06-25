/*!
    \file       cgcs_vector.h
    \brief      Header file for a vector ADT (elements of any pointer type)

    \author     Gemuele Aludino
    \date       05 Sep 2020
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
typedef struct cgcs_vector vector_t;

/*!
    \typedef
    \brief
*/
typedef void *voidptr;

/*!
    \typedef
    \brief
*/
typedef voidptr *vector_iterator_t;

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

void vector_init(vector_t *self, size_t capacity);

void vector_init_alloc_fn(vector_t *self, size_t capacity, 
                        void *(*allocfn)(size_t));

void vector_deinit(vector_t *self);
void vector_deinit_free_fn(vector_t *self, void (*freefn)(void *));

static voidptr vector_front(vector_t *self);
static voidptr vector_back(vector_t *self);
static voidptr vector_at(vector_t *self, const int index);
static voidptr vector_i(vector_t *self, size_t index);

static bool vector_empty(vector_t *self);
static size_t vector_size(vector_t *self);
static size_t vector_capacity(vector_t *self);

bool vector_resize(vector_t *self, size_t n);
bool vector_resize_alloc_free_fn(vector_t *self, size_t n, 
                              void *(*allocfn)(size_t), void (*freefn)(void *));

bool vector_shrink_to_fit(vector_t *self);
bool vector_shrink_to_fit_alloc_free_fn(vector_t *self, 
                                     void *(*allocfn)(size_t), void (*freefn)(void *));

static vector_iterator_t vector_begin(vector_t *self);
static vector_iterator_t vector_end(vector_t *self);

vector_iterator_t vector_insert(vector_t *self, vector_iterator_t it,
                                  const void *valaddr);
vector_iterator_t vector_insert_alloc_free_fn(vector_t *self, vector_iterator_t it,
                                          const void *valaddr, 
                                          void *(*allocfn)(size_t), void (*freefn)(void *));
vector_iterator_t vector_insert_range(vector_t *self,
                                        vector_iterator_t it,
                                        vector_iterator_t beg,
                                        vector_iterator_t end);
vector_iterator_t vector_insert_range_alloc_free_fn(vector_t *self,
                                        vector_iterator_t it,
                                        vector_iterator_t beg,
                                        vector_iterator_t end,
                                        void *(*allocfn)(size_t),
                                        void (*freefn)(void *));

vector_iterator_t vector_erase(vector_t *self, vector_iterator_t it);
vector_iterator_t vector_erase_range(vector_t *self, vector_iterator_t beg,
                                       vector_iterator_t end);

void vector_push_back(vector_t *self, const void *valaddr);
void vector_push_back_alloc_free_fn(vector_t *self, const void *valaddr, 
                             void *(*allocfn)(size_t), void (*freefn)(void *));

void vector_pop_back(vector_t *self);

void vector_push_front(vector_t *self, const void *valaddr);
void vector_push_front_alloc_free_fn(vector_t *self, const void *valaddr,
                             void *(*allocfn)(size_t), void (*freefn)(void *));

void vector_pop_front(vector_t *self);

void vector_clear(vector_t *self);

void vector_foreach(vector_t *self, void (*func)(void *));
void vector_foreach_b(vector_t *self, void (^block)(void *));

static void vector_foreach_range(vector_t *self, void (*func)(void *),
                                vector_iterator_t beg, vector_iterator_t end);
static void vector_foreach_range_b(vector_t *self, void (^block)(void *),
                                  vector_iterator_t beg, vector_iterator_t end);

int vector_search(vector_t *self,
                 int (*cmpfn)(const void *, const void *),
                 const void *valaddr);
int vector_search_b(vector_t *self, int (^cmp_b)(const void *, const void *), const void *valaddr);

int vector_search_range(vector_t *self,
                       int (*cmpfn)(const void *, const void *),
                       const void *valaddr, vector_iterator_t beg,
                       vector_iterator_t end);
int vector_search_range_b(vector_t *self,
                       int (^cmp_b)(const void *, const void *),
                       const void *valaddr, vector_iterator_t beg,
                       vector_iterator_t end);

vector_iterator_t vector_find(vector_t *self,
                                int (*cmp_b)(const void *, const void *),
                                const void *valaddr);
vector_iterator_t vector_find_b(vector_t *self,
                                int (^cmp_b)(const void *, const void *),
                                const void *valaddr);

vector_iterator_t vector_find_range(vector_t *self,
                                      int (*cmpfn)(const void *, const void *),
                                      const void *valaddr, vector_iterator_t beg,
                                      vector_iterator_t end);
vector_iterator_t vector_find_range_b(vector_t *self,
                                      int (^cmp_b)(const void *, const void *),
                                      const void *valaddr, vector_iterator_t beg,
                                      vector_iterator_t end);

void vector_qsort(vector_t *self,
                 int (*cmpfn)(const void *, const void *));
void vector_qsort_b(vector_t *self,
                 int (^cmp_b)(const void *, const void *));

void vector_qsort_position(vector_t *self,
                          int (*cmpfn)(const void *, const void *),
                          vector_iterator_t pos);
void vector_qsort_position_b(vector_t *self,
                          int (^cmp_b)(const void *, const void *),
                          vector_iterator_t pos);

void vector_mergesort(vector_t *self, int (*cmpfn)(const void *, const void *));
void vector_mergesort_b(vector_t *self, int (^cmp_b)(const void *, const void *));

void vector_mergesort_position(vector_t *self, int (*cmpfn)(const void *, const void *), vector_iterator_t pos);
void vector_mergesort_position_b(vector_t *self, int (^cmp_b)(const void *, const void *), vector_iterator_t pos);

void vector_heapsort(vector_t *self, int (*cmpfn)(const void *, const void *));
void vector_heapsort_b(vector_t *self, int (^cmp_b)(const void *, const void *));

void vector_heapsort_position(vector_t *self, int (*cmpfn)(const void *, const void *), vector_iterator_t pos);
void vector_heapsort_position_b(vector_t *self, int (^cmp_b)(const void *, const void *), vector_iterator_t pos);

static vector_t *vector_new(size_t capacity);
static vector_t *vector_new_alloc_fn(size_t capacity, void *(*allocfn)(size_t));

static void vector_delete(vector_t *self);
static void vector_delete_free_fn(vector_t *self, void (*freefn)(void *));

/*!
    \brief

    \param[in]  self

    \return

    \code
        vector_t vec = CGCS_VECTOR_INITIALIZER;
        vector_init(&vec, capacity);
 
        char *str = NULL;
       
        str = strdup("front string"); // str is allocated by the caller
        vector_push_back(&vec, &str);      // &str is a (char **)

        str = strdup("back string");
        vector_push_back(&vec, &str);

        // Retrieve "front string".
        // vector_front returns a (void *), which makes this assignment possible.
        char **get = vector_front(&vec);

        // Run free_str on all pointers in vec.
        // free_str is a wrapper for free(), that deals with (char *).
        vector_foreach(&vec, free_str);
        
        // Now that the memory addressed by each pointer is freed,
        // we can deinitialize vec.
        vector_deinit(&vec);
    \endcode
*/
static inline voidptr vector_front(vector_t *self) {
    return self->m_impl.m_start;
}

/*!
    \brief

    \param[in]  self

    \return

    \code
        vector_t vec = CGCS_VECTOR_INITIALIZER;
        vector_init(&vec, capacity);
 
        char *str = NULL;
       
        str = strdup("front string"); // str is allocated by the caller
        vector_push_back(&vec, &str);      // &str is a (char **)

        str = strdup("back string");
        vector_push_back(&vec, &str);

        // Retrieve "back string".
        // vector_back returns a (void *), which makes this assignment possible.
        char **get = vector_back(&vec);

        // Run free_str on all pointers in vec.
        // free_str is a wrapper for free(), that deals with (char *).
        vector_foreach(&vec, free_str);
        
        // Now that the memory addressed by each pointer is freed,
        // we can deinitialize vec.
        vector_deinit(&vec);
    \endcode
*/
static inline voidptr vector_back(vector_t *self) {
    // m_finish is the address of one-past the last element;
    // we subtract m_finish by 1 to get the element at the back of vector_t
    return self->m_impl.m_finish - 1;
}

/*!
    \brief

    \param[in]  self
    \param[in]  index

    \return

    \code
        vector_t vec = CGCS_VECTOR_INITIALIZER;
        vector_init(&vec, capacity);
 
        char *str = NULL;
       
        str = strdup("front string"); // str is allocated by the caller
        vector_push_back(&vec, &str);      // &str is a (char **)

        str = strdup("middle string");
        vector_push_back(&vec, &str);

        str = strdup("back string");
        vector_push_back(&vec, &str);

        // Retrieve "middle string"
        // vector_at returns a (void *), which makes this assignment possible.
        char **get = vector_at(&vec, 1);

        // Run free_str on all pointers in vec.
        // free_str is a wrapper for free(), that deals with (char *).
        vector_foreach(&vec, free_str);
        
        // Now that the memory addressed by each pointer is freed,
        // we can deinitialize vec.
        vector_deinit(&vec);
    \endcode
*/
static inline voidptr vector_at(vector_t *self, const int index) {
    voidptr *result = self->m_impl.m_start + index;
    return result >= self->m_impl.m_finish ? NULL : result;
}

/*!
    \brief
 
    \param[in]  self
    \param[in]  index
 
    \return
 */
static inline voidptr vector_i(vector_t *self, size_t index) {
    return self->m_impl.m_start + index;
}

/*!
    \brief

    \param[in]  self

    \return
*/
static inline bool vector_empty(vector_t *self) {
    return self->m_impl.m_start == self->m_impl.m_finish;
}

/*!
    \brief

    \param[in]  self

    \return
*/
static inline size_t vector_size(vector_t *self) {
    return self->m_impl.m_finish - self->m_impl.m_start;
}

/*!
    \brief

    \param[in]  self

    \return
*/
static inline size_t vector_capacity(vector_t *self) {
    return self->m_impl.m_end_of_storage - self->m_impl.m_start;
}

/*!
    \brief

    \param[in]  self

    \return
*/
static inline vector_iterator_t vector_begin(vector_t *self) {
    // Same as vector_front.
    return self->m_impl.m_start;
}

/*!
    \brief

    \param[in]  self

    \return
*/
static inline vector_iterator_t vector_end(vector_t *self) {
    // Same as vector_back(self) + 1.
    return self->m_impl.m_finish;
}

/*!
    \brief

    \param[in]  self
    \param[in]  func
    \param[in]  beg
    \param[in]  end
*/
static inline void vector_foreach_range(vector_t *self,
                                           void (*func)(void *),
                                           vector_iterator_t beg,
                                           vector_iterator_t end) {
    while (beg < end) {
        func(beg++);
    }
}

static inline void vector_foreach_range_b(vector_t *self, void (^block)(void *),
                                  vector_iterator_t beg, vector_iterator_t end) {
    while (beg < end) {
        block(beg++);
    }
}

/*!
    \brief
 
    \param[in]  capacity
 
    \return
 */
static inline vector_t *vector_new(size_t capacity) {
    vector_t *v = NULL;
    
    if ((v = malloc(sizeof *v))) {
        vector_init(v, capacity);
    }
    
    return v;
}

/*!
    \brief
 
    \param[in]  capacity
    \param[in]  allocfn
 
    \return
 */
static inline vector_t *vector_new_alloc_fn(size_t capacity, void *(*allocfn)(size_t)) {
    vector_t *v = NULL;
    
    if ((v = allocfn(sizeof *v))) {
        vector_init_alloc_fn(v, capacity, allocfn);
    }
    
    return v;
}

/*!
    \brief
 
    \param[in]  self
 */
static inline void vector_delete(vector_t *self) {
    vector_deinit(self);
    free(self);
}

/*!
    \brief
 
    \param[in]  self
    \param[in]  freefn
 */
static inline void vector_delete_free_fn(vector_t *self, void (*freefn)(void *)) {
    vector_deinit_free_fn(self, freefn);
    freefn(self);
}

#endif /* CGCS_VECTOR_H */
