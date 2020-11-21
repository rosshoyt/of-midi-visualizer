#include "CircleOfFifths.h"

//--------------------------------------------------------------
CircleOfFifths::CircleOfFifths(std::string uid) : MIDIAnimationComponent(uid) {
	
}

//--------------------------------------------------------------
void CircleOfFifths::setup() {}

//--------------------------------------------------------------
void CircleOfFifths::setupGUI() {
	gui.add(drawOctavesToggle.set("Draw Octaves", true));
	gui.add(drawChromaticModeToggle.set("Draw Chromatic Circle", false));
	gui.add(noteCircleSizeSlider.set("Note Circle Size", 1.0, .001, 10));
	gui.add(octaveSizeMultiplierSlider.set("Octave Size Multiplier", 1, 0.1, 10));
	gui.add(noteMultiplierToggle.set("Note Multiplier ", 7, 1, 12));
	gui.add(drawHaloOnNotesToggle.set("Draw Halo on Notes", false));
	gui.add(noteColorSlider.setup("Octave Row Color", ofColor(86, 0, 200, 88), ofColor(0, 0), ofColor(255, 255)));
	
}

//--------------------------------------------------------------
void CircleOfFifths::update() {}

//--------------------------------------------------------------
void CircleOfFifths::draw() {

	// TODO add 'snap to cursor' control so that Circle of Fifths displays around cursor?
	//ofVec2f centerPos(ofGetMouseX(), ofGetMouseY());
	ofVec2f centerPos(animationWidth / 2.f, animationHeight / 2.f);
	float smallestDim = animationWidth > animationHeight ? animationHeight : animationWidth;

	for (auto channel : midiPortState->getAllChannelNotes()) {
		for (auto noteVel : channel) {
			auto octavePitch = utils::midi::getOctavePitchPair(noteVel.first);
			
			int pitchPosition = octavePitch.second;

			if(!drawChromaticModeToggle) {
				// convert chromatic pitch to circle of fifths
				pitchPosition = pitchPosition * noteMultiplierToggle % numPitches;
			}

			float circleSize = 40 * noteVel.second / 127.f * noteCircleSizeSlider;

			float rads = 2 * utils::math::pi * pitchPosition / numPitches; // The rotate function uses degrees!
			
			// TODO base this on smaller of height or width
			float radius = animationHeight / 2.f / numOctaves * octaveSizeMultiplierSlider;
			
			if (drawOctavesToggle) radius *= octavePitch.first + 1;
			else radius *= numOctaves / 2;
			
			auto point = centerPos + ofVec2f(radius * std::cos(rads), radius * std::sin(rads));

			ofColor aqua(0, 252, 255, alpha);
			/*ofColor purple(198, 0, 205, alpha);
			ofColor inbetween = aqua.getLerped(purple, float(octavePitch.second) / 12.f);*/

			ofSetColor(noteColorSlider);

			if (drawHaloOnNotesToggle) {
				drawHaloAroundPoint(point);
			} else {
				ofDrawCircle(point, circleSize);
			}


			
		}
	}

	
}

//--------------------------------------------------------------
void CircleOfFifths::drawHaloAroundPoint(ofVec2f point) {
	// Triangle Brush Source: https://github.com/openframeworks/ofBook/blob/master/chapters/intro_to_graphics/code/1_ii_e_Triangle_Brush/src/ofApp.cpp
	// Code for the final version of the brush

	int numTriangles = 10;
	int minOffset = 5;
	int maxOffset = 70;
	int alpha = 150;

	for (int t = 0; t < numTriangles; ++t) {
		float offsetDistance = ofRandom(minOffset, maxOffset);

		//ofVec2f mousePos(ofGetMouseX(), ofGetMouseY());

		// Define a triangle at the origin (0,0) that points to the right
		ofVec2f p1(0, 6.25);
		ofVec2f p2(25, 0);
		ofVec2f p3(0, -6.25);

		float rotation = ofRandom(360); // The rotate function uses degrees!
		p1.rotate(rotation);
		p2.rotate(rotation);
		p3.rotate(rotation);

		ofVec2f triangleOffset(offsetDistance, 0.0);
		triangleOffset.rotate(rotation);

		p1 += point + triangleOffset;
		p2 += point + triangleOffset;
		p3 += point + triangleOffset;

		ofColor aqua(0, 252, 255, alpha);
		ofColor purple(198, 0, 205, alpha);
		ofColor inbetween = aqua.getLerped(purple, ofRandom(1.0));
		ofSetColor(inbetween);

		ofDrawTriangle(p1, p2, p3);
	}
	// Alternate code, for the rotating triangle brush discussed in the chapter:

	//        ofVec2f mousePos(ofGetMouseX(), ofGetMouseY());
	//
	//        // Define a triangle at the origin (0,0) that points to the right
	//        ofVec2f p1(0, 25.0);
	//        ofVec2f p2(100, 0);
	//        ofVec2f p3(0, -25.0);
	//
	//        // Rotate the triangle points around the origin
	//        float rotation = ofRandom(360); // Uses degrees!
	//        p1.rotate(rotation);
	//        p2.rotate(rotation);
	//        p3.rotate(rotation);
	//
	//        // Shift the triangle to the mouse position
	//        p1 += mousePos;
	//        p2 += mousePos;
	//        p3 += mousePos;
	//
	//        ofSetColor(255, 50);
	//        ofDrawTriangle(p1, p2, p3);
	//}

	//// If the right mouse button is pressed...
	//if (ofGetMousePressed(OF_MOUSE_BUTTON_RIGHT)) {
	//    ofBackground(0);  // Erase the screen with a black background
	//}
}

//--------------------------------------------------------------
void CircleOfFifths::drawHaloAroundCursor() {
	ofVec2f mousePos(ofGetMouseX(), ofGetMouseY());
	drawHaloAroundPoint(mousePos);
}
