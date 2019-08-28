#include "dll_depth_openni.h"

#include <windows.h>
#include <iostream>
using namespace std;


//------------------------------------------------------------------------------
Module::Module() {
	hModule_ = 0; 
}

//------------------------------------------------------------------------------
#define CHECK_FUNCTION(name, fun_pointer) {if (!fun_pointer) { cout << "ERROR: no function " << name << endl; freeDLL(); return 2; } }

//------------------------------------------------------------------------------
///Загрузка DLL
int Module::loadDLL(const char* dll_file)
{
	HMODULE hModule;
	if ((hModule = LoadLibraryA(dll_file)) == NULL) {
		return 1;
	}
	hModule_ = hModule;

	device_count_ = (device_count_type)GetProcAddress(hModule_, "device_count");
	start_ = (start_type)GetProcAddress(hModule_, "start");
	update_ = (exec_type)GetProcAddress(hModule_, "update");
	stop_ = (exec_type)GetProcAddress(hModule_, "stop");
	started_ = (exec_type)GetProcAddress(hModule_, "started");
	depth_w_ = (exec_type)GetProcAddress(hModule_, "depth_w");
	depth_h_ = (exec_type)GetProcAddress(hModule_, "depth_h");
	depth_values_ = (depth_values_type)GetProcAddress(hModule_, "depth_values");
	simple_pixels_ = (simple_pixels_type)GetProcAddress(hModule_, "simple_pixels");
	depth_cloud_ = (depth_cloud_type)GetProcAddress(hModule_, "depth_cloud");


	CHECK_FUNCTION("device_count", device_count_);

	CHECK_FUNCTION("start", start_);
	CHECK_FUNCTION("update", update_);
	CHECK_FUNCTION("stop", stop_);

	CHECK_FUNCTION("started", started_);
	CHECK_FUNCTION("depth_w", depth_w_);
	CHECK_FUNCTION("depth_h", depth_h_);
	CHECK_FUNCTION("depth_values", depth_values_);
	CHECK_FUNCTION("simple_pixels", simple_pixels_);
	CHECK_FUNCTION("depth_cloud", depth_cloud_);

	return 0;
}

//------------------------------------------------------------------------------
///Выгрузка DLL
int Module::freeDLL() {
	if (!hModule_) return 4;
	FreeLibrary(hModule_);

	return 0;
}

//------------------------------------------------------------------------------
int Module::device_count() {
	if (!hModule_) return -1;
	return device_count_();
}

//------------------------------------------------------------------------------
int Module::start(int id, int device_index) {
	if (!hModule_) return -1;
	return start_(id, device_index);
}

//------------------------------------------------------------------------------
int Module::update(int id) {
	if (!hModule_) return -1;
	return update_(id);
}

//------------------------------------------------------------------------------
int Module::stop(int id) {
	if (!hModule_) return -1;
	return stop_(id);
}

//------------------------------------------------------------------------------
int Module::started(int i) {
	if (!hModule_) return -1;
	return started_(i);
}

//------------------------------------------------------------------------------
int Module::depth_w(int i) {
	if (!hModule_) return -1;
	return depth_w_(i);
}

//------------------------------------------------------------------------------
int Module::depth_h(int i) {
	if (!hModule_) return -1;
	return depth_h_(i);

}

//------------------------------------------------------------------------------
int Module::depth_values(int i, int *depth_w, int *depth_h, unsigned short *buffer) {
	if (!hModule_) return -1;
	return depth_values_(i, depth_w, depth_h, buffer);

}

//------------------------------------------------------------------------------
int Module::simple_pixels(int i, int min_dist, int max_dist, int *depth_w, int *depth_h, unsigned char **pixels) {
	if (!hModule_) return -1;
	return simple_pixels_(i, min_dist, max_dist, depth_w, depth_h, pixels);
}

//------------------------------------------------------------------------------
int Module::depth_cloud(int i, int min_dist, int max_dist, int *count, float **cloud_mm) {
	if (!hModule_) return -1;
	return depth_cloud_(i, min_dist, max_dist, count, cloud_mm);
}

//------------------------------------------------------------------------------
/*int Module::depth_cloud(int i, int max_dist, vector<ofPoint> &pcloud) {
	if (!hModule_) {
		pcloud.clear();
		return -1;
	}
	int n = 0;
	float *cloud = 0;
	depth_cloud(i, -10000, max_dist, &n, &cloud);
	pcloud.resize(n);
	for (int k = 0; k<n; k++) {
		pcloud[k] = ofPoint(cloud[3 * k], cloud[3 * k + 1], cloud[3 * k + 2]);
	}
}
*/

//------------------------------------------------------------------------------
