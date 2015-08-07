#include "ofApp.h"

void ofApp::addPaintSplatter(int x, int y) {
    //The epicenter of the paint splatter
    ofPoint splatterCenter(x + ofRandom(MAX_PAINT_VARIATION), y + ofRandom(MAX_PAINT_VARIATION));
    
    //The number of paint blobs to add
    int numSplatters = (int) ofRandom(MAX_PAINT_SPLATTERS);
    
    //Generate the paint blobs
    for(int i = 0; i < numSplatters; i++) {
        float r = ofRandom(10);
        
        //Create a new paint blob that will be a part of this paint splatter
        paintBlob pb;
        pb.color = canvasColor;
        pb.center = ofPoint(splatterCenter.x + ofRandom(-4, 4), splatterCenter.y + ofRandom(-4, 4));
        pb.radius = r;
        paintBlobs.push_back(pb);
    }
}

void ofApp::addPaintStroke(int x, int y, int radius, int variation) {
    //The radius and position of this paint blob are dependent on the mouse velocity
    float r = radius;

    //Create a new paint blob from the above parameters
    paintBlob pb;
    pb.color = paintColor;
    pb.center = ofPoint(x + ofRandom(-variation, variation), y + ofRandom(-variation, variation));
    pb.radius = r;
    paintBlobs.push_back(pb);
}

void ofApp::addScratch(int x, int y) {
    //Define the parameters
    ofPoint center(x + ofRandom(MAX_SCRATCH_VARIATION), y + ofRandom(MAX_SCRATCH_VARIATION));
    float alpha = ofRandom(140);
    ofColor color(pencilColor, alpha);
    float angle = ofDegToRad(ofRandom(360));
    
    //Create a new scratch from the above parameters
    scratch s;
    s.center = center;
    s.color = color;
    s.angle = angle;
    
    //The number of lines that make up this scratch mark
    int numScratchLines = (int)ofRandom(MIN_SCRATCH_LINES, MAX_SCRATCH_LINES);
    
    //Generate the polylines
    for (int i = 0; i < numScratchLines; i++) {
        ofPoint centerOffset(center.x + ofRandom(20), center.y + ofRandom(20));
        
        //Create a new scratch line that will be a part of this mark
        ofPolyline scratchLine;
        int numPoints = SCRATCH_RESOLUTION;
        float length = ofRandom(MAX_SCRATCH_LENGTH);
        float distAlongLength = 0.0;
        float step = length / numPoints;
        
        //Walk along the angled line, generating new points with slightly random offsets
        for (int j = 0; j < numPoints; j++) {
            float scratchX = distAlongLength * cos(angle) + centerOffset.x + ofRandom(1.0);
            float scratchY = distAlongLength * sin(angle) + centerOffset.y + ofRandom(1.0);
            distAlongLength += step;
            scratchLine.addVertex(scratchX, scratchY);
        }
        s.scratchLines.push_back(scratchLine);
    }
    scratches.push_back(s);
}

void ofApp::addTextSnippet(int x, int y) {
    twomblyText text;
    string randWord = twomblyDictionary[(int)ofRandom(twomblyDictionary.size())];
    text.setup(randWord, ofPoint(x, y), 0, letterForms);
    textSnippets.push_back(text);
}

void ofApp::addObjectForm(int x, int y) {
    int index = (int)ofRandom(objectForms.size());
    twomblyObject to;
    to.setup(ofPoint(x, y), ofRandom(-10, 10), objectForms[index]);
    twomblyObjects.push_back(to);
}

void ofApp::clearAll() {
    //-------------------------------------------------------------- CLEAR ALL FBOS
    pencilFbo.begin();
    ofClear(255, 255, 255, 0);
    pencilFbo.end();
    
    paintFbo.begin();
    ofClear(255, 255, 255, 0);
    paintFbo.end();
    
    paintFboRock.begin();
    ofClear(255, 255, 255, 0);
    paintFboRock.end();
    
    paintFboNoise.begin();
    ofClear(255, 255, 255, 0);
    paintFboNoise.end();
    
    //-------------------------------------------------------------- CLEAR ALL VECTORS
    scratches.clear();
    paintBlobs.clear();
    pencilLines.clear();
    textSnippets.clear();
    twomblyObjects.clear();
}

void ofApp::generateWords() {
    //Read in the text file
    ofBuffer buffer = ofBufferFromFile("Herodiade.txt");
    
    //Check to see if anything was actually loaded
    if(buffer.size()) {
        
        //Get each line of text and find words to add to the dictionary of available words
        while(buffer.isLastLine() == false) {
            string line = buffer.getNextLine();
            if(line.empty() == false) {
                vector<string> words = ofSplitString(line, " ");
                for(int i = 0; i < words.size(); i++) {
                    int numBackwards = (int)ofRandom(ofClamp(i, 0, 4));
                    if (words[i].size() > MIN_WORD_LENGTH) {
                        string working = words[i];
                        for (int j = 1; j < numBackwards; j++) {
                            working += " " + words[i-j];
                        }
                        twomblyDictionary.push_back(working);
                        cout << "Added \"" + working << "\" to the dictionary." << endl;
                    }
                }
            }
        }
        
    }
}

void ofApp::generateLetterForms() {
    //A vector of polylines that will store all of the loaded letter forms
    vector<ofPolyline> lines;
    
    //Load the XML file
    ofxXmlSettings paths;
    if(paths.loadFile("letters.xml")){
        paths.pushTag("paths");
        
        //Get the number of XML elements labeled "path" under "paths"
        int numberOfSavedPaths = paths.getNumTags("path");
        for(int i = 0; i < numberOfSavedPaths; i++) {
            paths.pushTag("path", i);
            
            //Create a new polyline from this XML element
            ofPolyline pl;
            
            //Get the number of XML elements labeled "position" under this "path"
            int numberOfSavedPoints = paths.getNumTags("position");
            for(int j = 0; j < numberOfSavedPoints; j++) {
                paths.pushTag("position", j);
                
                //Create a new point from this XML element
                ofPoint p;
                p.x = paths.getValue("X", 0.0); //A double
                p.y = paths.getValue("Y", 0.0); //A double
                
                //Add this point to the current polyline
                pl.addVertex(p);
                
                paths.popTag();
            }
            paths.popTag();
            
            //Add this polyline to the vector
            lines.push_back(pl);
        }
        
        //Set this vector of polylines to a global variable
        letterForms = lines;
        cout << "Loaded " + ofToString(letterForms.size()) << " letter forms." << endl;
    }
}

void ofApp::generateObjects() {
    //A vector of polylines that will store all of the loaded letter forms
    vector<ofPolyline> lines;
    
    //Load the XML file
    ofxXmlSettings paths;
    if(paths.loadFile("objects.xml")){
        paths.pushTag("paths");
        
        //Get the number of XML elements labeled "path" under "paths"
        int numberOfSavedPaths = paths.getNumTags("path");
        for(int i = 0; i < numberOfSavedPaths; i++) {
            paths.pushTag("path", i);
            
            //Create a new polyline from this XML element
            ofPolyline pl;
            
            //Get the number of XML elements labeled "position" under this "path"
            int numberOfSavedPoints = paths.getNumTags("position");
            for(int j = 0; j < numberOfSavedPoints; j++) {
                paths.pushTag("position", j);
                
                //Create a new point from this XML element
                ofPoint p;
                p.x = paths.getValue("X", 0.0); //A double
                p.y = paths.getValue("Y", 0.0); //A double
                
                //Add this point to the current polyline
                pl.addVertex(p);
                
                paths.popTag();
            }
            paths.popTag();
            
            //Add this polyline to the vector
            lines.push_back(pl);
        }
        
        //Set this vector of polylines to a global variable
        objectForms = lines;
        cout << "Loaded " + ofToString(objectForms.size()) << " object forms." << endl;
    }
}

string ofApp::boolToOffOn(bool _b) {
    if (_b) {
        return "ON";
    }
    else {
        return "OFF";
    }
}

ofColor ofApp::boolToColor(bool _b) {
    if (_b) {
        return pencilColor;
    }
    else {
        return ofColor::red;
    }
}

//--------------------------------------------------------------
void ofApp::setup(){
    //Turn on transparency
    ofEnableAlphaBlending();
    
    //Generate a list of words
    generateWords();
    
    //Generate a list of letter forms (polylines)
    generateLetterForms();
    generateObjects();
    testLetterForm = 0;
    testObjectForm = 0;
    
    //Initialize colors
    canvasColor.set(249, 242, 223);
    pencilColor.set(25, 25, 25, 140);
    
#if PALETTE == 1
    //Custom palette 1
    canvasColor.set(255, 244, 232);
    paintColors.push_back(ofColor(255, 207, 185, 80));
    paintColors.push_back(ofColor(255, 204, 120, 170));
    paintColors.push_back(ofColor(237, 99, 63, 170));
    paintColors.push_back(ofColor(152, 146, 149, 170));
    paintColors.push_back(ofColor(199, 146, 157, 170));
    paintColors.push_back(ofColor(186, 86, 58, 60));
    paintColors.push_back(ofColor(13, 140, 204, 60));
    paintColors.push_back(ofColor(237, 204, 195, 100));
    paintColors.push_back(ofColor(16, 1, 1, 160));
#elif PALETTE == 2
    //Custom palette 2
    paintColors.push_back(ofColor(156, 205, 201, 170));
    paintColors.push_back(ofColor(248, 177, 113, 170));
    paintColors.push_back(ofColor(233, 158, 137, 170));
    paintColors.push_back(ofColor(214, 115, 109, 170));
    paintColors.push_back(ofColor(12, 14, 26, 40));
    paintColors.push_back(ofColor(243, 220, 189, 170));
    paintColors.push_back(ofColor(192, 49, 55, 170));
    paintColors.push_back(ofColor(84, 110, 81, 40));
#elif PALETTE == 3
    //Procedurally generated palette
    paletteGenerator.setup("reference.jpg");
    paintColors = paletteGenerator.getPalette();
#endif
    paintColor = paintColors[0]; 
    
    //Pencil variables
    pencilFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, 8); //THE PENCIL FBO
    pencilFbo.begin();
    ofClear(255, 255, 255, 0);
    pencilFbo.end();
    
    ofSetLineWidth(10);
    drawSecondary = false;
    
    //Paint variables
    paintFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, 4); //THE MAIN FBO
    paintFbo.begin();
    ofClear(255, 255, 255, 0);
    paintFbo.end();
    
    noiseFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, 4); //THE COPY FBO
    noiseFbo.begin();
    ofClear(255, 255, 255, 0);
    noiseFbo.end();
    
    paintFboRock.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, 4); //THE ANIMATED FBO
    paintFboRock.begin();
    ofClear(255, 255, 255, 0);
    paintFboRock.end();
    
    paintFboNoise.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, 4); //THE ANIMATED FBO
    paintFboNoise.begin();
    ofClear(255, 255, 255, 0);
    paintFboNoise.end();
    
    paintStill.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, 4); //THE ANIMATED FBO
    paintStill.begin();
    ofClear(255, 255, 255, 0);
    paintStill.end();
    
    //Shaders
    displacementShader.load("displacement.vert", "displacement.frag");
    noiseShader.load("displacement.vert", "noise.frag");
    
    //Textures
    ofSetTextureWrap(GL_REPEAT);
    displacer.loadImage("displacement.jpg");
    brush.loadImage("brush.png");
    
    ofBackground(canvasColor);
    ofSetVerticalSync(false);
    
    //Control
    toggleScratches = true;
    togglePaint = true;
    toggleText = true;
    toggleCurling = true;
    toggleObjects = true;
}

//--------------------------------------------------------------
void ofApp::update(){
//    if (ofGetElapsedTimef() > 5) {
//        system("open /Users/MichaelWalczyk/Documents/openFrameworks/apps/myApps/drawingTool_05/bin/drawingTool_03.app &");
//        ofExit();
//    }
}

//--------------------------------------------------------------
void ofApp::draw(){
#ifdef DEBUG
    ofSetWindowTitle("" + ofToString(ofGetFrameRate()));
#else
    ofSetWindowTitle("Twombly Generator");
#endif
    
    
    
    
    //-------------------------------------------------------------- RENDER THE NOISE INTO AN FBO
    noiseFbo.begin();
    ofClear(255, 255, 255, 0);
    ofSetColor(255, 255, 255);
    noiseShader.begin();
    noiseShader.setUniform1f("time", ofGetElapsedTimef());
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    noiseShader.end();
    noiseFbo.end();
    //--------------------------------------------------------------
    
    
    
    
    //-------------------------------------------------------------- DRAW THE PAINT INTO AN FBO
    paintFbo.begin();
    ofClear(255, 255, 255, 0);
    
//    ofSetColor(ofColor::white); //Feedback
//    paintFboNoise.draw(0, 0);
    
//    ofSetColor(ofColor::white);
//    paintStill.draw(0, 0);
    
    //Draw all of the paint blobs
    for(int i = 0; i < paintBlobs.size(); i++) {
        ofSetColor(paintBlobs[i].color, 60);
        brush.draw(paintBlobs[i].center - ofPoint(paintBlobs[i].radius * 2, paintBlobs[i].radius * 2), paintBlobs[i].radius * 4, paintBlobs[i].radius * 4);
        
        ofSetColor(paintBlobs[i].color);
        ofCircle(paintBlobs[i].center, paintBlobs[i].radius);
    }
    
//    if (paintBlobs.size() > 100) {
//        paintBlobs.pop_front();
//    }

    paintFbo.end();
    //--------------------------------------------------------------
    
//    if (ofGetFrameNum()% 30 == 0) {
//        paintStill.begin();
//        ofSetColor(canvasColor);
//        paintFbo.draw(0, 0);
//        paintStill.end();
//        
//        paintBlobs.clear();
//    }
 
    
    
    //-------------------------------------------------------------- DRAW THE PENCIL INTO AN FBO
    pencilFbo.begin();

    ofSetColor(pencilColor, 100);
    if(ofGetMousePressed()) {
        
        //Secondary stroke
        if(drawSecondary) {
            for(int i = 0; i < secondaryLines.size(); i++) {
                ofPolyline secondaryLineSmoothed = secondaryLine.getSmoothed(PENCIL_SMOOTHING);
                secondaryLineSmoothed.draw();
            }
        }
        
        //Main stroke
        ofPolyline currentPencilLineSmoothed = currentPencilLine.getSmoothed(PENCIL_SMOOTHING);
        currentPencilLineSmoothed.draw();
    }
    
    //Draw all of the pencil lines
    for(int i = 0; i < pencilLines.size(); i++) {
        pencilLines[i].draw();
    }
    
    //Draw all of the scratches
    for(int i = 0; i < scratches.size(); i++) {
        ofSetColor(scratches[i].color);
        for(int j = 0; j < scratches[i].scratchLines.size(); j++) {
            scratches[i].scratchLines[j].draw();
        }
    }
    
    //Since the lines and scratches have been drawn into the FBO, clear the vectors
    pencilLines.clear();
    scratches.clear();
    
    pencilFbo.end();
    //--------------------------------------------------------------

    

    
    //-------------------------------------------------------------- DISPLACE THE PAINT WITH THE ROCK TEXTURE
    paintFboRock.begin();
    displacementShader.begin();
    
    displacementShader.setUniform1f("displacementAmountX", 60.0); //was 30 and 10
    displacementShader.setUniform1f("displacementAmountY", 20.0);
    displacementShader.setUniformTexture("tex0",paintFbo, 0);
    displacementShader.setUniformTexture("tex1",displacer.getTextureReference(), 1);
    paintFbo.draw(0, 0);
    
    displacementShader.end();
    paintFboRock.end();
    //-------------------------------------------------------------- 

    
    
    
    
    //-------------------------------------------------------------- DISPLACE THE PAINT WITH THE NOISE TEXTURE
    paintFboNoise.begin();
    displacementShader.begin();
    
    displacementShader.setUniform1f("displacementAmountX", 1.0);
    displacementShader.setUniform1f("displacementAmountY", 1.0);
    displacementShader.setUniformTexture("tex0",paintFboRock, 0);
    displacementShader.setUniformTexture("tex1",noiseFbo.getTextureReference(), 1);
    ofSetColor(255, 255, 255);
    paintFboRock.draw(0, 0);
    
    displacementShader.end();
    paintFboNoise.end();
    //--------------------------------------------------------------
    
    
    
    //Draw the final FBO
    ofSetColor(ofColor::white);
    paintFboNoise.draw(0, 0);
    
    

    
    //-------------------------------------------------------------- DISPLACE THE PENCIL WITH THE ROCK TEXTURE
    displacementShader.begin();
    
    displacementShader.setUniform1f("displacementAmountX", 1.0);
    displacementShader.setUniform1f("displacementAmountY", 1.5);
    displacementShader.setUniformTexture("tex0",pencilFbo, 0);
    displacementShader.setUniformTexture("tex1",displacer.getTextureReference(), 1);
    ofSetColor(0,0,0);
    pencilFbo.draw(0, 0);

    displacementShader.end();
    //--------------------------------------------------------------
    
    
    //Draw the text and objects
    for(int i = 0; i < textSnippets.size(); i++) {
        textSnippets[i].draw();
    }
    for(int i = 0; i < twomblyObjects.size(); i++) {
        twomblyObjects[i].draw();
    }
    

    
    
#ifdef DEBUG
    letterForms[testLetterForm].draw();
    objectForms[testObjectForm].draw();
    paletteGenerator.draw();
#endif

    

    
    if (!savingScreen) {
        ofSetColor(pencilColor);
        ofDrawBitmapString("Press 'C' to clear the canvas", ofPoint(20, 20));
        ofDrawBitmapString("Press 'S' to save and print a screenshot", ofPoint(20, 40));
        
        ofSetColor(boolToColor(toggleScratches));
        ofDrawBitmapString("Press '1' to toggle pencil scratches. Currently: " + boolToOffOn(toggleScratches), ofPoint(20, 60));
        
        ofSetColor(boolToColor(togglePaint));
        ofDrawBitmapString("Press '2' to toggle paint strokes. Currently: " + boolToOffOn(togglePaint), ofPoint(20, 80));
        
        ofSetColor(boolToColor(toggleText));
        ofDrawBitmapString("Press '3' to toggle text generation. Currently: " + boolToOffOn(toggleText), ofPoint(20, 100));
        
        ofSetColor(boolToColor(toggleCurling));
        ofDrawBitmapString("Press '4' to toggle pencil curling. Currently: " + boolToOffOn(toggleCurling), ofPoint(20, 120));
        
        ofSetColor(boolToColor(toggleObjects));
        ofDrawBitmapString("Press '5' to toggle Twombly-esque forms. Currently: " + boolToOffOn(toggleObjects), ofPoint(20, 140));
    
    }
    else {
        //Save a screenshot and send it to the printer
        ofSaveScreen("print.jpg");
        system("lp -d HP_Photosmart_D110_series /Users/MichaelWalczyk/Documents/openFrameworks/apps/myApps/drawingTool_05/bin/data/print.jpg");
        savingScreen = false;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 's' || key == 'S') {
        savingScreen = true;
    }
    if (key == 'c' || key == 'C') {
        clearAll();
    }
    if (key == '1') {
        toggleScratches = !toggleScratches;
    }
    if (key == '2') {
        togglePaint = !togglePaint;
    }
    if (key == '3') {
        toggleText = !toggleText;
    }
    if (key == '4') {
        toggleCurling = !toggleCurling;
    }
    if (key == '5') {
        toggleObjects = !toggleObjects;
    }
    if (key == OF_KEY_UP) {
        testLetterForm++;
        if (testLetterForm > letterForms.size()-1) {
            testLetterForm = letterForms.size()-1;
        }
    }
    if (key == OF_KEY_DOWN) {
        testLetterForm--;
        if (testLetterForm < 0) {
            testLetterForm = 0;
        }
    }
    if (key == OF_KEY_RIGHT) {
        testObjectForm++;
        if (testObjectForm > objectForms.size()-1) {
            testObjectForm = objectForms.size()-1;
        }
    }
    if (key == OF_KEY_LEFT) {
        testObjectForm--;
        if (testObjectForm < 0) {
            testObjectForm = 0;
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    //The difference between the previous mouse position and the current mouse position
    float mouseVelocity = ofDist(ofGetPreviousMouseX(), ofGetPreviousMouseY(), x, y);
    
    //Secondary stroke
    if (drawSecondary) {
        for(int i = 0; i < secondaryLines.size(); i++) {
            if(ofRandom(1.0) > 0.5) {
                //Draw with a random offset
                secondaryLines[i].addVertex(x + ofRandom(MAX_PENCIL_VARIATION), y + ofRandom(MAX_PENCIL_VARIATION));
            }
            else {
                //Draw normally
                secondaryLines[i].addVertex(x, y);
            }
        }
    }
    
    //Main stroke
    if(ofRandom(1.0) > 0.6) {
        //Draw with a random offset
        currentPencilLine.addVertex(x + ofRandom(MAX_PENCIL_VARIATION), y + ofRandom(MAX_PENCIL_VARIATION));
    }
    float pctCurly = ofMap(mouseVelocity, 0, 50, 0.95, 0.2, true);
    if(ofRandom(1.0) > pctCurly && toggleCurling) {
        //Draw a curly line
        
        //First, pick a center point for the curl
        ofPoint squigglyCenter(x + ofRandom(MAX_PENCIL_VARIATION * 2), y + ofRandom(MAX_PENCIL_VARIATION * 2));
        
        //Calculate the radius of the curl
        float r = ofDist(x, y, squigglyCenter.x, squigglyCenter.y);
        
        //Determine a circle resolution for the curl
        int resolution = (int)ofRandom(MAX_SQUIGGLY_RESOLUTION);
        
        //From the resolution, calculate an angle step
        float angleOffset = ofDegToRad(360.0 / resolution);
        
        //Construct the curl and add it to the current polyline
        for(int i = 0; i < resolution; i++) {
            float squigglyX = r * cos(angleOffset * i) + x + ofRandom(MAX_PENCIL_VARIATION);
            float squigglyY = r * sin(angleOffset * i) + y + ofRandom(MAX_PENCIL_VARIATION);
            currentPencilLine.addVertex(squigglyX, squigglyY);
        }
    }
    else {
        //Draw normally
        currentPencilLine.addVertex(x, y);
    }
    
    //Paint stroke
    float pctRadius = ofMap(mouseVelocity, 0, 50, 5, 50, true);
    float pctVariation = ofMap(mouseVelocity, 0, 50, 5, MAX_PAINT_VARIATION, true);
    if (ofRandom(1.0) > 0.6 && togglePaint) {
        addPaintStroke(x, y, pctRadius, pctVariation);
    }
    
    //Paint splatters
    if (ofRandom(1.0) > 0.98 && togglePaint) {
        addPaintSplatter(x, y);
    }
    
    //Scratches
    if (ofRandom(1.0) > 0.98 && toggleScratches) {
        addScratch(x, y);
    }
    
    //Text
    if (ofRandom(1.0) > 0.996 && toggleText) { 
        addTextSnippet(x, y);
    }
    
    //Objects
    if (ofRandom(1.0) > 0.996 && toggleObjects) {
        addObjectForm(x, y);
    }
    
    //Save the mouse's position
    lastMousePosition.set(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //Secondary stroke
    if (ofRandom(1.0) > 0.5) {
        drawSecondary = true;
        
        //The number of secondary pencil strokes that will follow the main stroke
        int numLines = (int)ofRandom(MAX_SECONDARY_LINES);
        
        //Create a bunch of new polylines
        for(int i = 0; i < numLines; i++) {
            ofPolyline newSecondaryLine;
            secondaryLine = newSecondaryLine;
            secondaryLine.addVertex(x + ofRandom(MAX_PENCIL_VARIATION), y + ofRandom(MAX_PENCIL_VARIATION));
            secondaryLines.push_back(secondaryLine);
        }
    }
    else {
        drawSecondary = false;
    }
    
    //Main stroke
    currentPencilLine.clear();
    currentPencilLine.addVertex(x, y);
    
    //Choose a new paint color
    paintColor = paintColors[(int) ofRandom(paintColors.size())];
    
    lastMousePosition.set(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    //Secondary stroke
    if(drawSecondary) {
        for(int i = 0; i < secondaryLines.size(); i++) {
            secondaryLines[i].addVertex(x + ofRandom(MAX_PENCIL_VARIATION), y + ofRandom(MAX_PENCIL_VARIATION));
            pencilLines.push_back(secondaryLines[i].getSmoothed(PENCIL_SMOOTHING));
            drawSecondary = false;
        }
        secondaryLines.clear();
    }
    
    //Main stroke
    currentPencilLine.addVertex(x, y);
    pencilLines.push_back(currentPencilLine.getSmoothed(PENCIL_SMOOTHING));
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
