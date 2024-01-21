/*
    Copyright (c) 2024 BB-301 <fw3dg3@gmail.com> [Official repository](https://github.com/BB-301/c-generic-doubly-linked-list)

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the “Software”), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

/**
 * @file
 */

#ifndef _G2L_H_
#define _G2L_H_

#include <stdbool.h>
#include <sys/types.h>

/**
 * @brief An opaque data type used as a container for the
 * **generic doubly linked list** implementation, and which
 * must be instantiated using the \ref g2l_create function.
 * @see g2l_create, g2l_destroy
 * @par Example:
 * @include examples/quick_example.c
 */
typedef struct g2l_t g2l_t;

/**
 * @brief The function that must be used to instantiate a new linked list
 * object (i.e., \ref g2l_t ).
 * @param data_size The size, in bytes, of the data type that will be
 * stored in the created instance.
 * @param abort_on_enomem Whether \ref ENOMEM errors should result in
 * the process being aborted (`true`) or whether the function should
 * simply return the \ref NULL pointer and let the application deal
 * with the error.
 * @return \ref g2l_t* A pointer to the created linked list object.
 * @note - If \p abort_on_enomem is set to `false` and an error occurs,
 * that error will be due to an internal call to \ref malloc failing,
 * in which case \ref errno will contain \ref ENOMEM , which will have
 * been set by \ref malloc .
 * @note - Setting \p abort_on_enomem to `false` will also require the
 * application to inspect the value returned by \ref g2l_push (and its
 * \ref g2l_enqueue alias), which function also internally makes use
 * of \ref malloc , and which could consequently obtain \ref ENOMEM .
 * @see g2l_destroy
 */
g2l_t *g2l_create(size_t data_size, bool abort_on_enomem);

/**
 * @brief A function that can be used to clear (i.e., empty) a linked list
 * object.
 * @param self A pointer to the \ref g2l_t instance to be cleared.
 */
void g2l_clear(g2l_t *self);

/**
 * @brief The function that should be used to destroy a linked list object
 * once it is no longer needed by the application.
 * @param self A pointer to the \ref g2l_t instance to be destroyed.
 * @see g2l_create
 */
void g2l_destroy(g2l_t *self);

/**
 * @brief A function that can be used to retrieve the current number
 * of items contained in the linked list object \p self .
 * @param self A pointer to the \ref g2l_t object for which to retrieve
 * the number of internal elements.
 * @return \ref size_t The number of elements in the \p self linked list object.
 */
size_t g2l_size(g2l_t const *self);

/**
 * @brief The function that must be used to add a new element to
 * the linked list object \p self .
 * @param self A pointer to the \ref g2l_t instance into which to push
 * the new element.
 * @param data A pointer to arbitrary memory of size defined when instantiating
 * the object using \ref g2l_create , which is to be copied and stored inside
 * the linked list object \p self .
 * @return \ref int An integer value that will be `0` if the new element was successfully
 * added, else it will be \ref ENOMEM .
 * @note - If \p self has been instantiated with \ref g2l_create 's `abort_on_enomem`
 * argument set to `true`, then this function will always return `0`, because possible
 * \ref ENOMEM errors obtained using \ref malloc will result in the process being aborted.
 * Therefore, in such cases (i.e., when setting `abort_on_enomem = true`), the output of
 * the \ref g2l_push should simply be ignored.
 * @note - The \ref g2l_enqueue function is an alias for \ref g2l_push .
 * @see g2l_enqueue
 */
int g2l_push(g2l_t *self, void const *data);

/**
 * @brief The function that must be used to "pop" the stack (and optionally retrieve
 * the value contained in the list's top-most (i.e., youngest) element).
 * @param self A pointer to the \ref g2l_t instance for which to remove the top-most
 * (i.e., youngest) element.
 * @param data A pointer to memory into which the list's top-most element's data should be copied
 * before the element is freed. The \ref NULL pointer can be passed if the data is not
 * needed by the application.
 * @return \ref bool A boolean value indicating whether the operation was successful (i.e,
 * whether a value was popped). A `false` value simply means that the list was already
 * empty at the moment when the operation was performed, else the value will be `true`.
 */
bool g2l_pop(g2l_t *self, void *data);

/**
 * @brief The function that must be used to "shift" the list's oldest element (and optionally
 * retrieve the value contained in that element).
 * @param self A pointer to the \ref g2l_t instance for which to remove the oldest
 * element.
 * @param data A pointer to memory into which the list's oldest element's data should be copied
 * before the element is freed. The \ref NULL pointer can be passed if the data is not
 * needed by the application.
 * @return \ref bool A boolean value indicating whether the operation was successful (i.e,
 * whether a value was shifted). A `false` value simply means that the list was already
 * empty at the moment when the operation was performed, else the value will be `true`.
 * @note - The \ref g2l_dequeue function is an alias for \ref g2l_shift .
 * @see g2l_dequeue
 */
bool g2l_shift(g2l_t *self, void *data);

/**
 * @brief An alias to \ref g2l_push , a function that can be used to "enqueue"
 * a new element to the linked list object \p self .
 * @param self A pointer to the \ref g2l_t instance into which to enqueue (i.e., push)
 * the new element.
 * @param data A pointer to arbitrary memory of size defined when instantiating
 * the object using \ref g2l_create , which is to be copied and stored inside
 * the linked list object \p self .
 * @return \ref int An integer value that will be `0` if the new element was successfully
 * added, else it will be \ref ENOMEM .
 * @note - See the documentation for \ref g2l_push for a discussion about the possible
 * returned values.
 * @note - The \ref g2l_enqueue function is an alias for \ref g2l_push .
 * @see g2l_push
 */
bool g2l_enqueue(g2l_t *self, void const *data);

/**
 * @brief An alias to \ref g2l_shift , this function can be used to
 * "dequeue" (i.e., shift) the list's oldest element (and optionally
 * retrieve the value contained in that element).
 * @param self A pointer to the \ref g2l_t instance for which to remove the oldest
 * element.
 * @param data A pointer to memory into which the list's oldest element's data should be copied
 * before the element is freed. The \ref NULL pointer can be passed if the data is not
 * needed by the application.
 * @return \ref bool A boolean value indicating whether the operation was successful (i.e,
 * whether a value was removed). A `false` value simply means that the list was already
 * empty at the moment when the operation was performed, else the value will be `true`.
 * @note - The \ref g2l_dequeue function is an alias for \ref g2l_shift .
 * @see g2l_shift
 */
bool g2l_dequeue(g2l_t *self, void *data);

#endif
