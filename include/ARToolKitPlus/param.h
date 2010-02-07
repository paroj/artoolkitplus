/*
    Copyright (C) 2010  ARToolkitPlus Authors

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Authors:
      Daniel Wagner
 */


#ifndef __ARTOOLKIT_PARAM_HEADERFILE__
#define __ARTOOLKIT_PARAM_HEADERFILE__

#include <ARToolKitPlus/config.h>

namespace ARToolKitPlus {

class ARParam
{
  public:
	int      xsize, ysize;
	ARFloat  mat[3][4];
	ARFloat  dist_factor[4];
};

typedef struct {
    int      xsize, ysize;
    double   mat[3][4];
    double   dist_factor[4];
} ARParamDouble;

typedef struct {
    int      xsize, ysize;
    ARFloat   matL[3][4];
    ARFloat   matR[3][4];
    ARFloat   matL2R[3][4];
    ARFloat   dist_factorL[4];
    ARFloat   dist_factorR[4];
} ARSParam;

}  // namespace ARToolKitPlus


#endif // __ARTOOLKIT_PARAM_HEADERFILE__
