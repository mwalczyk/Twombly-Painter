#include "twomblyPaletteGenerator.h"

twomblyPaletteGenerator::twomblyPaletteGenerator() {
    
}

void twomblyPaletteGenerator::setup(string _fileName) {
    ofImage source;
    source.loadImage(_fileName);
    int imgWidth = source.getWidth();
    int imgHeight = source.getHeight();
    for (int x = 0; x < imgWidth; x += SAMPLE_RESOLTION) {
        for (int y = 0; y < imgHeight; y += SAMPLE_RESOLTION) {
            ofColor sampleColor = source.getPixelsRef().getColor(x, y); //Make this smarter
            palette.push_back(ofColor(sampleColor, SAMPLE_ALPHA));
        }
    }
    refinePalette();
}

//Fix this spacing issue for better debugging
void twomblyPaletteGenerator::draw() {
    int numPerRow = 10;
    int rowNum = 1;
    for (int i = 0; i < palette.size(); i++) {
        ofSetColor(palette[i]);
        ofRect((i % numPerRow) * 20, rowNum * 20, 20, 20);
        if (i % numPerRow == 0) {
            rowNum++;
        }
    }
}

//Make this smarter
void twomblyPaletteGenerator::refinePalette() {
    for (int i = 0; i < palette.size(); i++) {
        ofColor currentColor = palette[i];
        for (int j = i + 1; j < palette.size() - 1; j++) {
            float difference = 0;
            difference += abs(currentColor.r - palette[j].r);
            difference += abs(currentColor.g - palette[j].g);
            difference += abs(currentColor.b - palette[j].b);
            if (difference < MIN_SAMPLE_DIFFERENCE) {
                palette.erase(palette.begin() + j);
            }
        }
    }
}

vector<ofColor>& twomblyPaletteGenerator::getPalette() {
    return palette;
}