#ifndef LIGHT_H
#define LIGHT_H

void light_begin(void (*task)(void*));
void light_trigger();
void light_set_level(float level);
void light_ramp(float target_level, float time);

#endif