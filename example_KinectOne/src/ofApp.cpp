#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	vector<string> serials = ofxDepthCameras::get_serials();
	cout << "Connected " << ofxDepthCameras::model() << " devices:" << endl;
	for (int i = 0; i < serials.size(); i++) {
		cout << i + 1 << ":\t" << serials[i] << endl;
	}
	if (serials.size() == 0) {
		cout << "No connected " << ofxDepthCameras::model() << " Kinect One devices!" << endl;
	}
	else {
		device_.start(0, serials[0]);
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
	device_.stop();
}

//--------------------------------------------------------------
void ofApp::update(){
	device_.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);

	if (draw_textures) {

		if (device_.connected(0)) {

			float min_d = 500;
			float max_d = 2000;
			int w, h;
			const unsigned char *pixels;
			if (device_.depth_pixels8(0, min_d, max_d, w, h, pixels)) {
				ofTexture tex;
				tex.loadData(pixels, w, h, GL_LUMINANCE);
				ofSetColor(255);
				tex.draw(0, 0);
			}
		}
	}

	if (draw_points) {
		if (device_.connected(0)) {
			ofPoint corner0(-10000, -10000, 100);
			ofPoint corner1(10000, 10000, 10000);
			int count;
			const glm::vec3 *points;
			if (device_.point_cloud(0, count, points, corner0, corner1)) {
				ofMesh mesh;
				if (count > 0) {
					mesh.addVertices(points, count);
				}
				cam.begin();

				ofPushMatrix();
				ofScale(0.1, 0.1, -0.1);
				ofSetColor(255, 0, 0);
				mesh.drawVertices();
				ofPopMatrix();
				cam.end();
			}
		}
	}

	ofDrawBitmapStringHighlight("FPS: " + ofToString(ofGetFrameRate(), 0) + "  Press 1,2 to enable/disable texture and points", 20, 20);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == '1') draw_textures = !draw_textures;
	if (key == '2') draw_points = !draw_points;

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
