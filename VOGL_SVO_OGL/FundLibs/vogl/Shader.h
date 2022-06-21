#pragma once
#include "glad 46/include/glad/glad.h"
#include <string>

typedef struct pipeprog {
	GLuint id;
	GLuint* progs;
	void create(GLuint type, GLuint typeBIT, const char* path, GLuint num);
	void create_from_text(GLuint type, GLuint typeBIT, std::string text, GLuint num);
	void gen(GLuint shads);
	void bind();
};