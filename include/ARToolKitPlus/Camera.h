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

#ifndef __ARTOOLKIT_CAMERA_HEADERFILE__
#define __ARTOOLKIT_CAMERA_HEADERFILE__

#include <cstring>

#include <ARToolKitPlus/config.h>
#include <ARToolKitPlus/param.h>
#include <string>

using std::string;

namespace ARToolKitPlus {

class Camera: public ARParam {
public:
    virtual void observ2Ideal(ARFloat ox, ARFloat oy, ARFloat *ix, ARFloat *iy) = 0;
    virtual void ideal2Observ(ARFloat ix, ARFloat iy, ARFloat *ox, ARFloat *oy) = 0;
    virtual bool loadFromFile(const char* filename) = 0;
    virtual Camera* clone() = 0;
    virtual bool changeFrameSize(const int frameWidth, const int frameHeight) = 0;
    virtual void printSettings() = 0;

    string getFileName() const {
        return fileName;
    }

protected:
    void setFileName(const char* filename) {
        fileName = filename;
    }

    string fileName;
};

} // namespace ARToolKitPlus

#endif // __ARTOOLKIT_CAMERA_HEADERFILE__
