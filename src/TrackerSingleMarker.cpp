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

#include <iostream>
#include <ARToolKitPlus/TrackerSingleMarker.h>

using std::cerr;
using std::endl;

namespace ARToolKitPlus {

TrackerSingleMarker::TrackerSingleMarker(int imWidth, int imHeight, int maxImagePatterns, int pattWidth, int pattHeight,
        int pattSamples, int maxLoadPatterns) :
    Tracker(imWidth, imHeight, maxImagePatterns, pattWidth, pattHeight, pattSamples, maxLoadPatterns) {
    thresh = 100;

    patt_width = 2.0;
    patt_center[0] = patt_center[1] = 0.0;
}

bool TrackerSingleMarker::init(const char* nCamParamFile, ARFloat nNearClip, ARFloat nFarClip) {
    if (!this->checkPixelFormat()) {
        cerr << "ARToolKitPlus: Invalid Pixel Format!" << endl;
        return false;
    }

    // init some "static" members from artoolkit
    // (some systems don't like such large global members
    // so we allocate this manually)
    if (this->marker_infoTWO == NULL) {
        this->marker_infoTWO = new ARMarkerInfo2[MAX_IMAGE_PATTERNS];
    }

    // initialize applications
    if (nCamParamFile) {
        return loadCameraFile(nCamParamFile, nNearClip, nFarClip);
    }

    return true;
}

std::vector<int> TrackerSingleMarker::calc(const uint8_t* nImage, ARMarkerInfo** nMarker_info, int* nNumMarkers) {
    std::vector<int> detected;

    if (nImage == NULL)
        return detected;

    confidence = 0.0f;

    // detect the markers in the video frame
    //
    if (arDetectMarker(nImage, this->thresh, &marker_info, &marker_num) < 0) {
        return detected;
    }

    // copy all valid ids
    for (int j = 0; j < marker_num; j++) {
        if (marker_info[j].id != -1) {
            detected.push_back(marker_info[j].id);
        }
    }

    if (nMarker_info)
        *nMarker_info = marker_info;

    if (nNumMarkers)
        *nNumMarkers = marker_num;

    return detected;
}

int TrackerSingleMarker::selectBestMarkerByCf() {
    // find best visible marker
    int best = -1;

    for (int j = 0; j < marker_num; j++) {
        if (marker_info[j].id != -1) {
            if (best == -1)
                best = j;
            else if (marker_info[best].cf < marker_info[j].cf)
                best = j;
        }
    }

    if (best != -1) {
        // there was something detected
        best = marker_info[best].id; // we want the id and not the offset
        selectDetectedMarker(best);
    }

    return best;
}

void TrackerSingleMarker::selectDetectedMarker(const int id) {
    for (int i = 0; i < marker_num; i++) {
        if (marker_info[i].id == id) {
            executeSingleMarkerPoseEstimator(&marker_info[i], patt_center, patt_width, patt_trans);
            convertTransformationMatrixToOpenGLStyle(patt_trans, this->gl_para);
            confidence = marker_info[i].cf;
        }
    }
}

int TrackerSingleMarker::addPattern(const char* nFileName) {
    int patt_id = arLoadPatt(const_cast<char*> (nFileName));

    if (patt_id < 0) {
        cerr << "ARToolKitPlus: error loading pattern " << nFileName << endl;
    }

    return patt_id;
}

void TrackerSingleMarker::getARMatrix(ARFloat nMatrix[3][4]) const {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 4; j++)
            nMatrix[i][j] = patt_trans[i][j];
}

} // namespace ARToolKitPlus
