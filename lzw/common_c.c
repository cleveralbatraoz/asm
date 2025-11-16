#include "common.h"

#include <stdbool.h>
#include <stdint.h>

bool is_power_of_two(uint32_t value)
{
    return value && !(value & (value - 1));
}
