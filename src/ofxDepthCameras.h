#pragma once
//Unified interface for working with depth cameras

#include "ofMain.h"

//To compile your project successfully, 
//please check that you are using only one CPP file, which corresponds to your camera model.
//For example, for Kinect One use "ofxDepthCameras_KinectOne.cpp" file, 
//and exclude all other "ofxDepthCameras_...cpp" files from Visual Studio's project 
//(just exclude, not delete it!:)

class ofxDepthCameras
{
public:
	ofxDepthCameras();

	static string model() { return "Kinect One"; }
	static int device_count();
	static vector<string> get_serials();

	bool start(int i, int device_index);
	bool start(int i, string device_serial);

	bool update();
	bool stop();

	bool connected(int i);

	int depth_w(int i);
	int depth_h(int i);

	bool is_frame_new(int i);

	//depth values in millimetres
	bool depth_pixels16(int i, int &depth_w, int &depth_h, const unsigned short * &pixels);
	
	//depth values 0..255 for texture
	bool depth_pixels8(int i, int min_dist, int max_dist, int &depth_w, int &depth_h, const unsigned char * &pixels);	
																							
	bool point_cloud(int i, int &count, const float *&cloud_xyz_mm, ofPoint corner0 = ofPoint(-10000, -10000, -10000), ofPoint corner1 = ofPoint(10000, 10000, 10000));
	bool point_cloud(int i, int &count, const ofPoint *&pcloud, ofPoint corner0 = ofPoint(-10000, -10000, -10000), ofPoint corner1 = ofPoint(10000, 10000, 10000));
	bool point_cloud(int i, int &count, const glm::vec3 *&pcloud, ofPoint corner0 = ofPoint(-10000, -10000, -10000), ofPoint corner1 = ofPoint(10000, 10000, 10000));

private:
	
};
