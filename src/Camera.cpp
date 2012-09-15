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
 *  Thomas Pintaric
 *  Pavel Rojtberg
 */

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include <ARToolKitPlus/Camera.h>

#ifdef AR_WITH_BOOST
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#endif

using namespace std;

#define CAMERA_ADV_HEADER "ARToolKitPlus_CamCal_Rev02"
#define CAMERA_ADV_MAX_UNDIST_ITERATIONS 20

namespace ARToolKitPlus {

Camera::Camera() : xsize(-1), ysize(-1), undist_iterations(0) {
    std::fill(&mat[0][0], &mat[0][0]+3*4, 0);
}

bool Camera::loadARTKCalib(const char* filename) {
    string hdr;

    ifstream camf(filename);
    camf.exceptions(ifstream::eofbit | ifstream::failbit | ifstream::badbit);

    getline(camf, hdr);
    hdr = hdr.substr(0, strlen(CAMERA_ADV_HEADER));

    if (hdr != CAMERA_ADV_HEADER) {
        camf.close();
        return false;
    }

    try {
        camf >> xsize >> ysize;
        camf >> cc[0] >> cc[1];
        camf >> fc[0] >> fc[1];

        for(int i = 0; i < 6; i++) {
            camf >> kc[i];
        }

        camf >> undist_iterations;
    } catch (ifstream::failure& e) {
        cerr << "Could not read Camera calibration file" << endl;
        camf.close();
        return false;
    }

    camf.close();

    undist_iterations = min(undist_iterations, CAMERA_ADV_MAX_UNDIST_ITERATIONS);

    mat[0][0] = fc[0]; // fc_x
    mat[1][1] = fc[1]; // fc_y
    mat[0][2] = cc[0]; // cc_x
    mat[1][2] = cc[1]; // cc_y
    mat[2][2] = 1.0;

    return true;
}

bool Camera::loadFromFile(const string& filename) {
    if(filename.substr(filename.length() - 3, 3) == "xml") {
        return loadOpenCVCalib(filename.c_str());
    } else {
        return loadARTKCalib(filename.c_str());
    }
}

bool Camera::loadOpenCVCalib(const char* filename) {
#ifdef AR_WITH_BOOST
    using namespace boost::property_tree;

    stringstream ss;
    ptree xmlfile;

    try {
        read_xml(filename, xmlfile);

        xsize = xmlfile.get<int>("opencv_storage.image_width");
        ysize = xmlfile.get<int>("opencv_storage.image_height");

        ss.str(xmlfile.get<string>("opencv_storage.distortion_coefficients.data"));

        for (int i = 0; i < 4; i++) {
            ss >> kc[i];
        }
        kc[4] = 0;
        kc[5] = 10;

        ss.str(xmlfile.get<string>("opencv_storage.camera_matrix.data"));

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                ss >> mat[i][j];
            }
        }
    } catch (ptree_error& err) {
        cerr << "Could not read Camera calibration file: " << err.what() << endl;
        return false;
    }

    return true;
#else
    cerr << "ARToolKitPlus compiled without OpenCV Camera calibration support!" << endl;
    return false;
#endif
}


void Camera::observ2Ideal(ARFloat ox, ARFloat oy, ARFloat *ix, ARFloat *iy) {
    if (undist_iterations <= 0) {
        *ix = ox;
        *iy = oy;
    } else {
        const ARFloat xd[2] = { (ox - cc[0]) / fc[0], (oy - cc[1]) / fc[1] };
        const ARFloat k1 = kc[0];
        const ARFloat k2 = kc[1];
        const ARFloat k3 = kc[4];
        const ARFloat p1 = kc[2];
        const ARFloat p2 = kc[3];

        ARFloat x[2] = { xd[0], xd[1] };
        for (int kk = 0; kk < undist_iterations; kk++) {
            const ARFloat x0_sq = (x[0] * x[0]);
            const ARFloat x1_sq = (x[1] * x[1]);
            const ARFloat x0_x1 = (x[0] * x[1]);
            const ARFloat r_2 = x0_sq + x1_sq;
            const ARFloat r_2_sq = (r_2 * r_2);
            const ARFloat k_radial = 1 + k1 * r_2 + k2 * (r_2_sq) + k3 * (r_2 * r_2_sq);
            const ARFloat delta_x[2] = { 2 * p1 * x0_x1 + p2 * (r_2 + 2 * x0_sq), p1 * (r_2 + 2 * x1_sq) + 2 * p2
                    * x0_x1 };
            x[0] = xd[0] - delta_x[0];
            x[1] = xd[1] - delta_x[1];
            x[0] /= k_radial;
            x[1] /= k_radial;
        }

        *ix = (x[0] * fc[0]) + cc[0];
        *iy = (x[1] * fc[1]) + cc[1];
    }
}

void Camera::ideal2Observ(ARFloat ix, ARFloat iy, ARFloat *ox, ARFloat *oy) {
    const ARFloat xu[2] = { (ix - cc[0]) / fc[0], (iy - cc[1]) / fc[1] };

    const ARFloat r2 = (xu[0] * xu[0]) + (xu[1] * xu[1]);
    const ARFloat r4 = r2 * r2;
    const ARFloat r6 = r4 * r2;
    const ARFloat cdist = 1 + kc[0] * r2 + kc[1] * r4 + kc[4] * r6;

    const ARFloat a1 = 2 * xu[0] * xu[1];
    const ARFloat a2 = r2 + 2 * (xu[0] * xu[0]);
    const ARFloat a3 = r2 + 2 * (xu[1] * xu[1]);

    *ox = (xu[0] * cdist) + (kc[2] * a1 + kc[3] * a2);
    *oy = (xu[1] * cdist) + (kc[2] * a3 + kc[3] * a1);
}

Camera* Camera::clone() {
    Camera* pCam = new Camera();
    pCam->xsize = xsize;
    pCam->ysize = ysize;
    unsigned int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 4; j++)
            pCam->mat[i][j] = mat[i][j];
    //for (i = 0; i < 4; i++)
    //    pCam->dist_factor[i] = dist_factor[i];
    pCam->cc[0] = cc[0];
    pCam->cc[1] = cc[1];
    pCam->fc[0] = fc[0];
    pCam->fc[1] = fc[1];
    for (i = 0; i < 6; i++)
        pCam->kc[i] = kc[i];
    pCam->undist_iterations = undist_iterations;
    return ((Camera*) pCam);
}

bool Camera::changeFrameSize(const int frameWidth, const int frameHeight) {
    if (frameWidth <= 0 || frameHeight <= 0)
        return (false);
    const ARFloat scale = (ARFloat) frameWidth / (ARFloat) xsize;
    xsize = frameWidth;
    ysize = frameHeight;

    for (int i = 0; i < 4; i++) {
        mat[0][i] *= scale;
        mat[1][i] *= scale;
    }

    cc[0] *= scale;
    cc[1] *= scale;
    fc[0] *= scale;
    fc[1] *= scale;

    return (true);
}

void Camera::printSettings() {
    printf("ARToolKitPlus: CamSize %d , %d\n", xsize, ysize);
    printf("ARToolKitPlus: cc = [%.2f  %.2f]  fc = [%.2f  %.2f]\n", mat[0][2], mat[1][2], mat[0][0], mat[1][1]);
    printf("ARToolKitPlus: kc = [%.4f %.4f %.4f %.4f %.4f %.4f]\n", kc[0], kc[1], kc[2], kc[3], kc[4], kc[5]);
    printf("ARToolKitPlus: undist_iterations = %i\n", undist_iterations);
}

string Camera::getFileName() const {
    return fileName;
}

} // namespace ARToolKitPlus
