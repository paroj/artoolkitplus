/*
    Copyright (C) 2010  ARToolkitPlus Authors

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Authors:
      Daniel Wagner
 */


#ifndef __ARTOOLKITPLUS_HEADERFILE__
#define __ARTOOLKITPLUS_HEADERFILE__

#include <vector>


#if defined ARTOOLKITPLUS_DLL && defined _WIN32
#  ifdef ARTOOLKITPLUS_EXPORTS
#    define ARTOOLKITPLUS_API __declspec(dllexport)
#  else
#    define ARTOOLKITPLUS_API __declspec(dllimport)
#  endif // ARTOOLKITPLUS_EXPORTS
#else
#  define ARTOOLKITPLUS_API
#endif // ARTOOLKITPLUS_DLL


#define ARTOOLKITPLUS_VERSION_MAJOR 2
#define ARTOOLKITPLUS_VERSION_MINOR 2


namespace ARToolKitPlus
{

enum PIXEL_FORMAT {
	PIXEL_FORMAT_ABGR = 1,
	PIXEL_FORMAT_BGRA = 2,
	PIXEL_FORMAT_BGR = 3,
	PIXEL_FORMAT_RGBA = 4,
	PIXEL_FORMAT_RGB = 5,
	PIXEL_FORMAT_RGB565 = 6,
	PIXEL_FORMAT_LUM = 7
};


enum UNDIST_MODE {
	UNDIST_NONE,
	UNDIST_STD,
	UNDIST_LUT
};


enum IMAGE_PROC_MODE {
	IMAGE_HALF_RES,
	IMAGE_FULL_RES
};


enum HULL_TRACKING_MODE {
	HULL_OFF,
	HULL_FOUR,
	HULL_FULL
};

// ARToolKitPlus versioning
//
enum ARTKP_VERSION {
	VERSION_MAJOR = ARTOOLKITPLUS_VERSION_MAJOR,
	VERSION_MINOR = ARTOOLKITPLUS_VERSION_MINOR
};


//enum IDMARKER_MODE {
//	IDMARKER_SIMPLE,
//	IDMARKER_BCH
//};

enum MARKER_MODE {
	MARKER_TEMPLATE,
	MARKER_ID_SIMPLE,
	MARKER_ID_BCH,
	//MARKER_ID_BCH2		// upcomming, not implemented yet
};

enum POSE_ESTIMATOR {
	POSE_ESTIMATOR_ORIGINAL,			// original "normal" pose estimator
	POSE_ESTIMATOR_ORIGINAL_CONT,		// original "cont" pose estimator
	POSE_ESTIMATOR_RPP					// new "Robust Planar Pose" estimator
};


class TrackerSingleMarker;
class MemoryManager;


struct CornerPoint
{
	CornerPoint() : x(0), y(0)
	{}
 
	CornerPoint(int nX, int nY) : x(static_cast<short>(nX)), y(static_cast<short>(nY))
	{}

	short x,y;
};

typedef std::vector<CornerPoint> CornerPoints;

}  // namespace ARToolKitPlus



#endif //__ARTOOLKITPLUS_HEADERFILE__
