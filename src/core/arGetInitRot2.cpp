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

#include <ARToolKitPlus/arGetInitRot2Sub.h>
#include <ARToolKitPlus/Tracker.h>

namespace ARToolKitPlus {

int Tracker::arGetInitRot2(ARMarkerInfo *marker_info, ARFloat rot[3][3], ARFloat center[2], ARFloat width) {
    rpp_float err = 1e+20;
    rpp_mat R, R_init;
    rpp_vec t;

    int dir = marker_info->dir;
    rpp_vec ppos2d[4];
    rpp_vec ppos3d[4];
    const unsigned int n_pts = 4;
    const rpp_float model_z = 0;
    const rpp_float iprts_z = 1;

    ppos2d[0][0] = marker_info->vertex[(4 - dir) % 4][0];
    ppos2d[0][1] = marker_info->vertex[(4 - dir) % 4][1];
    ppos2d[0][2] = iprts_z;
    ppos2d[1][0] = marker_info->vertex[(5 - dir) % 4][0];
    ppos2d[1][1] = marker_info->vertex[(5 - dir) % 4][1];
    ppos2d[1][2] = iprts_z;
    ppos2d[2][0] = marker_info->vertex[(6 - dir) % 4][0];
    ppos2d[2][1] = marker_info->vertex[(6 - dir) % 4][1];
    ppos2d[2][2] = iprts_z;
    ppos2d[3][0] = marker_info->vertex[(7 - dir) % 4][0];
    ppos2d[3][1] = marker_info->vertex[(7 - dir) % 4][1];
    ppos2d[3][2] = iprts_z;

    ppos3d[0][0] = center[0] - width * (ARFloat) 0.5;
    ppos3d[0][1] = center[1] + width * (ARFloat) 0.5;
    ppos3d[0][2] = model_z;
    ppos3d[1][0] = center[0] + width * (ARFloat) 0.5;
    ppos3d[1][1] = center[1] + width * (ARFloat) 0.5;
    ppos3d[1][2] = model_z;
    ppos3d[2][0] = center[0] + width * (ARFloat) 0.5;
    ppos3d[2][1] = center[1] - width * (ARFloat) 0.5;
    ppos3d[2][2] = model_z;
    ppos3d[3][0] = center[0] - width * (ARFloat) 0.5;
    ppos3d[3][1] = center[1] - width * (ARFloat) 0.5;
    ppos3d[3][2] = model_z;

    const rpp_float cc[2] = { arCamera->mat[0][2], arCamera->mat[1][2] };
    const rpp_float fc[2] = { arCamera->mat[0][0], arCamera->mat[1][1] };

    rpp::arGetInitRot2_sub(err, R, t, cc, fc, ppos3d, ppos2d, n_pts, R_init, true, 0, 0, 0);

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            rot[i][j] = (ARFloat) R[i][j];

    return 0;
}

} // namespace ARToolKitPlus
