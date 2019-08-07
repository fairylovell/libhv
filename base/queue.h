#ifndef HW_QUEUE_H_
#define HW_QUEUE_H_

/*
 * queue
 * FIFO: push_back,pop_front
 */

#include <assert.h> // for assert
#include <stdlib.h> // for malloc,realloc,free
#include <string.h> // for memset,memmove

#include "hbase.h"

// #include <deque>
// typedef std::deque<type> qtype;
#define QUEUE_DECL(type, qtype) \
struct qtype {      \
    type*   ptr;    \
    size_t  size;   \
    size_t  maxsize;\
    size_t  _offset;\
};                  \
typedef struct qtype qtype;\
\
static inline type* qtype##_data(qtype* p) {\
    return p->ptr + p->_offset;\
}\
\
static inline int qtype##_size(qtype* p) {\
    return p->size;\
}\
\
static inline int qtype##_maxsize(qtype* p) {\
    return p->maxsize;\
}\
\
static inline int qtype##_empty(qtype* p) {\
    return p->size == 0;\
}\
\
static inline type* qtype##_front(qtype* p) {\
    return p->size == 0 ? NULL : p->ptr + p->_offset;\
}\
\
static inline type* qtype##_back(qtype* p) {\
    return p->size == 0 ? NULL : p->ptr + p->_offset + p->size-1;\
}\
\
static inline void qtype##_init(qtype* p, int maxsize) {\
    p->_offset = 0;\
    p->size = 0;\
    p->maxsize = maxsize;\
    SAFE_ALLOC(p->ptr, sizeof(type) * maxsize);\
}\
\
static inline void qtype##_clear(qtype* p) {\
    p->_offset = 0;\
    p->size = 0;\
    memset(p->ptr, 0, sizeof(type) * p->maxsize);\
}\
\
static inline void qtype##_cleanup(qtype* p) {\
    SAFE_FREE(p->ptr);\
    p->_offset = p->size = p->maxsize = 0;\
}\
\
static inline void qtype##_resize(qtype* p, int maxsize) {\
    p->maxsize = maxsize;\
    int bytes = sizeof(type) * maxsize;\
    p->ptr = (type*)safe_realloc(p->ptr, bytes);\
}\
\
static inline void qtype##_double_resize(qtype* p) {\
    assert(p->maxsize != 0);\
    return qtype##_resize(p, p->maxsize*2);\
}\
\
static inline void qtype##_push_back(qtype* p, type* elem) {\
    if (p->size == p->maxsize) {\
        qtype##_double_resize(p);\
    }\
    else if (p->_offset + p->size == p->maxsize) {\
        memmove(p->ptr, p->ptr + p->_offset, p->size);\
        p->_offset = 0;\
    }\
    p->ptr[p->_offset + p->size] = *elem;\
    p->size++;\
}\
static inline void qtype##_pop_front(qtype* p) {\
    assert(p->size > 0);\
    p->size--;\
    if (++p->_offset == p->maxsize) p->_offset = 0;\
}\
\
static inline void qtype##_pop_back(qtype* p) {\
    assert(p->size > 0);\
    p->size--;\
}\

#endif // HW_QUEUE_H_
