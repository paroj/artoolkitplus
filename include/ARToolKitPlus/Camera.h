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

#ifndef __ARTOOLKIT_CAMERAADVIMPL_HEADERFILE__
#define __ARTOOLKIT_CAMERAADVIMPL_HEADERFILE__

#include <string>
#include <ARToolKitPlus/config.h>


namespace ARToolKitPlus {

#define CAMERA_ADV_HEADER "ARToolKitPlus_CamCal_Rev02"
#define CAMERA_ADV_MAX_UNDIST_ITERATIONS 20

/**
 * this class used to be called ARParam in the classical ARToolkit
 * so do not wonder because of the method names
 */
class AR_EXPORT Camera {
public:
    int xsize, ysize;
    ARFloat mat[3][4];
    ARFloat dist_factor[4];

    void observ2Ideal(ARFloat ox, ARFloat oy, ARFloat *ix, ARFloat *iy);
    void ideal2Observ(ARFloat ix, ARFloat iy, ARFloat *ox, ARFloat *oy);
    bool loadFromFile(const char* filename);
    Camera* clone();
    bool changeFrameSize(const int frameWidth, const int frameHeight);
    void printSettings();
    std::string getFileName() const;
protected:
    std::string fileName;

    // http://www.vision.caltech.edu/bouguetj/calib_doc/htmls/parameters.html
    ARFloat cc[2];
    ARFloat fc[2];
    ARFloat kc[6];
    int undist_iterations;
};

} // namespace ARToolKitPlus

#endif // __ARTOOLKIT_CAMERAADVIMPL_HEADERFILE__
