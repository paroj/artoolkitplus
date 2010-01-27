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


#ifndef __GPP_HEADERFILE__
#define __GPP_HEADERFILE__

#include <gpp.h>


#define GPPMUL2(a,b, res, bits)  \
	gppMul_n_32s((a), (b), &res, bits);

#define GPPMUL3(a,b,c, res, bits)  \
	gppMul_n_32s((a), (b), &_tmp1, bits);  \
	gppMul_n_32s(_tmp1, (c), &res, bits);

#define GPPMUL4(a,b,c,d, res, bits)  \
	gppMul_n_32s((a), (b), &_tmp1, bits);  \
	gppMul_n_32s(_tmp1, (c), &_tmp2, bits);  \
	gppMul_n_32s(_tmp2, (d), &res, bits);

#define GPPDIV2(a,b, res, bits)  \
	gppDiv_n_32s((a), (b), &res, bits);


#endif //__GPP_HEADERFILE__
