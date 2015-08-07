#pragma once

#include "ofMain.h"

#define SAMPLE_RESOLTION 30
#define SAMPLE_ALPHA 220//was 170
#define MIN_SAMPLE_DIFFERENCE 30

class twomblyPaletteGenerator {
public:
    //Functions
    twomblyPaletteGenerator();
    void setup(string _fileName);
    void draw();
    void refinePalette();
    vector<ofColor>& getPalette();
    
    //Variables
    vector<ofColor> palette;
};