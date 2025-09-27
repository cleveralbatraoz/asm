#include <stdio.h>

#include "print.h"

int main(void)
{
    {
        char out[128];
        print(out, "%-10i", "DEAD");
        puts(out);
    }

    {
        char out[128];
        print(out, "%i", "-1");
        puts(out);
    }

    {
        char out[128];
        print(out, "%i", "-FFFF");
        puts(out);
    }

    {
        char out[128];
        print(out, "%i", "FFFF");
        puts(out);
    }

    {
        char out[128];
        print(out, "%i", "-10");
        puts(out);
    }

    return 0;
}
