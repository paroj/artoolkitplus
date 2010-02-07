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

#include "rpp_types.h"

void robustPlanarPose(rpp_float &err, rpp_mat &R, rpp_vec &t, const rpp_float cc[2], const rpp_float fc[2],
        const rpp_vec *model, const rpp_vec *iprts, const unsigned int model_iprts_size, const rpp_mat R_init,
        const bool estimate_R_init, const rpp_float epsilon, const rpp_float tolerance,
        const unsigned int max_iterations);
/*

 [OUTPUT]

 err: squared reprojection error
 R:   rotation matrix (iprts[n] = R*model[n]+t)
 t:   translation vector (iprts[n] = R*model[n]+t)

 [INPUT]

 cc:    camera's principal point [x,y]
 fc:    camera's focal length    [x,y]
 model: 3d points [x,y,z]
 iprts: 2d projections [x,y,1]

 model_iprts_size:   number of 2d/3d point correspondences
 R_init:             initial estimate of the rotation matrix R
 estimate_R_init:    when true, the estimate in R_init is ignored
 epsilon*:           see below (default: 1E-8)
 tolerance*:         see below (default: 1E-5)
 max_iterations*:    max. number of iterations (0 = infinite)

 *) the following code fragment illustrates the use of epsilon,
 tolerance and max_iterations:

 while((ABS(( old_err - new_err ) / old_err) > tolerance) &&
 ( new_err > epsilon ) &&
 ( max_iterations == 0 || iterations < max_iterations ))
 {
 NEW ITERATION
 }

 */
