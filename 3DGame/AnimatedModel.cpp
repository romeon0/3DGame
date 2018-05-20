#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <list>
#include <algorithm>
#include <iterator>
#include "AnimatedModel.h"
#include "GameWorld.h"
#include "Helper.h"
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_operation.hpp>
using std::cout;
using std::endl;
using std::pair;
using std::map;
using std::list;
using std::vector;




AnimatedModel::AnimatedModel(string filePath) {
	modelTransformations = new map<string, mat4>();
	path = filePath;
	initModel(filePath);
}
AnimatedModel::AnimatedModel(){
	modelTransformations = new map<string, mat4>();
}
AnimatedModel::~AnimatedModel() {
	for(int a =0; a < textures.size(); ++a)
	glDeleteTextures(1, &textures[a].id);
}


void AnimatedModel::processNode(aiNode* node, const aiScene* scene) {
	Helper h;
	string nodeName = node->mName.C_Str();
	mat4 nodeTransformation = h.aiMatrix4x4ToGlm(node->mTransformation);
	//modelTransformations->insert(make_pair(nodeName,nodeTransformation));

	cout << "NodeName: " << node->mName.C_Str() << endl;
	cout << "NodeTransformation:\n";
	h.showMatrix(nodeTransformation);

	//process all meshes
	unsigned int nrMeshes = node->mNumMeshes;
	for (int a = 0; a < nrMeshes; ++a) {
		aiMesh* mesh = scene->mMeshes[a];
		BoneMesh processedMesh = processMesh(mesh, scene);
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

	for (int a = 0; a < node->mNumChildren; ++a) {
		processNode(node->mChildren[a], scene);
	}
}

BoneMesh AnimatedModel::processMesh(aiMesh* mesh, const aiScene* scene) {
	vector<BoneVertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	//cout << "MeshName--: " << mesh->mName.C_Str() << endl;

	/*Read vertices*/
	unsigned int nrVertices = mesh->mNumVertices;
	for (int a = 0; a < nrVertices; ++a) {
		BoneVertex vertex;
		vec3 tmpVector;

		//position
		tmpVector.x = mesh->mVertices[a].x;
		tmpVector.y = mesh->mVertices[a].y;
		tmpVector.z = mesh->mVertices[a].z;
		vertex.position = tmpVector;

		if (mesh->HasNormals()) {
			tmpVector.x = mesh->mNormals[a].x;
			tmpVector.y = mesh->mNormals[a].y;
			tmpVector.z = mesh->mNormals[a].z;
			vertex.normal = tmpVector;
		}

		//textures(only first texture)
		if (mesh->mTextureCoords[0]) {
			vec2 texCoord;
			texCoord.x = mesh->mTextureCoords[0][a].x;
			texCoord.y = mesh->mTextureCoords[0][a].y;
			vertex.texCoords = texCoord;
		}

		vertices.push_back(vertex);
	}

	/*Read faces*/
	unsigned int nrFaces = mesh->mNumFaces;
	for (int a = 0; a < nrFaces; ++a) {
		aiFace face = mesh->mFaces[a];
		for (int b = 0; b < face.mNumIndices; ++b) {
			indices.push_back(face.mIndices[b]);
		}
	}
	/*Read materials
	In shaders we need to name textures in next mode:
	1. texture_diffuseN, where N is number of texture from 1 to MAX_SAMPLER_NUMBER;
	2. texture_specularN, 1<=N<=MAX_SAMPLER_NUMBER
	3. texture_normalN, 1<=N<=MAX_SAMPLER_NUMBER
	*/
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	if (material) {
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
	/*BoneVertex vert;

	//no bones
	if (mesh->mNumBones == 0) return BoneMesh(vertices, indices, textures);

	
	//- processing assimp bones;
	//- extracting bone ids and bone weights to weightsMap;
	
	multimap<int, pair<int, float>> weightsMap;
	int nrBones = mesh->mNumBones;
	for (int boneNumber = 0; boneNumber < nrBones; ++boneNumber) {
		aiBone* bone = mesh->mBones[boneNumber];
		processBoneInfo(mesh, bone, boneNumber, weightsMap);
	}
	//assign bone weights and ids to vertices
	vec4 weights(0.0f, 0.0f, 0.0f, 0.0f);
	vec4 ids(0.0f, 0.0f, 0.0f, 0.0f);
	int counter = 0;
	for (pair<int, pair<int, float>> value : weightsMap) {
		int boneNumber = value.first;
		pair<int, float> vertexInfo = value.second;
		vec4 vertex = vertices.at(vertexInfo.first).boneWeights;
		for (int a = 0; a < MAX_BONES; ++a) {
			if (vertex[a] == 0.0) {
				vertices.at(vertexInfo.first).boneWeights[a] = vertexInfo.second;
				vertices.at(vertexInfo.first).boneIds[a] = boneNumber;
				break;
			}
		}
	}*/

	BoneMesh boneMesh(vertices, indices, textures);
	return boneMesh;
}


vector<Texture> AnimatedModel::loadMaterialTextures(aiMaterial* material, aiTextureType texType, string texName) {
	vector<Texture> textures;
	unsigned int nrTextures = material->GetTextureCount(texType);

	for (int a = 0; a < nrTextures; ++a) {
		aiString texPath;
		aiReturn ret = material->GetTexture(texType, a, &texPath);
		if (ret != aiReturn_SUCCESS) {
			cout << "Model::texture: Something wrong with material.GetTexture()" << endl;
			continue;
		}

		unsigned int nrLoadedTextures = this->textures.size();
		bool alreadyLoaded = false;
		for (int b = 0; b < nrLoadedTextures; ++b) {
			if (strcmp(texPath.C_Str(), this->textures[b].path.C_Str()) == 0) {
				alreadyLoaded = true;
				textures.push_back(this->textures[b]);
				break;
			}
		}

		if (!alreadyLoaded) {
			Texture texture;
			string directory = path.substr(0, path.find('/', 0));
			texture.id = loadTextureFromFile(texPath.C_Str(), directory);
			texture.path = texPath;
			texture.type = texName;
			textures.push_back(texture);
			this->textures.push_back(texture);
		}
	}

	return textures;
}

unsigned int AnimatedModel::loadTextureFromFile(const string& path, const string& directory, bool gamma) {
	unsigned int id;
	glGenTextures(1, &id);


	int width, height, nrComponents;
	string matPath = string(directory+"/"+path);

	unsigned char* data = stbi_load(matPath.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		cout << "Can't load texture '" << matPath.c_str() << "'" << endl;
	}
	stbi_image_free(data);

	return id;
}

bool AnimatedModel::getStatus() {
	return loaded;
}

void AnimatedModel::draw(Shader shader) {
	for (BoneMesh m : meshes) {
		m.draw(shader);
	}
}

string AnimatedModel::getFilePath() { return path; }

int AnimatedModel::getBoneCount() {
	return boneIds.size();
}

mat4 AnimatedModel::getMatrix() { 
	mat4 scal = mat4(1.0f);
	mat4 pos = mat4(1.0f);
	scal[0][0] = matrix[0][0];
	scal[1][1] = matrix[1][1];
	scal[2][2] = matrix[2][2];

	pos[3][0] = matrix[3][0];
	pos[3][1] = matrix[3][1];
	pos[3][2] = matrix[3][2];
	return pos*rotationMatrix*scal;
}


void AnimatedModel::extractData(string const &path, bool gamma) {
	this->path = path;
	initModel(path);

}

void AnimatedModel::translate(float x, float y, float z) {
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
void AnimatedModel::translate(Tile tile) {
	matrix = glm::translate(matrix, glm::vec3(tile.getX(), 1.0f, tile.getY()));
	vec3 currCoord = matrix*vec4(1.0f);
	this->x = currCoord.x;
	this->y = currCoord.y;
	this->z = currCoord.z;
}

void AnimatedModel::goTo(float x, float y, float z) {
	this->x = matrix[3][0] = x;
	//this->y = matrix[3][1] = y;
	this->z = matrix[3][2] = z;
}


AnimatedModel& AnimatedModel::operator=(const AnimatedModel& model) {
	loaded = model.loaded;
	meshes = model.meshes;
	textures = model.textures;

	globalInvTransMatrix = model.globalInvTransMatrix;
	path = model.path;
	width = model.width;
	height = model.height;
	depth = model.depth;
	x = model.x;
	y = model.y;
	z = model.z;
	matrix = model.matrix;
	rootBone = model.rootBone;
	animData = model.animData;
	rootBone = model.rootBone;
	boneIds = model.boneIds;
	boneInfo = model.boneInfo;
	return *this;
}

AnimatedModel* AnimatedModel::operator=(const AnimatedModel* model) {
	loaded = model->loaded;
	meshes = model->meshes;
	textures = model->textures;

	globalInvTransMatrix = model->globalInvTransMatrix;
	path = model->path;
	width = model->width;
	height = model->height;
	depth = model->depth;
	x = model->x;
	y = model->y;
	z = model->z;
	matrix = model->matrix;
	rootBone = model->rootBone;
	animData = model->animData;
	rootBone = model->rootBone;
	boneIds = model->boneIds;
	boneInfo =  model->boneInfo;
	return this;
}


//@param axis : 1=x, 2=y, 3=z
void AnimatedModel::rotateRad(float rad, int axis) {
	assert(axis <= 3 && axis >= 1);

	if (axis == 1)
		rotationMatrix = glm::rotate(mat4(1.0f), rad, vec3(1.0f, 0.0f, 0.0f));
	else if (axis == 2)
		rotationMatrix = glm::rotate(mat4(1.0f), rad, vec3(0.0f, 1.0f, 0.0f));
	else
		rotationMatrix = glm::rotate(mat4(1.0f), rad, vec3(0.0f, 0.0f, 1.0f));
}

//@param axis : 1=x, 2=y, 3=z
void AnimatedModel::rotateDegree(float degree, int axis) {
	assert(axis <= 3 && axis >= 1);

	if (axis == 1)
		rotationMatrix = glm::rotate(rotationMatrix, radians(degree), vec3(1.0f, 0.0f, 0.0f));
	else if (axis == 2)
		rotationMatrix = glm::rotate(rotationMatrix, radians(degree), vec3(0.0f, 1.0f, 0.0f));
	else
		rotationMatrix = glm::rotate(rotationMatrix, radians(degree), vec3(0.0f, 0.0f, 1.0f));
}


bool AnimatedModel::isBone(aiAnimation* animation, string nodeName) {
	aiNodeAnim** channels = animation->mChannels;
	unsigned int nrChannels = animation->mNumChannels;
	bool found = false;
	for (int a = 0; a < nrChannels; ++a) {
		string name(channels[a]->mNodeName.C_Str());
		if (name.compare(nodeName) == 0) {
			found = true;
			break;
		}
	}
	return found;
}
void AnimatedModel::createSkeleton(const aiScene* scene, aiNode* node, Bone* upgrNode) {
	
	Helper h;
	string nodeName = node->mName.C_Str();
	mat4 nodeTransformation = h.aiMatrix4x4ToGlm(node->mTransformation);
	aiNode* parent = node->mParent;
	string parentName = parent == NULL ? "NO ROOT" : parent->mName.C_Str();
	

	//check if node is bone
	bool nodeIsBone = isBone(scene->mAnimations[0], nodeName);
	if(nodeIsBone)
	cout << "IS Bone: " << nodeName << endl;

	//insert ID map to NAME
	if (nodeIsBone)
		boneIds.insert(make_pair(nodeName, lastBoneId++));

	//check if is root bone
	if (nodeIsBone && upgrNode == NULL) {
		Bone* newRoot = new Bone();
		newRoot->transformation = nodeTransformation;
		newRoot->name = nodeName;
		newRoot->parent = NULL;
		rootBone = upgrNode = newRoot;
	}

	//for each child do the same operations
	for (int nrChild = 0; nrChild < node->mNumChildren; ++nrChild) {
		if (nodeIsBone) {
			aiNode* child = node->mChildren[nrChild];
			Bone* upgrChild = new Bone();
			upgrChild->transformation = h.aiMatrix4x4ToGlm(child->mTransformation);
			upgrChild->name = child->mName.C_Str();
			upgrChild->parent = upgrNode;
			upgrNode->children.push_back(upgrChild);
			createSkeleton(scene, node->mChildren[nrChild], upgrChild);
		}
		else
			createSkeleton(scene, node->mChildren[nrChild], upgrNode);
	}
}
void AnimatedModel::extractBoneInformation(const aiScene* scene, aiNode* node) {
	Helper h;
	string nodeName = node->mName.C_Str();
	mat4 nodeTransformation = h.aiMatrix4x4ToGlm(node->mTransformation);
	aiNode* parent = node->mParent;
	string parentName = parent == NULL ? "NO_PARENT" : parent->mName.C_Str();

	//cout << "--Node '"+nodeName+"' Transformation--\n";
	//h.showMatrix(nodeTransformation);

	//iterate all meshes to save bone information and vertex weights
	unsigned int* meshesIndexes = node->mMeshes;
	int nrMeshes = node->mNumMeshes;
	for (int nrMesh = 0; nrMesh < nrMeshes; ++nrMesh) {
		aiMesh* mesh = scene->mMeshes[meshesIndexes[nrMesh]];

		aiBone** bones = mesh->mBones;
		if (bones != NULL) {
			int nrBones = mesh->mNumBones;
			for (int nrBone = 0; nrBone < nrBones; ++nrBone) {
				aiBone* bone = bones[nrBone];
				string boneName = bone->mName.C_Str();
				unsigned int boneId = boneIds.at(boneName);

				mat4 boneOffset = h.aiMatrix4x4ToGlm(bone->mOffsetMatrix);

				cout << "-------BoneName: " << boneName << endl;
				//	cout << "--Bone '" << boneName << "' Offset--\n";
				//	h.showMatrix(boneOffset);

				//mapping bone name to bone id
				BoneInfo info;
				info.BoneOffset = boneOffset;
				info.boneId = boneId;

				bool found = false;
				for (BoneInfo b : boneInfo) {
					if (b.boneId == boneId) {
						found = true;
					}
				}
				if (!found)
					boneInfo.push_back(info);

				//add vertex weights and bone ids
				int nrWeights = bone->mNumWeights;
				for (int nrWeight = 0; nrWeight < nrWeights; ++nrWeight) {
					aiVertexWeight vertexWeight = bone->mWeights[nrWeight];
					float weight = vertexWeight.mWeight;
					uint vertexId = vertexWeight.mVertexId;
					meshes.at(nrMesh).addBoneData(vertexId, weight, boneId);
				}
			}
			cout << "-------------------\n";
		}
	}

	//for each child do the same operations
	for (int nrChild = 0; nrChild < node->mNumChildren; ++nrChild) {
		extractBoneInformation(scene, node->mChildren[nrChild]);
	}
}
void AnimatedModel::initBoneTransform(aiNode* node) {
	Helper h;
	string currNodeName(node->mName.C_Str());
	
	if(boneIds.find(currNodeName)!=boneIds.end()){
		int nodeId = boneIds.at(currNodeName);
		mat4 transformation = h.aiMatrix4x4ToGlm(node->mTransformation);
		boneTransformations.insert(make_pair(nodeId,mat4(1.0f)));
	}
	for (int nrChild = 0; nrChild < node->mNumChildren; ++nrChild) {
		initBoneTransform(node->mChildren[nrChild]);
	}
}
void AnimatedModel::initModel(string filePath) {
	path = filePath;
	Helper h;
	loaded = true;
	cout << "AnimatedModel: Loading model '" << filePath << "'..." << endl;
	Importer importer;
	unsigned int flags = aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace | aiProcess_GenNormals | aiProcess_LimitBoneWeights;
	const aiScene* scene = importer.ReadFile(filePath, flags);
	if (!scene || scene->mFlags&& AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "Model::'" << filePath << "':" << importer.GetErrorString() << endl;
		loaded = false;
		return;
	}


	//---init
	modelTransformations = new map<string, mat4>();
	globalInvTransMatrix = inverse(h.aiMatrix4x4ToGlm(scene->mRootNode->mTransformation));
	matrix = mat4(1.0f);
	//---------------------

	//---process skin
	cout << "StartMatrix: \n";
	h.showMatrix(matrix);
	processNode(scene->mRootNode, scene);
	cout << "FinalMatrix: \n";
	h.showMatrix(matrix);
	cout << "Skin processed----------------\n";
	//---------------------

	//---extract bone information
	if (scene->mAnimations != NULL) {
		if (scene->mAnimations[0]->mChannels != NULL) {
			//create bone hierarchy
			createSkeleton(scene, scene->mRootNode, rootBone);
			
			//extract vertex weights and bone indices
			extractBoneInformation(scene, scene->mRootNode);

			//insert bone offsets for bones that dont change model(I believe this is temporal solution...)
			vector<int> allIds;
			vector<int> infoIds;
			vector<int> differenceIds;
			for (pair<string, int> boneId : boneIds) {
				allIds.push_back(boneId.second);
			}
			for (BoneInfo info : boneInfo) {
				infoIds.push_back(info.boneId);
			}

			sort(allIds.begin(), allIds.end());
			sort(infoIds.begin(), infoIds.end());

			std::set_difference(allIds.begin(),
				allIds.end(),
				infoIds.begin(),
				infoIds.end(), 
				std::back_inserter(differenceIds));
			cout << "Difference\n";
			for (int a = 0; a < differenceIds.size(); ++a) {
				cout << differenceIds.at(a) << endl;
				BoneInfo info;
				info.boneId = differenceIds.at(a);
				info.BoneOffset = mat4(1.0f);
				boneInfo.push_back(info);
			}
			
			cout << "IDs: \n";
			for (auto it = boneIds.begin(); it != boneIds.end(); ++it) {
				pair<string, int> id = (pair<string, int>)*it;
				cout << id.first << ": " << id.second << endl;
			}
			cout << "BoneInfo: \n";
			for (int a = 0; a < boneInfo.size(); ++a) {
				cout << boneInfo.at(a).boneId << endl;
			}

//			showSkeleton(rootBone);
			//system("pause");
		}
		else loaded = false;
	}
	else loaded = false;
	meshes.at(0).update();//notify mesh that vertices were update
	cout << "Bone information extracted-----------\n";
	//------------------

	//---extract animation information
	cout << "Model: Adding animation information...\n";
	if (scene->mAnimations != NULL) {
		animData = new AnimData();
		aiAnimation** animations = scene->mAnimations;
		aiAnimation* animation = animations[0];
		animData->ticksPerSecond = animation->mTicksPerSecond;
		animData->numChannels = animation->mNumChannels;
		animData->duration = animation->mDuration;
		animData->name = animation->mName.C_Str();

		vector<KeyFrame*> keyFrames;
		KeyFrame* keyFrame;
		if (animation->mNumChannels > 0) {
			aiNodeAnim** channels = animation->mChannels;
			int nrKeyFrames = channels[0]->mNumPositionKeys;
			for (int nrKeyFrame = 0; nrKeyFrame < nrKeyFrames; ++nrKeyFrame) {
				aiNodeAnim* channel = channels[0];
				double keyTime = channel->mRotationKeys[nrKeyFrame].mTime;
				map<int, BoneTransform> poses;
				for (int nrChannel = 0; nrChannel < animation->mNumChannels; ++nrChannel) {
					channel = channels[nrChannel];
					string channelName(channel->mNodeName.C_Str());
					quat rotKey = h.assimpQuatToGlmQuat(channel->mRotationKeys[nrKeyFrame].mValue);
					vec3 scalKey = h.assimpVec3ToGlmVec3(channel->mScalingKeys[nrKeyFrame].mValue);
					vec3 posKey = h.assimpVec3ToGlmVec3(channel->mPositionKeys[nrKeyFrame].mValue);
					BoneTransform transform;
					transform.rotation = rotKey;
					transform.scaling = scalKey;
					transform.position = posKey;
					cout << "* ChannelName: " << channelName << endl;

					if (boneIds.find(channelName) != boneIds.end()) {
					//	cout <<"- Bone: "<< channelName << endl;
					//	cout << "BoneTransform\n";
					//	h.showMatrix(transform.toMatrix());
						int boneId = boneIds.at(channelName);
						poses.insert(make_pair(boneId, transform));
					}
				}
				keyFrame = new KeyFrame(keyTime, poses);
				keyFrames.push_back(keyFrame);
			}
			animData->keyFrames = keyFrames;

			//show info
			if(false)
			for (int nrKeyFrame = 0; nrKeyFrame < keyFrames.size(); ++nrKeyFrame) {
				KeyFrame* keyFrame = keyFrames.at(nrKeyFrame);
				cout << "Time: " << keyFrame->getTimeStamp() << endl;
				map<int, BoneTransform> transforms = keyFrame->getPoses();
				for (auto it = transforms.begin(); it != transforms.end(); ++it) {
					pair<int, BoneTransform> transform = (pair<int, BoneTransform>)*it;
					BoneTransform t = transform.second;
					cout << "- Bone ID: " << transform.first << endl;
					cout << "Position: ";
					h.showVector(t.position);
					cout << "Scaling: ";
					h.showVector(t.scaling);
					cout << "Rotation: ";
					h.showQuat(t.rotation);
				}
			}
		}
	}
	else loaded = false;
	
	//init bone transformation
	initBoneTransform(scene->mRootNode);

	/*vector<BoneVertex> vertexes = meshes.at(0).getVertices();
	for (int a = 0; a < vertexes.size(); ++a) {
		BoneVertex v = vertexes.at(a);
		cout << "Vertex "<<a<<"\n";
		cout << "- IDs:\n";
		h.showVector(v.boneIds);
		cout << "- Weights:\n";
		h.showVector(v.boneWeights);
	}*/

	

	cout << "Model: Animation information added.--------------\n";
}
void AnimatedModel::updatePose(map<int, mat4> currentPose, Bone* bone, mat4 parentTransform) {
	
	cout << "BoneName: " << bone->name << endl;//leftLeg1

	if (boneIds.find(bone->name) == boneIds.end()) {
		cout << "This ID not exists\n";
	}
	int boneId = boneIds.at(bone->name);
	cout << "B1\n";
	mat4 currLocalTransform = currentPose.at(boneId);
	cout << "B2\n";
	mat4 globalTransform = parentTransform*currLocalTransform;
	cout << "B3\n";
	boneTransformations.at(boneId) = globalInvTransMatrix *globalTransform * boneInfo.at(boneId).BoneOffset;



	for (Bone* childBone : bone->children) {
		updatePose(currentPose, childBone, globalTransform);
	} 
	
}
KeyFrame** AnimatedModel::getNextAndPrevFrames(double animTime) {
	cout << "CheckPoint\n";
	vector<KeyFrame*> frames = animData->keyFrames;
	KeyFrame* prevFrame = frames[0];
	KeyFrame* nextFrame = frames[0];
	cout << "CheckPoint1\n";
	for (int nrFrame = 1; nrFrame < frames.size(); ++nrFrame) {
		nextFrame = frames[nrFrame];
		if (nextFrame->getTimeStamp() > animTime)
			break;
		prevFrame = nextFrame;
	}
	cout << "CheckPoint2\n";
	KeyFrame** prevAndNext = new KeyFrame*[2];
	prevAndNext[0] = prevFrame;
	prevAndNext[1] = nextFrame;
	cout << "CheckPoint3\n";
	return prevAndNext;
}
void AnimatedModel::update(float elapsedTime) {
	if (!loaded) { cout << "Animation of '" << path << "' not loaded!\n";  return; }
	Helper h;

	/*//calculate time
	currentFrameTime += elapsedTime;
	float timeInSeconds = currentFrameTime / 1000.0f;
	if (timeInSeconds >= animData->duration) {
		currentFrameTime = fmod(currentFrameTime, animData->duration*1000.0f);
	}
	float ticksPerSecond = animData->ticksPerSecond != 0 ? animData->ticksPerSecond : 25.0f;
	float timeInTicks = ticksPerSecond*timeInSeconds;
	float animationTime = animData->duration == 0 ? 0.0f : (float)fmod(timeInTicks, animData->duration);*/

	elapsedTime /= 1000.0;
	currentFrameTime += elapsedTime;
	if (currentFrameTime >= animData->duration) {
		currentFrameTime = fmod(currentFrameTime, animData->duration);
	}

	/*cout << "TimeInSeconds: " << timeInSeconds << endl;
	cout << "ticksPerSecond: " << ticksPerSecond << endl;
	cout << "timeInTicks: " << timeInTicks << endl;
	cout << "animationTime: " << animationTime << endl;
	cout << "duration: " <<  animData.duration << endl;
	cout << "---------------------" << endl;*/

	//get frames that are bounds of current time
	KeyFrame** frames = getNextAndPrevFrames(currentFrameTime);
	KeyFrame* prevFrame = frames[0];
	KeyFrame* nextFrame = frames[1];
	float totalTime = nextFrame->getTimeStamp() - prevFrame->getTimeStamp();
	float currentTime = currentFrameTime - prevFrame->getTimeStamp();
	float progress = currentTime / totalTime;

	//calculate current pose
	map<int, mat4> currentPose;
	for (auto iterBoneId = boneIds.begin(); iterBoneId != boneIds.end(); ++iterBoneId) {
		int boneId = ((pair<string, int>)*iterBoneId).second;
		BoneTransform prevTransform = prevFrame->getPoses().at(boneId);
		BoneTransform nextTransform = nextFrame->getPoses().at(boneId);
		BoneTransform currentTransform = BoneTransform::interpolate(prevTransform, nextTransform, progress);
		mat4 currentTransformationMatrix = currentTransform.toMatrix();
		currentPose.insert(make_pair(boneId, currentTransformationMatrix));
	}

	/*printf("PrevTimeStamp: %.5f\n\
		NextTimeStamp: %.5f\n\
		AnimDuration: %.5f\n\
		ElapsedTime: %.5f\n\
		TotalTime: %.5f\n\
		CurrentTime: %.5f \n\
		Progress: %.5f\n", prevFrame->getTimeStamp(), nextFrame->getTimeStamp(),
		animData->duration, elapsedTime, totalTime, currentTime, progress);
	cout << "-------------\n";*/
	
	//update current pose
	updatePose(currentPose, rootBone, mat4(1.0f));
}

///////////////////////-------------------------PROCESSING------------------------//////////////////////////////////////

bool AnimatedModel::isSelected() {
	return selected;
}
void AnimatedModel::setSelected(bool value) { selected = value; }

vec3 AnimatedModel::getVolume() {
	return vec3(width, height, depth);
}

vec3 AnimatedModel::getCoords() {
	return vec3(x, y, z);
}