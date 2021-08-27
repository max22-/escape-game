#include "profilab.h"
#include <Arduino.h>

void profilab_encode(profilab_data_t data, /* return value --> */ unsigned char (*buffer)[12])
{
    double n = data.n;
    for(int i = 0; i < sizeof(*buffer); i++) (*buffer)[i] = 0;
    (*buffer)[11] = data.channel;
    unsigned char sign = n < 0;
    if(n < 0) n*= -1;
    int exp = 0;
    for(exp = 0; n >= 2.0; n /= 2.0, ++exp);
    for(; n < 1.0; n*= 2.0, --exp);
    exp += 16383;
    (*buffer)[9] = exp / 256;
    (*buffer)[8] = exp % 256;
    if(sign) (*buffer)[8] |= 1 << 8;
    n *= 128;
    for(int i = 7; i >= 1; i--) {
        unsigned char t = (unsigned char)n;
        (*buffer)[i] = t;
        n = (n - t) * 256;
    }
}

profilab_data_t profilab_decode(unsigned char *buffer)
{
    profilab_data_t result;
    result.channel = buffer[11];
    double n;

    n = 0;
    for(int i = 1; i <= 7; i++)
        n = buffer[i] + n / 256.0;
    n /= 128.0;
    if(buffer[9] < 0)
        n *= -1.0;
    unsigned short exp = buffer[8] + (buffer[9] << 8);
    double exponent = exp - 16383;
    n *= pow(2, exponent);

    result.n = n;
    return result;
}