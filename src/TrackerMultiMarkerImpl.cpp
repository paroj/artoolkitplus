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
 Pavel Rojtberg
 */

#include <ARToolKitPlus/TrackerMultiMarkerImpl.h>

namespace ARToolKitPlus {

TrackerMultiMarkerImpl::TrackerMultiMarkerImpl(int imWidth, int imHeight, int pattWidth, int pattHeight, int pattSamples, int maxLoadPatterns, int maxImagePatterns) :
	TrackerImpl::TrackerImpl(imWidth, imHeight, pattWidth, pattHeight, pattSamples, maxLoadPatterns, maxImagePatterns) {
	useDetectLite = true;
	numDetected = 0;

	config = 0;

	this->thresh = 150;
	detectedMarkerIDs = new int[this->MAX_IMAGE_PATTERNS];
	detectedMarkers = new ARMarkerInfo[this->MAX_IMAGE_PATTERNS];
}

TrackerMultiMarkerImpl::~TrackerMultiMarkerImpl() {
	cleanup();
	delete[] detectedMarkerIDs;
	delete[] detectedMarkers;
	if (config)
		arMultiFreeConfig( config);
}

bool TrackerMultiMarkerImpl::init(const char* nCamParamFile, const char* nMultiFile, ARFloat nNearClip,
		ARFloat nFarClip) {
	// init some "static" from TrackerMultiMarker
	//
	if (this->marker_infoTWO == NULL)
		this->marker_infoTWO = new ARMarkerInfo2[TrackerImpl::MAX_IMAGE_PATTERNS];

	if (!loadCameraFile(nCamParamFile, nNearClip, nFarClip))
		return false;

	if (config)
		arMultiFreeConfig( config);

	if ((config = arMultiReadConfigFile(nMultiFile)) == NULL)
		return false;

	//printf("INFO: %d markers loaded from config file\n", config->marker_num);

	return true;
}

int TrackerMultiMarkerImpl::calc(const unsigned char* nImage) {
	numDetected = 0;
	int tmpNumDetected;
	ARMarkerInfo *tmp_markers;

	if (useDetectLite) {
		if (arDetectMarkerLite(const_cast<unsigned char*> (nImage), this->thresh, &tmp_markers, &tmpNumDetected) < 0)
			return 0;
	} else {
		if (arDetectMarker(const_cast<unsigned char*> (nImage), this->thresh, &tmp_markers, &tmpNumDetected) < 0)
			return 0;
	}

	for (int i = 0; i < tmpNumDetected; i++)
		if (tmp_markers[i].id != -1) {
			detectedMarkers[numDetected] = tmp_markers[i];
			detectedMarkerIDs[numDetected++] = tmp_markers[i].id;
			if (numDetected >= this->MAX_IMAGE_PATTERNS) // increase this value if more markers should be possible to be detected in one image...
				break;
		}

	if (executeMultiMarkerPoseEstimator(tmp_markers, tmpNumDetected, config) < 0)
		return 0;

	convertTransformationMatrixToOpenGLStyle(config->trans, this->gl_para);
	return numDetected;
}

void TrackerMultiMarkerImpl::getDetectedMarkers(int*& nMarkerIDs) {
	nMarkerIDs = detectedMarkerIDs;
}

void TrackerMultiMarkerImpl::getARMatrix(ARFloat nMatrix[3][4]) const {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 4; j++)
			nMatrix[i][j] = config->trans[i][j];
}

size_t TrackerMultiMarkerImpl::getMemoryRequirements() {
	size_t size = sizeof(TrackerMultiMarkerImpl);

	size += TrackerImpl::getDynamicMemoryRequirements();

	return size;
}

} // namespace ARToolKitPlus
