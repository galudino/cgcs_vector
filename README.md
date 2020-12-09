# cgcs_vector

A data structure for the C language; a buffer (vector) of pointers.

<b>Note:</b><br>
This document, and this repo -- is a work in progress.

## Building:

The minimum version of C required is `C99`.

Repository is currently being overhauled.
A `CMakeLists.txt` file will be provided once maintenance is complete.

For now, you may build the test client and `cgcs_vector` source code 
with this `Terminal` command:

```c
cc -std=c99 -pedantic-errors -Wall -Werr -o main cgcs_vector_test.c cgcs_vector.c
```

If you want just the `cgcs_vector` library, (the `.o` file), you may use this `Terminal` command:

```c
cc -std=c99 -pedantic-errors -Wall -Werror -c cgcs_vector.c -o cgcs_vector.o
```

If using an IDE, simply bring over `cgcs_vector.h` and `cgcs_vector.c`
into your project's IDE.

## Foreword on <code><b>struct</b> cgcs_vector</code>, aka <code>cgcs_vector</code>

The C language leaves it up to the user to create their own data structures.<br>
Whenever I would write C++, I would always be grateful for <code>std::vector</code>,<br>
as it is a very useful data structure to have, it's versatile, and it's fast.

My goal with <code>cgcs_vector</code> is to have a data structure<br>
that can be just as useful, when you must program in C.

I have been down this rabbit hole before, with one of my previous repositories,<br>
<code>gcslib</code>, which was meant to be an entire container library for C, which was loosely inspired by the C++ STL.<br>
Over time, I ended up abandoning it, realizing that I had over engineered everything. 

In practice, all I ever really needed was a useful "vector" data structure,<br>
so I think it's best to focus my sights on that.

Although I am taking inspiration from C++, there is no use in trying to reinvent the wheel.<br>
I also don't think there is much use in trying to implement "generics",<br>
or to create too many abstractions.

In the <code>gcslib</code> repository, there is a subdirectory, <code>test__vptr</code> --<br>
and this new repo is pretty much picking up where I left off with <code>test__vptr</code>.

<code><b>struct</b> cgcs_vector</code>'s alias, or <code><b>typedef</b></code> is <code>cgcs_vector</code>.

## How is it implemented

Often times while writing in C, large objects (instances of <code>struct</code>),<br>
or strings (represented as <code>char *</code>) have to be dynamically allocated.<br>
We do not have constructors/destructors, references, or RAII in C.

I have decided to implement <code>cgcs_vector</code> as follows<br>
(The memory layout is inspired from the GCC implementation of <code>std::vector</code>)

```
typedef struct cgcs_vector cgcs_vector;
typedef void *voidptr;                      
typedef voidptr *cgcs_vector_iterator;

// The "vector" structure
struct cgcs_vector {
    struct cgcs_vector_base {
        voidptr *m_start;
        voidptr *m_finish;
        voidptr *m_end_of_storage;
    } m_impl;
};
```
<b>We are ultimately dealing with a buffer of pointers.<br>
Each block is <code>sizeof(void *)</code> bytes large.</b>

- ### <code><b>typedef struct</b> cgcs_vector cgcs_vector </code>

    We alias <code><b>struct</b> cgcs_vector</code> with <code>cgcs_vector</code>.

- ### <code><b>typedef void *</b> voidptr</code>

    <code>m_start</code> in <code><b>struct</b> cgcs_vector_base</code><br>
    is a pointer that addresses a <b>buffer of pointers</b>.<br>
    By aliasing <code><b>void *</b></code>, I believe this idea is made clearer.

    <code>m_finish</code> addresses one-past the last block in <code>m_start</code>.<br>
    If <code>m_start</code> and <code>m_finish</code> are the same address, <code>m_start</code> is an empty buffer.

    <code>m_end_of_storage</code> addresses the last block in <code>m_start</code>.<br>
    If <code>m_finish</code> and <code>m_end_of_storage</code> are the same address,<br>
    the buffer will be full after using the space at <code>m_finish</code>.

- ### <code><b>typedef</b> voidptr *cgcs_vector_iterator</code>

    Since we can do pointer arithmetic on a <code>voidptr *</code>, or <code>void **</code>,<br>
    we will alias it so that the user knows that it can be used as an iterator.

## Sample Usage

Since the C language does not have the <code>namespace</code> concept,<br>
we can 'fake' it by prefixing <code>cgcs_</code> to all of the <code><b>struct</b></code> types, functions, etc.

<code>gcslib</code> was my old C container library;<br>
I have been using '<code>gcs</code>' to prefix/namespace much of my work.<br>
Since I have been using '<code>gcs</code>' in my C++ libraries as well,<br>
I am using '<code>cgcs</code>' to denote that this is a C library.

<code>cgcs::vector</code> is 'approximated' using
```
cgcs_vector
```

Here is what a small program would look like using <code>cgcs_vector</code>:

```
#include "cgcs_vector.h"

#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[]) {
    size_t capacity = 2;
    cgcs_vector_t vec;
    cgcs_vinit(&vec, capacity);

    char *str = NULL;

    str = strdup("beta");
    cgcs_vpushb(&vec, &str);

    str = strdup("charlie");
    cgcs_vpushb(&vec, &str);

    str = strdup("alpha");
    cgcs_vpushb(&vec, &str);

    cgcs_vector_iterator it = cgcs_vbegin(&vec);
    cgcs_vector_iterator end = cgcs_vend(&vec);
    char **curr = NULL;

    while (it < end) {
        curr = (char **)(it++);
        printf("iterator: %s\n", *curr);
        free(*curr);
    }

    cgcs_vdeinit(&vec);
    return 0;
}
```

We define a capacity of 2,<br>
then we create an instance of <code>cgcs_vector</code>, <code>vec</code>, on the stack.<br>

We then call the <code>init</code> function on <code>&vec</code>.<br>
This will allocate memory for vec's internal buffer.

Then, we are going to create 3 strings, dynamically allocated, client-side.<br>
We'll push each string using the <code>pushb</code> function, which is short for <b>push_back</b>.<br>
Notice we provide the <b>address of <code>str</code></b>.

We are transferring ownership of these pointers<br> to <code>vec</code>,<br>
but we must deallocate the memory for these strings at some point<br>
when we are finished with them.

Next, we iterate over <code>vec</code>, by retrieving its <b>iterators</b>.<br>

Iterator retrieval is merely a notational convenience, since<br>
the <code>begin</code> function returns the <code>m_start</code> field,<br>
and the <code>end</code> function returns the <code>m_finish</code> field.<br>

After we iterate over the pointers, print them, and then free them --<br>
we call the <code>deinit</code> function on <code>&vec</code>,<br>
which will free <code>vec</code>'s internal buffer.

### Approximating the <code>using</code> statement from C++

By using the directive

```
#define USE_CGCS_VECTOR
```

before

```
#include "cgcs_vector.h"
```

...we can write code like this:

```
#define USE_CGCS_VECTOR
#include "cgcs_vector.h"

#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[]) {
    size_t capacity = 2;
    vector_t vec;
    vinit(&vec, capacity);

    char *str = NULL;

    str = strdup("beta");
    vpushb(&vec, &str);

    str = strdup("charlie");
    vpushb(&vec, &str);

    str = strdup("alpha");
    vpushb(&vec, &str);

    vector_iterator it = vbegin(&vec);
    vector_iterator end = vend(&vec);
    char **curr = NULL;

    while (it < end) {
        curr = (char **)(it++);
        printf("iterator: %s\n", *curr);
        free(*curr);
    }

    vdeinit(&vec);
    return 0;
}
```

What results is the omission of '<code>cgcs_</code>' from this library's types and functions.
