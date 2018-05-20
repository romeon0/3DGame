/*#include <list>
#include <map>
#include <iostream>
#include "KeyFrame.h"
#include "Animation.h"
using std::vector;
using std::map;

map<string, map<int, BoneTransform>> Animation::getSkeletonPoses(Bone b, float currTime) {
	return skeletonPoses;
}
void Animation::setError(int error) {
	this->error = error;
}

float Animation::getDuration() {
	return duration;
}

string Animation::getName() {
	return name;
}
float Animation::getTicksPerSecond() {
	return ticksPerSecond;
}

pair<int, BoneTransform>* Animation::getFramesOnSides(Bone bone, float currAnimTimeTick) {
	map<int, BoneTransform> boneTransforms = skeletonPoses.at(bone.getName());
	if (boneTransforms.size() < 2) return NULL;

	pair<int, BoneTransform> start;
	pair<int, BoneTransform> end;
	int counter = 0;
	for (pair<int, BoneTransform> trans : boneTransforms) {
		if (trans.first < currAnimTimeTick) {
			start = trans;
		}

		if (trans.first > currAnimTimeTick) {
			end = trans;
		}
	}

	std::cout << "Selected Starttime: " << start.first << std::endl;
	std::cout << "Selected Endindex: " << end.first << std::endl;

	return new pair<int, BoneTransform>[2]{ start,end };
}

bool Animation::hasError() { return error == 0 ? false : true; }
int Animation::getError() { return error; }*/