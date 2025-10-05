#include "format.h"

#include "flags.h"

#include <stdbool.h>
#include <stddef.h>

void parse_format(const char *fmt_str, struct Format *fmt_spec)
{
    fmt_spec->flags = 0x0;
    fmt_spec->width = 0;

    if (fmt_str == NULL || *fmt_str != '%')
    {
        return;
    }

    ++fmt_str;

    bool run = true;
    while (run && *fmt_str != '\0')
    {
        switch (*fmt_str)
        {
        case '-': {
            setAlignLeft(&fmt_spec->flags);
            setLeadingZeroesDisable(&fmt_spec->flags);
            break;
        }
        case '+': {
            setSignAll(&fmt_spec->flags);
            break;
        }
        case ' ': {
            if (!isSignAll(fmt_spec->flags))
            {
                setSignSpace(&fmt_spec->flags);
            }
            break;
        }
        case '0': {
            if (isAlignLeft(fmt_spec->flags))
            {
                run = false;
            }
            else
            {
                setLeadingZeroesEnable(&fmt_spec->flags);
            }
            break;
        }
        default: {
            run = false;
        }
        }

        if (run)
        {
            ++fmt_str;
        }
    }

    while (*fmt_str != '\0' && '0' <= *fmt_str && *fmt_str <= '9')
    {
        fmt_spec->width = fmt_spec->width * 10 + *fmt_str - '0';
        ++fmt_str;
    }
}
