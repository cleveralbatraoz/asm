#include "flags.h"

#include <stdbool.h>
#include <stdint.h>

bool isSignDefault(uint8_t flags)
{
    return ((flags >> fSignMaskOffset) & fSignMask) == fSignDefault;
}

bool isSignAll(uint8_t flags)
{
    return ((flags >> fSignMaskOffset) & fSignMask) == fSignAll;
}

bool isSignSpace(uint8_t flags)
{
    return ((flags >> fSignMaskOffset) & fSignMask) == fSignSpace;
}

void setSignDefault(uint8_t *flags)
{
    *flags &= ~(fSignMask << fSignMaskOffset);
    *flags |= fSignDefault << fSignMaskOffset;
}

void setSignAll(uint8_t *flags)
{
    *flags &= ~(fSignMask << fSignMaskOffset);
    *flags |= fSignAll << fSignMaskOffset;
}

void setSignSpace(uint8_t *flags)
{
    *flags &= ~(fSignMask << fSignMaskOffset);
    *flags |= fSignSpace << fSignMaskOffset;
}

bool isLeadingZeroesDisable(uint8_t flags)
{
    return ((flags >> fLeadingZeroesOffset) & fLeadingZeroesMask) == fLeadingZeroesDisable;
}

bool isLeadingZeroesEnable(uint8_t flags)
{
    return ((flags >> fLeadingZeroesOffset) & fLeadingZeroesMask) == fLeadingZeroesEnable;
}

void setLeadingZeroesDisable(uint8_t *flags)
{
    *flags &= ~(fLeadingZeroesMask << fLeadingZeroesOffset);
    *flags |= fLeadingZeroesDisable << fLeadingZeroesOffset;
}

void setLeadingZeroesEnable(uint8_t *flags)
{
    *flags &= ~(fLeadingZeroesMask << fLeadingZeroesOffset);
    *flags |= fLeadingZeroesEnable << fLeadingZeroesOffset;
}

bool isAlignLeft(uint8_t flags)
{
    return ((flags >> fAlignOffset) & fAlignMask) == fAlignLeft;
}

bool isAlignRight(uint8_t flags)
{
    return ((flags >> fAlignOffset) & fAlignMask) == fAlignRight;
}

void setAlignLeft(uint8_t *flags)
{
    *flags &= ~(fAlignMask << fAlignOffset);
    *flags |= fAlignLeft << fAlignOffset;
}

void setAlignRight(uint8_t *flags)
{
    *flags &= ~(fAlignMask << fAlignOffset);
    *flags |= fAlignRight << fAlignOffset;
}
