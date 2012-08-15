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

#include <ARToolKitPlus/TrackerMultiMarker.h>

namespace ARToolKitPlus {

TrackerMultiMarker::TrackerMultiMarker(int imWidth, int imHeight, int maxImagePatterns, int pattWidth, int pattHeight,
        int pattSamples, int maxLoadPatterns) :
    Tracker(imWidth, imHeight, maxImagePatterns, pattWidth, pattHeight, pattSamples, maxLoadPatterns) {
    useDetectLite = true;
    numDetected = 0;

    config = 0;
    thresh = 150;

    detectedMarkerIDs = new int[MAX_IMAGE_PATTERNS];
    detectedMarkers = new ARMarkerInfo[MAX_IMAGE_PATTERNS];
}

TrackerMultiMarker::~TrackerMultiMarker() {
	delete[] detectedMarkerIDs;
	delete[] detectedMarkers;

	if (config)
		arMultiFreeConfig(config);
}

bool TrackerMultiMarker::init(const char* const nCamParamFile, const char* const nMultiFile, ARFloat nNearClip,
		ARFloat nFarClip) {
    // init some "static" members from artoolkit
    // (some systems don't like such large global members
    // so we allocate this manually)
	if (this->marker_infoTWO == NULL) {
		this->marker_infoTWO = new ARMarkerInfo2[MAX_IMAGE_PATTERNS];
	}

	if (config) {
		arMultiFreeConfig(config);
	}

	if ((config = arMultiReadConfigFile(nMultiFile)) == NULL) {
		return false;
	}

    // initialize applications
    if (nCamParamFile) {
        return loadCameraFile(nCamParamFile, nNearClip, nFarClip);
    }

    return true;
}

int TrackerMultiMarker::calc(const uint8_t* nImage) {
	numDetected = 0;
	int tmpNumDetected;
	ARMarkerInfo *tmp_markers;

	if (useDetectLite) {
		if (arDetectMarkerLite(nImage, this->thresh, &tmp_markers, &tmpNumDetected) < 0)
			return 0;
	} else {
		if (arDetectMarker(nImage, this->thresh, &tmp_markers, &tmpNumDetected) < 0)
			return 0;
	}

	for (int i = 0; i < tmpNumDetected; i++)
		if (tmp_markers[i].id != -1) {
			detectedMarkers[numDetected] = tmp_markers[i];
			detectedMarkerIDs[numDetected++] = tmp_markers[i].id;
			if (numDetected >= MAX_IMAGE_PATTERNS) // increase this value if more markers should be possible to be detected in one image...
				break;
		}

	if (executeMultiMarkerPoseEstimator(tmp_markers, tmpNumDetected, config) < 0)
		return 0;

	convertTransformationMatrixToOpenGLStyle(config->trans, this->gl_para);
	return numDetected;
}

void TrackerMultiMarker::getDetectedMarkers(int*& nMarkerIDs) {
	nMarkerIDs = detectedMarkerIDs;
}

void TrackerMultiMarker::getARMatrix(ARFloat nMatrix[3][4]) const {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 4; j++)
			nMatrix[i][j] = config->trans[i][j];
}

} // namespace ARToolKitPlus
