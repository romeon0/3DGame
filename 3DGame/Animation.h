/*#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <map>
#include "KeyFrame.h"
using std::string;
using std::vector;
using std::map;
using std::pair;

class Animation {
	map<string, map<int, BoneTransform>> skeletonPoses;
	int frameCount;
	float duration;
	float ticksPerSecond;
	int error = 0;
	string name;
	Animation() {}
	Animation(const Animation& anim) {}
public:
	Animation(string name, float duration, float ticksPerSecond, std::map<string, map<int, BoneTransform>> skeletonPoses) {
		this->skeletonPoses = skeletonPoses;
		this->duration = duration;
		this->name = name;
		this->ticksPerSecond = ticksPerSecond;
	}
	map<string, map<int, BoneTransform>> getSkeletonPoses(Bone b, float currTime);

	float getDuration();
	string getName();
	float getTicksPerSecond();
	std::pair<int,BoneTransform>* getFramesOnSides(Bone bone, float currAnimTimeTick);

	void setError(int error);
	bool hasError();
	int getError();

};
#endif*/