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
