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
    =====================
    Example: Unit testing
    =====================

    This file is used to test the library's API to make sure that it behaves
    as expected.
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "g2l.h"

static void test_custom_data_type_with_pop_and_shift(void);
static void test_push_shift_order(void);
static void test_push_pop_order(void);
static void test_null_data_type_and_basic_stuff(void);

int main(void)
{
    test_custom_data_type_with_pop_and_shift();
    test_push_shift_order();
    test_push_pop_order();
    test_null_data_type_and_basic_stuff();
    return 0;
}

static void test_custom_data_type_with_pop_and_shift(void)
{
    struct my_custom_data_type
    {
        char character;
        int integer;
    };

    g2l_t *list = g2l_create(sizeof(struct my_custom_data_type), true);

    struct my_custom_data_type tmp;
    const int char_start = (int)'a';

    for (int i = 0; i < 5; i++)
    {
        tmp.integer = i;
        tmp.character = char_start + i;
        g2l_push(list, &tmp);
    }

    g2l_shift(list, &tmp);
    assert(tmp.integer == 0);
    assert(tmp.character == 'a');

    g2l_pop(list, &tmp);
    assert(tmp.integer == 4);
    assert(tmp.character == 'e');

    g2l_pop(list, &tmp);
    assert(tmp.integer == 3);
    assert(tmp.character == 'd');

    g2l_shift(list, &tmp);
    assert(tmp.integer == 1);
    assert(tmp.character == 'b');

    assert(g2l_size(list) == 1);

    g2l_pop(list, &tmp);
    assert(tmp.integer == 2);
    assert(tmp.character == 'c');

    assert(!g2l_pop(list, NULL));

    g2l_destroy(list);
}

static void test_push_shift_order(void)
{
    g2l_t *list = g2l_create(sizeof(int), true);
    int tmp;
    const int n = 10;
    for (int i = 0; i < n; i++)
    {
        tmp = i;
        g2l_push(list, &tmp);
    }
    int i = 0;
    while (g2l_shift(list, &tmp))
    {
        assert(tmp == i);
        i++;
    }
    g2l_destroy(list);
}

static void test_push_pop_order(void)
{
    g2l_t *list = g2l_create(sizeof(int), true);
    int tmp;
    const int n = 10;
    for (int i = 0; i < n; i++)
    {
        tmp = i;
        g2l_push(list, &tmp);
    }
    int i = n - 1;
    while (g2l_pop(list, &tmp))
    {
        assert(tmp == i);
        i--;
    }
    g2l_destroy(list);
}

static void test_null_data_type_and_basic_stuff(void)
{
    g2l_t *list = g2l_create(0, true);
    assert(g2l_size(list) == 0);
    g2l_push(list, NULL);
    assert(g2l_size(list) == 1);
    g2l_push(list, NULL);
    assert(g2l_size(list) == 2);
    assert(g2l_pop(list, NULL));
    assert(g2l_pop(list, NULL));
    assert(!g2l_pop(list, NULL));
    for (int i = 0; i < 5; i++)
    {
        g2l_push(list, NULL);
    }
    assert(g2l_size(list) == 5);
    g2l_clear(list);
    assert(g2l_size(list) == 0);
    g2l_destroy(list);
}
