#pragma once
#ifndef ANIMATEDMODEL_H_
#define ANIMATEDMODEL_H_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "mesh.h"
#ifndef  STBI_LOADED_
	#define STBI_LOADED_
	#include <stb/stb_image.h>
#endif
using namespace Assimp;
using namespace std;
using glm::vec3;
using glm::vec2;
using glm::ivec4;
using glm::mat4;
using glm::fquat;

typedef unsigned int uint;

struct BoneTransform;
class Bone;
class KeyFrame;
class Animation;
class AnimationLoader;
class AnimatedModel;
class Animator;


struct BoneTransform {
	//Parent coordinate system 
	vec3 position;
	fquat rotation;
	vec3 scaling;
};
class Bone {
private:
	mat4 transformMatrix;
	list<Bone> children;
	unsigned int id;
	string name;
public:
	void addChild(Bone child);
	mat4 getTransformMatrix();
	string getName();
};
class KeyFrame {
	/*
		Transformation of each bone per frame
				POSES
			Key(bone name)  Value(rot,scal and pos)
			Leg  (2.3,2.1,5.6)(45,56,12)
			Arm  (6.1,1.7,7.2)(89,56,42)
			Head  (10.3,1.1,1.8)(11,34,93)
			....
	*/
	map<string, BoneTransform> poses;//current poses of bones
	float timeStamp;//time when frame starts
public:
	KeyFrame(float timeStamp, map<string, BoneTransform> poses);

	float getTimeStamp();
	map<string, BoneTransform> getPoses();
};

class Animation {
	list<KeyFrame> frames;
	int frameCount;
	float duration;
	float ticksPerSecond;
	int error = 0;
	string name;
	Animation() {}
	Animation(const Animation& anim) {}
public:
	Animation(string name, float duration, float ticksPerSecond, list<KeyFrame> frames) {
		this->frames = frames;
		this->duration = duration;
		this->name = name;
		this->ticksPerSecond = ticksPerSecond;
	}
	list<KeyFrame> getFrames();

	int getFramesCount();
	float getDuration();
	string getName();

	void setError(int error);
	bool isLoaded();
};


class AnimationLoader {
public:
	AnimationLoader();
	Animation* loadAnimation(string modelPath);
	Animation* loadAnimations(string modelPath);
};

class Animator {
	AnimationLoader loader;
	Animation* currentAnim;
	float animationTime;
	AnimatedModel* unit;
public:
	Animator();
	void setUnit(AnimatedModel model);
	void update(float time);
	void render();
};

class AnimatedModel {
private:
	bool loaded = false;
	vector<Mesh> meshes;
	vector<Texture> textures;
	string directory;
	string path;

	//animation
	Bone root;
	Animator animator;
public:
	//(des/con)structors
	AnimatedModel(string filePath);
	~AnimatedModel();

	/*ANIMATION*/
	/*-------------------------------------*/



	/*-------------------------------------*/

	//getters/
	bool getStatus();
	string getFilePath();

	//initializing
	void initModel(string filePath);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType texType, string texName);
	unsigned int loadTextureFromFile(const string& path, const string& directory, bool gamma = false);
	void draw(Shader shader);
};



#endif