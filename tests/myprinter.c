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
