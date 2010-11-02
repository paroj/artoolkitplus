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
 */

#include "librpp.h"
#include "rpp.h"
#include "rpp_vecmat.h"
#include "rpp_const.h"

using namespace rpp;

void robustPlanarPose(rpp_float &err, rpp_mat &R, rpp_vec &t, const rpp_float cc[2], const rpp_float fc[2],
        const rpp_vec *model, const rpp_vec *iprts, const unsigned int model_iprts_size, const rpp_mat R_init,
        const bool estimate_R_init, const rpp_float epsilon, const rpp_float tolerance,
        const unsigned int max_iterations) {
    vec3_array _model;
    vec3_array _iprts;
    _model.resize(model_iprts_size);
    _iprts.resize(model_iprts_size);

    mat33_t K, K_inv;
    mat33_eye(K);
    K.m[0][0] = (real_t) fc[0];
    K.m[1][1] = (real_t) fc[1];
    K.m[0][2] = (real_t) cc[0];
    K.m[1][2] = (real_t) cc[1];

    mat33_inv(K_inv, K);

    for (unsigned int i = 0; i < model_iprts_size; i++) {
        vec3_t _v, _v2;
        vec3_assign(_v, (real_t) model[i][0], (real_t) model[i][1], (real_t) model[i][2]);
        _model[i] = _v;
        vec3_assign(_v, (real_t) iprts[i][0], (real_t) iprts[i][1], (real_t) iprts[i][2]);
        vec3_mult(_v2, K_inv, _v);
        _iprts[i] = _v2;
    }

    options_t options;
    options.max_iter = max_iterations;
    options.epsilon = (real_t) (epsilon == 0 ? DEFAULT_EPSILON : epsilon);
    options.tol = (real_t) (tolerance == 0 ? DEFAULT_TOL : tolerance);
    if (estimate_R_init)
        mat33_set_all_zeros(options.initR);
    else {
        mat33_assign(options.initR, (real_t) R_init[0][0], (real_t) R_init[0][1], (real_t) R_init[0][2],
                (real_t) R_init[1][0], (real_t) R_init[1][1], (real_t) R_init[1][2], (real_t) R_init[2][0],
                (real_t) R_init[2][1], (real_t) R_init[2][2]);
    }

    real_t _err;
    mat33_t _R;
    vec3_t _t;

    robust_pose(_err, _R, _t, _model, _iprts, options);

    for (int j = 0; j < 3; j++) {
        R[j][0] = (rpp_float) _R.m[j][0];
        R[j][1] = (rpp_float) _R.m[j][1];
        R[j][2] = (rpp_float) _R.m[j][2];
        t[j] = (rpp_float) _t.v[j];
    }
    err = (rpp_float) _err;
}
