#include "mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdexcept>
#include <vector>
namespace se {

mesh load_model(const std::string_view &filepath) {
	Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath.data(), aiProcess_Triangulate | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error(importer.GetErrorString());
    }

    aiMesh* aimesh = scene->mMeshes[0];

    se::mesh context;

    std::vector<float> tex_coord;
    std::vector<unsigned> indices;

    for (int i = 0; i < aimesh->mNumVertices; i++) {
        if (aimesh->mTextureCoords[0] != nullptr) {
            tex_coord.push_back(aimesh->mTextureCoords[0][i].x);
            tex_coord.push_back(aimesh->mTextureCoords[0][i].y);
        } else {
            tex_coord.push_back(0.f);
            tex_coord.push_back(0.f);
        }
    }

    for (int i = 0; i < aimesh->mNumFaces; i++) {
        aiFace face = aimesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    unsigned
        data_size = sizeof(float) * aimesh->mNumVertices * 3,
        normal_size = sizeof(float) * aimesh->mNumVertices * 3,
        tex_size = sizeof(float) * aimesh->mNumVertices * 2,
        tangent_size = sizeof(float) * aimesh->mNumVertices * 3,
        bitangent_size = sizeof(float) * aimesh->mNumVertices * 3,
        element_size = sizeof(unsigned) * indices.size();
    
    context.m_size = indices.size();

    glGenVertexArrays(1, &context.m_vao);
    glBindVertexArray(context.m_vao);

    glGenBuffers(1, &context.m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context.m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_size, indices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &context.m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, context.m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 
        data_size + 
        normal_size + 
        tex_size + 
        tangent_size + 
        bitangent_size,
        NULL, 
        GL_STATIC_DRAW
    );

    glBufferSubData(GL_ARRAY_BUFFER, 0, data_size, aimesh->mVertices);
    glBufferSubData(GL_ARRAY_BUFFER, data_size, normal_size, aimesh->mNormals);
    glBufferSubData(GL_ARRAY_BUFFER, data_size + normal_size, tex_size, tex_coord.data());
    glBufferSubData(GL_ARRAY_BUFFER, data_size + normal_size + tex_size, tangent_size, aimesh->mTangents);
    glBufferSubData(GL_ARRAY_BUFFER, data_size + normal_size + tex_size + tangent_size, bitangent_size, aimesh->mBitangents);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(data_size));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(data_size + normal_size));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)(data_size + normal_size + tex_size));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)(data_size + normal_size + tex_size + tangent_size));

    return context;
}
}