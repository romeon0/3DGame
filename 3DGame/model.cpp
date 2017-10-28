//#pragma comment(lib, "glew32s.lib")


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include "model.h"
using namespace Assimp;
using namespace glm;



unsigned int TextureFromFile(const char *path, const string &directory, bool gamma);

	/*  Functions   */
	// constructor, expects a filepath to a 3D model.
	Model::Model() {}
	Model::Model(string const &path, bool gamma) : gammaCorrection(gamma)
	{
		loadModel(path);
	}
	Model::~Model() {
		//textures_loaded.clear();
		//meshes.clear();
	}

	void Model::extractData(string const &path, bool gamma) {
		gammaCorrection = gamma;
		loadModel(path);
	}

	vector<Vertex> Model::getVertices() {
		vector<Vertex> vertices;
		for (Mesh m : meshes) {
			for (Vertex v : m.getVertices()) {
				vertices.insert(vertices.end(), v);
			}
		}
		return vertices;
	}

	vector<unsigned int> Model::getIndices() {
		vector<unsigned int> indices;
		for (Mesh m : meshes) {
			for (unsigned i : m.getIndices()) {
				indices.insert(indices.end(), i);
			}
		}
		return indices;
	}

	// draws the model, and thus all its meshes
	void Model::draw(Shader shader)
	{
		if(meshes.size()==0)
			cout << "NNULL meshes" << endl;

		for (Mesh m : meshes) {
			m.draw(shader);
		}
	}

	void Model::translate(float x, float y, float z) {
		matrix = glm::translate(matrix, glm::vec3(x, y, z));
		vec3 currCoord = matrix*vec4(1.0f);
		this->x = currCoord.x;
		this->y = currCoord.y;
		this->z = currCoord.z;
		//glm::vec3 c = matrix * vec4(1.0);
		cout << "Player coords: " << this->x << ", " << this->y << ", " << this->z << endl;
	}

	void Model::goTo(float x, float y, float z) {
		matrix = glm::translate(mat4(1.0f), glm::vec3(x, y, z));
		vec3 currCoord = matrix*vec4(1.0f);
		this->x = x;
		this->y = y;
		this->z = z;
		//glm::vec3 c = matrix * vec4(1.0);
		cout << "Player coords2: " << this->x << ", " << this->y << ", " << this->z << endl;
	}

	mat4 Model::fakeTranslate(float x, float y, float z) {//TODO remove the function
		return glm::translate(matrix, glm::vec3(x, y, z));
	}

	void Model::scale(float x, float y, float z) {
		/*matrix = glm::scale(matrix, glm::vec3(x, y, z));
		width /= x;
		height /= y;
		depth /= z;
		recalculateMeshVolumes();*/
		//TODO implement scale
	}
	glm::mat4 Model::getMatrix() {
		return matrix;
	}
	float Model::getWidth() { return width; }
	float Model::getHeight() { return height; }
	float Model::getDepth() { return depth; }
	glm::vec3 Model::getVolume() { return glm::vec3(width, height, depth); }
	glm::vec3 Model::getCoords() {
		vec4 coords = matrix*vec4(1.0f);
		return vec3(coords.x, coords.y, coords.z);
	}
	void Model::setCoords(glm::vec3 coord) {
		matrix = glm::translate(matrix, coord);
	}


	/*  Functions   */
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void Model::recalculateMeshVolumes() {
		/*for (int a = 0; a < meshes.size(); ++a) {
		meshes[a].calculateVolume();
		}*/
		//TODO implement updating coords and params of meshes
	}

	void Model::loadModel(string const &path)
	{
		cout << "---Starting loading '" << path << "' model...\n";
		cout << "Reading from file..." << endl;
		directory = path.substr(0, path.find('/', 0));
		cout << "Directory: " << directory << endl;

		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path,
			aiProcess_Triangulate |
			aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace);

		const char* str = importer.GetErrorString();
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		else cout << "Model readed from file.\n";
		// retrieve the directory path of the filepath

		// process ASSIMP's root node recursively
	//	cout << "Starting node processing...\n";
		processNode(scene->mRootNode, scene);
		cout << "Model coords: " << x << ", " << y << ", " << z << endl;
		cout << "Model volume: " << width << ", " << height << ", " << depth << endl;
		//	cout << "Nodes processed.\n";
		cout << "---Model '" << path << "' loaded.\n";
	}

	int a = 0;
	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void Model::processNode(aiNode *node, const aiScene *scene)
	{
		// process each mesh located at the current node
		unsigned int numMeshes = node->mNumMeshes;
		for (unsigned int i = 0; i < numMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			Mesh processedMesh = processMesh(mesh, scene);
			vec3 params = processedMesh.getVolume();
			vec3 coords = processedMesh.getCoords();
			if (params.x > width) width = params.x;
			if (params.y > height) height = params.y;
			if (params.z > depth) depth = params.z;
			if (coords.x < x) x = coords.x;
			if (coords.y < y) y = coords.y;
			if (coords.z < z) z = coords.z;
			meshes.push_back(processedMesh);
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}

	}

	Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
	{
		// data to fill
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

		if (mesh->mVertices == NULL) cout << "NULL mesh vertices!\n";
		if (mesh->mNormals == NULL) cout << "NULL mesh normals!\n";
		if (mesh->mTextureCoords == NULL) cout << "NULL mesh textureCoords!\n";
		if (mesh->mTangents == NULL) cout << "NULL mesh Tangents!\n";
		if (mesh->mBitangents == NULL) cout << "NULL mesh BiTangents!\n";

		// Walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
							  // positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			// texture coordinates
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			// tangent
			if (mesh->mTangents) {
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.Tangent = vector;
			}

			// bitangent
			if (mesh->mBitangents) {
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.Bitangent = vector;
			}
			vertices.push_back(vertex);

		}
		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// process materials
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		if (material == NULL) cout << "NULL material!\n";
		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		// 1. diffuse maps
		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		// return a mesh object created from the extracted mesh data


		return Mesh(vertices, indices, textures);
	}

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
	{
		vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			aiReturn ret = mat->GetTexture(type, i, &str);
			
			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   // if texture hasn't been loaded already, load it
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), this->directory);
				texture.type = typeName;
				texture.path = str;
				textures.push_back(texture);
				textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
		cout << "here3" << endl;
		return textures;
	}


unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	string matPath = directory + "/";
	matPath += path;
	cout << "MatPath: " << matPath << endl;

	unsigned char *data = stbi_load(matPath.c_str(), &width, &height, &nrComponents, 0);

	cout << "here!" << endl;
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		else
			cout << "ERROR!" << endl;
		cout << "here1-1: "<<
			format << ", " <<
			width << ", " <<
			height << ", "<<
			textureID << ", "
			<< endl;
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		cout << "here1-1-1!" << endl;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		cout << "here1-1-2!" << endl;
		stbi_image_free(data);
		cout << "here1-2!" << endl;
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	cout << "here-2!" << endl;

	return textureID;
}
