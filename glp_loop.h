#ifdef __cplusplus
extern "C"
{
#endif

#ifndef game_loop_loop_h
#define game_loop_loop_h

#include <stdint.h>

void glp_loop_init(int fps);
void glp_loop_update();

float glp_get_dt();

// return in microseconds
uint32_t glp_get_time();

#endif // game_loop_loop_h

#ifdef __cplusplus
}
#endif