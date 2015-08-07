#pragma once

#include "ofMain.h"

class twomblyText {
public:
    //Functions
    twomblyText();
    void setup(string _text, ofPoint _position, float _rotation, vector<ofPolyline> &_letterForms);
    void draw();
    
    //Variables
    ofFbo drawFbo;
    ofColor color;
    ofTrueTypeFont font;
    string text;
    ofPoint position;
    float rotation;
    int boundingWidth;
    int boundingHeight;
};