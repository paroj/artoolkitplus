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

#include <stdio.h>
#include <math.h>
#include <ARToolKitPlus/Tracker.h>
#include <ARToolKitPlus/matrix.h>

namespace ARToolKitPlus {

static ARFloat norm(ARFloat a, ARFloat b, ARFloat c);
static ARFloat dot(ARFloat a1, ARFloat a2, ARFloat a3, ARFloat b1, ARFloat b2, ARFloat b3);

int Tracker::arCameraDecomp(Camera *source, Camera *icpara, ARFloat trans[3][4]) {
    icpara->xsize = source->xsize;
    icpara->ysize = source->ysize;

    return arCameraDecompMat(source->mat, icpara->mat, trans);
}

int Tracker::arCameraDecompMat(ARFloat source[3][4], ARFloat cpara[3][4], ARFloat trans[3][4]) {
    int r, c;
    ARFloat Cpara[3][4];
    ARFloat rem1, rem2, rem3;

    if (source[2][3] >= 0) {
        for (r = 0; r < 3; r++) {
            for (c = 0; c < 4; c++) {
                Cpara[r][c] = source[r][c];
            }
        }
    } else {
        for (r = 0; r < 3; r++) {
            for (c = 0; c < 4; c++) {
                Cpara[r][c] = -(source[r][c]);
            }
        }
    }

    for (r = 0; r < 3; r++) {
        for (c = 0; c < 4; c++) {
            cpara[r][c] = 0.0;
        }
    }
    cpara[2][2] = norm(Cpara[2][0], Cpara[2][1], Cpara[2][2]);
    trans[2][0] = Cpara[2][0] / cpara[2][2];
    trans[2][1] = Cpara[2][1] / cpara[2][2];
    trans[2][2] = Cpara[2][2] / cpara[2][2];
    trans[2][3] = Cpara[2][3] / cpara[2][2];

    cpara[1][2] = dot(trans[2][0], trans[2][1], trans[2][2], Cpara[1][0], Cpara[1][1], Cpara[1][2]);
    rem1 = Cpara[1][0] - cpara[1][2] * trans[2][0];
    rem2 = Cpara[1][1] - cpara[1][2] * trans[2][1];
    rem3 = Cpara[1][2] - cpara[1][2] * trans[2][2];
    cpara[1][1] = norm(rem1, rem2, rem3);
    trans[1][0] = rem1 / cpara[1][1];
    trans[1][1] = rem2 / cpara[1][1];
    trans[1][2] = rem3 / cpara[1][1];

    cpara[0][2] = dot(trans[2][0], trans[2][1], trans[2][2], Cpara[0][0], Cpara[0][1], Cpara[0][2]);
    cpara[0][1] = dot(trans[1][0], trans[1][1], trans[1][2], Cpara[0][0], Cpara[0][1], Cpara[0][2]);
    rem1 = Cpara[0][0] - cpara[0][1] * trans[1][0] - cpara[0][2] * trans[2][0];
    rem2 = Cpara[0][1] - cpara[0][1] * trans[1][1] - cpara[0][2] * trans[2][1];
    rem3 = Cpara[0][2] - cpara[0][1] * trans[1][2] - cpara[0][2] * trans[2][2];
    cpara[0][0] = norm(rem1, rem2, rem3);
    trans[0][0] = rem1 / cpara[0][0];
    trans[0][1] = rem2 / cpara[0][0];
    trans[0][2] = rem3 / cpara[0][0];

    trans[1][3] = (Cpara[1][3] - cpara[1][2] * trans[2][3]) / cpara[1][1];
    trans[0][3] = (Cpara[0][3] - cpara[0][1] * trans[1][3] - cpara[0][2] * trans[2][3]) / cpara[0][0];

    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            cpara[r][c] /= cpara[2][2];
        }
    }

    return 0;
}

static ARFloat norm(ARFloat a, ARFloat b, ARFloat c) {
    return ((ARFloat) sqrt(a * a + b * b + c * c));
}

static ARFloat dot(ARFloat a1, ARFloat a2, ARFloat a3, ARFloat b1, ARFloat b2, ARFloat b3) {
    return (a1 * b1 + a2 * b2 + a3 * b3);
}

} // namespace ARToolKitPlus
