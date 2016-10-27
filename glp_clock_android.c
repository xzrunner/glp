#include "glp_clock.h"

struct glp_clock* 
glp_clock_create() {
    return NULL;
}

void 
glp_clock_release(struct glp_clock* clk) {
}

void 
glp_clock_start(struct glp_clock* clk) {
}

uint32_t 
glp_clock_get_time(struct glp_clock* clk) {
	return 0;
}

uint32_t 
glp_clock_get_during(struct glp_clock* clk, bool reset) {
	return 0;
}