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

namespace ARToolKitPlus {

ARFloat Tracker::arGetTransMatCont(ARMarkerInfo *marker_info, ARFloat prev_conv[3][4], ARFloat center[2],
        ARFloat width, ARFloat conv[3][4]) {
    ARFloat err1, err2;
    ARFloat wtrans[3][4];
    int i, j;

    err1 = arGetTransMatContSub(marker_info, prev_conv, center, width, conv);
    if (err1 > AR_GET_TRANS_CONT_MAT_MAX_FIT_ERROR) {
        err2 = arGetTransMat(marker_info, center, width, wtrans);
        if (err2 < err1) {
            for (j = 0; j < 3; j++) {
                for (i = 0; i < 4; i++)
                    conv[j][i] = wtrans[j][i];
            }
            err1 = err2;
        }
    }

    return err1;
}

ARFloat Tracker::arGetTransMatContSub(ARMarkerInfo *marker_info, ARFloat prev_conv[3][4], ARFloat center[2],
        ARFloat width, ARFloat conv[3][4]) {
    ARFloat rot[3][3];
    ARFloat ppos2d[4][2];
    ARFloat ppos3d[4][2];
    int dir;
    ARFloat err;
    int i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            rot[i][j] = prev_conv[i][j];
        }
    }

    dir = marker_info->dir;
    ppos2d[0][0] = marker_info->vertex[(4 - dir) % 4][0];
    ppos2d[0][1] = marker_info->vertex[(4 - dir) % 4][1];
    ppos2d[1][0] = marker_info->vertex[(5 - dir) % 4][0];
    ppos2d[1][1] = marker_info->vertex[(5 - dir) % 4][1];
    ppos2d[2][0] = marker_info->vertex[(6 - dir) % 4][0];
    ppos2d[2][1] = marker_info->vertex[(6 - dir) % 4][1];
    ppos2d[3][0] = marker_info->vertex[(7 - dir) % 4][0];
    ppos2d[3][1] = marker_info->vertex[(7 - dir) % 4][1];
    ppos3d[0][0] = center[0] - width * (ARFloat) 0.5;
    ppos3d[0][1] = center[1] + width * (ARFloat) 0.5;
    ppos3d[1][0] = center[0] + width * (ARFloat) 0.5;
    ppos3d[1][1] = center[1] + width * (ARFloat) 0.5;
    ppos3d[2][0] = center[0] + width * (ARFloat) 0.5;
    ppos3d[2][1] = center[1] - width * (ARFloat) 0.5;
    ppos3d[3][0] = center[0] - width * (ARFloat) 0.5;
    ppos3d[3][1] = center[1] - width * (ARFloat) 0.5;

    for (i = 0; i < AR_GET_TRANS_MAT_MAX_LOOP_COUNT; i++) {
        err = arGetTransMat3(rot, ppos2d, ppos3d, 4, conv, arCamera);
        if (err < AR_GET_TRANS_MAT_MAX_FIT_ERROR)
            break;
    }
    return err;
}

} // namespace ARToolKitPlus
