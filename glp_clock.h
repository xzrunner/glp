#ifdef __cplusplus
extern "C"
{
#endif

#ifndef game_loop_clock_h
#define game_loop_clock_h

#include <stdbool.h>
#include <stdint.h>

struct glp_clock;

struct glp_clock* glp_clock_create();
void glp_clock_release(struct glp_clock*);

void glp_clock_start(struct glp_clock*);

// return in microseconds
uint32_t glp_clock_get_time(struct glp_clock*);
uint32_t glp_clock_get_during(struct glp_clock*, bool reset);

#endif // game_loop_clock_h

#ifdef __cplusplus
}
#endif