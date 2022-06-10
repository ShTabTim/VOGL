#pragma once
#pragma comment(lib, "opengl32.lib")
#include <Windows.h>
#include <cinttypes>

//>cam_sets
void vogl_set_mod_size(float size);

void vogl_set_cam_pos(float x, float y, float z);
void vogl_set_cam_ang(float x, float y, float z);

void vogl_set_cam_up(float up);
void vogl_set_size(uint32_t w, uint32_t h);

void vogl_set_mod_pos(float x, float y, float z);
void vogl_set_mod_ang(float x, float y, float z);
//<cam_sets
//>cam_work
void vogl_update_cam();
//<cam_work

//>vogl_init
void vogl_start_init(HDC canvas);
void vogl_end_init();
void vogl_set_hdc(HDC canvas);//for reset hdc
//<vogl_init
//>vogl_draw
void vogl_set_vox_buf_data(uint32_t location, uint32_t size, const void* data);
void vogl_end_draw();
//<vogl_draw
//>vogl_finit
void vogl_finit(HWND win);
//<vogl_finit

//>use/draw voxels
//>svo
void vogl_use_svo();
void vogl_draw_svo();
//<svo
//>var
void vogl_use_var();
void vogl_draw_var();
//<var
//<use/draw voxels