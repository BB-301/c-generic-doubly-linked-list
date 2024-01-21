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

/*
    ======================
    Example: Quick example
    ======================

    This is a quick "getting started" example used to illustrate
    the basic structure of a program that uses lib<g2l>. This example
    is featured in the [repository](https://github.com/BB-301/c-generic-doubly-linked-list)'s
    README.md file.

    Here, the `g2l_t` type is used to implement both a stack and a queue.
    The stack's data type is a `char`, while the queue's data type is
    an application defined structure (named `struct my_data_type`) containing
    two `int` members; namely, `value_1` and `value_2`.
*/

#include <stdio.h>
#include <stdlib.h>

#include "g2l.h"

#define NUMBER_OF_ITEMS (4)
#define DIVIDER "---------------------------------"

struct my_data_type
{
    int value_1;
    int value_2;
};

static void demo_char_data_type(void);
static void demo_custom_data_type(void);

int main(void)
{
    demo_char_data_type();
    demo_custom_data_type();
    fprintf(stdout, "\n");

    return 0;
}

static void demo_char_data_type(void)
{
    fprintf(stdout, "\n%s\nDEMO: stack with 'char' data type\n%s\n\n", DIVIDER, DIVIDER);

    char tmp;
    g2l_t *stack = g2l_create(sizeof(char), true);

    tmp = 'a';
    for (int i = 0; i < NUMBER_OF_ITEMS; i++)
    {
        g2l_push(stack, &tmp);
        fprintf(stdout, "pushed: %c [n = %zu]\n", tmp, g2l_size(stack));
        tmp += 1;
    }
    fprintf(stdout, "\n");

    while (g2l_size(stack) > 0)
    {
        g2l_pop(stack, &tmp);
        fprintf(stdout, "popped: %c [n = %zu]\n", tmp, g2l_size(stack));
    }

    g2l_destroy(stack);
}

static void demo_custom_data_type(void)
{
    fprintf(stdout, "\n%s\nDEMO: queue with custom data type\n%s\n\n", DIVIDER, DIVIDER);

    struct my_data_type tmp;
    g2l_t *queue = g2l_create(sizeof(struct my_data_type), true);

    for (int i = 0; i < NUMBER_OF_ITEMS; i++)
    {
        tmp.value_1 = i + i;
        tmp.value_2 = i * i;
        // NOTE: `g2l_enqueue` is simply an alias for `g2l_push`
        g2l_enqueue(queue, &tmp);
        fprintf(
            stdout,
            "enqueued: struct { .value_1 = %i, .value_2 = %i } [n = %zu]\n",
            tmp.value_1, tmp.value_2, g2l_size(queue));
    }
    fprintf(stdout, "\n");

    while (g2l_size(queue) > 0)
    {
        // NOTE: `g2l_dequeue` is simply an alias for `g2l_shift`
        g2l_dequeue(queue, &tmp);
        fprintf(
            stdout,
            "dequeued: struct { .value_1 = %i, .value_2 = %i } [n = %zu]\n",
            tmp.value_1, tmp.value_2, g2l_size(queue));
    }

    g2l_destroy(queue);
}
