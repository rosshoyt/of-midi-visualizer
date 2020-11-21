#include "ADSRVisualizer.h"

//--------------------------------------------------------------
ADSRVisualizer::ADSRVisualizer(std::string uid) : MIDIAnimationComponent(uid) {}

//--------------------------------------------------------------
void ADSRVisualizer::setup() {
	//setup Colors
	for (int i = 0; i < 32; i++) {
		colors.push_back(utils::color::getRandomColor());
	}
}

//--------------------------------------------------------------
void ADSRVisualizer::setupGUI() {		
	for (int i = 0; i < scXDefaults.size(); ++i) {
		auto pNumber = std::to_string(i + 1);
		splineControlX.push_back(ofParameter<double>().set("x" + pNumber, scXDefaults[i], minF, maxF));
		splineControlY.push_back(ofParameter<double>().set("y" + pNumber, scYDefaults[i], minF, maxF));
		gui.add(splineControlX[i]);
		gui.add(splineControlY[i]);
	}
	/*gui.add(splineControlX2.set("control point X", .5f, minF, maxF));
	gui.add(splineControlY2.set("spline Amount", .75f, minF, maxF));*/
	gui.add(changeSpeed.set("color speed ms", 300, 1, 5000));
	gui.add(circleSize.set("circle size", 5.f, 0.1f, 100.f));
}

//--------------------------------------------------------------
void ADSRVisualizer::prepare() {}

//--------------------------------------------------------------
void ADSRVisualizer::takedown() {}

//--------------------------------------------------------------
void ADSRVisualizer::update() {}

//--------------------------------------------------------------
bool ADSRVisualizer::shouldChange() {
	//update color
	auto nowMS = ofGetSystemTimeMillis();
	if (nowMS > lastColorChange + changeSpeed) {
		lastColorChange = nowMS;
		return true;
	}
	return false;
}

//--------------------------------------------------------------
void ADSRVisualizer::draw() {

	//gui.draw();
	int maxVel = 0;
	int numNotes = 0;
	for (auto channel : midiPortState->getAllChannelNotes()) {
		for (auto note : channel) {
			maxVel = std::max(note.second, maxVel);
			++numNotes;
		}
	}

	tk::spline spline;
	std::vector<double> xTemps, yTemps;

	//update time-based params
	if (shouldChange()) {
		ofSetColor(utils::color::getRandomColor()); //colors[i % colors.size()]);
		// update which y val to augment
		if (currentYToChange > maxYToChange) {
			currentYToChange = minYToChange;
		}
		else {
			++currentYToChange;
		}

	}

	auto size = splineControlX.size();
	for (auto i = 0; i < size; i++) {
		//if(i < size - 1)
		xTemps.push_back(splineControlX[i]);
		yTemps.push_back(i == currentYToChange && numNotes > 0 ? splineControlY[i] * maxVel / 128.f : splineControlY[i]);
	}

	//std::sort(xTemps.begin(), xTemps.end());//, sortDouble);

	spline.set_points(xTemps, yTemps);



	/*spline.set_points({ scXDefaults[0], splineControlX[1], splineControlX[2], splineControlX[3], scXDefaults[4] },
	{ splineControlY[0], splineControlY[1], splineControlY[2], splineControlY[3], splineControlY[4] }
	);
	*/
	//float width = ofGetWidth(), height = ofGetHeight();



	for (int i = 0; i < numPoints; ++i) {

		double xF = float(i) / numPoints;
		float heightScale = spline(xF);

		auto circleX = animationWidth * xF;
		auto circleY = animationHeight - animationHeight * heightScale;
		ofDrawCircle({ circleX , circleY }, circleSize);

		//std::cout << "Height Scale = " << heightScale << " for x = " << i << '\n';
	}
}