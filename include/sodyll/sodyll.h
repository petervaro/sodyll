/* INFO **
** INFO */

#ifndef SODYLL_H_4985298626809681
#define SODYLL_H_4985298626809681 1

/* Include standard headers */
#include <stdio.h>
#include <stdbool.h>


/*----------------------------------------------------------------------------*/
/* On Macintosh platform */
#if defined __APPLE__ &&                                                       \
    defined __MACH__
        #define SO_LIBRARY_EXTENSTION ".dylib"
/* On other POSIX platforms */
#elif defined unix        ||                                                   \
      defined __unix      ||                                                   \
      defined __unix__    ||                                                   \
      defined __linux__   ||                                                   \
      defined __FreeBSD__ ||                                                   \
      defined __CYGWIN__
        #define SO_LIBRARY_EXTENSTION ".so"
/* On Windows platform */
#elif defined WIN32   ||                                                       \
      defined _WIN32  ||                                                       \
      defined __WIN32
        #define SO_LIBRARY_EXTENSTION ".dll"
/* On other platforms */
#else
    #define SO_LIBRARY_EXTENSTION
#endif



/*----------------------------------------------------------------------------*/
/* On POSIX conforming platforms */
#if defined unix        ||                                                     \
    defined __unix      ||                                                     \
    defined __unix__    ||                                                     \
    defined __linux__   ||                                                     \
    defined __FreeBSD__ ||                                                     \
    defined __CYGWIN__  ||                                                     \
    (defined __APPLE__ && defined __MACH__)

    #include <dlfcn.h>

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    typedef enum
    {
        so_LAZY   = RTLD_LAZY,
        so_NOW    = RTLD_NOW,
        so_GLOBAL = RTLD_GLOBAL,
        so_LOCAL  = RTLD_LOCAL,
        /* TODO: what if the following values are not defined as CPP tokens, but
                 are enums? How to detect those? what does the POSIX say about
                 the implementation details of these objects? */
        #ifdef RTLD_NODELETE
            so_NODELETE = RTLD_NODELETE,
        #else
            so_NODELETE = 0,
        #endif
        #ifdef RTLD_NOLOAD
            so_NOLOAD = RTLD_NOLOAD,
        #else
            so_NOLOAD = 0,
        #endif
        #ifdef RTLD_DEEPBIND
            so_DEEPBIND = RTLD_DEEPBIND,
        #else
            so_DEEPBIND = 0,
        #endif
    } so_Flags;


    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    #define SO_EXPORT


    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    static inline void*
    so_Open(const char *path,
            so_Flags    flags)
    {
        void *handler = NULL;

        /* Clear errors */
        (void)dlerror();

        /* Check arguments */
        if (!path)
            fputs("`path' is NULL\n", stderr);
        else if ((flags & so_LAZY) != so_LAZY &&
                 (flags & so_NOW)  != so_NOW)
            fputs("Invalid `flags': either `so_LAZY' or "
                  "`so_NOW' must be specified\n", stderr);
        /* Get handler */
        else if (!(handler = dlopen(path, flags)))
        {
            fputs(dlerror(), stderr);
            fputc('\n', stderr);
        }

        return handler;
    }


    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    static inline void*
    so_GetData(void       *restrict handler,
               const char *restrict symbol)
    {
        const char *error;
        void       *data = NULL;

        /* Clear errors */
        (void)dlerror();

        /* Check arguments */
        if (!handler)
            fputs("`handler' is NULL\n", stderr);
        else if (!symbol)
            fputs("`symbol' is NULL\n", stderr);
        /* Get data object */
        else if (!(data = dlsym(handler, symbol)) &&
                 (error = dlerror()))
        {
            fputs(error, stderr);
            fputc('\n', stderr);
        }

        return data;
    }


    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    static inline void
    (*so_GetFunc(void       *restrict handler,
                 const char *restrict symbol))(void)
    {
        const char  *error;
        void       (*function)(void) = NULL;

        /* Clear errors */
        (void)dlerror();

        /* Check arguments */
        if (!handler)
            fputs("`handler' is NULL\n", stderr);
        else if (!symbol)
            fputs("`symbol' is NULL\n", stderr);
        /* Get function object */
        else if (!(*(void **)(&function) = dlsym(handler, symbol)) &&
                 (error = dlerror()))
        {
            fputs(error, stderr);
            fputc('\n', stderr);
        }

        return function;
    }


    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    static inline bool
    so_Close(void *handler)
    {
        bool signal;

        /* Clear errors */
        (void)dlerror();

        if (!handler)
            return false;
        else if ((signal = dlclose(handler)))
        {
            fputs(dlerror(), stderr);
            fputc('\n', stderr);
        }

        return signal;
    }

/* On windows platforms */
#elif defined WIN32   ||                                                       \
      defined _WIN32  ||                                                       \
      defined __WIN32

    /* !!! The following implementation has never been tested under Windows.
           Based on the available documentations it will work, but obviously it
           is not a guarantee for anything */

    /* TODO: Validate the arguments before their usages */

    /* TODO: How to determine the error on Windows? Look for something similar
             to dlerror() on POSIX, or the meanings of the return values */

    #include <windows.h>

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    typedef enum
    {
        so_LAZY     = 0;
        so_NOW      = 0;
        so_GLOBAL   = 0;
        so_LOCAL    = 0;
        so_NODELETE = 0;
        so_NOLOAD   = 0;
        so_DEEPBIND = 0;
    } so_Flags;


    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    #define SO_EXPORT __declspec(dllexport)


    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    static inline void*
    so_Open(const char *path,
            so_Flags    _)
    {
        (void)_;
        return (void *)LoadLibrary((LPCTSTR)path);
    }


    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    static inline void*
    so_GetData(void       *restrict handler,
               const char *restrict symbol)
    {
        return (void *)GetProcAddress((HMODULE)handler, (LPCTSTR)symbol);
    }


    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    static inline void
    (*so_GetFunc(void))(void       *restrict handler,
                        const char *restrict symbol)
    {
        void (*function)(void);
        *(void **)(&function) = (void *)GetProcAddress((HMODULE)handler,
                                                       (LPCTSTR)symbol);
        return function;
    }


    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    static inline bool
    so_Close(void *handler)
    {
        return (bool)FreeLibrary((HMODULE)handler);
    }

/* On other platforms */
#else
    #error "sodyll does not support this platform"
#endif /* platform */
#endif /* SODYLL_H_4985298626809681 */
