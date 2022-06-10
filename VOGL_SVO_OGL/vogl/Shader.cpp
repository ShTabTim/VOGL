#include "Shader.h"
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <iostream>

std::string v_rdFile(const char* path) {
    std::ifstream fil;
    fil.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        std::stringstream fStream;
        fil.open(path);
        fStream << fil.rdbuf();
        fil.close();
        return fStream.str();
    } catch (std::ifstream::failure e) {
        MessageBox(nullptr, L"ERROR::FILE::FILE_NOT_SUCCESFULLY_READ", L"ass", MB_ICONERROR);
    }
    return 0;
}

//error//
void errorGLshader(const wchar_t* name, GLuint id) {
    char* error_log = new char[4096];
    glGetProgramInfoLog(id, 4096, NULL, error_log);
    if (error_log[0] != 0) {
        LPWSTR ww = new wchar_t[4096];
        MultiByteToWideChar(0, 0, error_log, 4096, ww, 4096);
        MessageBox(nullptr, ww, name, MB_OK);
    }
}
//error//

//pipeprog//
void pipeprog::create(GLuint type, GLuint typeBIT, const char* path, GLuint num) {
    std::string code = v_rdFile(path);
    const GLchar* t[] = { code.c_str() };
    progs[num] = glCreateShaderProgramv(type, sizeof(t) / sizeof(void*), t);
    errorGLshader(L"shader", progs[num]);
    glUseProgramStages(id, typeBIT, progs[num]);
}
void pipeprog::create_from_text(GLuint type, GLuint typeBIT, std::string text, GLuint num) {
    const char* t[] = { text.c_str() };
    progs[num] = glCreateShaderProgramv(type, sizeof(t) / sizeof(void*), t);
    errorGLshader(L"shader", progs[num]);
    glUseProgramStages(id, typeBIT, progs[num]);
}
void pipeprog::gen(GLuint shads) { glGenProgramPipelines(1, &id); progs = new GLuint[shads]; }
void pipeprog::bind() { glBindProgramPipeline(id); }
//pipeprog//