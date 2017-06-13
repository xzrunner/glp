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

#ifdef _WIN32
void close_vsync() {
	typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;
	const char* extensions = (char*)glGetString(GL_EXTENSIONS);
	if (strstr(extensions, "WGL_EXT_swap_control") == 0) {
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

static uint32_t
_fix_during(uint32_t during) {
	uint32_t milliseconds = (uint32_t)(during / 1000);
	return milliseconds > 1000 ? 30 : milliseconds;
}

void 
glp_loop_update() {
	if (!S.clk) {
		return;
	}

	S.tpf_real = _fix_during(glp_clock_get_during(S.clk, false));
	
	if (S.sleep_time > 0) {
#ifdef _WIN32
		Sleep(S.sleep_time);
#else
		usleep(1000 * S.sleep_time);
#endif
	}

	uint32_t cost = _fix_during(glp_clock_get_during(S.clk, true));
	S.sleep_time += (S.tpf_game - cost);
}

float 
glp_get_dt() {
	return S.tpf_real;
}

uint32_t 
glp_get_time() {
	return S.clk ? glp_clock_get_time(S.clk) : 0;
}