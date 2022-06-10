#pragma once
#include "vox_rend.h"
#include <vector>

typedef struct ogl_svo_node{
	uint32_t mask, child;
	ogl_svo_node(uint32_t mask = 0, uint32_t child = 0);
}ogl_svo_node;

class ogl_svo : public vox_rend<ogl_svo_node> {
private:
	std::vector<ogl_svo_node> voxel_buffer;
	uint32_t edit_layer = 6;

private:

	bool inline node_is_empty(uint32_t vs_ind) const;
	uint32_t inline get_node_id_without_0(uint32_t vs_ind) const;

	typedef struct ind_opm {
		uint32_t i_vs, i_new;
		ind_opm(uint32_t i_vs, uint32_t i_new);
	}ind_opm;

	void optimize_node(std::vector<ogl_svo_node>* new_vs, ind_opm my_ind);

public:
	void init();
	void finit();

	void set_vox(int32_t x, int32_t y, int32_t z, ogl_svo_node v);
	ogl_svo_node get_vox(int32_t x, int32_t y, int32_t z);

	bool is_in_x(int32_t x);
	bool is_in_y(int32_t y);
	bool is_in_z(int32_t z);

	uint32_t get_edit_layer() const;
	void set_edit_layer(uint32_t edit_layer);

	uint32_t get_buf_size();
	const void* get_buf_data();
	void optimize_voxel_buffer();
};												
