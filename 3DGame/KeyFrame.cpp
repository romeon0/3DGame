#include <map>
#include <iterator>
#include "KeyFrame.h"
using std::map;
using std::iterator;

KeyFrame::KeyFrame(float timeStamp, map<int, BoneTransform> poses) {
	this->timeStamp = timeStamp;
	this->poses = poses;
}
float KeyFrame::getTimeStamp() {
	return timeStamp;
}
map<int, BoneTransform> KeyFrame::getPoses() {
	return poses;
}

BoneTransform* KeyFrame::getPose(int boneId) {
	if (poses.find(boneId) != poses.end()) {
		return &poses[boneId];
	}

	return NULL;
}