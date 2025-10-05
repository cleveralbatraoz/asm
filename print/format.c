#include "format.h"

#include "flags.h"

#include <stdbool.h>
#include <stddef.h>

void parse_format(const char *fmt_str, struct Format *fmt_spec)
{
    fmt_spec->flags = 0x0;
    fmt_spec->width = 0;

    if (fmt_str == NULL)
    {
        return;
    }

    if (*fmt_str == '%')
    {
        ++fmt_str;
    }

    bool run = true;
    while (run && *fmt_str != '\0')
    {
        switch (*fmt_str)
        {
        case '-':
            fmt_spec->flags |= FLAG_LEFT_ALIGN;
            fmt_spec->flags &= ~FLAG_ZERO_PAD;

            break;
        case '+':
            fmt_spec->flags |= FLAG_SIGN_PLUS;
            fmt_spec->flags &= ~FLAG_SIGN_SPACE;

            break;
        case ' ':
            if (!HAS_SIGN_PLUS(fmt_spec->flags))
            {
                fmt_spec->flags |= FLAG_SIGN_SPACE;
            }

            break;
        case '0':
            if (!HAS_LEFT_ALIGN(fmt_spec->flags) && !HAS_ZERO_PAD(fmt_spec->flags))
            {
                fmt_spec->flags |= FLAG_ZERO_PAD;
            }
            break;
        default:
            run = false;
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
