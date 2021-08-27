#ifndef PROFILAB_H
#define PROFILAB_H

typedef struct {
  unsigned char channel;
  double n;  
} profilab_data_t;

void profilab_encode(profilab_data_t data, /* return value -> */ unsigned char (*buffer)[12]);
profilab_data_t profilab_decode(unsigned char *buffer);

#endif