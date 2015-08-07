#include "twomblyObject.h"

twomblyObject::twomblyObject() {
    
}

void twomblyObject::setup(ofPoint _position, float _rotation, ofPolyline &_objectForm) {
    //Load the color
    color.set(25, 25, 25, 140);
    
    //General parameters
    position = _position;
    rotation = _rotation;
    
    //Calculate the size of FBO needed
    boundingWidth = OBJECT_RESOLUTION;
    boundingHeight = OBJECT_RESOLUTION;
    
    //Allocate and clear the FBO
    drawFbo.allocate(boundingWidth, boundingHeight, GL_RGBA, 8);
    drawFbo.begin();
    ofClear(0, 0);
    drawFbo.end();
    
    for (int i = 0; i < _objectForm.size(); i++) {
        if(ofRandom(1.0) > 0.95) {
            _objectForm[i].x += ofRandom(-3.0, 3.0);
            _objectForm[i].y += ofRandom(-3.0, 3.0);
        }
    }
    
    //Draw the object into the FBO
    drawFbo.begin();
    
    float scale = ofRandom(0.3, 1.0);
    ofSetColor(color);
    ofPushMatrix();
    ofScale(scale, scale);
    _objectForm.draw();
    ofPopMatrix();
    
    drawFbo.end();
}



void twomblyObject::draw() {
    //Draw the FBO at the correct position
    ofPushMatrix();

    ofTranslate(position);
    ofRotateZ(rotation);
    ofSetColor(ofColor::white);
    drawFbo.draw(0, 0);
    
    ofPopMatrix();
}
