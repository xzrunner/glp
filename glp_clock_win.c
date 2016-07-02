#include "glp_clock.h"

#include <Windows.h>

struct glp_clock {
	LARGE_INTEGER freq;
	LARGE_INTEGER start, last;
	LARGE_INTEGER tmp;
};

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

double 
glp_clock_get_time(struct glp_clock* clk) {
	QueryPerformanceCounter(&clk->tmp);
	double time = (double)((clk->tmp.QuadPart - clk->start.QuadPart) * 1000 / clk->freq.QuadPart);
	return time;
}

double 
glp_clock_get_during(struct glp_clock* clk, bool reset) {
	QueryPerformanceCounter(&clk->tmp);
	double during = (double)((clk->tmp.QuadPart - clk->last.QuadPart) * 1000 / clk->freq.QuadPart);
	if (reset) {
		clk->last = clk->tmp;
	}
	return during;
}