#include "ofxDepthCameras.h"

//Kinect One camera support
//Requires ofxKuKinectV2 addon in the oF project
//NOTE: Currently cameras not reconnect automatically, consider using Watchdog and restart app for that

#include "ofxKuKinectV2.h"

struct KinectOne_Data {
	KinectOne_Data() {
		kinect_on_.resize(N);
		isframe_.resize(N);
		kinect_status.resize(N);
	}

	//maximal number of kinects
	static const int N = 4;

	ofxKuKinectV2 kinect_[N];

	vector<int> kinect_on_;
	vector<bool> isframe_;		
	
	float kinect_time_[N];		
	static const int KINECT_NONE = 0;
	static const int KINECT_WAIT = 1;
	static const int KINECT_OK = 2;
	static const int KINECT_ERROR = 3;
	vector<int> kinect_status;

};

KinectOne_Data kinectone_data_;

//------------------------------------------------------------------------------
ofxDepthCameras::ofxDepthCameras() {

}

//------------------------------------------------------------------------------
string ofxDepthCameras::model() { 
	return "Kinect One"; 
}
	
//------------------------------------------------------------------------------
int ofxDepthCameras::device_count() {
	return get_serials().size();
}

//------------------------------------------------------------------------------
vector<string> ofxDepthCameras::get_serials() {
	vector<string> serials;
	ofxKuKinectV2 tmp;
	vector <ofxKuKinectV2::KinectDeviceInfo> deviceList = tmp.getDeviceList();
	for (int i = 0; i < deviceList.size(); i++) {
		serials.push_back(deviceList[i].serial);
	}
	return serials;
}

//------------------------------------------------------------------------------
bool ofxDepthCameras::start(int i, int device_index) {
	auto serials = get_serials();
	if (device_index >= 0 && device_index < serials.size()) {
		return start(i, serials[device_index]);
	}
	return false;
}

//------------------------------------------------------------------------------
bool ofxDepthCameras::start(int i, string device_serial) {
	auto &data = kinectone_data_;
	if (i >= 0 && i < data.N) {
		data.kinect_on_[i] = 0;

		//data.kinect_status[i] = data.KINECT_NONE;

		KinectV2Settings settings;
		settings.rgb = false;
		cout << "Starting Kinect One " << device_serial << endl;
		bool connected = data.kinect_[i].open(device_serial, settings);
		if (connected) {
			data.kinect_on_[i] = 1;
			data.kinect_status[i] = data.KINECT_WAIT;
		}
		else {
			cout << "   Error connecting" << endl;
			data.kinect_status[i] = data.KINECT_ERROR;

		}
		data.kinect_time_[i] = -1;
		return true;
	}
	else return false;
}

//------------------------------------------------------------------------------
bool ofxDepthCameras::update() {
	auto &data = kinectone_data_;

	float time = ofGetElapsedTimef();
	for (int i = 0; i < data.N; i++) {
		data.isframe_[i] = false;
	}

	for (int i = 0; i < data.N; i++) {
		if (data.kinect_on_[i]) {
			data.kinect_[i].update();
			if (data.kinect_[i].isFrameNew()) {
				data.isframe_[i] = true;
				data.kinect_time_[i] = time;
				data.kinect_status[i] = data.KINECT_OK;
			}
		}
	}

	return true;
}

//------------------------------------------------------------------------------
bool ofxDepthCameras::stop() {
	auto &data = kinectone_data_;
	for (int i=0; i<data.N; i++) {
		if (data.kinect_on_[i]) {
			data.kinect_[i].close();
		}
	}
	return true;
}

//------------------------------------------------------------------------------
bool ofxDepthCameras::connected(int i) {
	auto &data = kinectone_data_;
	if (i >= 0 && i < data.N) {
		return data.kinect_on_[i];
	}
	return false;
}

//------------------------------------------------------------------------------
int ofxDepthCameras::depth_w(int i) {
	return ofxKuKinectV2::depth_w;
}

//------------------------------------------------------------------------------
int ofxDepthCameras::depth_h(int i) {
	return ofxKuKinectV2::depth_h;
}

//------------------------------------------------------------------------------
bool ofxDepthCameras::is_frame_new(int i) {
	auto &data = kinectone_data_;
	return data.isframe_[i];
}

//------------------------------------------------------------------------------
bool ofxDepthCameras::depth_pixels16(int i, int &w, int &h, const unsigned short * &pixels) {
	w = h = 0;
	pixels = NULL;

	if (connected(i)) {
		auto &data = kinectone_data_;
		auto &kinect = data.kinect_[i];
		ofShortPixels &pix = kinect.getShortDepthPixels();
		w = pix.getWidth();
		h = pix.getHeight();
		if (w > 0 && h > 0) {
			pixels = pix.getData();
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
bool ofxDepthCameras::depth_pixels8(int i, int min_dist, int max_dist, int &w, int &h, const unsigned char * &pixels) {
	w = h = 0;
	pixels = NULL;

	if (connected(i)) {
		auto &data = kinectone_data_;
		auto &kinect = data.kinect_[i];

		ofPixels &pix = kinect.getDepthPixels(min_dist, max_dist);
		w = pix.getWidth();
		h = pix.getHeight();
		if (w > 0 && h > 0) {
			pixels = pix.getData();
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
bool ofxDepthCameras::point_cloud(int i, int &count, const float *&cloud_xyz_mm, ofPoint corner0, ofPoint corner1) {
	count = 0;
	cloud_xyz_mm = NULL;

	if (connected(i)) {
		auto &data = kinectone_data_;
		auto &kinect = data.kinect_[i];

		const vector<ofPoint> &pnt = kinect.getPointCloudInsideVolume(corner0, corner1);
		count = pnt.size();
		if (count > 0) {
			cloud_xyz_mm = &pnt[0].x;
		}
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
bool ofxDepthCameras::point_cloud(int i, int &count, const ofPoint *&pcloud, ofPoint corner0, ofPoint corner1) {
	const float *cloud_xyz_mm;
	bool res = point_cloud(i, count, cloud_xyz_mm, corner0, corner1);
	pcloud = (const ofPoint *)cloud_xyz_mm;
	return res;
}

//------------------------------------------------------------------------------
bool ofxDepthCameras::point_cloud(int i, int &count, const glm::vec3 *&pcloud, ofPoint corner0, ofPoint corner1) {
	const float *cloud_xyz_mm;
	bool res = point_cloud(i, count, cloud_xyz_mm, corner0, corner1);
	pcloud = (const glm::vec3 *)cloud_xyz_mm;
	return res;
}

//------------------------------------------------------------------------------
