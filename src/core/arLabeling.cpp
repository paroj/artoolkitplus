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
 */

#include <stdlib.h>
#include <stdio.h>
#include <ARToolKitPlus/Tracker.h>

namespace ARToolKitPlus {

void put_zero(uint8_t *p, int size) {
    while ((size--) > 0)
        *(p++) = 0;
}

// in order to get no speed loss we use the preprocessor
// to create 5 different versions of labeling2, for the
// five different major pixel formats
#define _DEF_PIXEL_FORMAT_ABGR
#define LABEL_FUNC_NAME arLabeling_ABGR
#include "arLabelingImpl.h"
#undef _DEF_PIXEL_FORMAT_ABGR

#define _DEF_PIXEL_FORMAT_BGR
#define LABEL_FUNC_NAME arLabeling_BGR
#include "arLabelingImpl.h"
#undef _DEF_PIXEL_FORMAT_BGR

#define _DEF_PIXEL_FORMAT_RGB
#define LABEL_FUNC_NAME arLabeling_RGB
#include "arLabelingImpl.h"
#undef _DEF_PIXEL_FORMAT_RGB

#define _DEF_PIXEL_FORMAT_RGB565
#define LABEL_FUNC_NAME arLabeling_RGB565
#include "arLabelingImpl.h"
#undef _DEF_PIXEL_FORMAT_RGB565

#define _DEF_PIXEL_FORMAT_LUM
#define LABEL_FUNC_NAME arLabeling_LUM
#include "arLabelingImpl.h"
#undef _DEF_PIXEL_FORMAT_LUM

int16_t*
Tracker::arLabeling(const uint8_t *image, int thresh, int *label_num, int **area, ARFloat **pos, int **clip,
        int **label_ref) {
    int16_t* ret = NULL;

    switch (pixelFormat) {
    case PIXEL_FORMAT_ABGR:
        ret = arLabeling_ABGR(image, thresh, label_num, area, pos, clip, label_ref);
        break;

    case PIXEL_FORMAT_BGRA:
    case PIXEL_FORMAT_BGR:
        ret = arLabeling_BGR(image, thresh, label_num, area, pos, clip, label_ref);
        break;

    case PIXEL_FORMAT_RGBA:
    case PIXEL_FORMAT_RGB:
        ret = arLabeling_RGB(image, thresh, label_num, area, pos, clip, label_ref);
        break;

    case PIXEL_FORMAT_RGB565:
        ret = arLabeling_RGB565(image, thresh, label_num, area, pos, clip, label_ref);
        break;

    case PIXEL_FORMAT_LUM:
        ret = arLabeling_LUM(image, thresh, label_num, area, pos, clip, label_ref);
        break;
    }

    return ret;
}

} // namespace ARToolKitPlus
