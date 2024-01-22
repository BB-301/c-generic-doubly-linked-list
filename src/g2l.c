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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "g2l.h"

#ifndef G2L_SRC_FILE_NAME
#define G2L_SRC_FILE_NAME "g2l.c"
#endif

#define LIBRARY_ERROR_PREFIX "[library error]"         // An actual error in the library implementation
#define PROGRAMMING_ERROR_PREFIX "[programming error]" // An programming error (i.e, made by the application using the library)

static void *g2l_pop_internal(g2l_t *self);
static void *g2l_shift_internal(g2l_t *self);

struct my_node
{
    void *data;
    struct my_node *previous;
    struct my_node *next;
};

struct g2l_t
{
    size_t n;
    size_t data_size;
    struct my_node *head;
    struct my_node *tail;
    bool abort_on_enomem;
};

g2l_t *g2l_create(size_t data_size, bool abort_on_enomem)
{
    g2l_t *self = malloc(sizeof(g2l_t));
    if (self == NULL)
    {
        if (abort_on_enomem)
        {
            perror("malloc()");
            abort();
        }
        if (errno != ENOMEM) // Based on my understanding, this should not be possible.
        {
            fprintf(stderr, "%s Oups... Expecting errno to contain '%s' but contains '%s'\n", LIBRARY_ERROR_PREFIX, strerror(ENOMEM), strerror(errno));
            abort();
        }
        return NULL;
    }
    self->data_size = data_size;
    self->abort_on_enomem = abort_on_enomem;
    self->n = 0;
    self->head = NULL;
    self->tail = NULL;
    return self;
}

void g2l_clear(g2l_t *self)
{
    struct my_node *tmp;
    while ((tmp = self->head) != NULL)
    {
        self->head = tmp->next;
        free(tmp->data);
        free(tmp);
        self->n -= 1;
    }
    if (self->n != 0)
    {
        fprintf(stderr, "%s Oups... Something is wrong. This should not be possible.\n", LIBRARY_ERROR_PREFIX);
        abort();
    }
    self->head = NULL;
    self->tail = NULL;
}

void g2l_destroy(g2l_t *self)
{
    g2l_clear(self);
    free(self);
}

size_t g2l_size(g2l_t const *self)
{
    return self->n;
}

int g2l_push(g2l_t *self, void const *data)
{
    if (self->data_size == 0 && data != NULL)
    {
        fprintf(stderr, "[file:%s][line:%i] %s %s 'data' argument should be NULL when 'data_size = 0'\n", G2L_SRC_FILE_NAME, __LINE__, __func__, PROGRAMMING_ERROR_PREFIX);
        abort();
    }
    if (self->data_size > 0 && data == NULL)
    {
        fprintf(stderr, "[file:%s][line:%i] %s %s 'data' argument should not be NULL because 'data_size = %zu'\n", G2L_SRC_FILE_NAME, __LINE__, __func__, PROGRAMMING_ERROR_PREFIX, self->data_size);
        abort();
    }
    struct my_node *node = malloc(sizeof(struct my_node));
    if (node == NULL)
    {
        if (self->abort_on_enomem)
        {
            perror("malloc()");
            abort();
        }
        if (errno != ENOMEM) // Based on my understanding, this should not be possible.
        {
            fprintf(stderr, "%s Oups... Expecting errno to contain '%s' but contains '%s'\n", LIBRARY_ERROR_PREFIX, strerror(ENOMEM), strerror(errno));
            abort();
        }
        return errno;
    }
    node->previous = NULL;
    node->next = NULL;
    node->data = NULL;
    if (self->data_size > 0)
    {
        node->data = malloc(self->data_size);
        if (node->data == NULL)
        {
            if (self->abort_on_enomem)
            {
                perror("malloc()");
                abort();
            }
            if (errno != ENOMEM) // Based on my understanding, this should not be possible.
            {
                fprintf(stderr, "%s Oups... Expecting errno to contain '%s' but contains '%s'\n", LIBRARY_ERROR_PREFIX, strerror(ENOMEM), strerror(errno));
                abort();
            }
            free(node);
            return errno;
        }
        memcpy(node->data, data, self->data_size);
    }
    if (self->n == 0)
    {
        self->head = node;
        self->tail = node;
    }
    else
    {
        struct my_node *tmp = self->head;
        tmp->previous = node;
        self->head = node;
        self->head->next = tmp;
    }
    self->n += 1;
    return 0;
}

bool g2l_pop(g2l_t *self, void *data)
{
    if (self->data_size == 0 && data != NULL)
    {
        fprintf(stderr, "[file:%s][line:%i] %s %s expecting 'data' argument to be NULL pointer for 'data_size = 0'\n", G2L_SRC_FILE_NAME, __LINE__, __func__, PROGRAMMING_ERROR_PREFIX);
        abort();
    }
    if (self->n == 0)
    {
        return false;
    }
    void *tmp = g2l_pop_internal(self);
    if (data != NULL)
    {
        memcpy(data, tmp, self->data_size);
    }
    if (tmp != NULL)
    {
        free(tmp);
    }
    return true;
}

bool g2l_shift(g2l_t *self, void *data)
{
    if (self->data_size == 0 && data != NULL)
    {
        fprintf(stderr, "[file:%s][line:%i] %s %s expecting 'data' argument to be NULL pointer for 'data_size = 0'\n", G2L_SRC_FILE_NAME, __LINE__, __func__, PROGRAMMING_ERROR_PREFIX);
        abort();
    }
    if (self->n == 0)
    {
        return false;
    }
    void *tmp = g2l_shift_internal(self);
    if (data != NULL)
    {
        memcpy(data, tmp, self->data_size);
    }
    if (tmp != NULL)
    {
        free(tmp);
    }
    return true;
}

bool g2l_enqueue(g2l_t *self, void const *data)
{
    return g2l_push(self, data);
}

bool g2l_dequeue(g2l_t *self, void *data)
{
    return g2l_shift(self, data);
}

static void *g2l_pop_internal(g2l_t *self)
{
    if (self->n == 0)
    {
        fprintf(stderr, "%s g2l_pop_internal() should not be called for 'self->n = 0'\n", LIBRARY_ERROR_PREFIX);
        abort();
    }
    struct my_node *tmp = self->head;
    self->head = tmp->next;
    if (self->head != NULL)
    {
        self->head->previous = NULL;
    }
    self->n -= 1;
    void *data = tmp->data;
    free(tmp);
    if (self->n == 0)
    {
        self->tail = NULL;
    }
    return data;
}

static void *g2l_shift_internal(g2l_t *self)
{
    if (self->n == 0)
    {
        fprintf(stderr, "%s g2l_shift_internal() should not be called for 'self->n = 0'\n", LIBRARY_ERROR_PREFIX);
        abort();
    }
    struct my_node *tmp = self->tail;
    self->tail = tmp->previous;
    if (self->tail != NULL)
    {
        self->tail->next = NULL;
    }
    self->n -= 1;
    void *data = tmp->data;
    free(tmp);
    if (self->n == 0)
    {
        self->head = NULL;
    }
    return data;
}
