#pragma once
#ifndef ANIMATEDMODEL_H
#define ANIMATEDMODEL_H

#include <string>
#include <vector>
#include <map>
#include <glm/vec3.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "BoneMesh.h"
#include "mesh.h"
#include "Tile.h"
#include "KeyFrame.h"
using std::multimap;
using glm::quat;
using glm::vec3;
using glm::mat4;
using std::string;
using std::vector;
using namespace Assimp;

struct Bone {
	string name;
	Bone* parent;
	vector<Bone*> children;
	mat4 transformation;
};

struct AnimData {
	double ticksPerSecond = 0;
	int numChannels = 0;
	double duration = 0;
	vector<KeyFrame*> keyFrames;
	string name;
};


struct BoneInfo {
	mat4 BoneOffset;
	int boneId;
};

class AnimatedModel {
private:
	//initializing
	void initModel(string filePath);
	void processNode(aiNode* node, const aiScene* scene);
	BoneMesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType texType, string texName);
	unsigned int loadTextureFromFile(const string& path, const string& directory, bool gamma = false);
	void extractBoneInformation(const aiScene* scene, aiNode* currNode);
	void createSkeleton(const aiScene* scene, aiNode* node, Bone* upgrNode);
	KeyFrame** getNextAndPrevFrames(double animTime);
	void AnimatedModel::initBoneTransform(aiNode* node);
	//other
	void updatePose(map<int, mat4> currentPose, Bone* bone, mat4 parentTransform);
	bool isBone(aiAnimation* animation, string nodeName);
	//void showSkeleton(Bone* bone);
public:
	bool loaded = false;
	string path;
	float currentFrameTime = 0;
	//bones attributes
	const int MAX_BONES = 4;
	map<string, int> boneIds;
	vector<BoneInfo> boneInfo;
	map<string, mat4>* modelTransformations;
	mat4 globalInvTransMatrix;
	int lastBoneId = 0;
	//model attributes
	float width = 0, height = 0, depth = 0;
	float x = 1000, y = 1000, z = 1000;
	glm::mat4 matrix = mat4(1.0f);
	glm::mat4 rotationMatrix = mat4(1.0f);
	bool selected = false;//if object is selected
	vector<BoneMesh> meshes;
	vector<Texture> textures;
	//animation attributes
	Bone* rootBone = NULL;
	AnimData* animData;
	map<int, mat4> boneTransformations;

	//(des/con)structors
	AnimatedModel(string filePath);
	AnimatedModel();
	~AnimatedModel();

	//getters
	bool getStatus();
	string getFilePath();
	int getBoneCount();
	mat4 getMatrix();
	bool isSelected();
	vec3 getCoords();
	vec3 getVolume();

	//setters
	void setSelected(bool value);

	//control
	void translate(float x, float y, float z);
	void translate(Tile tile);
	void goTo(float x, float y, float z);
	void extractData(string const &path, bool gamma = false);//if used empty constructor
	void rotateRad(float rad, int axis);
	void rotateDegree(float degree, int axis);
	void draw(Shader shader);
	void update(float elapsedTime);
	
	//other
	AnimatedModel& operator=(const AnimatedModel& model);
	AnimatedModel* operator=(const AnimatedModel* model);
};
#endif