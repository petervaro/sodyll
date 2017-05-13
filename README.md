[![[license: GPLv3]][1]][2]
[![[standard: ISO C11]][3]][4]

- - -

![sodyll][5]

- [What is `sodyll`?](#what-is-sodyll)
- [Why is it called `sodyll`?](#why-is-it-called-sodyll)
- [Dependencies](#dependencies)
- [Install](#install)
- [Tutorials](#tutorials)
- [Documentation](#documentation)
- [License](#license)



What is `sodyll`?
-----------------

`sodyll` is a tiny, single header, wrapper *library* which makes opening,
closing and loading objects and functions from dynamically loaded libraries easy
and safe in a strictly conforming and portable way.



Why is it called `sodyll`?
--------------------------

The word itself does not make any sense, except it is built from the letters of
the most widely adopted platform's extensions for dynamically loaded libraries.
That is, **SO**dyll for UNIX-like system's *shared object*; so**DYL**l for
`dylib` on MacOS system's *dynamic library*; and so**D**y**LL** on Windows
system's *dynamically linked library*.


Dependencies
------------

On POSIX compilant platforms, one has to link it to `libdl`, that is: `-ldl`.


Install
-------

One should move the content of the `include` folder of this repository to
anywhere, where the compiler can see it. And then one should include it like:

```c
#include <sodyll/sodyll.h>
```

For example on linux, to be available on the system level, one should do this:

```bash
$ sudo cp include/* /usr/local/include
```


Tutorials
---------

The following code snippet is compiled to a dynamically linked library, called
`myprinter` plus the platform specific extension:

```c
#include <stdio.h>
#include <sodyll/sodyll.h>

/* Declarations */
extern SO_EXPORT const char *TEST_STRING;

SO_EXPORT void
MyPrinter(const char *string);

/* Definitions */
SO_EXPORT const char *TEST_STRING = "testing...";

SO_EXPORT void
MyPrinter(const char *string)
{
    puts(string);
}
```



The following code snippet will load the symbols `TEST_STRING` and `MyPrinter`
from the above mentioned dynamically loaded library:

```c
#include <stdio.h>
#include <stdlib.h>
#include <sodyll/sodyll.h>

int
main(void)
{
    void        *shared_object;
    const char  *text;
    void       (*printer)(const char*);
    int          exit_code = EXIT_SUCCESS;

    /* Open shared object */
    #define SHARED_OBJECT "myprinter" SO_LIBRARY_EXTENSTION
    if (!(shared_object = so_Open(SHARED_OBJECT, so_LAZY)))
    {
        fputs("Cannot load shared object: `" SHARED_OBJECT "'\n", stderr);
        exit_code = EXIT_FAILURE;
        goto open_failed;
    }

    /* Load data object */
    #define OBJECT "TEST_STRING"
    if (!(text = *(const char **)so_GetData(shared_object, OBJECT)))
    {
        fputs("Cannot get object: `" OBJECT "' "
              "from shared object `" SHARED_OBJECT "'\n", stderr);
        exit_code = EXIT_FAILURE;
        goto clean_up;
    }

    /* Load function object */
    #define FUNCTION "MyPrinter"
    if (!(printer = (void(*)(const char*))so_GetFunc(shared_object, FUNCTION)))
    {
        fputs("Cannot get function: `" FUNCTION "' "
              "from shared object `" SHARED_OBJECT "'\n", stderr);
        exit_code = EXIT_FAILURE;
        goto clean_up;
    }

    /* Invoke loaded function */
    printer(text);

    /* Close shared object */
    clean_up:
    if (so_Close(shared_object))
        fputs("Cannot close shared object: `" SHARED_OBJECT "'\n", stderr);

    open_failed:
    return exit_code;
}
```



Documentation
-------------

- - -
```c
#define SO_LIBRARY_EXTENSION
```
This macro expands to a nul-terminated `const char*` string object, which is:
`".so"` on UNIX like systems, `".dylib"` on MacOS and `".dll"` on Windows.



- - -
```c
#define SO_EXPORT
```
This macro should be used to indicate which objects needs to be *exported* by
the dynamically loaded library.



- - -
```c
typedef enum
{
    so_LAZY,
    so_NOW,
    so_GLOBAL,
    so_LOCAL,
    so_NODELETE,
    so_NOLOAD,
    so_DEEPBIND
} so_Flags;
```
This type is to indicate how to load the dynamically linked library. They have
the same meaning as the similarly named dlfcn ones. For further info on them,
please consult the documentation of those flags. (`man dlopen(3)`)

On Windows, these flags have no effects.

The `so_NODELETE`, `so_NOLOAD` and `so_DEEPBIND` flags may have no effects if
they are not defined by the platform.


- - -
```c
void*
so_Open(const char *path,
        so_Flags    flags);
```
This function will open a dynamically loaded library, (that is, it will only
increase the reference counter if the library is already loaded or it will load
it for the first time) specified by the file name at `path` in the given mode,
specified by `flags`.

`path` has to contain the extension as well, not just the file name. The
function will search the `LD_LIBRARY_PATH` for the library.

`flags` has to be either `so_LAZY` or `so_NOW`, other values can be `|`-ed to
these.

Returns a valid, opaque handler pointer on success, `NULL` otherwise.


- - -
```c
void*
so_GetData(void       *restrict handler,
           const char *restrict symbol);
```
This function will look up and get a pointer to the object in the library
specified by `handler` and named as `symbol`.

Returns a pointer to an exported object on success, `NULL` otherwise.


- - -
```c
void
(*so_GetFunc(void       *restrict handler,
             const char *restrict symbol))(void);
```
This function will look up and get a pointer to the function in the library
specified by `handler` and named as `symbol`.

Returns a pointer to an exported function on success, `NULL` otherwise.


- - -
```c
bool
so_Close(void *handler);
```
This function will close the dynamically linked library, (that is, it will only
decrease the reference counter if others are using this library, or it will
unload it if the caller is holding the last reference to it), specified by the
`handler` to it.



License
-------

Copyright &copy; 2017 **Peter Varo**

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program, most likely a file in the root directory, called 'LICENSE'.
If not, see <http://www.gnu.org/licenses>.

- - -

The font used and modified in the logo is called **Selima Script**. Its license
allows both commercial and personal use. (Lisense type unknown.)

Copyright &copy; 2016 **JROH Creative**

<!-- -->

[1]: img/badges/gnu.png
[2]: http://www.gnu.org/licenses/gpl.html
[3]: img/badges/c11.png
[4]: http://www.open-std.org/jtc1/sc22/WG14/www/docs/n1570.pdf
[5]: img/logo.png?raw=true "SODYLL"
