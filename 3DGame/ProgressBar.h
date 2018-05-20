#pragma once
#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_

#include "SimpleModel.h"

class ProgressBar {
private:
	double currProcent;
	double maxValue;
	double currValue;
	double x, y;
	double maxWidth, currWidth;
	double scrWidth,scrHeight;
	double height;
	SimpleModel* model;
	bool checked;

	void init() {
		vec3 min = vec3(x, y, 0.0f);
		vec3 max = vec3(x + currWidth, y + height, 0.0f);

		Vertex vert1, vert2, vert3, vert4;
		vector<Vertex> vertices;
		vector<unsigned int> indices;

		vert1.Position = vec3(min.x, max.y, min.z);
		vert2.Position = vec3(max.x, max.y, min.z);
		vert3.Position = vec3(max.x, min.y, min.z);
		vert4.Position = vec3(min.x, min.y, min.z);
		vert1.TexCoords = vec2(0.0f, 0.0f);
		vert2.TexCoords = vec2(1.0f, 0.0f);
		vert3.TexCoords = vec2(1.0f, 1.0f);
		vert4.TexCoords = vec2(0.0f, 1.0f);
		vertices.push_back(vert1);
		vertices.push_back(vert2);
		vertices.push_back(vert3);
		vertices.push_back(vert4);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(0);

		model = new SimpleModel(vertices, indices);
		checked = true;
	}

	void update() {
		currProcent = currValue / maxValue;
		currWidth = maxWidth * currProcent;
		init();
	}
public:
	ProgressBar(double x, double y, double width, double height, double scrWidth, double scrHeight,double maxValue) {
		
		this->scrWidth = scrWidth;
		this->scrHeight = scrHeight;
		this->x = ((x) / (scrWidth / 2.0f) - 1.0f);
		this->y = ((y) / (scrHeight / 2.0f) - 1.0f);
		this->maxWidth = this->currWidth = (((x+width) / (scrWidth / 2.0f) - 1.0f)) - this->x;
		this->height = (((y+height) / (scrHeight / 2.0f) - 1.0f)) - this->y;
		this->maxValue = currValue = maxValue;

		cout << "1:" << x << " " << y << " " << height << " " << width << " " << scrHeight << " " << scrWidth << endl;
		cout << "2: " << this->x << " " << this->y << " " << this->height << " " << this->maxWidth << " " << this->scrHeight << " " << this->scrWidth << endl;

		init();
		currProcent = 1.0;
	}

	SimpleModel* getModel() { return model; }

	void add(double value) {
		double newValue = currValue + value;
		if (newValue > maxValue)
			currValue = maxValue;
		else if (newValue < 0)
			currValue = 0;
		else
			currValue = newValue;
		update();
	}
};

#endif