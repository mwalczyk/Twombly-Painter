#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "twomblyText.h"
#include "twomblyObject.h"
#include "twomblyPaletteGenerator.h"

//#define DEBUG
#define PALETTE 2

#define MAX_SECONDARY_LINES 6
#define MAX_PENCIL_VARIATION 10
#define MAX_SQUIGGLY_RESOLUTION 40
#define PENCIL_SMOOTHING 2

#define MIN_SCRATCH_LINES 10
#define MAX_SCRATCH_LINES 40
#define MAX_SCRATCH_LENGTH 60
#define MAX_SCRATCH_VARIATION 20
#define SCRATCH_RESOLUTION 10

#define MAX_PAINT_VARIATION 50
#define MAX_PAINT_SPLATTERS 20

#define MIN_WORD_LENGTH 4

struct paintBlob {
    ofColor color;
    ofPoint center;
    float radius;
};

struct scratch {
    ofColor color;
    ofPoint center;
    float angle;
    vector<ofPolyline> scratchLines;
};

class ofApp : public ofBaseApp {
    
public:
    //Default functions
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    //Custom functions
    void addPaintSplatter(int x, int y);
    void addPaintStroke(int x, int y, int radius, int variation);
    void addScratch(int x, int y);
    void addTextSnippet(int x, int y);
    void addObjectForm(int x, int y);
    void clearAll();
    void generateWords();
    void generateLetterForms();
    void generateObjects();
    string boolToOffOn(bool _b);
    ofColor boolToColor(bool _b);

    //Colors
    ofColor             canvasColor;
    ofColor             pencilColor;
    ofColor             paintColor;
    vector<ofColor>     paintColors;
    twomblyPaletteGenerator paletteGenerator;
    
    //Drawing objects
    vector<twomblyObject> twomblyObjects;
    vector<ofPolyline>  objectForms;
    int                 testObjectForm;
    
    //Text
    vector<twomblyText> textSnippets;
    vector<string>      twomblyDictionary;
    vector<ofPolyline>  letterForms;
    int                 testLetterForm;
    
    //Pencil
    ofPolyline          currentPencilLine;
    ofPolyline          secondaryLine;
    vector<ofPolyline>  secondaryLines;
    vector<ofPolyline>  pencilLines;
    ofFbo               pencilFbo;
    bool                drawSecondary;
    
    //Scratches
    vector<scratch>     scratches;
    
    //Paint
    deque<paintBlob>   paintBlobs;
    ofImage             brush;
    ofPoint             lastMousePosition;
    ofFbo               paintFbo;
    ofFbo               paintFboRock;
    ofFbo               paintFboNoise;
    ofFbo               paintStill;
    ofFbo               noiseFbo;
    
    //Shaders
    ofShader            displacementShader;
    ofShader            noiseShader;
    
    //Textures
    ofImage             displacer;
    ofImage             paperTexture;
    
    //Control
    bool toggleScratches;
    bool togglePaint;
    bool toggleText;
    bool toggleCurling;
    bool toggleObjects;
    bool savingScreen;
};
