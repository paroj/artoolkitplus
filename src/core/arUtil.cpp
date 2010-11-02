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
#include <ARToolKitPlus/Camera.h>
#include <ARToolKitPlus/matrix.h>

namespace ARToolKitPlus {

int Tracker::arInitCparam(Camera *pCam) {
    // if the camera parameters change, the undistortion LUT has to be rebuilt.
    // (this is done automatically in arCameraObserv2Ideal_LUT or arCameraIdeal2Observ_LUT)
    //
    if (undistO2ITable && (arImXsize != pCam->xsize || arImYsize != pCam->ysize)) {
        delete[] undistO2ITable;
        undistO2ITable = NULL;
    }

    arImXsize = pCam->xsize;
    arImYsize = pCam->ysize;

    return (0);
}

int Tracker::arGetLine(int x_coord[], int y_coord[], int vertex[], ARFloat line[4][3], ARFloat v[4][2]) {
    ARMat *input, *evec;
    ARVec *ev, *mean;
    ARFloat w1;
    int st, ed, n;
    int i, j;

    ev = Vector::alloc(2);
    mean = Vector::alloc(2);
    evec = Matrix::alloc(2, 2);
    for (i = 0; i < 4; i++) {
        w1 = (ARFloat) (vertex[i + 1] - vertex[i] + 1) * (ARFloat) 0.05 + (ARFloat) 0.5;
        st = (int) (vertex[i] + w1);
        ed = (int) (vertex[i + 1] - w1);
        n = ed - st + 1;
        input = Matrix::alloc(n, 2);
        for (j = 0; j < n; j++) {
            ARFloat x, y;
            (this->*arCameraObserv2Ideal_func)(arCamera, (ARFloat) x_coord[st + j], (ARFloat) y_coord[st + j], &x, &y);
            input->m[j * 2 + 0] = x;
            input->m[j * 2 + 1] = y;
        }
        if (arMatrixPCA(input, evec, ev, mean) < 0) {
            Matrix::free(input);
            Matrix::free(evec);
            Vector::free(mean);
            Vector::free(ev);
            return (-1);
        }
        line[i][0] = evec->m[1];
        line[i][1] = -evec->m[0];
        line[i][2] = -(line[i][0] * mean->v[0] + line[i][1] * mean->v[1]);
        Matrix::free(input);
    }
    Matrix::free(evec);
    Vector::free(mean);
    Vector::free(ev);

    for (i = 0; i < 4; i++) {
        w1 = line[(i + 3) % 4][0] * line[i][1] - line[i][0] * line[(i + 3) % 4][1];
        if (w1 == 0.0)
            return (-1);
        v[i][0] = (line[(i + 3) % 4][1] * line[i][2] - line[i][1] * line[(i + 3) % 4][2]) / w1;
        v[i][1] = (line[i][0] * line[(i + 3) % 4][2] - line[(i + 3) % 4][0] * line[i][2]) / w1;
    }

    return (0);
}

int Tracker::arUtilMatMul(ARFloat s1[3][4], ARFloat s2[3][4], ARFloat d[3][4]) {
    int i, j;

    for (j = 0; j < 3; j++) {
        for (i = 0; i < 4; i++) {
            d[j][i] = s1[j][0] * s2[0][i] + s1[j][1] * s2[1][i] + s1[j][2] * s2[2][i];
        }
        d[j][3] += s1[j][3];
    }

    return 0;
}

int Tracker::arUtilMatInv(ARFloat s[3][4], ARFloat d[3][4]) {
    ARMat *mat;
    int i, j;

    mat = Matrix::alloc(4, 4);
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 4; i++) {
            mat->m[j * 4 + i] = s[j][i];
        }
    }
    mat->m[3 * 4 + 0] = 0;
    mat->m[3 * 4 + 1] = 0;
    mat->m[3 * 4 + 2] = 0;
    mat->m[3 * 4 + 3] = 1;
    Matrix::selfInv(mat);
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 4; i++) {
            d[j][i] = mat->m[j * 4 + i];
        }
    }
    Matrix::free(mat);

    return 0;
}

} // namespace ARToolKitPlus
