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

// Simple example to demonstrate usage of ARToolKitPlus
// This sample does not open any graphics window. It just
// loads test images and shows how to use the ARToolKitPlus API.

#include <cstdio>
#include <ARToolKitPlus/TrackerSingleMarker.h>

using ARToolKitPlus::TrackerSingleMarker;

int main(int argc, char** argv) {
    // switch this between true and false to test
    // simple-id versus BCH-id markers
    const bool useBCH = false;

    const int width = 320, height = 240, bpp = 1;
    size_t numPixels = width * height * bpp;
    size_t numBytesRead;
    const char *fName = useBCH ? "data/image_320_240_8_marker_id_bch_nr0100.raw"
            : "data/image_320_240_8_marker_id_simple_nr031.raw";

    unsigned char cameraBuffer[numPixels];

    // try to load a test camera image.
    // these images files are expected to be simple 8-bit raw pixel
    // data without any header. the images are expetected to have a
    // size of 320x240.
    if (FILE* fp = fopen(fName, "rb")) {
        numBytesRead = fread(cameraBuffer, 1, numPixels, fp);
        fclose(fp);
    } else {
        printf("Failed to open %s\n", fName);
        return -1;
    }

    if (numBytesRead != numPixels) {
        printf("Failed to read %s\n", fName);
        return -1;
    }

    // create a tracker that does:
    //  - 6x6 sized marker images (required for binary markers)
    //  - samples at a maximum of 6x6
    //  - works with luminance (gray) images
    //  - can load a maximum of 0 non-binary pattern
    //  - can detect a maximum of 8 patterns in one imagege
    TrackerSingleMarker tracker(width, height, 8, 6, 6, 6, 0);

    tracker.setPixelFormat(ARToolKitPlus::PIXEL_FORMAT_LUM);
    //tracker.setLoadUndistLUT(true);

    // load a camera file.
    if (!tracker.init("data/PGR_M12x0.5_2.5mm.cal", 1.0f, 1000.0f)) // load MATLAB file
    {
        printf("ERROR: init() failed\n");
        return -1;
    }

    tracker.getCamera()->printSettings();

    // define size of the marker in OpenGL units
    tracker.setPatternWidth(2.0);

    // the marker in the BCH test image has a thin border...
    tracker.setBorderWidth(useBCH ? 0.125 : 0.25);

    // set a threshold. alternatively we could also activate automatic thresholding
    tracker.setThreshold(150);

    // let's use lookup-table undistortion for high-speed
    // note: LUT only works with images up to 1024x1024
    tracker.setUndistortionMode(ARToolKitPlus::UNDIST_LUT);

    // switch to simple ID based markers
    // use the tool in tools/IdPatGen to generate markers
    tracker.setMarkerMode(useBCH ? ARToolKitPlus::MARKER_ID_BCH : ARToolKitPlus::MARKER_ID_SIMPLE);

    // do the OpenGL camera setup
    // glMatrixMode(GL_PROJECTION)
    // glLoadMatrixf(tracker.getProjectionMatrix());

    // here we go, just two calls to find the camera pose
    std::vector<int> markerId = tracker.calc(cameraBuffer);
    tracker.selectBestMarkerByCf();
    float conf = tracker.getConfidence();

    // use the result of calc() to setup the OpenGL transformation
    // glMatrixMode(GL_MODELVIEW)
    // glLoadMatrixf(tracker.getModelViewMatrix());

    printf("\n\nFound marker %d  (confidence %d%%)\n\nPose-Matrix:\n  ", markerId[0], (int(conf * 100.0f)));
    for (int i = 0; i < 16; i++)
        printf("%.2f  %s", tracker.getModelViewMatrix()[i], (i % 4 == 3) ? "\n  " : "");

    return 0;
}
