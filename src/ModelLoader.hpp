#ifndef _MODEL_LOADER_HPP_
#define _MODEL_LOADER_HPP_

#include <GL/glew.h>
#include <string>

struct RenderContext {
    GLuint vertexArray;
    GLuint vertexBuffer;
    GLuint vertexIndexBuffer;
    unsigned size;
};

class ModelLoader {
    public:
        RenderContext load(std::string path);
};

static ModelLoader gModelLoader;

#endif
