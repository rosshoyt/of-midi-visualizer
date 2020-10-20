#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() : abletonController(), midiPortState(16, false), noteGridAnimation(&midiPortState, "2D Note Grid"), animated3DMesh(&midiPortState, "3D Mesh"), meshFromImage(&midiPortState, "Mesh From Image"), texturedSphere(&midiPortState, "Textured Sphere") {}

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofSetWindowTitle("OpenFrameworks MIDI Visualizer - Ross Hoyt Music");
    ofEnableAntiAliasing();

    animationComponents.insert({ noteGridAnimation.getUID(), &noteGridAnimation });
    animationComponents.insert({ animated3DMesh.getUID(), &animated3DMesh });
    animationComponents.insert({ meshFromImage.getUID(), &meshFromImage });
    //animationComponents.insert({ texturedSphere.getUID(), &texturedSphere });

    // list to tactrack the animationUIDS to add to dropdown menu
    
    
    // setup animation components
    for (auto& pair : animationComponents) {
        animationComponents[pair.first]->setup();
        // track UIDS of animations to add to the animationSelectorDropdown
        animationUIDS.push_back(pair.first);
    }

    //// instantiate the animation selector dropdown and set position
    animationSelectorDropdown = new ofxDatGuiDropdown("SELECT AN ANIMATION", animationUIDS);
    animationSelectorDropdown->setPosition(WIDTH, 0);
    //// register to listen for change events
    animationSelectorDropdown->onDropdownEvent(this, &ofApp::onDropdownEvent);
    animationSelectorDropdown->expand();

    // set animation ID to first animation entry in map (TODO refactor, currently must be at least 1 animation 
    currentAnimationUID = animationComponents.begin()->first;


    // Set ofxGUI menu position for note grid animation
    noteGridAnimation.setMenuXY(WIDTH, HEIGHT / 2);

    // initialize network MIDI port
    midiPortState.setupMIDIPort();
}

//--------------------------------------------------------------
void ofApp::update(){
    animationComponents[currentAnimationUID]->update();
    animationSelectorDropdown->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    animationComponents[currentAnimationUID]->draw();
    animationSelectorDropdown->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch (key) {
    case ' ':
        std::cout << "Pressed Space\n";
        abletonController.processSpacebar();
    }

    animated3DMesh.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    noteGridAnimation.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e){
    currentAnimationUID = animationUIDS[e.child];
    animationSelectorDropdown->expand();
}
