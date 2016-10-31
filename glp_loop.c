#include "glp_loop.h"
#include "glp_clock.h"

#include <stdio.h>

#ifdef _WIN32
	#include <windows.h>
	#include <GL/gl.h>
#else
	#include <unistd.h>
#endif

struct loop_state {
	struct glp_clock* clk;
	float sleep_time;

	int fps_game;
	// time per frame
	float tpf_game, tpf_real;
};

static struct loop_state S;

#define FPS_SMOOTHING 0.99f

#ifdef _WIN32
void close_vsync() {
	typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;
	const char* extensions = (char*)glGetString(GL_EXTENSIONS);
	if(strstr(extensions, "WGL_EXT_swap_control") == 0) {
		return;
	} else {
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");
		if(wglSwapIntervalEXT) {
			wglSwapIntervalEXT(0);
		}
	}
}
#else
void close_vsync() {}
#endif // _WIN32

void 
glp_loop_init(int fps) {
	close_vsync();

	S.clk = glp_clock_create();
	glp_clock_start(S.clk);

	S.sleep_time = 0;

	S.fps_game = fps;
	S.tpf_game = 1.0f / fps * 1000.0f;
	S.tpf_real = 0;
}

void 
glp_loop_update() {
	uint32_t tpf_last = glp_clock_get_during(S.clk, false);
	S.tpf_real = (S.tpf_real * FPS_SMOOTHING) + tpf_last * (1.0f - FPS_SMOOTHING);
	
	if (S.sleep_time > 0) {
		//printf("sleep %f \n", S.sleep_time);
#ifdef _WIN32
		Sleep(S.sleep_time);
#else
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