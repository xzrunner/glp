#include "glp_clock.h"

#include <mach/mach_time.h>

#include <stdlib.h>
#include <string.h>

static const int64_t ONE_THOUSAND = 1000;
static const int64_t ONE_MILLION = 1000 * 1000;

struct glp_clock {
	mach_timebase_info_data_t timebase_info;
	uint64_t start, last;
};

struct glp_clock* 
glp_clock_create() {
	struct glp_clock* clk = (struct glp_clock*)malloc(sizeof(*clk));
	memset(clk, 0, sizeof(*clk));
	(void) mach_timebase_info(&clk->timebase_info);
	return clk;
}

void 
glp_clock_release(struct glp_clock* clk) {
	free(clk);
}

void 
glp_clock_start(struct glp_clock* clk) {
	clk->start = mach_absolute_time();
	clk->last = clk->start;
}

uint32_t 
glp_clock_get_time(struct glp_clock* clk) {
	uint64_t tmp;
	tmp = mach_absolute_time();
	uint32_t time = (uint32_t)(((tmp - clk->start) * clk->timebase_info.numer) / (ONE_THOUSAND * clk->timebase_info.denom));
	return time;
}

uint32_t 
glp_clock_get_during(struct glp_clock* clk, bool reset) {
	uint64_t tmp;
	tmp = mach_absolute_time();
	uint32_t during = (uint32_t)(((tmp - clk->last) * clk->timebase_info.numer) / (ONE_THOUSAND * clk->timebase_info.denom));
	if (reset) {
		clk->last = tmp;
	}	
	return during;
}