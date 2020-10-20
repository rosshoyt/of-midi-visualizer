#pragma once
#include "ofMain.h"
#include "MIDIPortState.h"

#include "AnimationComponent.h"

class TexturedSphere : public AnimationComponent {
public:
	TexturedSphere(MIDIPortState* midiPortState, std::string uid): AnimationComponent(midiPortState, uid){

	}

	void setup() {
		ofDisableAlphaBlending();
		ofEnableDepthTest();
		light.enable();
		light.setPosition(ofVec3f(100, 100, 200));
		light.lookAt(ofVec3f(0, 0, 0));
		ofLoadImage(mTex, "textures/earth.jpg");
		mTex.generateMipmap();
		mTex.setTextureMinMagFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
		ofDisableArbTex();
	}

	void prepare() override {	
		
	}

	void takedown() override {
	    light.disable();
		ofEnableAlphaBlending();
		ofDisableDepthTest();
		ofEnableArbTex();
	}

	void update() {
	
	}

	void draw() {
		cam.begin();
		mTex.bind();
		sphere.draw();
		mTex.unbind();
		cam.end();
	}

	void drawGUI() {}

	
private:
	ofSpherePrimitive sphere;
	ofLight light;
	ofTexture mTex;
	ofEasyCam cam;

};