#pragma once

std::string var_comp = std::string("")+
"#version 450 core																									  \n"+
"																													  \n"+
"layout(std140, binding = 3) uniform inputs {																		  \n"+
"    float mod_size;																								  \n"+
"																													  \n"+
"    float mod_pos_x;																								  \n"+
"    float mod_pos_y;																								  \n"+
"    float mod_pos_z;																								  \n"+
"																													  \n"+
"    float mod_ang_x;																								  \n"+
"    float mod_ang_y;																								  \n"+
"    float mod_ang_z;																								  \n"+
"																													  \n"+
"    float up;																										  \n"+
"    uint w;																										  \n"+
"    uint h;																										  \n"+
"																													  \n"+
"    float pos_x;																									  \n"+
"    float pos_y;																									  \n"+
"    float pos_z;																									  \n"+
"																													  \n"+
"    float ang_x;																									  \n"+
"    float ang_y;																									  \n"+
"    float ang_z;																									  \n"+
"};																													  \n"+
"																													  \n"+
"layout(std430, binding = 0) readonly buffer voxels_block {															  \n"+
"    uint voxels[];																									  \n"+
"};																													  \n"+
"																													  \n"+
"layout(std430, binding = 1) buffer image_block {																	  \n"+
"    vec4 image[];																									  \n"+
"};																													  \n"+
"																													  \n"+
"layout(local_size_x = 8, local_size_y = 8) in;																		  \n"+
"																													  \n"+
"vec2 cube_intersect(vec3 neg_ro, vec3 inv_rd, vec3 size) {															  \n"+
"    vec3 t0 = inv_rd * neg_ro;																						  \n"+
"    vec3 t1 = t0 + inv_rd * size;																					  \n"+
"    vec3 t2 = min(t0, t1);																							  \n"+
"    float f1 = max(max(t2.x, t2.y), t2.z);																			  \n"+
"    t2 = max(t0, t1);																								  \n"+
"    float f2 = min(min(t2.x, t2.y), t2.z);																			  \n"+
"    if (f1 > f2 || f2 <= 0) return vec2(0);																		  \n"+
"    return vec2(f1, f2);																							  \n"+
"}																													  \n"+
"																													  \n"+
"vec3 getColor(uint id) {																							  \n"+
"    return vec3(id & 0xFF, (id >> 8) & 0xFF, (id >> 16) & 0xFF) / 0xFF;											  \n"+
"}																													  \n"+
"																													  \n"+
"struct Hit {																										  \n"+
"    vec3 n;																										  \n"+
"    uint id;																										  \n"+
"    float l;																										  \n"+
"};																													  \n"+
"																													  \n"+
"																													  \n"+
"bool castRay(vec3 ro, vec3 rd, inout Hit hit) {																	  \n" +
"    vec3 inv_rd = 1 / rd;																							  \n" +
"    if (rd.x == 0) inv_rd.x = 0xFFFFFFF;																			  \n" +
"    if (rd.y == 0) inv_rd.y = 0xFFFFFFF;																			  \n" +
"    if (rd.z == 0) inv_rd.z = 0xFFFFFFF;																			  \n" +
"																													  \n" +
"    vec2 b = cube_intersect(-ro, inv_rd, mod_size * vec3(voxels[0], voxels[1], voxels[2]));						  \n" +
"																													  \n" +
"    if (b.y < 0.001f)																								  \n" +
"        return false;																								  \n" +
"																													  \n" +
"    vec3 n_ro = ro + rd * max(b.x + 0.001f, 0);																	  \n" +
"    n_ro /= mod_size;																								  \n" +
"																													  \n" +
"    vec3 vox_pos = floor(n_ro);																					  \n" +
"																													  \n" +
"    vec3 step = sign(inv_rd);																						  \n" +
"    vec3 tDelta = step * inv_rd;																					  \n" +
"																													  \n" +
"    vec3 fr = fract(n_ro);																							  \n" +
"																													  \n" +
"    vec3 tmax = tDelta;																							  \n" +
"																													  \n" +
"    tmax.x *= (inv_rd.x > 0) ? (1 - fr.x) : fr.x;																	  \n" +
"    tmax.y *= (inv_rd.y > 0) ? (1 - fr.y) : fr.y;																	  \n" +
"    tmax.z *= (inv_rd.z > 0) ? (1 - fr.z) : fr.z;																	  \n" +
"																													  \n" +
"    while (true) {																									  \n" +
"        if (vox_pos.x < 0 || vox_pos.x >= voxels[0] ||																  \n" +
"            vox_pos.y < 0 || vox_pos.y >= voxels[1] ||																  \n" +
"            vox_pos.z < 0 || vox_pos.z >= voxels[2])																  \n" +
"            return false;																							  \n" +
"																													  \n" +
"        hit.id = voxels[3 + (uint(vox_pos.x) * voxels[1] + uint(vox_pos.y)) * voxels[2] + uint(vox_pos.z)];		  \n" +
"        if (hit.id != 0) {																							  \n" +
"            hit.l = length(vox_pos * mod_size - ro);//самаркан														  \n" +
"            return true;																							  \n" +
"        }																											  \n" +
"																													  \n" +
"        if (tmax.x < tmax.y) {																						  \n" +
"            if (tmax.x < tmax.z) {																					  \n" +
"                tmax.x += tDelta.x;																				  \n" +
"                vox_pos.x += step.x;																				  \n" +
"                hit.n = vec3(-step.x, 0, 0);																		  \n" +
"            }																										  \n" +
"            else {																									  \n" +
"                tmax.z += tDelta.z;																				  \n" +
"                vox_pos.z += step.z;																				  \n" +
"                hit.n = vec3(0, 0, -step.z);																		  \n" +
"            }																										  \n" +
"        }																											  \n" +
"        else {																										  \n" +
"            if (tmax.y < tmax.z) {																					  \n" +
"                tmax.y += tDelta.y;																				  \n" +
"                vox_pos.y += step.y;																				  \n" +
"                hit.n = vec3(0, -step.y, 0);																		  \n" +
"            }																										  \n" +
"            else {																									  \n" +
"                tmax.z += tDelta.z;																				  \n" +
"                vox_pos.z += step.z;																				  \n" +
"                hit.n = vec3(0, 0, -step.z);																		  \n" +
"            }																										  \n" +
"        }																											  \n" +
"    }																												  \n" +
"																													  \n" +
"    return false;																									  \n" +
"}																													  \n" +
"																													  \n" +
"vec2 rot(vec2 o, float s, float c) {																				  \n" +
"    return vec2(o.y * s + o.x * c, o.y * c - o.x * s);																  \n" +
"}																													  \n" +
"																													  \n" +
"void main() {																										  \n" +
"    vec2 scr_cord = vec2(gl_GlobalInvocationID.x, gl_GlobalInvocationID.y);										  \n" +
"																													  \n" +
"    vec3 rd = normalize(vec3((2 * scr_cord.x - w), (2 * scr_cord.y - h), up));										  \n" +
"																													  \n" +
"    rd.xy = rot(rd.xy, sin(ang_z), cos(ang_z));																	  \n" +
"    rd.yz = rot(rd.yz, sin(ang_y), cos(ang_y));																	  \n" +
"    rd.xz = rot(rd.xz, sin(ang_x), cos(ang_x));																	  \n" +
"																													  \n" +
"    rd.xy = rot(rd.xy, sin(mod_ang_z), cos(mod_ang_z));															  \n" +
"    rd.yz = rot(rd.yz, sin(mod_ang_y), cos(mod_ang_y));															  \n" +
"    rd.xz = rot(rd.xz, sin(mod_ang_x), cos(mod_ang_x));															  \n" +
"																													  \n" +
"    vec3 ro = vec3(pos_x - mod_pos_x, pos_y - mod_pos_y, pos_z - mod_pos_z);										  \n" +
"																													  \n" +
"    ro.xy = rot(ro.xy, sin(mod_ang_z), cos(mod_ang_z));															  \n" +
"    ro.yz = rot(ro.yz, sin(mod_ang_y), cos(mod_ang_y));															  \n" +
"    ro.xz = rot(ro.xz, sin(mod_ang_x), cos(mod_ang_x));															  \n" +
"																													  \n" +
"    Hit hit;																										  \n" +
"																													  \n" +
"    if (!castRay(ro, rd, hit)) return;																				  \n" +
"																													  \n" +
"    uint id = uint((scr_cord.x + 0.5f) * h + scr_cord.y);															  \n" +
"																													  \n" +
"    if (image[id].a > 0 && image[id].a < hit.l) return;															  \n" +
"																													  \n" +
"    image[id] = vec4(getColor(hit.id), hit.l);																		  \n" +
"}                                                                                                                    \n";

std::string svo_comp = std::string("") +
"#version 450 core																																					 \n"+
"																																									 \n"+
"layout(std140, binding = 3) uniform inputs {																														 \n"+
"	float mod_size;																																					 \n"+
"																																									 \n"+
"	float mod_pos_x;																																				 \n"+
"	float mod_pos_y;																																				 \n"+
"	float mod_pos_z;																																				 \n"+
"																																									 \n"+
"	float mod_ang_x;																																				 \n"+
"	float mod_ang_y;																																				 \n"+
"	float mod_ang_z;																																				 \n"+
"																																									 \n"+
"	float up;																																						 \n"+
"	uint w;																																							 \n"+
"	uint h;																																							 \n"+
"																																									 \n"+
"	float pos_x;																																					 \n"+
"	float pos_y;																																					 \n"+
"	float pos_z;																																					 \n"+
"																																									 \n"+
"	float ang_x;																																					 \n"+
"	float ang_y;																																					 \n"+
"	float ang_z;																																					 \n"+
"};																																									 \n"+
"																																									 \n"+
"struct vox { uint id, child; };																																	 \n"+
"																																									 \n"+
"layout(std430, binding = 0) readonly buffer voxels_block {																											 \n"+
"	vox voxels[];																																					 \n"+
"};																																									 \n"+
"																																									 \n"+
"layout(std430, binding = 1) buffer image_block {																													 \n"+
"	vec4 image[];																																					 \n"+
"};																																									 \n"+
"																																									 \n"+
"layout(local_size_x = 8, local_size_y = 8) in;																														 \n"+
"																																									 \n"+
"#define MaxLayer 12																																				 \n"+
"#define LodLevel 4																																					 \n"+
"#define LodModer 1.47																																				 \n"+
"																																									 \n"+
"vec2 cube_intersect(vec3 neg_ro, vec3 inv_rd, float size) {																										 \n"+
"	vec3 t0 = inv_rd * neg_ro;																																		 \n"+
"	vec3 t1 = t0 + inv_rd * size;																																	 \n"+
"	vec3 t2 = min(t0, t1);																																			 \n"+
"	float f1 = max(max(t2.x, t2.y), t2.z);																															 \n"+
"	t2 = max(t0, t1);																																				 \n"+
"	float f2 = min(min(t2.x, t2.y), t2.z);																															 \n"+
"	if (f1 > f2 || f2 <= 0) return vec2(0);																															 \n"+
"	return vec2(f1, f2);																																			 \n"+
"}																																									 \n"+
"																																									 \n"+
"vec3 getColor(uint id) {																																			 \n"+
"	return vec3(id & 0xFF, (id >> 8) & 0xFF, (id >> 16) & 0xFF) / 0xFF;																								 \n"+
"}																																									 \n"+
"																																									 \n"+
"struct Hit {																																						 \n"+
"	vec3 n;																																							 \n"+
"	uint id;																																						 \n"+
"	float l;																																						 \n"+
"};																																									 \n"+
"																																									 \n"+
"struct OctreeContext {																																				 \n"+
"	uint index;																																						 \n"+
"	ivec3 sub_coords;																																				 \n"+
"	vec3 ro;																																						 \n"+
"	vec3 pt;																																						 \n"+
"	vec3 t_max;																																						 \n"+
"	float t_max_min;																																				 \n"+
"};																																									 \n"+
"void advance(inout OctreeContext context, in ivec3 step, vec3 tDelta, inout Hit hit) {																				 \n"+
"	if (context.t_max.x < context.t_max.y) {																														 \n"+
"		if (context.t_max.z < context.t_max.x) {																													 \n"+
"			context.t_max_min = context.t_max.z;																													 \n"+
"			context.t_max.z += tDelta.z;//tMaxZ += tDelta.z																											 \n"+
"			context.sub_coords.z += step.z;//vpos.z += step.z;																										 \n"+
"			hit.n = vec3(0, 0, -step.z);																															 \n"+
"		}																																							 \n"+
"		else {																																						 \n"+
"			context.t_max_min = context.t_max.x;																													 \n"+
"			context.t_max.x += tDelta.x;//tMaxX += tDelta.x;																										 \n"+
"			context.sub_coords.x += step.x;//vpos.x += step.x;																										 \n"+
"			hit.n = vec3(-step.x, 0, 0);																															 \n"+
"		}																																							 \n"+
"	}																																								 \n"+
"	else {																																							 \n"+
"		if (context.t_max.z < context.t_max.y) {																													 \n"+
"			context.t_max_min = context.t_max.z;																													 \n"+
"			context.t_max.z += tDelta.z;//tMaxZ += tDelta.z;																										 \n"+
"			context.sub_coords.z += step.z;//vpos.z += step.z;																										 \n"+
"			hit.n = vec3(0, 0, -step.z);																															 \n"+
"		}																																							 \n"+
"		else {																																						 \n"+
"			context.t_max_min = context.t_max.y;																													 \n"+
"			context.t_max.y += tDelta.y;//tMaxY += tDelta.y;																										 \n"+
"			context.sub_coords.y += step.y;//vpos.y += step.y;																										 \n"+
"			hit.n = vec3(0, -step.y, 0);																															 \n"+
"		}																																							 \n"+
"	}																																								 \n"+
"}																																									 \n"+
"																																									 \n"+
"bool castRay(const vec3 ro, const vec3 rd, out Hit hit) {																											 \n"+
"	vec3 inv_rd = 1 / rd;																																			 \n"+
"	if (rd.x == 0) inv_rd.x = 0xFFFFFFF;																															 \n"+
"	if (rd.y == 0) inv_rd.y = 0xFFFFFFF;																															 \n"+
"	if (rd.z == 0) inv_rd.z = 0xFFFFFFF;																															 \n"+
"																																									 \n"+
"	vec2 b = cube_intersect(-ro, inv_rd, mod_size);																													 \n"+
"	if (b.y < 0.001f)																																				 \n"+
"		return false;																																				 \n"+
"	float current_size = mod_size * 0.5f;																															 \n"+
"	ivec3 step_vec = ivec3(sign(rd));																																 \n"+
"	ivec3 dir = max(step_vec, 0);																																	 \n"+
"	vec3 t_delta = abs(current_size * inv_rd);																														 \n"+
"	bool skip = false;																																				 \n"+
"	uint current_stack_index = 0;																																	 \n"+
"																																									 \n"+
"	OctreeContext stack[MaxLayer];																																	 \n"+
"																																									 \n"+
"	stack[0].index = 0;																																				 \n" +
"	stack[0].ro = ro + rd * max(b.x, 0);																															 \n" +
"	stack[0].pt = -ro;																																				 \n" +
"																																									 \n" +
"	stack[0].sub_coords = min(ivec3(stack[0].ro / current_size), 1);																								 \n" +
"	stack[0].t_max_min = 0;																																			 \n" +
"	stack[0].t_max = ((dir + stack[0].sub_coords) * current_size - stack[0].ro) * inv_rd;																			 \n" +
"																																									 \n" +
"	uint sub_index;																																					 \n" +
"																																									 \n" +
"	while (current_size > 0) {																																		 \n" +
"		OctreeContext context = stack[current_stack_index];																											 \n" +
"																																									 \n" +
"		sub_index = 4 * context.sub_coords.x + 2 * context.sub_coords.y + context.sub_coords.z;																		 \n" +
"																																									 \n" +
"		if (voxels[voxels[context.index].child + sub_index].id == 0 || skip) { // is_empty or skip																	 \n" +
"			advance(stack[current_stack_index], step_vec, t_delta, hit);																							 \n" +
"																																									 \n" +
"			skip = false;																																			 \n" +
"																																									 \n" +
"			if (clamp(stack[current_stack_index].sub_coords, 0, 1) != stack[current_stack_index].sub_coords) {														 \n" +
"																																									 \n" +
"				if (current_stack_index == 0) return false;																											 \n" +
"																																									 \n" +
"				current_stack_index--;																																 \n" +
"				current_size *= 2;																																	 \n" +
"				t_delta *= 2;																																		 \n" +
"																																									 \n" +
"				skip = true;																																		 \n" +
"			}																																						 \n" +
"																																									 \n" +
"			continue;																																				 \n" +
"		}																																							 \n" +
"#ifdef USE_LOD																																						 \n" +
"		hit.l = context.pt.x * context.pt.x + context.pt.y * context.pt.y + context.pt.z * context.pt.z;															 \n" +
"#endif																																								 \n" +
"		if (voxels[voxels[context.index].child + sub_index].child == 0																								 \n" +
"#ifdef USE_LOD																																						 \n" +
"			|| current_size * (2 << LodLevel) <= hit.l * LodModer																									 \n" +
"#endif																																								 \n" +
"			) { // is_leaf																																			 \n" +
"			hit.id = voxels[voxels[context.index].child + sub_index].id;																							 \n" +
"#ifndef USE_LOD																																					 \n" +
"			hit.l = context.pt.x * context.pt.x + context.pt.y * context.pt.y + context.pt.z * context.pt.z;														 \n" +
"#endif																																								 \n" +
"			//hit.l = cube_intersect(context.pt, inv_rd, current_size * 2).x;																						 \n" +
"			hit.l = sqrt(hit.l);																																	 \n" +
"			return true;																																			 \n" +
"		}																																							 \n" +
"																																									 \n" +
"		if (current_stack_index > MaxLayer) return false;																											 \n" +
"																																									 \n" +
"		current_stack_index++;																																		 \n" +
"																																									 \n" +
"		stack[current_stack_index].index = voxels[context.index].child + sub_index;																					 \n" +
"		stack[current_stack_index].ro = context.ro + context.t_max_min * rd - vec3(context.sub_coords) * current_size;												 \n" +
"		stack[current_stack_index].pt = context.pt + context.sub_coords * current_size;																				 \n" +
"																																									 \n" +
"		current_size *= 0.5f;																																		 \n" +
"		t_delta *= 0.5f;																																			 \n" +
"																																									 \n" +
"		stack[current_stack_index].sub_coords = min(ivec3(stack[current_stack_index].ro / current_size), 1);														 \n" +
"		stack[current_stack_index].t_max_min = 0;																													 \n" +
"		stack[current_stack_index].t_max = ((dir + stack[current_stack_index].sub_coords) * current_size - stack[current_stack_index].ro) * inv_rd;					 \n" +
"	}																																								 \n" +
"	return false;																																					 \n" +
"}																																									 \n" +
"																																									 \n" +
"vec2 rot(vec2 o, float s, float c) {																																 \n" +
"	return vec2(o.y * s + o.x * c, o.y * c - o.x * s);																												 \n" +
"}																																									 \n" +
"																																									 \n" +
"void main() {																																						 \n" +
"	vec2 scr_cord = vec2(gl_GlobalInvocationID.x, gl_GlobalInvocationID.y);																							 \n" +
"																																									 \n" +
"	vec3 rd = normalize(vec3((2 * scr_cord.x - w), (2 * scr_cord.y - h), up));																						 \n" +
"																																									 \n" +
"	rd.xy = rot(rd.xy, sin(ang_z), cos(ang_z));																														 \n" +
"	rd.yz = rot(rd.yz, sin(ang_y), cos(ang_y));																														 \n" +
"	rd.xz = rot(rd.xz, sin(ang_x), cos(ang_x));																														 \n" +
"																																									 \n" +
"	rd.xy = rot(rd.xy, sin(mod_ang_z), cos(mod_ang_z));																												 \n" +
"	rd.yz = rot(rd.yz, sin(mod_ang_y), cos(mod_ang_y));																												 \n" +
"	rd.xz = rot(rd.xz, sin(mod_ang_x), cos(mod_ang_x));																												 \n" +
"																																									 \n" +
"	vec3 ro = vec3(pos_x - mod_pos_x, pos_y - mod_pos_y, pos_z - mod_pos_z);																						 \n" +
"																																									 \n" +
"	ro.xy = rot(ro.xy, sin(mod_ang_z), cos(mod_ang_z));																												 \n" +
"	ro.yz = rot(ro.yz, sin(mod_ang_y), cos(mod_ang_y));																												 \n" +
"	ro.xz = rot(ro.xz, sin(mod_ang_x), cos(mod_ang_x));																												 \n" +
"																																									 \n" +
"	Hit hit;																																						 \n" +
"																																									 \n" +
"	if (!castRay(ro, rd, hit)) return;																																 \n" +
"																																									 \n" +
"	uint id = uint((scr_cord.x + 0.5f) * h + scr_cord.y);																											 \n" +
"																																									 \n" +
"	if (image[id].a > 0 && image[id].a < hit.l) return;																												 \n" +
"																																									 \n" +
"	image[id] = vec4(getColor(hit.id), hit.l);																														 \n" +
"}                                                                                                                                                                   \n";