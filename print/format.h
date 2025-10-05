#pragma once

#include <stdint.h>

struct Format
{
    uint8_t flags;
    uint32_t width;
};

void parse_format(const char *fmt_str, struct Format *fmt_spec);
