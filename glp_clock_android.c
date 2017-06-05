#include "glp_clock.h"

#include <time.h>

struct glp_clock {
	struct timespec res;
	uint64_t start, last;
};

struct glp_clock* 
glp_clock_create() {
	struct glp_clock* clk = (struct glp_clock*)malloc(sizeof(*clk));
	memset(clk, 0, sizeof(*clk));
	return clk;
}

static const int64_t ONE_THOUSAND = 1000;
static const int64_t ONE_MILLION = ONE_THOUSAND * ONE_THOUSAND;

void 
glp_clock_release(struct glp_clock* clk) {
	free(clk);
}

static uint64_t
_get_absolute_time(struct glp_clock* clk) {
	clock_gettime(CLOCK_REALTIME, &clk->res);
	return ONE_MILLION * clk->res.tv_sec + (double) clk->res.tv_nsec / ONE_THOUSAND;
}

void 
glp_clock_start(struct glp_clock* clk) {
	clk->start = _get_absolute_time(clk);
	clk->last = clk->start;
}

uint32_t 
glp_clock_get_time(struct glp_clock* clk) {
	uint64_t tmp = _get_absolute_time(clk);
	return tmp - clk->start;
}

uint32_t 
glp_clock_get_during(struct glp_clock* clk, bool reset) {
	uint64_t tmp = _get_absolute_time(clk);
	uint32_t during = tmp - clk->last;
	if (reset) {
		clk->last = tmp;
	}
	return during;
}