# ofxDepthCameras - openFrameworks addon for professional working with various models of depth cameras.

Author: Denis Perevalov

## Installing

1. Use Project Generator to add addon, and corresponding addon for support of particular type of cameras 
(see examples descriptions below).

2. Check that you are using addon's "ofxDepthCameras_CAMERAMODEL.cpp" file in the project, 
and exclude all other "ofxDepthCameras_...cpp" files from the project in Visual Studio.

## Supported cameras

* Microsoft Kinect One

* Intel Realsense D415, D435

## Examples

* example_KinectOne - example of using Microsoft Kinect One. It requires ofxKuKinectV2 addon (by perevalovds).

After creating the project using Project Generator, 
keep in project only following ofxDepthCameras's files:
 src/ofxDepthCameras.h and src/ofxDepthCameras_KinectOne.cpp.

* example_Realsense - example of using Intel Realsense cameras D415,D435. It requires ofxRealsense addon (by perevalovds).

After creating the project using Project Generator, 
keep in project only following ofxDepthCameras's files:
 src/ofxDepthCameras.h and src/ofxDepthCameras_Realsense2.cpp.


## Supported OS, oF version

Windows 10, oF 0.10.1, Visual Studio C++ 2017, 64/32 bit



