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

#include <ARToolKitPlus/Tracker.h>
#include <cmath>

#ifndef M_PI
# define M_PI       3.14159265358979323846  /* pi */
#endif

namespace ARToolKitPlus {

ARFloat Tracker::arModifyMatrix(ARFloat rot[3][3], ARFloat trans[3], ARFloat cpara[3][4], ARFloat vertex[][3],
        ARFloat pos2d[][2], int num) {
    ARFloat factor;
    ARFloat a, b, c;
    ARFloat a1, b1, c1;
    ARFloat a2, b2, c2;
    ARFloat ma = 0, mb = 0, mc = 0;
    ARFloat combo[3][4];
    ARFloat hx, hy, h, x, y;
    ARFloat err, minerr;
    int t1, t2, t3;
    int s1 = 0, s2 = 0, s3 = 0;
    int i, j;

    arGetAngle(rot, &a, &b, &c);

    a2 = a;
    b2 = b;
    c2 = c;
    factor = (ARFloat) (10.0 * M_PI / 180.0);
    for (j = 0; j < 15; j++) {
        minerr = 1000000000.0;
        for (t1 = -1; t1 <= 1; t1++) {
            for (t2 = -1; t2 <= 1; t2++) {
                for (t3 = -1; t3 <= 1; t3++) {

                    a1 = a2 + factor * t1;
                    b1 = b2 + factor * t2;
                    c1 = c2 + factor * t3;

                    arGetNewMatrix(a1, b1, c1, trans, NULL, cpara, combo);

                    err = 0.0;
                    for (i = 0; i < num; i++) {
                        hx = combo[0][0] * vertex[i][0] + combo[0][1] * vertex[i][1] + combo[0][2] * vertex[i][2]
                                + combo[0][3];
                        hy = combo[1][0] * vertex[i][0] + combo[1][1] * vertex[i][1] + combo[1][2] * vertex[i][2]
                                + combo[1][3];
                        h = combo[2][0] * vertex[i][0] + combo[2][1] * vertex[i][1] + combo[2][2] * vertex[i][2]
                                + combo[2][3];
                        x = hx / h;
                        y = hy / h;

                        err += (pos2d[i][0] - x) * (pos2d[i][0] - x) + (pos2d[i][1] - y) * (pos2d[i][1] - y);
                    }

                    if (err < minerr) {
                        minerr = err;
                        ma = a1;
                        mb = b1;
                        mc = c1;
                        s1 = t1;
                        s2 = t2;
                        s3 = t3;
                    }
                }
            }
        }

        if (s1 == 0 && s2 == 0 && s3 == 0 /*&& ss1 == 0 && ss2 == 0 && ss3 == 0*/)
            factor *= 0.5;
        a2 = ma;
        b2 = mb;
        c2 = mc;
    }

    arGetRot(ma, mb, mc, rot);

    return minerr / num;
}

ARFloat Tracker::arModifyMatrix2(ARFloat rot[3][3], ARFloat trans[3], ARFloat cpara[3][4], ARFloat vertex[][3],
        ARFloat pos2d[][2], int num) {
    ARFloat factor;
    ARFloat a, b, c;
    ARFloat a1, b1, c1;
    ARFloat a2, b2, c2;
    ARFloat ma, mb, mc;
    ARFloat combo[3][4];
    ARFloat hx, hy, h, x, y;
    ARFloat err;
    ARFloat minerr;
    int t1, t2, t3, tt1, tt2, tt3;
    ARFloat tfact[5] = { 0.96f, 0.98f, 1.0f, 1.02f, 1.04f };
    ARFloat modtrans[3], mmodtrans[3];
    int s1, s2, s3;
    int i, j;

    arGetAngle(rot, &a, &b, &c);

    a2 = a;
    b2 = b;
    c2 = c;

    // initialise to some value
    ma = a;
    mb = b;
    mc = c;
    mmodtrans[0] = trans[0];
    mmodtrans[1] = trans[1];
    mmodtrans[2] = trans[2];
    minerr = 1000000000.0;
    s1 = -2; s2 = -2; s3 = -2;


    factor = (ARFloat) (40.0 * M_PI / 180.0);
    for (j = 0; j < 15; j++) {
        minerr = 1000000000.0;
        for (t1 = -1; t1 <= 1; t1++) {
            for (t2 = -1; t2 <= 1; t2++) {
                for (t3 = -1; t3 <= 1; t3++) {
                    for (tt1 = -2; tt1 <= 2; tt1++) {
                        for (tt2 = -2; tt2 <= 2; tt2++) {
                            for (tt3 = -2; tt3 <= 2; tt3++) {

                                a1 = a2 + factor * t1;
                                b1 = b2 + factor * t2;
                                c1 = c2 + factor * t3;
                                modtrans[0] = trans[0] * tfact[tt1 + 2];
                                modtrans[1] = trans[1] * tfact[tt2 + 2];
                                modtrans[2] = trans[2] * tfact[tt3 + 2];

                                arGetNewMatrix(a1, b1, c1, modtrans, NULL, cpara, combo);

                                err = 0.0;
                                for (i = 0; i < num; i++) {
                                    hx = combo[0][0] * vertex[i][0] + combo[0][1] * vertex[i][1] + combo[0][2]
                                            * vertex[i][2] + combo[0][3];
                                    hy = combo[1][0] * vertex[i][0] + combo[1][1] * vertex[i][1] + combo[1][2]
                                            * vertex[i][2] + combo[1][3];
                                    h = combo[2][0] * vertex[i][0] + combo[2][1] * vertex[i][1] + combo[2][2]
                                            * vertex[i][2] + combo[2][3];
                                    x = hx / h;
                                    y = hy / h;

                                    err += (pos2d[i][0] - x) * (pos2d[i][0] - x) + (pos2d[i][1] - y)
                                            * (pos2d[i][1] - y);
                                }

                                if (err < minerr) {
                                    minerr = err;
                                    ma = a1;
                                    mb = b1;
                                    mc = c1;
                                    mmodtrans[0] = modtrans[0];
                                    mmodtrans[1] = modtrans[1];
                                    mmodtrans[2] = modtrans[2];

                                    s1 = t1;
                                    s2 = t2;
                                    s3 = t3;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (s1 == 0 && s2 == 0 && s3 == 0 /*&& ss1 == 0 && ss2 == 0 && ss3 == 0*/)
            factor *= 0.5;
        a2 = ma;
        b2 = mb;
        c2 = mc;
        trans[0] = mmodtrans[0];
        trans[1] = mmodtrans[1];
        trans[2] = mmodtrans[2];
    }

    arGetRot(ma, mb, mc, rot);

    return minerr / num;
}
} // namespace ARToolKitPlus
