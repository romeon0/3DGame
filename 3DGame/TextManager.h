#pragma once
#ifndef TEXTMANAGER_H_
#define TEXTMANAGER_H_

#include <map>
#include <list>
#include "Text.h"
using std::map;
using std::list;

class TextManager {
private:
	map<string, Text*> texts;
	map<string, bool> drawText;
	map<string, SimpleModel*> fonts;
public:
	void addText(string name, Text* text) {
		texts.insert(make_pair(name, text));
		drawText.insert(make_pair(name, true));
	}
	void addText(string name, string text, string fontType, double screenX, double screenY, double fontSizeX, double fontSizeY, double SCR_WIDTH, double SCR_HEIGHT) {
		for (pair<string, SimpleModel*> font : fonts) {
			if (font.first.compare(fontType) == 0) {
				addText(name, new Text(text, font.second, screenX, screenY, fontSizeX,fontSizeY, SCR_WIDTH, SCR_HEIGHT));
				drawText.insert(make_pair(name, true));
				break;
			}
		}
	}
	void addText(string name, string text, SimpleModel* model, double screenX, double screenY, double fontSizeX, double fontSizeY, double SCR_WIDTH, double SCR_HEIGHT) {
		addText(name, new Text(text, model, screenX, screenY,fontSizeX,fontSizeY, SCR_WIDTH, SCR_HEIGHT));
		drawText.insert(make_pair(name, true));
	}
	void addFont(string type, SimpleModel* model) {
		fonts.insert(make_pair(type, model));
	}
	void updateText(string textName, string text, double scrWidth, double scrHeight) {
		if (texts.find(textName) != texts.end()) {
			texts.at(textName)->updateText(text,scrWidth,scrHeight);
		}
	}

	void removeText(string name) {
		if (texts.find(name) != texts.end()) {
			texts.erase(name);
		}
	}

	void draw(Shader shader) {
		for (pair<string,Text*> text : texts) {
			shader.use();
			if(drawText.at(text.first))
				text.second->getModel()->draw(shader,GL_TRIANGLES);
		}
	}

	void blockDraw(string textName) {
		drawText.at(textName) = false;
	}
	void allowDraw(string textName) {
		drawText.at(textName) = true;
	}
};

#endif 
