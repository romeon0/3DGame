#include "AnimatedModel.h"
#include "Helper.h"
//Bone
void Bone::addChild(Bone child) {
	children.push_back(child);
}
mat4 Bone::getTransformMatrix() { return transformMatrix; }
string Bone::getName() { return name; }

//KeyFrame
KeyFrame::KeyFrame(float timeStamp, map<string, BoneTransform> poses) {
	this->timeStamp = timeStamp;
	this->poses = poses;
}
float KeyFrame::getTimeStamp() {
	return timeStamp;
}
map<string, BoneTransform> KeyFrame::getPoses() {
	return poses;
}

//Animation
list<KeyFrame> Animation::getFrames() {
	return frames;
}
int Animation::getFramesCount() { return frameCount; }
void Animation::setError(int error) {
	this->error = error;
}
bool Animation::isLoaded() {
	return error == 0 ? true : false;
}
float Animation::getDuration() {
	return duration;
}

//Animator
void Animator::setUnit(AnimatedModel model) {
	cout << "here1\n";
	unit = &model;
	currentAnim = loader.loadAnimation(model.getFilePath());
	cout << "here2\n";
}
void Animator::update(float time) {
	if (!unit || !currentAnim->isLoaded()) return;
}
void Animator::render() {

}
Animator::Animator() {

}

mat4 invModelMatrix;
//AnimationLoader
AnimationLoader::AnimationLoader() {}
Animation* AnimationLoader::loadAnimation(string modelPath) {
	Animation* anim=NULL;
	Importer importer;

	unsigned int postprocessingFlags = 0;// aiProcess_FlipUVs | aiProcess_Triangulate;
	const aiScene* scene = importer.ReadFile(modelPath, postprocessingFlags);
	if (!scene || scene->mFlags&&AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "Can't read '" << modelPath << "' file" << endl;
		return NULL;
	}
	else cout << "Readed from file.\n";
	if (!scene->HasAnimations()) {
		return NULL;
	}
	else cout << "Starting animation processing...\n";


	aiAnimation* animation = scene->mAnimations[0];//only first animation

	list<KeyFrame> frames;
	map<unsigned int, BoneTransform> poses;
	string name(animation->mName.C_Str());
	float duration = (float)animation->mDuration;
	float ticksPerSecond = (float)animation->mTicksPerSecond;
	int meshChannels = animation->mNumMeshChannels;

	cout << "Animation\n";
	cout << "--------------------------\n";
	cout << "AnimName: " << animation->mName.C_Str() << endl;
	int nrChannels = animation->mNumChannels;
	cout << "NrChannels: " << nrChannels << endl;
	cout << "NrMeshChannels: " << meshChannels << endl;

	for (int a = 0; a < nrChannels; ++a) {
		aiNodeAnim* channel = animation->mChannels[a];
		cout << "Channel name:" << channel->mNodeName.C_Str() << endl;
		int nrKeys = channel->mNumPositionKeys;
		for (int c = 0; c < nrKeys; ++c) {
			aiVector3D posKeyValue = channel->mPositionKeys[c].mValue;
			aiVector3D scalKeyValue = channel->mScalingKeys[c].mValue;
			aiQuaternion rotKeyValue = channel->mRotationKeys[c].mValue;
			double posKeyTime = channel->mPositionKeys[c].mTime;
			double rotKeyTime = channel->mRotationKeys[c].mTime;
			double scalKeyTime = channel->mScalingKeys[c].mTime;
			cout << "Position key " << c 
				<< ": (" << posKeyValue.x << " "<< posKeyValue.y << " " << posKeyValue.z << ") :: (" 
				<< posKeyTime << " " << rotKeyTime << " " << scalKeyTime << ")" << endl;
			/*cout << "Scalling key " << c << ": (" << scalKeyValue.x << " "
			<< scalKeyValue.y << " " << scalKeyValue.z << "):: " << posKeyTime << endl;
			cout << "Rotation key " << c << ": (" << rotKeyValue.x << " "
			<< rotKeyValue.y << " " << rotKeyValue.z << "):: " << posKeyTime << endl;
			cout << "----\n";*/
		}
	}

	cout << "--------------------------";

	return anim;
}



Animation* AnimationLoader::loadAnimations(string modelPath) {
	/*Animation* anims=NULL;
	Importer importer;

	unsigned int postprocessingFlags = aiProcess_FlipUVs | aiProcess_Triangulate;
	const aiScene* scene = importer.ReadFile(modelPath, postprocessingFlags);
	if (!scene || scene->mFlags&&AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "Can't read '" << modelPath << "' file" << endl;
		return anims;//NULL value
	}

	uint nrAnims = scene->mNumAnimations;
	for (int a = 0; a < nrAnims; ++a) {
		aiAnimation* animation = scene->mAnimations[a];
		uint nrAnimChannels = animation->mNumChannels;
		animation->
			for (int b = 0; b < nrAnimChannels; ++b) {
				aiNodeAnim* channel = animation->mChannels[b];
				channel->
			}
	}
	*/
	return NULL;
}

//AnimatedModel
AnimatedModel::AnimatedModel(string filePath) {
	path = filePath;
	directory = path.substr(0, path.find('/', 0));
	initModel(filePath);
}
AnimatedModel::~AnimatedModel() {
	for(int a =0; a < textures.size(); ++a)
	glDeleteTextures(1, &textures[a].id);
}

void AnimatedModel::initModel(string filePath) {
	cout << "Model: Loading model '" << filePath << "'..." << endl;
	Importer importer;
	unsigned int flags = aiProcess_GenNormals | aiProcess_FlipUVs | aiProcess_Triangulate;
	const aiScene* scene = importer.ReadFile(filePath, flags);
	if (!scene || scene->mFlags&& AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "Model::'" << filePath << "':" << importer.GetErrorString() << endl;
		loaded = false;
		return;
	}

	processNode(scene->mRootNode, scene);

	loaded = true;
	cout << "Model: Loaded" << endl;
}

void AnimatedModel::processNode(aiNode* node, const aiScene* scene) {

	unsigned int nrMeshes = node->mNumMeshes;
	for (int a = 0; a < nrMeshes; ++a) {
		aiMesh* mesh = scene->mMeshes[a];
		Mesh processedMesh = processMesh(mesh, scene);
		meshes.push_back(processedMesh);
	}

	unsigned int nrChildren = node->mNumChildren;
	for (int a = 0; a < nrChildren; ++a) {
		processNode(node->mChildren[a], scene);
	}
}

Mesh AnimatedModel::processMesh(aiMesh* mesh, const aiScene* scene) {
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	/*Read vertices*/
	unsigned int nrVertices = mesh->mNumVertices;
	for (int a = 0; a < nrVertices; ++a) {
		Vertex vertex;
		vec3 tmpVector;

		//position
		tmpVector.x = mesh->mVertices[a].x;
		tmpVector.y = mesh->mVertices[a].y;
		tmpVector.z = mesh->mVertices[a].z;
		vertex.Position = tmpVector;

		//tangents
		if (mesh->mTangents) {
			tmpVector.x = mesh->mTangents[a].x;
			tmpVector.y = mesh->mTangents[a].y;
			tmpVector.z = mesh->mTangents[a].z;
		}
		vertex.Tangent = tmpVector;

		//biTangents
		if (mesh->mBitangents) {
			tmpVector.x = mesh->mBitangents[a].x;
			tmpVector.y = mesh->mBitangents[a].y;
			tmpVector.z = mesh->mBitangents[a].z;
		}
		vertex.Bitangent = tmpVector;

		//textures(only first texture)
		if (mesh->mTextureCoords[0]) {
			vec2 texCoord;
			texCoord.x = mesh->mTextureCoords[0][a].x;
			texCoord.y = mesh->mTextureCoords[0][a].y;
			vertex.TexCoords = texCoord;
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
		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}

	return Mesh(vertices, indices, textures);
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
			texture.id = loadTextureFromFile(path, directory);
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
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		cout << "Can't load texture '" << path << "'" << endl;
	}
	stbi_image_free(data);

	return id;
}

bool AnimatedModel::getStatus() {
	return loaded;
}

void AnimatedModel::draw(Shader shader) {
	for (Mesh m : meshes) {
		m.draw(shader);
	}
}

string AnimatedModel::getFilePath() { return path; }

