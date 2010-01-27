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


#ifndef __ARTKPFIXEDBASE_GENERIC_HEADERFILE__
#define __ARTKPFIXEDBASE_GENERIC_HEADERFILE__

#include <assert.h>


// Generic MATHBASE implementation for all
// platforms. This implementation is totally
// unoptimized.
// (e.g. using the sin() function from math.h)
//
template <int PBITS_, int CHECK_>
class artkpFixedBase_generic
{
public:
	enum {
		PBITS = PBITS_,
		CHECK = CHECK_
	};


	static void checkInt(int nInteger)
	{
		__int64 v64 = ((__int64)nInteger) << PBITS;
		if(v64 != int(v64))
		{
			assert(false && "Integer to Fixed-Point conversion failed: the target's range was overflowed");
		};
	}

	static void checkFloat(float nFloat)
	{
		__int64 v64 = (__int64)(nFloat *  ((float)(1 << PBITS) + 0.5f));
		if(v64 != int(v64))
		{
			assert(false && "Float to Fixed-Point conversion failed: the target's range was overflowed");
		};
	}

	static void checkDouble(double nDouble)
	{
		__int64 v64 = (__int64)(nDouble *  ((double)(1 << PBITS) + 0.5f));
		if(v64 != int(v64))
		{
			assert(false && "Double to Fixed-Point conversion failed: the target's range was overflowed");
		};
	}

	static float floatFromFixed(int nFixed)
	{
		return nFixed/(float)(1 << PBITS);
	}	

	static double doubleFromFixed(int nFixed)
	{
		return nFixed/(double)(1 << PBITS);
	}

	static int fixedFromInt(int nV)
	{
		if(CHECK)
			checkInt(nV);
		return nV<<PBITS;
	}

	static int fixedFromFloat(float nV)
	{
		if(CHECK)
			checkFloat(nV);
		return (int)(nV *  (float)(1 << PBITS) + 0.5f);
	}

	static int fixedFromDouble(double nV)
	{
		if(CHECK)
			checkDouble(nV);
		return (int)(nV * (double)(1 << PBITS) + 0.5f);
	}

	static int inverse(int nFixed)
	{
		return (__int32)(((__int64)1<<(2*PBITS))/nFixed);
	}

	static int multiply(int nLeftFixed, int nRightFixed)
	{
		return (__int32)(((__int64)nLeftFixed * (__int64)nRightFixed) >> PBITS);
	}

	static int divide(int nLeftFixed, int nRightFixed)
	{
		return (__int32)(((__int64)nLeftFixed << PBITS) / nRightFixed);
	}

	static int cos(int nFixed)
	{
		return fixedFromDouble(::cos(floatFromFixed(nFixed)));
	}

	static int sin(int nFixed)
	{
		return fixedFromDouble(::sin(floatFromFixed(nFixed)));
	}

	static int fabs(int nFixed)
	{
		return nFixed<0 ? -nFixed : nFixed;
	}

	static int sqrt(int nFixed)
	{
		return fixedFromDouble(::sqrt(floatFromFixed(nFixed)));
	}

	static int inverseSqrt(int nFixed)
	{
		return inverse(sqrt(nFixed));
	}

	static int ceil(int nFixed)
	{
		int ret = (nFixed>>PBITS)<<PBITS;

		if(nFixed>=0 && ret<nFixed)
			ret += fixedFromInt(1);

		return ret;
	}

};


#endif //__ARTKPFIXEDBASE_GENERIC_HEADERFILE__
