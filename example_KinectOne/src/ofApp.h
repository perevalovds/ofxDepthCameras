#pragma once

//
//This example shows how to use ofxDepthCameras addon for working with Kinect One cameras.
//1. To compile successfully, set 64-bit cmpilation (because example contains 64-bit DLL's in its bin folder)
//2. Please check that you are using addon's "ofxDepthCameras_KinectOne.cpp" file in the project, 
//and exclude all other "ofxDepthCameras_...cpp" files from the project in Visual Studio.

#include "ofMain.h"


#include "ofxDepthCameras.h"


class ofApp : public ofBaseApp {

public:
	void setup();
	void exit();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofxDepthCameras device_;

	ofEasyCam cam;

	bool draw_textures = true;
	bool draw_points = true;

};
