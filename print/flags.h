#pragma once

#include <stdbool.h>
#include <stdint.h>

enum SignFlags
{
    fSignDefault = 0,
    fSignAll = 1,
    fSignSpace = 2,
};

enum LeadingZeroesFlags
{
    fLeadingZeroesDisable = 0,
    fLeadingZeroesEnable = 1,
};

enum AlignFlags
{
    fAlignRight = 0,
    fAlignLeft = 1,
};

enum Offsets
{
    fSignMaskOffset = 0,
    fLeadingZeroesOffset = 2,
    fAlignOffset = 3,
};

enum Masks
{
    fSignMask = 0x3,
    fLeadingZeroesMask = 0x1,
    fAlignMask = 0x1,
};

bool isSignDefault(uint8_t flags);
bool isSignAll(uint8_t flags);
bool isSignSpace(uint8_t flags);

void setSignDefault(uint8_t *flags);
void setSignAll(uint8_t *flags);
void setSignSpace(uint8_t *flags);

bool isLeadingZeroesDisable(uint8_t flags);
bool isLeadingZeroesEnable(uint8_t flags);
void setLeadingZeroesDisable(uint8_t *flags);

void setLeadingZeroesEnable(uint8_t *flags);

bool isAlignLeft(uint8_t flags);
bool isAlignRight(uint8_t flags);

void setAlignLeft(uint8_t *flags);
void setAlignRight(uint8_t *flags);
