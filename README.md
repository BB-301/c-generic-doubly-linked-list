<h1>lib&ltg2l&gt</h1>

A simple and experimental C implementation of a generic doubly linked list.

## Introduction

This C library project is an attempt at a [doubly linked list](https://en.wikipedia.org/wiki/Doubly_linked_list) implementation where the list's data type is generic; i.e., it can be specified by the application, depending on what is required for the use case at hand. While the [C programming language](https://en.wikipedia.org/wiki/C_(programming_language)) does not support [generic types](https://en.wikipedia.org/wiki/Generic_programming), a common way around that limitation is the use of the void pointer (i.e. `void *`), which is the approach taken here.

### Etymology

All of the types and functions exposed by this library's public API are namespaced using the `g2l_` prefix, where `g2l` stands for `generic doubly linked list`. More precisely, `g` stands for `generic`, while `2` stands for "double", which is a base word for "doubly", but `2` is also used to convey "L times 2", because (L)inked (L)ist has 2 L's, thus the `l` (i.e., lowercase of L).

## Design goal and fun facts

In computer science, [linked lists](https://en.wikipedia.org/wiki/Linked_list) have several applications, [queues](https://en.wikipedia.org/wiki/Queue_(abstract_data_type)) and [stacks](https://en.wikipedia.org/wiki/Stack_(abstract_data_type)) being two popular ones, and which latter two are in fact almost often used as illustrations in linked list tutorials.

When I started writing this project, I was planning on simply using the singly linked list, which corresponds to the most basic linked list type. But when writing the code for the classic [queue](./examples/basic_queue.c) and [stack](./examples/basic_stack.c) examples, I realized that, while implementing a stack with a singly linked list results in `O(1)` (read more about [Big O notation](https://en.wikipedia.org/wiki/Big_O_notation)) for both `push` and `pop` operations, the queue implementation based on the singly linked list could only achieve `O(1)` and `O(n)` for the analogous operations (i.e., either `enqueue` or `dequeue` can be `O(1)`, but the other will need to be `O(n)`). That fact led me to implementing a doubly linked list instead,  which allows for `O(1)` for both operations in both the `push/pop` and `enqueue/dequeue` pairs.

## A quick example

```c
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
```

## How it works

The library's public API is actually pretty simple. The `g2l_create` function is used to instantiate and return a pointer to a dynamically allocated object of type `g2l_t`, which serves as a container for the doubly linked list with generic data type. The factory function's first argument is named `data_size` and corresponds to the size (in bytes) of the data type that will be tracked by the list. For instance, if we want to use the list to store values of type `int`, then we proceed by instantiating the list as follows: `g2l_t *list = g2l_create(sizeof(int), true);`. The second argument, which is named `abort_on_enomem`, is of type `bool` and indicates whether the library should take care of aborting the process upon `ENOMEM` errors (which could technically occur when internally calling `malloc`) for the specific instance. If the application wants to handle such errors itself, then the `g2l_create` function should be called with `abort_on_enomem = false`.

Once instantiated, the list object can be used to push (using `g2l_push`) elements into it and pop (using `g2l_pop`) elements out of it. The API also have a function to shift (`g2l_shift`) an element out of it, which means that it will remove the list's oldest element. While "pushing" and "popping" is a terminology mostly employed when using a list as a "stack", "enqueuing" and "dequeuing" correspond to more appropriate terms when working with a "queue". For that reason, `g2l_push` has an alias named `g2l_enqueue`, and `g2l_shift` has an alias named `g2l_dequeue`. The [quick example section](#a-quick-example) section above clearly illustrates the use of those functions, as well as how the list can be used for storing arbitrary data types.

At this point, it will have become clear to the reader that this library only allows pushing and popping or shifting elements. In other words, there are no functions for traversing the list without consuming its elements. The current API is therefore, I have to admit, somewhat restrictive, but more on that in the [roadmap section](#roadmap) below.

## Files and directories explained

* [doxygen](./doxygen) — A directory that contains [Doxygen](https://github.com/doxygen/doxygen)-related stuff used to generate the [API documentation website](https://bb-301.github.io/c-generic-doubly-linked-list-docs) for this library.
* [examples](./examples) — A directory that contains standalone examples illustrating how the library's different features can be used. The [Makefile](./Makefile) declares a recipe for each example. For instance, to run [examples/quick_example.c](examples/quick_example.c) simply run `make example_quick_example` (without the `.c` extension at the end of the file name).
* [include](./include) — A directory that contains the header file [g2l.h](./include/g2l.h); i.e., the declarations for the library's public API.
* [src](./src) — A directory that contains the implementation file [g2l.c](./src/g2l.c), in which all of the definitions for the functions and types declared in [g2l.h](./include/g2l.h) are provided.
* [LICENSE](./LICENSE) — A file containing the license and copyright information for this project.
* [Makefile](./Makefile) — A `Makefile` (for use with [GNU Make](https://www.gnu.org/software/make/)), which is provided as a convenience, and which can be used to automate operations such as building the library, running the examples, building the API documentation website, and installing/uninstalling the library on the target system. You may run `make` or `make help` for a list of all relevant recipes. **WARNING**: If you ever decide to use `make install`, please first make sure that `/usr/local/{lib|include|man}` are valid installation paths on your system, and, if not, make sure to adjust them first. Installing and uninstalling at those locations will require `sudo` privileges.
* [VERSION](./VERSION) — A simple text file that contains the library's current version. This is used by the [Makefile](./Makefile) to generate the documentation website and to "suffix" the library binaries with the current version number.

## Disclaimer

This is an experimental library, so please use with caution, and please feel free to [let me know](#contact) if you find anything wrong in my implementation (or in the project in general).

## Roadmap

* I want to find and add more examples illustrating use cases for which this library could be useful.
* As alluded to in this document's [how it works](#how-it-works) section, the current API is somewhat limited, only allowing to push and pop or shift elements from the linked list. In a next version, I plan on adding more functions that will allow traversing a list without consuming it.

## Contact

If you have any questions, if you find bugs, or if you have suggestions for this project, please feel free to contact me by opening an issue on the [repository](https://github.com/BB-301/c-generic-doubly-linked-list/issues).

## License

This project is released under the [MIT License](./LICENSE).

## Copyright

Copyright (c) 2024 BB-301 (fw3dg3@gmail.com)