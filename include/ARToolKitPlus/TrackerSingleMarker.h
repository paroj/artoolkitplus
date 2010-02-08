/**
 * Copyright (C) 2010  ARToolkitPlus Authors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *  Daniel Wagner
 *  Pavel Rojtberg
 */

#ifndef __TRACKERSINGLEMARKER_HEADERFILE__
#define __TRACKERSINGLEMARKER_HEADERFILE__

#include <ARToolKitPlus/TrackerImpl.h>
#include <vector>

using std::vector;

namespace ARToolKitPlus {

/// Defines a simple interface for single-marker tracking with ARToolKitPlus
/**
 *  ARToolKitPlus::TrackerSingleMarker provides all methods to access ARToolKit for
 *  single marker tracking without needing to mess around with it low level methods directly.
 *
 *  If you need multi-marker tracking use TrackerMultiMarker.
 */
class TrackerSingleMarker: virtual public Tracker {
public:
    /**
     * initializes TrackerSingleMarker
     *  nCamParamFile is the name of the camera parameter file
     *  nLogger is an instance which implements the ARToolKit::Logger interface
     */
    virtual bool init(const char* nCamParamFile, ARFloat nNearClip, ARFloat nFarClip) = 0;

    /**
     * adds a pattern to ARToolKit
     * pass the patterns filename
     */
    virtual int addPattern(const char* nFileName) = 0;

    /**
     * calculates the transformation matrix
     * pass the image as RGBX (32-bits)
     * @return detected markers in image
     */
    virtual vector<int> calc(const uint8_t* nImage, ARMarkerInfo** nMarker_info = NULL, int* nNumMarkers = NULL) = 0;

    /**
     * manually select one of the detected markers
     * instead of using the best one
     */
    virtual void selectDetectedMarker(const int id) = 0;

    /**
     * Select the best marker based on Certainty Factor
     */
    virtual int selectBestMarkerByCf() = 0;

    /**
     * Sets the width and height of the patterns.
     */
    virtual void setPatternWidth(ARFloat nWidth) = 0;

    /**
     *  This method is primarily for compatibility issues with code previously using
     *  ARToolKit rather than ARToolKitPlus. patt_trans is the original transformation
     *  matrix ARToolKit calculates rather than the OpenGL style version of this matrix
     *  that can be retrieved via getModelViewMatrix().
     */
    virtual void getARMatrix(ARFloat nMatrix[3][4]) const = 0;

    /**
     * Returns the confidence value of the currently best detected marker.
     */
    virtual ARFloat getConfidence() const = 0;
};

}
; // namespace ARToolKitPlus


#endif //__TRACKERSINGLEMARKER_HEADERFILE__
