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
 *  Pavel Rojtberg
 */

/*
 * Test the correctness and performance of ARToolKitPlus
 * useful for finding regressions when changing the library
 */

#include <cassert>
#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>
#include <ARToolKitPlus/TrackerSingleMarker.h>
#include <ARToolKitPlus/TrackerMultiMarker.h>
#include <ARToolKitPlus/Camera.h>

using namespace std;
using ARToolKitPlus::TrackerSingleMarker;
using ARToolKitPlus::TrackerMultiMarker;
using ARToolKitPlus::Camera;

const float exSingleMV[16] = {0.305463, 0.255588, -0.917261, 0,
                         0.951461, -0.119979, 0.283421, 0,
                        -0.0376131, -0.959312, -0.279831, 0,
                         1.52149, 1.48403, 4.62924, 1};

const float exMultiMV[16] = {0.986011, -0.0710274, -0.150788, 0,
                             0.0578373, -0.702653, 0.709179, 0,
                             -0.156323, -0.707979, -0.688715, 0,
                             0.633425, -12.3363, 168.69, 1};

void testMatrix(const float* data, const float* expected) {
    for (int i = 0; i < 16; ++i) {
        assert(abs(data[i] - expected[i]) < 0.1);
        // cout << data[i] << ", ";
    }
}

void testCamera(Camera* cam) {
    assert(abs(cam->mat[0][2]-129.689) < 0.1);
    assert(abs(cam->mat[1][2]-106.731) < 0.1);
    assert(abs(cam->mat[0][1]) < 0.1);
    assert(abs(cam->mat[1][1]-246.63) < 0.1);
}

int main(int argc, char** argv) {
    const int width = 320, height = 240, bpp = 1;
    const size_t numPixels = width * height * bpp;
    const char *imageSingle = "data/image_320_240_8_marker_id_simple_nr031.raw";
    const char *imageMulti = "data/markerboard_480-499.raw";

    char singleBuffer[numPixels];
    char multiBuffer[numPixels];

    ifstream imf(imageSingle, ios::binary);
    imf.read(singleBuffer, numPixels);
    imf.close();

    imf.open(imageMulti, ios::binary);
    imf.read(multiBuffer, numPixels);
    imf.close();

    TrackerSingleMarker ts(width, height);
    ts.init("data/PGR_M12x0.5_2.5mm.cal", 1.0f, 1000.0f);
    ts.setPixelFormat(ARToolKitPlus::PIXEL_FORMAT_LUM);
    ts.setThreshold(150);
    ts.setUndistortionMode(ARToolKitPlus::UNDIST_LUT);

    testCamera(ts.getCamera());

    ts.calc((uint8_t*)singleBuffer);
    int best = ts.selectBestMarkerByCf();
    assert(best == 31);
    testMatrix(ts.getModelViewMatrix(), exSingleMV);

    clock_t t1 = clock();

    for (int i = 0; i < 3000; ++i) {
        ts.calc((uint8_t*)singleBuffer);
    }

    // for 3000 iterations (O3)
    float time = float(clock() - t1)/CLOCKS_PER_SEC;
    cout << "SingleMarker Time: " << time << " (" << time/0.74 << ")" << endl;

    TrackerMultiMarker tm(width, height);
    tm.init("data/PGR_M12x0.5_2.5mm.cal", "data/markerboard_480-499.cfg", 1.0f, 1000.0f);
    tm.setPixelFormat(ARToolKitPlus::PIXEL_FORMAT_LUM);
    tm.setBorderWidth(0.125f);
    tm.setUndistortionMode(ARToolKitPlus::UNDIST_LUT);

    const int num = tm.calc((uint8_t*)multiBuffer);
    assert(num == 4);
    testMatrix(tm.getModelViewMatrix(), exMultiMV);

    t1 = clock();

    for (int i = 0; i < 500; ++i) {
        tm.calc((uint8_t*)multiBuffer);
    }

    // for 500 iterations (O3)
    time = float(clock() - t1)/CLOCKS_PER_SEC;
    cout << "MultiMarker Time: " << time << " (" << time/0.89 << ")" << endl;

    return 0;
}
