#include "untils.h"

unsigned char reverse_byte(unsigned char val)
{
    return ((val << 6) & 192) |((val >> 6) & 3) |
            ((val << 4) & 48) | ((val >> 4) & 12);
}
