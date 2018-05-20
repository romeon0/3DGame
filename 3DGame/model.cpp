#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <glm/glm.hpp>
#include <vector>
#include <glm/mat4x4.hpp>
#include "model.h"
#include "AnimatedModel.h"
#include "Helper.h"
#include "GameWorld.h"
using namespace Assimp;
using namespace glm;

/*  Functions   */
// constructor, expects a filepath to a 3D model.
Model::Model() {
}
Model::Model(string const &path, bool gamma) : gammaCorrection(gamma)
{
	this->path = path;
	loadModel(path);
}
Model::~Model() {
	//textures_loaded.clear();
	//meshes.clear();
}

/*Getters----------------*/
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

glm::mat4 Model::getMatrix() {
	//TranslationM * RotationM * ScalingM;
	mat4 scal = mat4(1.0f);// glm::scale(mat4(1.0f), vec3(matrix[0][0], matrix[1][1], matrix[2][2]));
	mat4 pos = mat4(1.0f);
	scal[0][0] = matrix[0][0];
	scal[1][1] = matrix[1][1];
	scal[2][2] = matrix[2][2];

	pos[3][0] = matrix[3][0];
	pos[3][1] = matrix[3][1];
	pos[3][2] = matrix[3][2];
	return pos*rotationMatrix*scal;
}

float Model::getWidth() { return width; }

float Model::getHeight() { return height; }

float Model::getDepth() { return depth; }

glm::vec3 Model::getVolume() {
	return glm::vec3(
		width,
		height, 
		depth
	); 
}

glm::vec3 Model::getCoords() {
	
	return vec3(x, y, z);
}

string Model::getName() { return name; }

string Model::getPath() { return path; }
/*------------------------*/

/*Setters--------------------*/
void Model::setName(string name) { this->name = name; }

void Model::setCoords(glm::vec3 coord) {
	x = coord.x;
	y = coord.y;
	z = coord.z;
	matrix[3][0] = x;
	matrix[3][1] = y;
	matrix[3][2] = z;
}
/*---------------------------*/




/*Utilities----------*/
void Model::draw(Shader shader)
{
	if (meshes.size() == 0)
		cout << "NNULL meshes" << endl;

	for (Mesh m : meshes) {
		m.draw(shader);
	}
}

void Model::extractData(string const &path, bool gamma) {
	this->path = path;
	gammaCorrection = gamma;
	loadModel(path);
}

void Model::translate(float x, float y, float z) {
	//matrix = glm::translate(matrix, glm::vec3(x, y, z));
	this->x += x;
	this->y += y;
	this->z += z;
	matrix[3][0] = this->x;
	matrix[3][1] = this->y;
	matrix[3][2] = this->z;
	Helper h;
	vec3 currCoord = vec3(x, y, z);
	vec2 tile = h.toTileCoords(currCoord, GameWorld::getInstance().getWorldOrigin());
	cout << "Player coords: " << this->x << ", " << this->y << ", " << this->z << endl;
	cout << "Player TILE: " << tile.x << ", " << tile.y << endl;
}

void Model::goTo(float x, float y, float z) {
	this->x = matrix[3][0] = x;
	//this->y = matrix[3][1] = y;
	this->z = matrix[3][2] = z;

	cout << "GoTo worked: " << this->x << ", " << this->y << ", " << this->z << endl;
}

mat4 Model::fakeTranslate(float x, float y, float z) {//TODO remove the function
	return glm::translate(matrix, glm::vec3(x, y, z));
}

void Model::scale(float x, float y, float z) {
	//TODO implement scale
	matrix = glm::scale(matrix, vec3(x, y, z));
	width *= x;
	height *= y;
	depth *= z;
	recalculateMeshVolumes(x, y, z);
}

void Model::recalculateMeshVolumes(float x, float y, float z) {
	for (int a = 0; a < meshes.size(); ++a) {
		meshes[a].calculateVolume();
	}
	//TODO implement updating coords and params of meshes
}

/*---------------------*/


/*  Functions   */
// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.


void Model::loadModel(string const &path)
{
	cout << "---Starting loading '" << path << "' model...\n";
	cout << "Reading from file..." << endl;
	directory = path.substr(0, path.find('/', 0));

	unsigned int processFlags = aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace;


	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, processFlags);
	const char* str = importer.GetErrorString();
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	else cout << "Model readed from file.\n";

	//init matrices
	matrix = mat4(	1.0f,0.0,0.0,0.0,
					0.0f, 1.0, 0.0, 0.0, 
					0.0f, 0.0, 1.0, 0.0, 
					1.0f, 1.0, 1.0, 1.0);
	rotationMatrix = mat4(1.0f);
	scalingVector = vec3(1.0f, 1.0f, 1.0f);

	//process all nodes 
	processNode(scene->mRootNode, scene);

	//apply scaling
	if(matrix[0][0]!=0)
		width  = width*matrix[0][0];
	if (matrix[1][1] != 0)
		height = height*matrix[1][1];
	if (matrix[2][2] != 0)
		depth = depth*matrix[2][2];
	
	//applying position
	x = matrix[3][0];
	y = matrix[3][1] = matrix[3][1] + height/2.0f;//above terrain
	z = matrix[3][2];

	//rotationMatrix = glm::rotate(rotationMatrix, 45.0f, vec3(1.0f, 0.0f, 0.0f));
	cout << "---Model '" << path << "' loaded.\n";
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void Model::processNode(aiNode *node, const aiScene *scene)
{
	Helper h;
	mat4 nodeTransformation = h.aiMatrix4x4ToGlm(node->mTransformation);

	cout << "Node Transformation: \n";
	h.showMatrix(nodeTransformation);

	for (int a = 0; a < 4; ++a) {
		for (int b = 0; b < 4; ++b) {
			if (nodeTransformation[a][b] != 0)
				matrix[a][b] *= nodeTransformation[a][b];
		}
	}

//	matrix *= nodeTransformation;

	/*//----update model matrix
	if (nodeTransformation[0][0] != 0.0f)//Scalling X
		matrix[0][0] *= nodeTransformation[0][0];
	if (nodeTransformation[1][1] != 0.0f)//Scalling Y
		matrix[1][1] *= nodeTransformation[1][1];
	if (nodeTransformation[2][2] != 0.0f)//Scalling Z
		matrix[2][2] *= nodeTransformation[2][2];
	if (nodeTransformation[3][0] != 0.0f)//Translate X
		matrix[3][0] *= nodeTransformation[3][0];
	if (nodeTransformation[3][1] != 0.0f)//Translate Y
		matrix[3][1] *= nodeTransformation[3][1];
	if (nodeTransformation[3][2] != 0.0f)//Translate Z
		matrix[3][2] *= nodeTransformation[3][2];
	//-----

	if (nodeTransformation[1][2] != 0.0f && nodeTransformation[2][1] != 0.0f) {//then X axis rotation
		cout << "X axis rotation!\n";
		rotationMatrix = nodeTransformation;
	}
	else if (nodeTransformation[2][0] != 0.0f) {//then Y axis rotation
		cout << "Y axis rotation\n";
		rotationMatrix = nodeTransformation;
	}
	else if (nodeTransformation[1][0] != 0.0f) {//then Z axis rotation
		cout << "Z axis rotation\n";
		rotationMatrix = nodeTransformation;
	}*/

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
		//position
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// normals
		if (mesh->HasNormals()) {
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}
		else
			vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
		// texture coordinates
		if (mesh->HasTextureCoords(0)) // does the mesh contain texture coordinates?
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
	else {
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
	}
	

	// return a mesh object created from the extracted mesh data

	Mesh result(vertices, indices, textures);
	return result;
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
	return textures;
}


unsigned int Model::TextureFromFile(const char *path, const string &directory, bool gamma)
{

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	string matPath = directory + "/";
	matPath += path;

	unsigned char *data = stbi_load(matPath.c_str(), &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

bool Model::isSelected() {
	return selected;
}
void Model::setSelected(bool value) { selected = value; }

void Model::rotateRad(float rad, int axis) {
	assert(axis <= 3 && axis >= 1);

	if (axis == 1)
		rotationMatrix = glm::rotate(mat4(1.0f), rad, vec3(1.0f, 0.0f, 0.0f));
	else if (axis == 2)
		rotationMatrix = glm::rotate(mat4(1.0f), rad, vec3(0.0f, 1.0f, 0.0f));
	else
		rotationMatrix = glm::rotate(mat4(1.0f), rad, vec3(0.0f, 0.0f, 1.0f));
}
void Model::rotateDegree(float degree, int axis) {
	assert(axis <= 3 && axis >= 1);

	if (axis == 1)
		rotationMatrix = glm::rotate(rotationMatrix, radians(degree), vec3(1.0f, 0.0f, 0.0f));
	else if (axis == 2)
		rotationMatrix = glm::rotate(rotationMatrix, radians(degree), vec3(0.0f, 1.0f, 0.0f));
	else
		rotationMatrix = glm::rotate(rotationMatrix, radians(degree), vec3(0.0f, 0.0f, 1.0f));
}

glm::mat4 Model::getRotationMatrix() {
	return rotationMatrix;
}