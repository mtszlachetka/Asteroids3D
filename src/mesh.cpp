#include "mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdexcept>
#include <vector>
#include <iostream>
namespace se {


	mesh load_model(const std::string_view &filepath) {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filepath.data(), aiProcess_Triangulate | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			throw std::runtime_error(importer.GetErrorString());
		}

		aiMesh* aimesh = scene->mMeshes[0];

		std::vector<unsigned> indices;
		std::vector<vertex> vertices(aimesh->mNumVertices);

		for (int i = 0; i < aimesh->mNumVertices; i++) {
			vertices[i] = vertex{
				glm::vec3(aimesh->mVertices[i].x, aimesh->mVertices[i].y, aimesh->mVertices[i].z),
				glm::vec3(aimesh->mNormals[i].x, aimesh->mNormals[i].y, aimesh->mNormals[i].z),
				aimesh->mTextureCoords[0] ? glm::vec2(aimesh->mTextureCoords[0][i].x, aimesh->mTextureCoords[0][i].y) : glm::vec2(0.f),
				glm::vec3(aimesh->mTangents[i].x, aimesh->mTangents[i].y, aimesh->mTangents[i].z),
				glm::vec3(aimesh->mBitangents[i].x, aimesh->mBitangents[i].y, aimesh->mBitangents[i].z)
			};
		}

		for (int i = 0; i < aimesh->mNumFaces; i++) {
			aiFace face = aimesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		return mesh(vertices, indices);
	}
}