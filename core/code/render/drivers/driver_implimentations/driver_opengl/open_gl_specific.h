#pragma once
#ifndef _DEBUG
#define CHECK_GL_ERROR()

#else

namespace opengl {
    void glCheckError_(const char* file, int line);
}

#define CHECK_GL_ERROR() opengl::glCheckError_(__FILE__, __LINE__)
#endif