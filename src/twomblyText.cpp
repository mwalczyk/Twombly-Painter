#include "twomblyText.h"

twomblyText::twomblyText() {
    
}

void twomblyText::setup(string _text, ofPoint _position, float _rotation, vector<ofPolyline> &_letterForms) {
    color.set(25, 25, 25, 140);
   
    //General parameters
    text = _text;
    position = _position;
    rotation = _rotation;
    
    //Calculate the size of FBO needed to draw the text
    boundingWidth = 100 * text.size();
    boundingHeight = 100;
    
    //Allocate and clear the FBO
    drawFbo.allocate(boundingWidth, boundingHeight, GL_RGBA, 8);
    drawFbo.begin();
    ofClear(0, 0);
    drawFbo.end();
    
    //Draw the text into the FBO
    drawFbo.begin();
    ofSetColor(color);
    
    //Convert each character of the text to indices [0-25]
    char int_to_char[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    int xOffset = 20;
    int spaceOffset = 0;
    bool foundSpace = false;
    for(int i = 0; i < text.size(); i++) {
        //Convert the character to lower case
        char c = tolower(text[i]);
        
        //Check if the character is a space
        if (c == ' ') {
            foundSpace = true;
            continue;
        }
        
        //Loop through the array of indexed characters above to find a numerical value [0-25] corresponding to the current character
        for (int j = 0; j < sizeof(int_to_char); j++) {
            if (int_to_char[j] == c) {
                //Get the corresponding letter form and draw it into the FBO
                ofPushMatrix();
                
                    //If the previous character was a space, add an additional x-offset to this polyline
                    if (foundSpace) {
                        spaceOffset += xOffset / 2;
                        ofTranslate(xOffset*i + spaceOffset, 0);
                        foundSpace = false;
                    }
                    else {
                        ofTranslate(xOffset*i + spaceOffset, 0);
                        ofRotateZ(ofRandom(-5, 5));
                    }
                    
                    _letterForms[j].draw();
                    
                    //Randomly draw duplicates of the original letterform
                    if (ofRandom(1.0) > 0.9) {
                        ofPolyline duplicate = _letterForms[j].getSmoothed(10.0);
                        duplicate.draw();
                    }
                
                ofPopMatrix();
            }
        }
    }

    drawFbo.end();
}

void twomblyText::draw() {
    //Draw the FBO at the correct position
    ofPushMatrix();
        ofRotateZ(rotation);
        ofTranslate(position);
        ofSetColor(ofColor::white);
        drawFbo.draw(0, 0);
    ofPopMatrix();
}
