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

#ifndef __ARTOOLKITPLUS_TRACKERMULTIMARKERIMPL_HEADERFILE__
#define __ARTOOLKITPLUS_TRACKERMULTIMARKERIMPL_HEADERFILE__

#include <ARToolKitPlus/TrackerMultiMarker.h>
#include <ARToolKitPlus/TrackerImpl.h>

namespace ARToolKitPlus {

/**
 * TrackerMultiMarkerImpl implements the TrackerMultiMarker interface
 */
class TrackerMultiMarkerImpl: public TrackerMultiMarker, protected TrackerImpl {
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
    TrackerMultiMarkerImpl(int imWidth, int imHeight, int pattWidth = 6, int pattHeight = 6, int pattSamples = 6,
            int maxLoadPatterns = 0, int maxImagePatterns = 8);
    ~TrackerMultiMarkerImpl();

    /// initializes ARToolKit
    /// initializes TrackerSingleMarker
    /**
     *  nCamParamFile is the name of the camera parameter file
     *  nNearClip & nFarClip are near and far clipping values for the OpenGL projection matrix
     *  nLogger is an instance which implements the ARToolKit::Logger interface
     */
    virtual bool init(const char* nCamParamFile, const char* nMultiFile, ARFloat nNearClip, ARFloat nFarClip);

    /// calculates the transformation matrix
    /**
     *	pass the image as RGBX (32-bits) in 320x240 pixels.
     */
    virtual int calc(const unsigned char* nImage);

    /// Returns the number of detected markers used for multi-marker tracking
    virtual int getNumDetectedMarkers() const {
        return numDetected;
    }

    /// Enables usage of arDetectMarkerLite. Otherwise arDetectMarker is used
    /**
     * In general arDetectMarker is more powerful since it keeps history about markers.
     * In some cases such as very low camera refresh rates it is advantegous to change this.
     * Using the non-lite version treats each image independent.
     */
    virtual void setUseDetectLite(bool nEnable) {
        useDetectLite = nEnable;
    }

    virtual void getDetectedMarkers(int*& nMarkerIDs);

    virtual const ARMarkerInfo& getDetectedMarker(int nWhich) const {
        return detectedMarkers[nWhich];
    }

    virtual const ARMultiMarkerInfoT* getMultiMarkerConfig() const {
        return config;
    }

    /// Provides access to ARToolKit' internal version of the transformation matrix
    /**
     *  This method is primarily for compatibility issues with code previously using
     *  ARToolKit rather than ARToolKitPlus. This is the original transformation
     *  matrix ARToolKit calculates rather than the OpenGL style version of this matrix
     *  that can be retrieved via getModelViewMatrix().
     */
    virtual void getARMatrix(ARFloat nMatrix[3][4]) const;

    //
    // reimplement TrackerImpl into TrackerSingleMarker interface
    //
    // TODO: something like 'using cleanup;' would be nicer but does seem to work...
    //
    void cleanup() {
        TrackerImpl::cleanup();
    }
    bool setPixelFormat(PIXEL_FORMAT nFormat) {
        return TrackerImpl::setPixelFormat(nFormat);
    }
    bool loadCameraFile(const char* nCamParamFile, ARFloat nNearClip, ARFloat nFarClip) {
        return TrackerImpl::loadCameraFile(nCamParamFile, nNearClip, nFarClip);
    }
    void setLoadUndistLUT(bool nSet) {
        TrackerImpl::setLoadUndistLUT(nSet);
    }
    int arDetectMarker(uint8_t *dataPtr, int thresh, ARMarkerInfo **marker_info, int *marker_num) {
        return TrackerImpl::arDetectMarker(dataPtr, thresh, marker_info, marker_num);
    }
    int arDetectMarkerLite(uint8_t *dataPtr, int thresh, ARMarkerInfo **marker_info, int *marker_num) {
        return TrackerImpl::arDetectMarkerLite(dataPtr, thresh, marker_info, marker_num);
    }
    ARFloat arMultiGetTransMat(ARMarkerInfo *marker_info, int marker_num, ARMultiMarkerInfoT *config) {
        return TrackerImpl::arMultiGetTransMat(marker_info, marker_num, config);
    }
    ARFloat arGetTransMat(ARMarkerInfo *marker_info, ARFloat center[2], ARFloat width, ARFloat conv[3][4]) {
        return TrackerImpl::arGetTransMat(marker_info, center, width, conv);
    }
    ARFloat arGetTransMatCont(ARMarkerInfo *marker_info, ARFloat prev_conv[3][4], ARFloat center[2], ARFloat width,
            ARFloat conv[3][4]) {
        return TrackerImpl::arGetTransMatCont(marker_info, prev_conv, center, width, conv);
    }
    ARFloat rppMultiGetTransMat(ARMarkerInfo *marker_info, int marker_num, ARMultiMarkerInfoT *config) {
        return TrackerImpl::rppMultiGetTransMat(marker_info, marker_num, config);
    }
    ARFloat rppGetTransMat(ARMarkerInfo *marker_info, ARFloat center[2], ARFloat width, ARFloat conv[3][4]) {
        return TrackerImpl::rppGetTransMat(marker_info, center, width, conv);
    }
    int arLoadPatt(char *filename) {
        return TrackerImpl::arLoadPatt(filename);
    }
    int arFreePatt(int patno) {
        return TrackerImpl::arFreePatt(patno);
    }
    int arMultiFreeConfig(ARMultiMarkerInfoT *config) {
        return TrackerImpl::arMultiFreeConfig(config);
    }
    ARMultiMarkerInfoT *arMultiReadConfigFile(const char *filename) {
        return TrackerImpl::arMultiReadConfigFile(filename);
    }
    void activateBinaryMarker(int nThreshold) {
        TrackerImpl::activateBinaryMarker(nThreshold);
    }
    void setMarkerMode(MARKER_MODE nMarkerMode) {
        TrackerImpl::setMarkerMode(nMarkerMode);
    }
    void activateVignettingCompensation(bool nEnable, int nCorners = 0, int nLeftRight = 0, int nTopBottom = 0) {
        TrackerImpl::activateVignettingCompensation(nEnable, nCorners, nLeftRight, nTopBottom);
    }
    void changeCameraSize(int nWidth, int nHeight) {
        TrackerImpl::changeCameraSize(nWidth, nHeight);
    }
    void setUndistortionMode(UNDIST_MODE nMode) {
        TrackerImpl::setUndistortionMode(nMode);
    }
    bool setPoseEstimator(POSE_ESTIMATOR nMethod) {
        return TrackerImpl::setPoseEstimator(nMethod);
    }
    void setHullMode(HULL_TRACKING_MODE nMode) {
        TrackerImpl::setHullMode(nMode);
    }
    void setBorderWidth(ARFloat nFraction) {
        TrackerImpl::setBorderWidth(nFraction);
    }
    void setThreshold(int nValue) {
        TrackerImpl::setThreshold(nValue);
    }
    int getThreshold() const {
        return TrackerImpl::getThreshold();
    }
    void activateAutoThreshold(bool nEnable) {
        TrackerImpl::activateAutoThreshold(nEnable);
    }
    bool isAutoThresholdActivated() const {
        return TrackerImpl::isAutoThresholdActivated();
    }
    void setNumAutoThresholdRetries(int nNumRetries) {
        TrackerImpl::setNumAutoThresholdRetries(nNumRetries);
    }
    const ARFloat* getModelViewMatrix() const {
        return TrackerImpl::getModelViewMatrix();
    }
    const ARFloat* getProjectionMatrix() const {
        return TrackerImpl::getProjectionMatrix();
    }
    const char* getDescription() {
        return TrackerImpl::getDescription();
    }
    PIXEL_FORMAT getPixelFormat() const {
        return static_cast<PIXEL_FORMAT> (TrackerImpl::getPixelFormat());
    }
    int getBitsPerPixel() const {
        return static_cast<PIXEL_FORMAT> (TrackerImpl::getBitsPerPixel());
    }
    int getNumLoadablePatterns() const {
        return TrackerImpl::getNumLoadablePatterns();
    }
    void setImageProcessingMode(IMAGE_PROC_MODE nMode) {
        TrackerImpl::setImageProcessingMode(nMode);
    }
    Camera* getCamera() {
        return TrackerImpl::getCamera();
    }
    void setCamera(Camera* nCamera) {
        TrackerImpl::setCamera(nCamera);
    }
    void setCamera(Camera* nCamera, ARFloat nNearClip, ARFloat nFarClip) {
        TrackerImpl::setCamera(nCamera, nNearClip, nFarClip);
    }
    ARFloat calcOpenGLMatrixFromMarker(ARMarkerInfo* nMarkerInfo, ARFloat nPatternCenter[2], ARFloat nPatternSize,
            ARFloat *nOpenGLMatrix) {
        return TrackerImpl::calcOpenGLMatrixFromMarker(nMarkerInfo, nPatternCenter, nPatternSize, nOpenGLMatrix);
    }
    ARFloat executeSingleMarkerPoseEstimator(ARMarkerInfo *marker_info, ARFloat center[2], ARFloat width,
            ARFloat conv[3][4]) {
        return TrackerImpl::executeSingleMarkerPoseEstimator(marker_info, center, width, conv);
    }
    ARFloat executeMultiMarkerPoseEstimator(ARMarkerInfo *marker_info, int marker_num, ARMultiMarkerInfoT *config) {
        return TrackerImpl::executeMultiMarkerPoseEstimator(marker_info, marker_num, config);
    }
    const CornerPoints& getTrackedCorners() const {
        return TrackerImpl::getTrackedCorners();
    }

    size_t getMemoryRequirements();

protected:
    int numDetected;
    bool useDetectLite;

    ARMultiMarkerInfoT *config;
    int *detectedMarkerIDs;
    ARMarkerInfo *detectedMarkers;
};

}
; // namespace ARToolKitPlus

#endif //__ARTOOLKITPLUS_TRACKERMULTIMARKERIMPL_HEADERFILE__
