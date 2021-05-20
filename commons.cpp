#include "commons.h"

int setBitAmount(unsigned char c) {
    int count = 0;
    const unsigned char setter = 0x01;
    for (int i = 0; i < 8; ++i) {
        unsigned char skew = (c >> i);
        skew &= setter;
        if (skew) {
            ++count;
        }
    }
    return count;
}