#pragma once
//Класс библиотеки

#include <windows.h>
#include <string>

using namespace std;

class Module
{
public:
	Module();

	///Загрузка DLL
	int loadDLL(const char* dll_file);	

	///Выгрузка DLL
	int freeDLL();

	int device_count();

	//DLL_EXPORT int device_info(...);  //TODO
	//TODO restart settings

	int start(int i, int device_index);
	int update(int i);
	int stop(int i);


	int started(int i);
	int depth_w(int i);
	int depth_h(int i);

	int depth_values(int i, int *depth_w, int *depth_h, unsigned short *buffer);
	int simple_pixels(int i, int min_dist, int max_dist, int *depth_w, int *depth_h, unsigned char **pixels);	//for texture draw	
																								//returns filled buffer
	int depth_cloud(int i, int min_dist, int max_dist, int *count, float **cloud_mm);
	//int depth_cloud(int i, int max_dist, vector<ofPoint> &pcloud);


private:
	HMODULE hModule_;

	typedef  int(_cdecl* device_count_type)();
	typedef  int(_cdecl* exec_type)(int i);
	typedef  int(_cdecl*  start_type)(int i, int device_index);
	typedef  int(_cdecl* depth_values_type)(int i, int *depth_w, int *depth_h, unsigned short *buffer);
	typedef  int(_cdecl* simple_pixels_type)(int i, int min_dist, int max_dist, int *depth_w, int *depth_h, unsigned char **pixels);
	typedef  int(_cdecl* depth_cloud_type)(int i, int min_dist, int max_dist, int *count, float **cloud_mm);

	device_count_type				device_count_;
	start_type						start_;
	exec_type						update_;
	exec_type						stop_;
	exec_type						started_;
	exec_type						depth_w_;
	exec_type						depth_h_;
	depth_values_type				depth_values_;
	simple_pixels_type				simple_pixels_;
	depth_cloud_type				depth_cloud_;
};
