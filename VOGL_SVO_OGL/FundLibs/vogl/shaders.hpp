#pragma once
#include <string>

std::string vert_main = std::string("") +
"#version 450 core										  \n" +
"														  \n" +
"layout(std140, binding = 3) uniform inputs {			  \n" +
"	float mod_size;										  \n" +
"														  \n" +
"	float mod_pos_x;									  \n" +
"	float mod_pos_y;									  \n" +
"	float mod_pos_z;									  \n" +
"														  \n" +
"	float mod_ang_x;									  \n" +
"	float mod_ang_y;									  \n" +
"	float mod_ang_z;									  \n" +
"														  \n" +
"	float up;											  \n" +
"	uint w;												  \n" +
"	uint h;												  \n" +
"														  \n" +
"	float pos_x;										  \n" +
"	float pos_y;										  \n" +
"	float pos_z;										  \n" +
"														  \n" +
"	float ang_x;										  \n" +
"	float ang_y;										  \n" +
"	float ang_z;										  \n" +
"};														  \n" +
"														  \n" +
"const vec2 vertices[] = {								  \n" +
"	vec2(-2.0, -2.0),									  \n" +
"	vec2(-1.0,  4.0),									  \n" +
"	vec2(4.0, -1.0)										  \n" +
"};														  \n" +
"														  \n" +
"out vec2 FragCord;										  \n" +
"														  \n" +
"void main() {											  \n" +
"	gl_Position = vec4(vertices[gl_VertexID], 0.0, 1.0);  \n" +
"	FragCord = vec2(gl_Position.x * w, gl_Position.y * h);\n" +
"}                                                          " ;

std::string frag_main = std::string("") +
"#version 450 core																				   \n" +
"																								   \n" +
"layout(std140, binding = 3) uniform inputs {													   \n" +
"	float mod_size;																				   \n" +
"																								   \n" +
"	float mod_pos_x;																			   \n" +
"	float mod_pos_y;																			   \n" +
"	float mod_pos_z;																			   \n" +
"																								   \n" +
"	float mod_ang_x;																			   \n" +
"	float mod_ang_y;																			   \n" +
"	float mod_ang_z;																			   \n" +
"																								   \n" +
"	float up;																					   \n" +
"	uint w;																						   \n" +
"	uint h;																						   \n" +
"																								   \n" +
"	float pos_x;																				   \n" +
"	float pos_y;																				   \n" +
"	float pos_z;																				   \n" +
"																								   \n" +
"	float ang_x;																				   \n" +
"	float ang_y;																				   \n" +
"	float ang_z;																				   \n" +
"};																								   \n" +
"																								   \n" +
"layout(std430, binding = 1) buffer image_block {												   \n" +
"	vec4 image[];																				   \n" +
"};																								   \n" +
"																								   \n" +
"#define FogLevel 1																				   \n" +
"#define ZeroFog 0.1																			   \n" +
"																								   \n" +
"out vec4 FragColor;																			   \n" +
"in vec2 FragCord;																				   \n" +
"																								   \n" +
"void main() {																					   \n" +
"	vec2 scr_cord = vec2(clamp(FragCord.x + w, 0, 2 * w), clamp(FragCord.y + h, 0, 2 * h)) * 0.5;  \n" +
"	uint id = uint(scr_cord.x * h + scr_cord.y);												   \n" +
"	FragColor = image[id] * exp(-image[id].w * FogLevel - ZeroFog);								   \n" +
"	image[id] = vec4(1, 1, 1, -20);																   \n" +
"}                                                                                                   " ;

#include "shaders.inl"