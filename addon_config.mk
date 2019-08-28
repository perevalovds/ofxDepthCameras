meta:
	ADDON_NAME = ofxDepthCameras
	ADDON_DESCRIPTION = Unified interface for working with depth cameras
	ADDON_AUTHOR = Denis Perevalov
	ADDON_TAGS = "device"
	ADDON_URL = http://github.com/perevalovds/ofxDepthCameras

common:

	#ADDON_INCLUDES +=  libs/realsense2/include/

osx:
vs:	
	# x64
	#ADDON_LIBS += libs/realsense2/lib/x64/realsense2.lib
	#ADDON_DLLS_TO_COPY += libs/realsense2/lib/x64/realsense2.dll;libs/realsense2/lib/x64/Intel.Realsense.dll
	# Win32
	#ADDON_LIBS += libs/realsense2/lib/vs/Win32/realsense2.lib
	#ADDON_DLLS_TO_COPY += libs/realsense2/lib/Win32/realsense2.dll;libs/realsense2/lib/Win32/Intel.Realsense.dll
	
	
linux64:
linuxarmv6l:
linuxarmv7l:
msys2:
