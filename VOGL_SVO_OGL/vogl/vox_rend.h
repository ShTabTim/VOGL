#pragma once
#include <cinttypes>
#include <cmath>
#define max(a, b) (((a)>(b))?(a):(b))

template <typename vox_type> 
class vox_rend {
public:
	virtual void init() = 0;
	virtual void finit() = 0;

	virtual void set_vox(int32_t x, int32_t y, int32_t z, vox_type v) = 0;
	virtual vox_type get_vox(int32_t x, int32_t y, int32_t z) = 0;

	virtual bool is_in_x(int32_t x) = 0;
	virtual bool is_in_y(int32_t y) = 0;
	virtual bool is_in_z(int32_t z) = 0;

	virtual uint32_t get_buf_size() = 0;
	virtual const void* get_buf_data() = 0;

	void draw_line(int32_t x0, int32_t y0, int32_t z0, int32_t x1, int32_t y1, int32_t z1, vox_type v) {
		float x = x1-x0, y = y1-y0, z = z1-z0;
		uint32_t t = max(max(abs(x), abs(y)), abs(z))+1;
		float fgh = 1.0f/(float)t;
		float tdx = x*fgh;
		float tdy = y*fgh;
		float tdz = z*fgh;
		x = x0; y = y0; z = z0;
		for (set_vox(x, y, z, v);t--;(x += tdx),(y += tdy),(z += tdz),set_vox(x, y, z, v));
	}
};