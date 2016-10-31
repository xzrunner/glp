#ifdef __cplusplus
extern "C"
{
#endif

#ifndef game_loop_loop_h
#define game_loop_loop_h

void glp_loop_init(int fps);
void glp_loop_update();

int glp_loop_get_fps();
void glp_print_fps();

#endif // game_loop_loop_h