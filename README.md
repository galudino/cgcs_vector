# cgcs_vector

A data structure for the C language; a buffer (vector) of pointers.

<b>Note:</b><br>
This document, and this repo -- is a work in progress.

## Directories:
### `demo` - target to demonstrate usage of `cgcs_vector`
- <code>cgcs_vector_demo.c</code>
  - demonstrates usage of `cgcs_vector`.
- <code>CMakeLists.txt</code>
  - `cmake` instructions on building this target

### `src` - target for `cgcs_vector` library
- <code>cgcs_vector.c</code>
  - Implementation details
- <code>cgcs_vector.h</code>
  - Public declarations

## Building:

Run the included `makebuilds` script to have `cmake`<br>
create Unix-Makefile builds in the following modes:
- `Debug`
- `Release`
- `MinSizeRel`
- `RelWithDebInfo`

```
% ./makebuilds
```

A `build/make` subdirectory will be created with subdirectories<br>
of the modes described above. 

If we want to create a `Debug` build
of our demo program (which will also build the `cgcs_vector` library):

```
make -C ./build/make/Debug/demo
```

Generally,
```
make -C ./build/make/[build-mode]/[target-name]
```

If you want to use an alternative build system, i.e. Xcode or Visual Studio<br>
(see the list of supported generators on your system using `cmake -help`), invoke the following:
```
% cmake -S ./ -B ./build/[generator-name] -G "[generator-name]"
```

For example, for Xcode:
```
% cmake -S ./ -B ./build/xcode -G "Xcode"
```

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

<code><b>struct</b> cgcs_vector</code>'s alias, or <code><b>typedef</b></code> is <code>vector_t</code>.

## How is it implemented

Often times while writing in C, large objects (instances of <code>struct</code>),<br>
or strings (represented as <code>char *</code>) have to be dynamically allocated.<br>
We do not have constructors/destructors, references, or RAII in C.

I have decided to implement <code>cgcs_vector</code> as follows<br>
(The memory layout is inspired from the GCC implementation of <code>std::vector</code>)

```
typedef struct cgcs_vector vector_t;
typedef void *voidptr;                      
typedef voidptr *vector_iterator_t;

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

- ### <code><b>typedef struct</b> cgcs_vector cgcs_vector_t</code>

    We alias <code><b>struct</b> cgcs_vector</code> with <code>vector_t</code>.

- ### <code><b>typedef void *</b> voidptr</code>

    <code>m_start</code> in <code><b>struct</b> cgcs_vector_base</code><br>
    is a pointer that addresses a <b>buffer of pointers</b>.<br>
    By aliasing <code><b>void *</b></code>, I believe this idea is made clearer.

    <code>m_finish</code> addresses one-past the last block in <code>m_start</code>.<br>
    If <code>m_start</code> and <code>m_finish</code> are the same address, <code>m_start</code> is an empty buffer.

    <code>m_end_of_storage</code> addresses the last block in <code>m_start</code>.<br>
    If <code>m_finish</code> and <code>m_end_of_storage</code> are the same address,<br>
    the buffer will be full after using the space at <code>m_finish</code>.

- ### <code><b>typedef</b> voidptr *vector_iterator_t</code>

    Since we can do pointer arithmetic on a <code>voidptr *</code>, or <code>void **</code>,<br>
    we will alias it so that the user knows that it can be used as an iterator.

## Sample Usage

TODO - please see comments/documentation for now.
