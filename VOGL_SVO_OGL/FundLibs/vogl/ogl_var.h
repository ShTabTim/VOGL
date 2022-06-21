#pragma once
#include "vox_rend.h"

class ogl_var : public vox_rend<uint32_t> {
private:
	uint32_t ogl_voxel_buffer;
	uint32_t w, h, d;
	uint32_t *var;
public:
	void set_whd(uint32_t w, uint32_t h, uint32_t d);
	void init();
	void finit();

	void set_vox(int32_t x, int32_t y, int32_t z, uint32_t v);
	uint32_t get_vox(int32_t x, int32_t y, int32_t z);

	bool is_in_x(int32_t x);
	bool is_in_y(int32_t y);
	bool is_in_z(int32_t z);

	uint32_t get_w();
	uint32_t get_h();
	uint32_t get_d();

	uint32_t get_buf_size();
	const void* get_buf_data();
};
