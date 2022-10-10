#include <stdint.h>
#include <assert.h>

#include "general.h"

//------------------------------------------------------------------

int64_t get_hash (void *base, unsigned long len) 
{
    const unsigned int m = 0x5bd1e995;
    const unsigned int seed = 0;
    const int r = 24;

    unsigned long h = seed ^ len;

    const unsigned char *data = (const unsigned char *) base;
    assert (base);

    unsigned int k = 0;

    while (len >= 4) {

        k  = (unsigned) data[0];
        k |= (unsigned) (data[1] << 8);
        k |= (unsigned) (data[2] << 16);
        k |= (unsigned) (data[3] << 24);

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len  -= 4;

    }

    switch (len)
    {
        case 3: {
            h ^= (unsigned long) data[2] << 16;
        }

        case 2: {
            h ^= (unsigned long) data[1] << 8;
        }
        
        case 1: {
            h ^= data[0];
            h *= m;
        }

        default: {}
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return  (unsigned) h;
}
