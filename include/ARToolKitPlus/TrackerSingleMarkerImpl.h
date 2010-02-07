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

#ifndef __ARTOOLKITPLUS_TRACKERSINGLEMARKERIMPL_HEADERFILE__
#define __ARTOOLKITPLUS_TRACKERSINGLEMARKERIMPL_HEADERFILE__

#include <ARToolKitPlus/TrackerSingleMarker.h>
#include <ARToolKitPlus/TrackerImpl.h>

#define TrackerSingleMarkerImpl TrackerSingleMarkerImpl

namespace ARToolKitPlus {

/**
 * TrackerSingleMarkerImpl implements the TrackerSingleMarker interface
 */
class TrackerSingleMarkerImpl: public TrackerSingleMarker, protected TrackerImpl {
public:
    /**
     *  @param imWidth width of the source image in px
     *  @param imHeight height of the source image in px
     *  @param pattWidth describes the pattern image width (6 by default).
     *  @param pattHeight describes the pattern image height (6 by default).
     *  @param pattNum describes the maximum resolution at which a pattern is sampled from the camera image
     *  (6 by default, must a a multiple of pattWidth and pattHeight).
     *  @param maxLoadPatterns describes the maximum number of pattern files that can be loaded.
     *  @param maxImagePatterns describes the maximum number of patterns that can be analyzed in a camera image.
     *  Reduce maxLoadPatterns and maxImagePatterns to reduce memory footprint.
     */
    TrackerSingleMarkerImpl(int imWidth, int imHeight, int pattWidth = 6, int pattHeight = 6, int pattSamples = 6,
            int maxLoadPatterns = 0, int maxImagePatterns = 8);
    ~TrackerSingleMarkerImpl();

    /// initializes TrackerSingleMarker
    /**
     *  nCamParamFile is the name of the camera parameter file
     *  nLogger is an instance which implements the ARToolKit::Logger interface
     */
    virtual bool init(const char* nCamParamFile, ARFloat nNearClip, ARFloat nFarClip);

    /// adds a pattern to ARToolKit
    /**
     *  pass the patterns filename
     */
    virtual int addPattern(const char* nFileName);

    /// calculates the transformation matrix
    /**
     *	pass the image as RGBX (32-bits)
     */
    virtual vector<int> calc(const uint8_t* nImage, ARMarkerInfo** nMarker_info = NULL, int* nNumMarkers = NULL);

    /// Sets the width and height of the patterns.
    virtual void setPatternWidth(ARFloat nWidth) {
        patt_width = nWidth;
    }

    /// Provides access to ARToolKit' patt_trans matrix
    /**
     *  This method is primarily for compatibility issues with code previously using
     *  ARToolKit rather than ARToolKitPlus. patt_trans is the original transformation
     *  matrix ARToolKit calculates rather than the OpenGL style version of this matrix
     *  that can be retrieved via getModelViewMatrix().
     */
    virtual void getARMatrix(ARFloat nMatrix[3][4]) const;

    /// Returns the confidence value of the currently best detected marker.
    virtual ARFloat getConfidence() const {
        return confidence;
    }

    virtual void selectDetectedMarker(const int id);

    virtual int selectBestMarkerByCf();

    size_t getMemoryRequirements();

protected:
    ARFloat confidence;
    ARFloat patt_width;
    ARFloat patt_center[2];
    ARFloat patt_trans[3][4];

    // save the results of last calc call
    ARMarkerInfo *marker_info;
    int marker_num;
};

} // namespace ARToolKitPlus

#endif //__ARTOOLKITPLUS_TRACKERSINGLEMARKERIMPL_HEADERFILE__
