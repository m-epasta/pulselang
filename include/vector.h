/*
 * Originate from vector (present in cwd)
 * Modification on names and code
 */

#ifndef PULSE_VECTOR_H
#define PULSE_VECTOR_H

#include <stdlib.h>
#include <string.h>

#define Vector(type)  \
    struct {          \
        type* item;   \
        size_t size;  \
        size_t alloc; \
    }

#define vector_new() {0, 0, 0}
#define vector_init(arr) \
    do {                 \
        (arr).item = 0;  \
        (arr).size = 0;  \
        (arr).alloc = 0; \
    } while (0)
#define vector_free(arr)  \
    do {                  \
        free((arr).item); \
    } while (0)

typedef Vector(char) vector_char;
typedef Vector(signed char) vector_schar;
typedef Vector(unsigned char) vector_uchar;

typedef Vector(short) vector_short;
typedef Vector(int) vector_int;
typedef Vector(long) vector_long;

typedef Vector(unsigned short) vector_ushort;
typedef Vector(unsigned int) vector_uint;
typedef Vector(unsigned long) vector_ulong;

typedef Vector(char*) vector_str;
typedef Vector(const char*) vector_const_string;

/*** Access ***/

#define vector_item(arr, i) ((arr).item[i])
#define vector_size(arr) ((arr).size)
#define vector_alloc(arr) ((arr).alloc)
#define vector_empty(arr) ((arr).size == 0)

/*** Insertion (single item) ***/

#define vector_append(arr, ...)                     \
    do {                                            \
        vector_resize(arr, (arr).size + 1);         \
        (arr).item[(arr).size - 1] = (__VA_ARGS__); \
    } while (0)
#define vector_prepend(arr, ...)                         \
    do {                                                 \
        vector_resize(arr, (arr).size + 1);              \
        memmove((arr).item + 1, (arr).item,              \
                ((arr).size - 1) * sizeof(*(arr).item)); \
        (arr).item[0] = (__VA_ARGS__);                   \
    } while (0)
#define vector_insert(arr, i, ...)                                \
    do {                                                          \
        size_t index_ = (i);                                      \
        vector_resize(arr, (arr).size + 1);                       \
        memmove((arr).item + index_ + 1, (arr).item + index_,     \
                ((arr).size - index_ - 1) * sizeof(*(arr).item)); \
        (arr).item[index_] = (__VA_ARGS__);                       \
    } while (0)
#define vector_push(arr, ...) vector_append(arr, __VA_ARGS__)

/*** Insertion (multiple items) ***/

#define vector_append_items(arr, items, count)          \
    do {                                                \
        size_t count_ = (count), oldSize_ = (arr).size; \
        /* Don't memcpy NULL! */                        \
        if (count_) {                                   \
            vector_resize(arr, oldSize_ + count_);      \
            memcpy((arr).item + oldSize_, items,        \
                   count_ * sizeof(*(arr).item));       \
        }                                               \
    } while (0)

#define vector_prepend_items(arr, items, count)                      \
    do {                                                             \
        size_t count_ = (count), oldSize_ = (arr).size;              \
        vector_resize(arr, count_ + oldSize_);                       \
        /* Don't memcpy NULL! */                                     \
        if (count_) {                                                \
            memmove((arr).item + count_, (arr).item,                 \
                    oldSize_ * sizeof(*(arr).item));                 \
            memcpy((arr).item, items, count_ * sizeof(*(arr).item)); \
        }                                                            \
    } while (0)

#define vector_append_items_nullterminate(arr, items, count)                \
    do {                                                                    \
        size_t count_ = (count), oldSize_ = (arr).size;                     \
        vector_resize(arr, oldSize_ + count_ + 1);                          \
        memcpy((arr).item + oldSize_, items, count_ * sizeof(*(arr).item)); \
        (arr).item[--(arr).size] = 0;                                       \
    } while (0)

#define vector_prepend_items_nullterminate(arr, items, count)    \
    do {                                                         \
        size_t count_ = (count), oldSize_ = (arr).size;          \
        vector_resize(arr, count_ + oldSize_ + 1);               \
        memmove((arr).item + count_, (arr).item,                 \
                oldSize_ * sizeof(*(arr).item));                 \
        memcpy((arr).item, items, count_ * sizeof(*(arr).item)); \
        (arr).item[--(arr).size] = 0;                            \
    } while (0)

#if HAVE_TYPEOF
#define vector_appends(arr, ...) \
    vector_appends_t(arr, typeof((*(arr).item)), __VA_ARGS__)
#define vector_prepends(arr, ...) \
    vector_prepends_t(arr, typeof((*(arr).item)), __VA_ARGS__)
#endif

#define vector_appends_t(arr, type, ...)                              \
    do {                                                              \
        type src_[] = {__VA_ARGS__};                                  \
        vector_append_items(arr, src_, sizeof(src_) / sizeof(*src_)); \
    } while (0)
#define vector_prepends_t(arr, type, ...)                              \
    do {                                                               \
        type src_[] = {__VA_ARGS__};                                   \
        vector_prepend_items(arr, src_, sizeof(src_) / sizeof(*src_)); \
    } while (0)

/*** Removal ***/

/* Warning: Do not call vector_pop on an empty vector. */
#define vector_pop(arr) ((arr).item[--(arr).size])
#define vector_pop_check(arr) ((arr).size ? vector_pop(arr) : NULL)
/* Warning, slow: Requires copying all elements after removed item. */
#define vector_remove(arr, i)                                         \
    do {                                                              \
        size_t index_ = (i);                                          \
        if (index_ < arr.size - 1)                                    \
            memmove(&(arr).item[index_], &(arr).item[index_ + 1],     \
                    ((arr).size - 1 - index_) * sizeof(*(arr).item)); \
        (arr).size--;                                                 \
    } while (0)

/*** Replacement ***/

#define vector_from_items(arr, items, count)                     \
    do {                                                         \
        size_t count_ = (count);                                 \
        vector_resize(arr, count_);                              \
        memcpy((arr).item, items, count_ * sizeof(*(arr).item)); \
    } while (0)
#define vector_from_c(arr, c_array) \
    vector_from_items(arr, c_array, sizeof(c_array) / sizeof(*(c_array)))

/*** String buffer ***/

#define vector_append_string(arr, str)                    \
    do {                                                  \
        const char* str_ = (str);                         \
        vector_append_items(arr, str_, strlen(str_) + 1); \
        (arr).size--;                                     \
    } while (0)
#define vector_append_lit(arr, stringLiteral)                           \
    do {                                                                \
        vector_append_items(arr, stringLiteral, sizeof(stringLiteral)); \
        (arr).size--;                                                   \
    } while (0)

#define vector_prepend_string(arr, str)                              \
    do {                                                             \
        const char* str_ = (str);                                    \
        vector_prepend_items_nullterminate(arr, str_, strlen(str_)); \
    } while (0)
#define vector_prepend_lit(arr, stringLiteral)             \
    vector_prepend_items_nullterminate(arr, stringLiteral, \
                                       sizeof(stringLiteral) - 1)

#define vector_from_string(arr, str)                    \
    do {                                                \
        const char* str_ = (str);                       \
        vector_from_items(arr, str_, strlen(str_) + 1); \
        (arr).size--;                                   \
    } while (0)
#define vector_from_lit(arr, stringLiteral)                           \
    do {                                                              \
        vector_from_items(arr, stringLiteral, sizeof(stringLiteral)); \
        (arr).size--;                                                 \
    } while (0)

/*** String vector ***/

#define vector_string_append(arr, str)            \
    do {                                          \
        vector_resize(arr, (arr).size + 1);       \
        (arr).item[(arr).size - 1] = strdup(str); \
    } while (0)

#define vector_string_prepend(arr, str)                  \
    do {                                                 \
        vector_resize(arr, (arr).size + 1);              \
        memmove((arr).item + 1, (arr).item,              \
                ((arr).size - 1) * sizeof(*(arr).item)); \
        (arr).item[0] = strdup(str);                     \
    } while (0)

#define vector_string_insert(arr, i, str)                         \
    do {                                                          \
        size_t index_ = (i);                                      \
        vector_resize(arr, (arr).size + 1);                       \
        memmove((arr).item + index_ + 1, (arr).item + index_,     \
                ((arr).size - index_ - 1) * sizeof(*(arr).item)); \
        (arr).item[index_] = strdup(str);                         \
    } while (0)

#define vector_string_from_array(arr, str_array, count) \
    do {                                                \
        size_t count_ = (count);                        \
        vector_resize(arr, count_);                     \
        for (size_t i_ = 0; i_ < count_; i_++)          \
            (arr).item[i_] = strdup((str_array)[i_]);   \
    } while (0)

#define vector_string_from_c(arr, c_array) \
    vector_string_from_array(arr, c_array, sizeof(c_array) / sizeof(*(c_array)))

#define vector_string_free(arr)                                          \
    do {                                                                 \
        for (size_t i_ = 0; i_ < (arr).size; i_++) free((arr).item[i_]); \
        free((arr).item);                                                \
    } while (0)

#define vector_string_remove(arr, i)                                  \
    do {                                                              \
        free((arr).item[i]);                                          \
        size_t index_ = (i);                                          \
        if (index_ < arr.size - 1)                                    \
            memmove(&(arr).item[index_], &(arr).item[index_ + 1],     \
                    ((arr).size - 1 - index_) * sizeof(*(arr).item)); \
        (arr).size--;                                                 \
    } while (0)

/*** Size management ***/

#define vector_resize(arr, newSize) \
    vector_growalloc(arr, (arr).size = (newSize))
#define vector_resize0(arr, newSize)                             \
    do {                                                         \
        size_t oldSize_ = (arr).size, newSize_ = (newSize);      \
        (arr).size = newSize_;                                   \
        if (newSize_ > oldSize_) {                               \
            vector_growalloc(arr, newSize_);                     \
            memset(&(arr).item[oldSize_], 0,                     \
                   (newSize_ - oldSize_) * sizeof(*(arr).item)); \
        }                                                        \
    } while (0)

#define vector_realloc(arr, newAlloc)                                      \
    do {                                                                   \
        (arr).item = realloc(                                              \
            (arr).item, ((arr).alloc = (newAlloc)) * sizeof(*(arr).item)); \
    } while (0)
#define vector_growalloc(arr, need)                                     \
    do {                                                                \
        size_t need_ = (need);                                          \
        if (need_ > (arr).alloc)                                        \
            vector_realloc(arr, vector_next_alloc((arr).alloc, need_)); \
    } while (0)

#if HAVE_STATEMENT_EXPR == 1
#define vector_make_room(arr, room)                                \
    ({                                                             \
        size_t newAlloc = (arr).size + (room);                     \
        if ((arr).alloc < newAlloc) vector_realloc(arr, newAlloc); \
        (arr).item + (arr).size;                                   \
    })
#endif

static inline size_t vector_next_alloc(size_t alloc, size_t need) {
    if (alloc == 0) alloc = 1;
    while (alloc < need) alloc *= 2;
    return alloc;
}

/*** Traversal ***/

/*
 * vector_foreach(T *&i, vector(T) arr) {...}
 *
 * Traverse a vector.  `i` must be declared in advance as a pointer to an item.
 */
#define vector_foreach(i, arr) \
    for ((i) = &(arr).item[0]; (i) < &(arr).item[(arr).size]; (i)++)

/*
 * vector_foreach_reverse(T *&i, vector(T) arr) {...}
 *
 * Like vector_foreach, but traverse in reverse order.
 */
#define vector_foreach_reverse(i, arr) \
    for ((i) = &(arr).item[(arr).size]; (i)-- > &(arr).item[0];)

#endif /* CCAN_vector_H */

/*

vector_growalloc(arr, newAlloc) sees if the vector can currently hold newAlloc
items; if not, it increases the alloc to satisfy this requirement, allocating
slack space to avoid having to reallocate for every size increment.

vector_from_string(arr, str) copies a string to an vector_char.

vector_push(arr, item) pushes an item to the end of the vector.
vector_pop(arr) pops it back out.  Be sure there is at least one item in the
vector before calling. vector_pop_check(arr) does the same as vector_pop, but
returns NULL if there are no more items left in the vector.

vector_make_room(arr, room) ensures there's 'room' elements of space after the
end of the vector, and it returns a pointer to this space. Currently requires
HAVE_STATEMENT_EXPR, but I plan to remove this dependency by creating an inline
function.

The following require HAVE_TYPEOF==1 :

vector_appends(arr, item0, item1...) appends a collection of comma-delimited
items to the vector.
vector_prepends(arr, item0, item1...) prepends a collection of comma-delimited
items to the vector.\


Examples:

        vector(int)  arr;
        int        *i;

        vector_appends(arr, 0,1,2,3,4);
        vector_appends(arr, -5,-4,-3,-2,-1);
        vector_foreach(i, arr)
                printf("%d ", *i);
        printf("\n");

        vector_free(arr);


        typedef struct {int n,d;} Fraction;
        vector(Fraction) fractions;
        Fraction        *i;

        vector_appends(fractions, {3,4}, {3,5}, {2,1});
        vector_foreach(i, fractions)
                printf("%d/%d\n", i->n, i->d);

        vector_free(fractions);
*/
