#include "common.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool error(int16_t code)
{
    return code < 0;
}

char const *error_message(int16_t error_code)
{
    switch (error_code)
    {
    case INVALID_CODE: {
        return "Invalid code";
    }
    case TABLE_OVERFLOW: {
        return "Table overflow";
    }
    case DECODE_TABLE_INVARIANT_VIOLATION: {
        return "Table invariant violation";
    }
    }
    return "Unknown error";
}

bool is_valid_code(int16_t code)
{
    return code >= 0 && code < MAX_CODE && code != CLEAR_CODE && code != END_OF_INFORMATION;
}
