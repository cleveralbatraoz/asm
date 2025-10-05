#pragma once

#include <stdbool.h>
#include <stdint.h>

#define FLAG_SIGN_PLUS 0x01
#define FLAG_SIGN_SPACE 0x02
#define FLAG_ZERO_PAD 0x04
#define FLAG_LEFT_ALIGN 0x08

#define HAS_SIGN_PLUS(f) ((f)&FLAG_SIGN_PLUS)
#define HAS_SIGN_SPACE(f) ((f)&FLAG_SIGN_SPACE)
#define HAS_ZERO_PAD(f) ((f)&FLAG_ZERO_PAD)
#define HAS_LEFT_ALIGN(f) ((f)&FLAG_LEFT_ALIGN)
