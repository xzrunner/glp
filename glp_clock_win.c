#include "glp_clock.h"

#include <windows.h>

struct glp_clock {
	LARGE_INTEGER freq;
	LARGE_INTEGER start, last;
};

static const int64_t ONE_THOUSAND = 1000;
static const int64_t ONE_MILLION = 1000 * 1000;

struct glp_clock* 
glp_clock_create() {
	struct glp_clock* clk = (struct glp_clock*)malloc(sizeof(*clk));
	memset(clk, 0, sizeof(*clk));
	QueryPerformanceFrequency(&clk->freq);
	return clk;
}

void 
glp_clock_release(struct glp_clock* clk) {
	free(clk);
}

void 
glp_clock_start(struct glp_clock* clk) {
	QueryPerformanceCounter(&clk->start);
	clk->last = clk->start;
}

uint32_t 
glp_clock_get_time(struct glp_clock* clk) {
	LARGE_INTEGER tmp;
	QueryPerformanceCounter(&tmp);
	uint32_t time = (uint32_t)((tmp.QuadPart - clk->start.QuadPart) * ONE_MILLION / clk->freq.QuadPart);
	return time;
}

uint32_t 
glp_clock_get_during(struct glp_clock* clk, bool reset) {
	LARGE_INTEGER tmp;
	QueryPerformanceCounter(&tmp);
	uint32_t during = (uint32_t)((tmp.QuadPart - clk->last.QuadPart) * ONE_MILLION / clk->freq.QuadPart);
	if (reset) {
		clk->last = tmp;
	}
	return during;
}