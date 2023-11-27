#include "mesh_manager.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdexcept>
#include <vector>

render_context mesh_manager::load(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error(importer.GetErrorString());
    }

    aiMesh* mesh = scene->mMeshes[0];

    render_context context;

    std::vector<float> textureCoord;
    std::vector<unsigned> indices;

    for (int i = 0; i < mesh->mNumVertices; i++) {
        if (mesh->mTextureCoords[0] != nullptr) {
            textureCoord.push_back(mesh->mTextureCoords[0][i].x);
            textureCoord.push_back(mesh->mTextureCoords[0][i].y);
        } else {
            textureCoord.push_back(0.f);
            textureCoord.push_back(0.f);
        }
    }

    for (int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    unsigned
        vertex_data_buffer_size = sizeof(float) * mesh->mNumVertices * 3,
        vertex_normal_buffer_size = sizeof(float) * mesh->mNumVertices * 3,
        vertex_tex_buffer_size = sizeof(float) * mesh->mNumVertices * 2,
        vertex_tangent_buffer_size = sizeof(float) * mesh->mNumVertices * 3,
        vertex_bitangent_buffer_size = sizeof(float) * mesh->mNumVertices * 3,
        vertex_element_buffer_size = sizeof(unsigned) * indices.size();
    
    context.size = indices.size();

    glGenVertexArrays(1, &context.vertex_array);
    glBindVertexArray(context.vertex_array);

    glGenBuffers(1, &context.vertex_index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context.vertex_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_element_buffer_size, &indices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &context.vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, context.vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, 
        vertex_data_buffer_size + 
        vertex_normal_buffer_size + 
        vertex_tex_buffer_size + 
        vertex_tangent_buffer_size + 
        vertex_bitangent_buffer_size,
        NULL, 
        GL_STATIC_DRAW
    );

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_data_buffer_size, mesh->mVertices);
    glBufferSubData(GL_ARRAY_BUFFER, vertex_data_buffer_size, vertex_normal_buffer_size, mesh->mNormals);
    glBufferSubData(GL_ARRAY_BUFFER, vertex_data_buffer_size + vertex_normal_buffer_size, vertex_tex_buffer_size, &textureCoord[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vertex_bitangent_buffer_size + vertex_normal_buffer_size + vertex_tex_buffer_size, vertex_tangent_buffer_size, mesh->mTangents);
    glBufferSubData(GL_ARRAY_BUFFER, vertex_data_buffer_size + vertex_normal_buffer_size + vertex_tex_buffer_size + vertex_tangent_buffer_size, vertex_bitangent_buffer_size, mesh->mBitangents);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertex_data_buffer_size));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(vertex_normal_buffer_size + vertex_data_buffer_size));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertex_data_buffer_size + vertex_normal_buffer_size + vertex_tex_buffer_size));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertex_data_buffer_size + vertex_normal_buffer_size + vertex_tex_buffer_size + vertex_tangent_buffer_size));

    return context;
}