#ifndef _MODEL_LOADER_HPP_
#define _MODEL_LOADER_HPP_

#include <GL/glew.h>
#include <string>

struct renderContext {
    GLuint vertexArray;
    GLuint vertexBuffer;
    GLuint vertexIndexBuffer;
    unsigned size;
};

class ModelLoader {
    public:
        static renderContext load(std::string path);
};

#endif
