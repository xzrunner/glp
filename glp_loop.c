#include "glp_loop.h"
#include "glp_clock.h"

#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#elif defined(__MACOSX)
#include <unistd.h>
#endif // _WIN32

struct loop_state {
	struct glp_clock* clk;
	float sleep_time;

	int fps_game;
	// time per frame
	float tpf_game, tpf_real;
};

static struct loop_state S;

static void (*UPDATE_FUNC)();
static void* UPDATE_FUNC_UD;

#define FPS_SMOOTHING 0.9f

void 
glp_loop_init(int fps, void (*update)(void*), void* ud) {
	UPDATE_FUNC = update;
	UPDATE_FUNC_UD = ud;

	S.clk = glp_clock_create();
	glp_clock_start(S.clk);

	S.sleep_time = 0;

	S.fps_game = fps;
	S.tpf_game = 1.0f / fps * 1000.0f;
	S.tpf_real = 0;
}

void 
glp_loop_update() {
	UPDATE_FUNC(UPDATE_FUNC_UD);

	uint32_t tpf_last = glp_clock_get_during(S.clk, false);
	S.tpf_real = (S.tpf_real * FPS_SMOOTHING) + tpf_last * (1.0f - FPS_SMOOTHING);
	
	if (S.sleep_time > 0) {
//		printf("sleep %f \n", S.sleep_time);
#ifdef _WIN32
		Sleep(S.sleep_time);
#elif defined(__MACOSX)
		usleep(1000 * S.sleep_time);
#endif
	}

	uint32_t cost = glp_clock_get_during(S.clk, true);
	S.sleep_time += (S.tpf_game - cost);
}

int 
glp_loop_get_fps() {
	return (int)(1000.0f / S.tpf_real + 0.5f);
}

void 
glp_print_fps() {
	static int count = 0;
	if (++count == S.fps_game) {
		printf("\r[fps %d]    ", glp_loop_get_fps());
		count = 0;
	}
}