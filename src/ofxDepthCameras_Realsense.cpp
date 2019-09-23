#include "ofxDepthCameras.h"

//Realsense camera support
//Requires ofxRealsense addon in the oF project

#include "ofxRealsense.h"

struct Realsense_Data {
	Realsense_Data() {
		device_on_.resize(N);
		isframe_.resize(N);
		device_status.resize(N);
	}

	//Settings
	//NOTE: currently one settings are for all cameras
	ofxRealsense_Settings settings;

	//Maximal number of cameras
	static const int N = 4;

	//Devices
	ofxRealsense device_[N];

	//Holder for depth data
	//NOTE: this memory is not cleared after "close"
	vector<unsigned char> depth8[N];
	vector<glm::vec3> points[N];


	vector<int> device_on_;
	vector<bool> isframe_;		
	
	float device_time_[N];		
	static const int device_NONE = 0;
	static const int device_WAIT = 1;
	static const int device_OK = 2;
	static const int device_ERROR = 3;
	vector<int> device_status;

};

Realsense_Data Realsense_Data_;

//------------------------------------------------------------------------------
ofxDepthCameras::ofxDepthCameras() {

}

//------------------------------------------------------------------------------
string ofxDepthCameras::model() {
	return "Realsense";
}

//------------------------------------------------------------------------------
int ofxDepthCameras::device_count() {
	return get_serials().size();
}

//------------------------------------------------------------------------------
vector<string> ofxDepthCameras::get_serials() {
	return ofxRealsense::get_serials();
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
	auto &data = Realsense_Data_;
	if (i >= 0 && i < data.N) {
		data.device_on_[i] = 0;

		//data.device_status[i] = data.device_NONE;


		//TODO use settings, and allow to host reques list of supported resolutions
		ofxRealsense_Settings &S = data.settings;
		S.use_rgb = 0;

		S.depth_w = 640;
		S.depth_h = 480;
		S.depth_fps = 30;
		S.rgb_w = 640;
		S.rgb_h = 480;
		S.rgb_fps = 30;

		//device_.setup(serials[0], S);

		cout << "Starting Realsense " << device_serial << endl;
		data.device_[i].setup(device_serial, S);
		bool connected = data.device_[i].connected();
		if (connected) {
			data.device_on_[i] = 1;
			data.device_status[i] = data.device_WAIT;
		}
		else {
			cout << "   Error connecting" << endl;
			data.device_status[i] = data.device_ERROR;

		}
		data.device_time_[i] = -1;
	}
	else return false;
}

//------------------------------------------------------------------------------
bool ofxDepthCameras::update() {
	auto &data = Realsense_Data_;

	float time = ofGetElapsedTimef();
	for (int i = 0; i < data.N; i++) {
		data.isframe_[i] = false;
	}

	for (int i = 0; i < data.N; i++) {
		if (data.device_on_[i]) {
			data.device_[i].update();
			if (data.device_[i].isFrameNew()) {
				data.isframe_[i] = true;
				data.device_time_[i] = time;
				data.device_status[i] = data.device_OK;
			}
		}
	}

	return true;
}

//------------------------------------------------------------------------------
bool ofxDepthCameras::stop() {
	auto &data = Realsense_Data_;
	for (int i=0; i<data.N; i++) {
		if (data.device_on_[i]) {
			data.device_[i].close();
		}
	}
	return true;
}

//------------------------------------------------------------------------------
bool ofxDepthCameras::connected(int i) {
	auto &data = Realsense_Data_;
	if (i >= 0 && i < data.N) {
		return data.device_on_[i];
	}
	return false;
}

//------------------------------------------------------------------------------
int ofxDepthCameras::depth_w(int i) {
	return Realsense_Data_.settings.depth_w;
}

//------------------------------------------------------------------------------
int ofxDepthCameras::depth_h(int i) {
	return Realsense_Data_.settings.depth_h;
}

//------------------------------------------------------------------------------
bool ofxDepthCameras::is_frame_new(int i) {
	auto &data = Realsense_Data_;
	return data.isframe_[i];
}

//------------------------------------------------------------------------------
bool ofxDepthCameras::depth_pixels16(int i, int &w, int &h, const unsigned short * &pixels) {
	w = h = 0;
	pixels = 0;
	cout << "ofxDepthCameras_Realsense error: currently depth_pixels16 is not supported" << endl;
	return false;

	/*
	w = h = 0;
	pixels = NULL;

	if (connected(i)) {
		auto &data = Realsense_Data_;
		auto &device = data.device_[i];
		ofShortPixels &pix = device.get_depth_pixels_rgb;
		w = pix.getWidth();
		h = pix.getHeight();
		if (w > 0 && h > 0) {
			pixels = pix.getPixels();
			return true;
		}
		else {
			return false;
		}
	}
	return false;
	*/
}

//------------------------------------------------------------------------------
bool ofxDepthCameras::depth_pixels8(int i, int min_dist, int max_dist, int &w, int &h, const unsigned char * &pixels) {
	w = h = 0;
	pixels = NULL;

	if (connected(i)) {
		auto &data = Realsense_Data_;
		auto &device = data.device_[i];

		int w1, h1;
		bool result = device.get_depth_pixels_rgb(w1, h1, data.depth8[i]);
		if (!result) return false;
		w = w1;
		h = h1;
		pixels = &data.depth8[i][0];
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
bool ofxDepthCameras::point_cloud(int i, int &count, const float *&cloud_xyz_mm, ofPoint corner0, ofPoint corner1) {
	const glm::vec3 *pcloudglm;
	bool res = point_cloud(i, count, pcloudglm, corner0, corner1);
	if (count > 0) {
		cloud_xyz_mm = (const float *)(&pcloudglm[0].x);
	}
	else {
		cloud_xyz_mm = 0;
	}
	return res;
}

//------------------------------------------------------------------------------
bool ofxDepthCameras::point_cloud(int i, int &count, const ofPoint *&pcloud, ofPoint corner0, ofPoint corner1) {
	const glm::vec3 *pcloudglm;
	bool res = point_cloud(i, count, pcloudglm, corner0, corner1);
	if (count > 0) {
		pcloud = (const ofPoint *)(&pcloudglm[0].x);
	}
	else {
		pcloud = 0;
	}
	return res;
}

//------------------------------------------------------------------------------
bool ofxDepthCameras::point_cloud(int i, int &count, const glm::vec3 *&pcloud, ofPoint corner0, ofPoint corner1) {
	count = 0;
	pcloud = NULL;

	if (connected(i)) {
		auto &data = Realsense_Data_;
		auto &device = data.device_[i];
		auto &points = data.points[i];

		bool result = device.get_point_cloud(points);
		if (!result) return false;

		//discard points outside corners
		//TODO not optimal if corners are not used
		int n = points.size();
		for (int i = 0; i < n; ) {
			auto &p = points[i];
			if (p.z < corner0.z || p.z > corner1.z
				|| p.x < corner0.x || p.x > corner1.x
				|| p.y < corner0.y || p.y > corner1.y) {
				swap(points[i], points[n - 1]);
				n--;
			}
			else {
				i++;
			}
		}
		count = n;

		if (count > 0) {
			pcloud = &points[0];
		}
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
