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

#include <ARToolKitPlus/Tracker.h>
#include <iostream>
#include <cassert>

using std::cerr;
using std::endl;
using std::vector;

namespace ARToolKitPlus {

int Tracker::screenWidth;
int Tracker::screenHeight;

Tracker::Tracker(int imWidth, int imHeight, int maxImagePatterns, int pattWidth, int pattHeight, int pattSamples,
        int maxLoadPatterns) :
    PATTERN_WIDTH(pattWidth), PATTERN_HEIGHT(pattHeight), PATTERN_SAMPLE_NUM(pattSamples),
            MAX_LOAD_PATTERNS(maxLoadPatterns), MAX_IMAGE_PATTERNS(maxImagePatterns),
            WORK_SIZE(1024 * MAX_IMAGE_PATTERNS), sprev_info(2, vector<arPrevInfo> (MAX_IMAGE_PATTERNS)), patf(0),
            pat(MAX_LOAD_PATTERNS, vector<vector<int> > (4, vector<int> (PATTERN_HEIGHT * PATTERN_WIDTH * 3))),
            patBW(MAX_LOAD_PATTERNS, vector<vector<int> > (4, vector<int> (PATTERN_HEIGHT * PATTERN_WIDTH * 3))),
            evec(EVEC_MAX, vector<ARFloat> (PATTERN_HEIGHT * PATTERN_WIDTH * 3)),
            evecBW(EVEC_MAX, vector<ARFloat> (PATTERN_HEIGHT * PATTERN_WIDTH * 3)) {
    screenWidth = imWidth;
    screenHeight = imHeight;

    // dynamically allocate template arguments
    if(MAX_LOAD_PATTERNS > 0) {
        patf = new int[MAX_LOAD_PATTERNS];
        std::fill(patf, patf+MAX_LOAD_PATTERNS, 0);
    }

    patpow = new ARFloat[MAX_LOAD_PATTERNS][4];
    patpowBW = new ARFloat[MAX_LOAD_PATTERNS][4];
    epat = new ARFloat[MAX_LOAD_PATTERNS][4][EVEC_MAX];
    epatBW = new ARFloat[MAX_LOAD_PATTERNS][4][EVEC_MAX];
    prev_info = new arPrevInfo[MAX_IMAGE_PATTERNS];
    marker_infoL = new ARMarkerInfo[MAX_IMAGE_PATTERNS];

    // set default value to RGB888
    pixelFormat = PIXEL_FORMAT_RGB;
    pixelSize = 3;

    binaryMarkerThreshold = -1;

    autoThreshold.enable = false;
    autoThreshold.numRandomRetries = 2;

    wmarker_num = 0;
    prev_num = 0;
    sprev_num[0] = sprev_num[1] = 0;

    marker_infoTWO = NULL;

    pattern_num = -1;

    evecf = 0;
    evecBWf = 0;

    // we allocate all large data dynamically
    //
    l_imageL = NULL;
    l_imageL_size = 0;

    workL = new int[WORK_SIZE];
    work2L = new int[WORK_SIZE * 7];
    wareaL = new int[WORK_SIZE];
    wclipL = new int[WORK_SIZE * 4];
    wposL = new ARFloat[WORK_SIZE * 2];

    // set all right side structures to NULL
    l_imageR = NULL;
    workR = NULL;
    work2R = NULL;
    wareaR = NULL;
    wclipR = NULL;
    wposR = NULL;

    //arDebug                 = 0;
    //arImage                 = NULL;
    arFittingMode = DEFAULT_FITTING_MODE;
    arImageProcMode = DEFAULT_IMAGE_PROC_MODE;

    arCamera = NULL;
    loadCachedUndist = false;
    //arCamera;
    arImXsize = arImYsize = 0;
    arTemplateMatchingMode = DEFAULT_TEMPLATE_MATCHING_MODE;
    arMatchingPCAMode = DEFAULT_MATCHING_PCA_MODE;
    arImageL = NULL;
    //arImageR                = NULL;

    markerMode = MARKER_ID_SIMPLE;

    RGB565_to_LUM8_LUT = NULL;

    relBorderWidth = 0.25f;

    undistMode = UNDIST_STD;
    undistO2ITable = NULL;
    arCameraObserv2Ideal_func = &Tracker::arCameraObserv2Ideal_std;

    vignetting.enabled = false;
    vignetting.corners = vignetting.leftright = vignetting.bottomtop = 0;

    bchProcessor = NULL;

    poseEstimator = POSE_ESTIMATOR_RPP;
    hullTrackingMode = HULL_OFF;
}

Tracker::~Tracker() {
    delete[] patf;
    delete[] patpow;
    delete[] patpowBW;
    delete[] epat;
    delete[] epatBW;
    delete[] prev_info;
    delete[] marker_infoL;
    delete arCamera;
    delete bchProcessor;
    delete[] l_imageL;
    delete[] workL;
    delete[] work2L;
    delete[] wareaL;
    delete[] wclipL;
    delete[] wposL;
    delete RGB565_to_LUM8_LUT;
    delete[] undistO2ITable;
    delete[] marker_infoTWO;
}

bool Tracker::setPixelFormat(PIXEL_FORMAT nFormat) {
    PIXEL_FORMAT oldFormat = pixelFormat;

    switch (pixelFormat = nFormat) {
    case PIXEL_FORMAT_LUM:
        pixelSize = 1;
        return true;

    case PIXEL_FORMAT_RGB565:
        pixelSize = 2;
        return true;

    case PIXEL_FORMAT_BGR:
    case PIXEL_FORMAT_RGB:
        pixelSize = 3;
        return true;

    case PIXEL_FORMAT_ABGR:
    case PIXEL_FORMAT_BGRA:
    case PIXEL_FORMAT_RGBA:
        pixelSize = 4;
        return true;

    default:
        pixelFormat = oldFormat;
        return false;
    }
}

void Tracker::checkImageBuffer() {
    // we have to take care here when using a memory manager that can not free memory
    // (usually this image buffer should only be built once - unless we change camera resolution)
    //

    int newSize = screenWidth * screenHeight;

    if (newSize == l_imageL_size)
        return;

    if (l_imageL)
        delete[] l_imageL;

    l_imageL_size = newSize;

    l_imageL = new int16_t[newSize];
}

bool Tracker::checkPixelFormat() {
    switch (pixelFormat) {
    case PIXEL_FORMAT_LUM:
        return pixelSize == 1;

    case PIXEL_FORMAT_RGB565:
        return pixelSize == 2;

    case PIXEL_FORMAT_BGR:
    case PIXEL_FORMAT_RGB:
        return pixelSize == 3;

    case PIXEL_FORMAT_ABGR:
    case PIXEL_FORMAT_BGRA:
    case PIXEL_FORMAT_RGBA:
        return pixelSize == 4;

    default:
        return false;
    }
}

bool Tracker::loadCameraFile(const char* nCamParamFile, ARFloat nNearClip, ARFloat nFarClip) {
    Camera* cam = new Camera();

    if (!cam->loadFromFile(nCamParamFile)) {
        cerr << "ARToolKitPlus: Camera parameter load error!" << endl;
        return false;
    }

    if (arCamera)
        delete arCamera;

    arCamera = NULL;

    setCamera(cam, nNearClip, nFarClip);
    return true;
}

void Tracker::setCamera(Camera* nCamera) {
    arCamera = nCamera;

    arCamera->changeFrameSize(screenWidth, screenHeight);
    arInitCparam(arCamera);

    // Comment out if you want to get the matrix for camera calibration
    // printf("MAT=\n");
    // printf("[ %f %f %f;\n",arCamera->mat[0][0],arCamera->mat[0][1],arCamera->mat[0][2]);
    // printf("%f %f %f;\n",arCamera->mat[1][0],arCamera->mat[1][1],arCamera->mat[1][2]);
    // printf("%f %f %f ]\n",arCamera->mat[2][0],arCamera->mat[2][1],arCamera->mat[2][2]);

    buildUndistO2ITable(arCamera);
}

void Tracker::setCamera(Camera* nCamera, ARFloat nNearClip, ARFloat nFarClip) {
    setCamera(nCamera);

    Camera gCparam = *arCamera;

    for (int i = 0; i < 4; i++)
        gCparam.mat[1][i] = (gCparam.ysize - 1) * (gCparam.mat[2][i]) - gCparam.mat[1][i];

    convertProjectionMatrixToOpenGLStyle(&gCparam, nNearClip, nFarClip, gl_cpara);
}

ARFloat Tracker::calcOpenGLMatrixFromMarker(ARMarkerInfo* nMarkerInfo, ARFloat nPatternCenter[2], ARFloat nPatternSize,
        ARFloat *nOpenGLMatrix) {
    ARFloat tmpTrans[3][4];

    ARFloat err = executeSingleMarkerPoseEstimator(nMarkerInfo, nPatternCenter, nPatternSize, tmpTrans);
    convertTransformationMatrixToOpenGLStyle(tmpTrans, nOpenGLMatrix);

    return err;
}

void Tracker::convertTransformationMatrixToOpenGLStyle(ARFloat para[3][4], ARFloat gl_para[16]) {
    int i, j;

    for (j = 0; j < 3; j++) {
        for (i = 0; i < 4; i++) {
            gl_para[i * 4 + j] = para[j][i];
        }
    }
    gl_para[0 * 4 + 3] = gl_para[1 * 4 + 3] = gl_para[2 * 4 + 3] = 0.0;
    gl_para[3 * 4 + 3] = 1.0;
}

bool Tracker::convertProjectionMatrixToOpenGLStyle(Camera *param, ARFloat gnear, ARFloat gfar, ARFloat m[16]) {
    return convertProjectionMatrixToOpenGLStyle2(param->mat, param->xsize, param->ysize, gnear, gfar, m);
}

bool Tracker::convertProjectionMatrixToOpenGLStyle2(ARFloat cparam[3][4], int width, int height, ARFloat gnear,
        ARFloat gfar, ARFloat m[16]) {
    ARFloat icpara[3][4];
    ARFloat trans[3][4];
    ARFloat p[3][3], q[4][4];
    int i, j;

    if (arCameraDecompMat(cparam, icpara, trans) < 0) {
        cerr << "gConvGLcpara: Parameter error!" << endl;
        return false;
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            p[i][j] = icpara[i][j] / icpara[2][2];
        }
    }
    q[0][0] = (2.0f * p[0][0] / width);
    q[0][1] = (2.0f * p[0][1] / width);
    q[0][2] = ((2.0f * p[0][2] / width) - 1.0f);
    q[0][3] = 0.0f;

    q[1][0] = 0.0f;
    q[1][1] = (2.0f * p[1][1] / height);
    q[1][2] = ((2.0f * p[1][2] / height) - 1.0f);
    q[1][3] = 0.0f;

    q[2][0] = 0.0f;
    q[2][1] = 0.0f;
    q[2][2] = (gfar + gnear) / (gfar - gnear);
    q[2][3] = -2.0f * gfar * gnear / (gfar - gnear);

    q[3][0] = 0.0f;
    q[3][1] = 0.0f;
    q[3][2] = 1.0f;
    q[3][3] = 0.0f;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            m[i + j * 4] = q[i][0] * trans[0][j] + q[i][1] * trans[1][j] + q[i][2] * trans[2][j];
        }
        m[i + 3 * 4] = q[i][0] * trans[0][3] + q[i][1] * trans[1][3] + q[i][2] * trans[2][3] + q[i][3];
    }

    return true;
}

bool Tracker::calcCameraMatrix(const char* nCamParamFile, ARFloat nNear, ARFloat nFar, ARFloat *nMatrix) {
    Camera* pCam = new Camera();

    if (!pCam->loadFromFile(nCamParamFile)) {
        cerr << "ARToolKitPlus: Camera parameter load error!" << endl;
        return false;
    }

    pCam->changeFrameSize(Tracker::screenWidth, Tracker::screenHeight);

    int i;
    for (i = 0; i < 4; i++)
        pCam->mat[1][i] = (pCam->ysize - 1) * (pCam->mat[2][i]) - pCam->mat[1][i];

    ARFloat glcpara[16];

    if (!convertProjectionMatrixToOpenGLStyle(pCam, nNear, nFar, glcpara))
        return false;

    // convert to float (in case of ARFloat is def'ed to doubled
    for (i = 0; i < 16; i++)
        nMatrix[i] = (ARFloat) glcpara[i];

    return (true);
}

void Tracker::changeCameraSize(int nWidth, int nHeight) {
    screenWidth = nWidth;
    screenHeight = nHeight;

    arCamera->changeFrameSize(nWidth, nHeight);
    arInitCparam(arCamera);
}

void Tracker::setUndistortionMode(UNDIST_MODE nMode) {
    undistMode = nMode;
    switch (undistMode) {
    case UNDIST_NONE:
        arCameraObserv2Ideal_func = &Tracker::arCameraObserv2Ideal_none;
        break;

    case UNDIST_STD:
        arCameraObserv2Ideal_func = &Tracker::arCameraObserv2Ideal_std;
        break;

    case UNDIST_LUT:
        arCameraObserv2Ideal_func = &Tracker::arCameraObserv2Ideal_LUT;
        break;
    }
}

bool Tracker::setPoseEstimator(POSE_ESTIMATOR nMode) {
    poseEstimator = nMode;

    return true;
}

ARFloat Tracker::executeSingleMarkerPoseEstimator(ARMarkerInfo *marker_info, ARFloat center[2], ARFloat width,
        ARFloat conv[3][4]) {
    switch (poseEstimator) {
    case POSE_ESTIMATOR_ORIGINAL:
        return arGetTransMat(marker_info, center, width, conv);

    case POSE_ESTIMATOR_ORIGINAL_CONT:
        return arGetTransMatCont(marker_info, conv, center, width, conv);

    case POSE_ESTIMATOR_RPP:
        return rppGetTransMat(marker_info, center, width, conv);
    }

    return -1.0f;
}

ARFloat Tracker::executeMultiMarkerPoseEstimator(ARMarkerInfo *marker_info, int marker_num, ARMultiMarkerInfoT *config) {
    if(marker_num == 0) {
        // dont try anything if we have no markers
        return -1;
    }

    if (hullTrackingMode != HULL_OFF) {
        return arMultiGetTransMatHull(marker_info, marker_num, config);
    }

    switch (poseEstimator) {
    case POSE_ESTIMATOR_ORIGINAL:
        return arMultiGetTransMat(marker_info, marker_num, config);

    case POSE_ESTIMATOR_ORIGINAL_CONT:
        return arMultiGetTransMat(marker_info, marker_num, config);

    case POSE_ESTIMATOR_RPP:
        return rppMultiGetTransMat(marker_info, marker_num, config);
    }

    return -1.0f;
}

void Tracker::activateVignettingCompensation(bool nEnable, int nCorners, int nLeftRight, int nTopBottom) {
    vignetting.enabled = nEnable;
    vignetting.corners = nCorners;
    vignetting.leftright = nLeftRight;
    vignetting.bottomtop = nTopBottom;
}

void Tracker::setMarkerMode(MARKER_MODE nMarkerMode) {
    markerMode = nMarkerMode;
}

static void convertPixel16To24(unsigned short nPixel, unsigned char& nRed, unsigned char& nGreen, unsigned char& nBlue) {
    const unsigned short RED_MASK = 0x1F << 11;
    const unsigned short GREEN_MASK = 0x3F << 5;
    const unsigned short BLUE_MASK = 0x1F;

    nRed = (unsigned char) ((nPixel & RED_MASK) >> 8);
    nGreen = (unsigned char) ((nPixel & GREEN_MASK) >> 3);
    nBlue = (unsigned char) ((nPixel & BLUE_MASK) << 3);
}

void Tracker::checkRGB565LUT() {
    int i;

    if (RGB565_to_LUM8_LUT)
        return;

    RGB565_to_LUM8_LUT = new unsigned char[LUM_TABLE_SIZE];

#ifdef SMALL_LUM8_TABLE
    for(i=0; i<(LUM_TABLE_SIZE >> 6); i++)
    {
        unsigned char red,green,blue;

        convertPixel16To24(i<<6, red, green, blue);
        RGB565_to_LUM8_LUT[i] = (unsigned char)(((red<<1) + (green<<2) + green + blue)>>3);
    }
#else
    for (i = 0; i < LUM_TABLE_SIZE; i++) {
        unsigned char red, green, blue;

        convertPixel16To24(i, red, green, blue);
        RGB565_to_LUM8_LUT[i] = (unsigned char) (((red << 1) + (green << 2) + green + blue) >> 3);
    }
#endif //SMALL_LUM8_TABLE
}
} // namespace ARToolKitPlus
