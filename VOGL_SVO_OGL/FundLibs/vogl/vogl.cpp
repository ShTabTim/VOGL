#include "vogl.h"

#include "glad 46/include/glad/glad.h"
#include "shaders.hpp"
#include "Shader.h"
#include <vector>

uint32_t image_buffer;
uint32_t vox_buf;
uint32_t cam_ubo;

std::vector<pipeprog> renderers;
pipeprog drawer_voxels;
struct ubo_buf {
	float mod_size;
	float mod_pos[3];
	float mod_ang[3];

	float up = 128;
	uint32_t w = 256;
	uint32_t h = 256;

	float pos[3];
	float ang[3];
} ubo_buf;

HGLRC hRC;
HDC hdc;
PIXELFORMATDESCRIPTOR pfd;
uint32_t svo_id, var_id;

//sheet
uint32_t vogl_init_vox_shad(std::string text) {
	pipeprog renderer;
	renderer.gen(1);
	renderer.create_from_text(GL_COMPUTE_SHADER, GL_COMPUTE_SHADER_BIT, text, 0);
	renderer.bind();
	renderers.push_back(renderer);
	return renderers.size() - 1;
}
void vogl_draw_voxels(uint32_t id) {
	renderers[id].bind();
	glDispatchCompute(ubo_buf.w / 8, ubo_buf.h / 8, 1);
}
void vogl_bind_vox_shad(uint32_t id) {
	renderers[id].bind();
}
void vogl_create_buffers() {
	glGenBuffers(1, &cam_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, cam_ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(ubo_buf), &ubo_buf, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, cam_ubo);

	glGenBuffers(1, &image_buffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, image_buffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 4 * ubo_buf.w * ubo_buf.h, NULL, GL_DYNAMIC_DRAW);
	glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R32UI, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}

void vogl_create_vox_buf(uint32_t location) {
	glGenBuffers(1, &vox_buf);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, location, vox_buf);
}
//sheet


//>cam_sets
void vogl_set_mod_size(float size) {
	ubo_buf.mod_size = size;
}

void vogl_set_cam_pos(float x, float y, float z) {
	ubo_buf.pos[0] = x;
	ubo_buf.pos[1] = y;
	ubo_buf.pos[2] = z;
}

void vogl_set_cam_ang(float x, float y, float z) {
	ubo_buf.ang[0] = x;
	ubo_buf.ang[1] = y;
	ubo_buf.ang[2] = z;
}

void vogl_set_cam_up(float up) { ubo_buf.up = up; }
void vogl_set_size(uint32_t w, uint32_t h) { ubo_buf.w = w; ubo_buf.h = h; }

void vogl_set_mod_pos(float x, float y, float z) {
	ubo_buf.mod_pos[0] = x;
	ubo_buf.mod_pos[1] = y;
	ubo_buf.mod_pos[2] = z;
}
void vogl_set_mod_ang(float x, float y, float z) {
	ubo_buf.mod_ang[0] = x;
	ubo_buf.mod_ang[1] = y;
	ubo_buf.mod_ang[2] = z;
}
//<cam_sets
//>cam_work
void vogl_update_cam() {
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ubo_buf), &ubo_buf);
}
//<cam_work

//>vogl_init
void vogl_start_init(HDC canvas) {
	hdc = canvas;

	ZeroMemory(&pfd, sizeof(pfd));

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;

	SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);
	hRC = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hRC);

	gladLoadGL();
}
void vogl_end_init() {
	drawer_voxels.gen(2);
	drawer_voxels.create_from_text(GL_VERTEX_SHADER, GL_VERTEX_SHADER_BIT, vert_main, 0);
	drawer_voxels.create_from_text(GL_FRAGMENT_SHADER, GL_FRAGMENT_SHADER_BIT, frag_main, 1);
	drawer_voxels.bind();

	vogl_create_vox_buf(0);
	vogl_create_buffers();
}
void vogl_set_hdc(HDC canvas) { hdc = canvas; }
//<vogl_init
//>vogl_draw
void vogl_set_vox_buf_data(uint32_t location, uint32_t size, const void* data) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vox_buf);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);
	//glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STREAM_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, location, vox_buf);
}
void vogl_end_draw() {
	glBindProgramPipeline(drawer_voxels.id);
	//glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	SwapBuffers(hdc);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, image_buffer);
	glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R32UI, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
//<vogl_draw
//>vogl_finit
void vogl_finit(HWND win) {
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(win, GetDC(win));
}
//<vogl_finit

//>use/draw voxels
//>svo
void vogl_use_svo() { svo_id = vogl_init_vox_shad(svo_comp); }
void vogl_draw_svo() { vogl_draw_voxels(svo_id); }
//<svo
//>var
void vogl_use_var() { var_id = vogl_init_vox_shad(var_comp); }
void vogl_draw_var() { vogl_draw_voxels(var_id); }
//<var
//<use/draw voxels