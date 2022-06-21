#include "ogl_var.h"
#include "glad 46/include/glad/glad.h"
#include <iostream>

void ogl_var::set_whd(uint32_t w, uint32_t h, uint32_t d) {
	this->w = w;
	this->h = h;
	this->d = d;
}

void ogl_var::init() {
	var = new uint32_t[w*h*d+3];
	std::memset(var, 0, sizeof(uint32_t)*(w * h * d + 3));
	var[0] = w;
	var[1] = h;
	var[2] = d;
}

void ogl_var::finit() {
	delete[] var;
}

void ogl_var::set_vox(int32_t x, int32_t y, int32_t z, uint32_t v) {
	var[3+(h * x + y) * d + z] = v; 
}
uint32_t ogl_var::get_vox(int32_t x, int32_t y, int32_t z) {
	return var[3+(h * x + y) * d + z];
}

bool ogl_var::is_in_x(int32_t x){ return (x >= 0) && (x < w); }
bool ogl_var::is_in_y(int32_t y){ return (y >= 0) && (y < h); }
bool ogl_var::is_in_z(int32_t z){ return (z >= 0) && (z < d); }

uint32_t ogl_var::get_w() { return w; }
uint32_t ogl_var::get_h() { return h; }
uint32_t ogl_var::get_d() { return d; }

uint32_t ogl_var::get_buf_size() { return sizeof(uint32_t) * (w * h * d + 3); }
const void* ogl_var::get_buf_data() { return var; }