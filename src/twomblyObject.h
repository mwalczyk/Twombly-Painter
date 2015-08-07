#pragma once

#include "ofMain.h"

#define OBJECT_RESOLUTION 200

class twomblyObject {
public:
    //Functions
    twomblyObject();
    void setup(ofPoint _position, float _rotation, ofPolyline &_objectForm);
    void draw();
    
    //Variables
    ofFbo drawFbo;
    ofColor color;
    ofPoint position;
    float rotation;
    int boundingWidth;
    int boundingHeight;
};