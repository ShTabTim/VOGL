#include "ogl_svo.h"
#include "glad 46/include/glad/glad.h"

ogl_svo_node::ogl_svo_node(uint32_t mask, uint32_t child) : mask(mask), child(child) {}

bool inline ogl_svo::node_is_empty(uint32_t vs_ind) const {
	if (!voxel_buffer[vs_ind].mask) return true;//node has id == 0
	if (!voxel_buffer[vs_ind].child) return false;//node has no children

	for (uint8_t g(8); g--;)
		if (!node_is_empty(voxel_buffer[vs_ind].child + g)) return false;//node child is not empty
	return true;
}

uint32_t inline ogl_svo::get_node_id_without_0(uint32_t vs_ind) const {
	if (!voxel_buffer[vs_ind].child) return voxel_buffer[vs_ind].mask;
	uint32_t m_ch_id = get_node_id_without_0(voxel_buffer[vs_ind].child);
	if (!m_ch_id) return 0;
	for (uint8_t g(8); (g--) > 0;)
		if (get_node_id_without_0(voxel_buffer[vs_ind].child + g) != m_ch_id) return 0;
	return m_ch_id;
}

ogl_svo::ind_opm::ind_opm(uint32_t i_vs, uint32_t i_new) : i_vs(i_vs), i_new(i_new) {}

void ogl_svo::optimize_node(std::vector<ogl_svo_node>* new_vs, ind_opm my_ind) {
	ogl_svo_node& st_vox = voxel_buffer[my_ind.i_vs];

	if (!st_vox.child) {
		(*new_vs)[my_ind.i_new] = ogl_svo_node(st_vox.mask);
		return;
	}

	if (node_is_empty(my_ind.i_vs)) {
		(*new_vs)[my_ind.i_new] = ogl_svo_node();
		return;
	}

	uint32_t m_id = get_node_id_without_0(my_ind.i_vs);
	if (m_id) {
		(*new_vs)[my_ind.i_new] = ogl_svo_node(m_id);
		return;
	}

	(*new_vs)[my_ind.i_new] = ogl_svo_node(voxel_buffer[my_ind.i_vs].mask, new_vs->size());
	for (uint8_t g(8); g--;)
		(*new_vs).emplace_back();

	for (uint8_t g(8); g--;)
		optimize_node(new_vs, ind_opm(voxel_buffer[my_ind.i_vs].child + g, (*new_vs)[my_ind.i_new].child + g));
}

void ogl_svo::optimize_voxel_buffer() {
	std::vector<ogl_svo_node> new_vb;
	new_vb.emplace_back();
	optimize_node(&new_vb, ind_opm(0, 0));
	voxel_buffer.clear();
	voxel_buffer = new_vb;
	new_vb.clear();
}

void ogl_svo::init() {
	voxel_buffer.emplace_back(0);
}

void ogl_svo::finit() {
	voxel_buffer.clear();
}

void ogl_svo::set_vox(int32_t x, int32_t y, int32_t z, ogl_svo_node v) {
	uint32_t current_size = 2 << edit_layer;
	if (x >= current_size || y >= current_size || z >= current_size) return;
	uint32_t current_index = 0;

	while (current_size > 1) {
		current_size >>= 1;
		if (current_size == 1) {
			voxel_buffer[current_index] = v;
			break;
		}

		uint8_t sub_x = x / current_size;
		uint8_t sub_y = y / current_size;
		uint8_t sub_z = z / current_size;

		x -= current_size * sub_x;
		y -= current_size * sub_y;
		z -= current_size * sub_z;

		if (!(voxel_buffer[current_index].child)) {
			voxel_buffer[current_index].child = voxel_buffer.size();

			for (uint8_t i(8); i--;)
				voxel_buffer.emplace_back(ogl_svo_node(voxel_buffer[current_index].mask));

			if (!(voxel_buffer[current_index].mask && v.mask)) voxel_buffer[current_index].mask = v.mask;
		}

		current_index = voxel_buffer[current_index].child + 4 * sub_x + 2 * sub_y + sub_z;
	}
}

ogl_svo_node ogl_svo::get_vox(int32_t x, int32_t y, int32_t z) {
	uint32_t current_size = 1 << edit_layer;
	uint32_t current_index = 0;
	if (x > current_size || y > current_size || z > current_size)
		return 0;

	while (current_size > 0) {
		current_size >>= 1;

		if (current_size == 1 || !(voxel_buffer[current_index].child))
			return voxel_buffer[current_index];

		uint8_t sub_x = x / current_size;
		uint8_t sub_y = y / current_size;
		uint8_t sub_z = z / current_size;

		x -= current_size * sub_x;
		y -= current_size * sub_y;
		z -= current_size * sub_z;

		current_index = voxel_buffer[current_index].child + 4 * sub_x + 2 * sub_y + sub_z;
	}
}

bool ogl_svo::is_in_x(int32_t x) { return (x >= 0) && (x < (2 << edit_layer)); }
bool ogl_svo::is_in_y(int32_t y) { return (y >= 0) && (y < (2 << edit_layer)); }
bool ogl_svo::is_in_z(int32_t z) { return (z >= 0) && (z < (2 << edit_layer)); }

uint32_t ogl_svo::get_edit_layer() const { return edit_layer; }
void ogl_svo::set_edit_layer(uint32_t edit_layer) { this->edit_layer = edit_layer; }

uint32_t ogl_svo::get_buf_size() { return sizeof(ogl_svo_node) * voxel_buffer.size(); }
const void* ogl_svo::get_buf_data() { return voxel_buffer.data(); }