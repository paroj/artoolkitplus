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

#ifndef __RPP_TYPES_H__
#define __RPP_TYPES_H__

#include <vector>

#include <ARToolKitPlus/extra/rpp.h>

namespace rpp {

typedef double real_t;

// standard types
typedef real_t vec3[3];
struct vec3_t {
    vec3 v;
};
typedef std::vector<vec3_t> vec3_array;
typedef std::vector<vec3_t>::iterator vec3_array_iter;
typedef std::vector<vec3_t>::const_iterator vec3_array_const_iter;

typedef real_t mat33[3][3];
struct mat33_t {
    mat33 m;
};
typedef std::vector<mat33_t> mat33_array;
typedef std::vector<mat33_t>::iterator mat33_array_iter;
typedef std::vector<mat33_t>::const_iterator mat33_array_const_iter;

typedef std::vector<real_t> scalar_array;

struct pose_t {
    mat33_t R;
    vec3_t t;
    real_t E;
    mat33_t PoseLu_R;
    vec3_t PoseLu_t;
    real_t obj_err;
};

typedef std::vector<pose_t> pose_vec;

struct quat_t {
    vec3_t v;
    real_t s;
};

struct options_t {
    mat33_t initR;
    real_t tol;
    real_t epsilon;
    unsigned int max_iter;
};

} // namespace rpp
#endif
