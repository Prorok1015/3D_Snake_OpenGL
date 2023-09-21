#pragma once
#ifndef _DEBUG
#define CHECK_GL_ERROR()

#else

namespace opengl {
    void glCheckError_(const char* file, int line);
}
namespace gl = opengl;

#define CHECK_GL_ERROR() glCheckError_(__FILE__, __LINE__)
#endif