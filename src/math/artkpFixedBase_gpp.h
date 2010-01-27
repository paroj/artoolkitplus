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


#ifndef __ARTKPFIXEDBASE_GPP_HEADERFILE__
#define __ARTKPFIXEDBASE_GPP_HEADERFILE__


#include <gpp.h>
#ifdef DEBUG
  #pragma comment( lib, "gpp_WMMX40_d.lib" )
#else
  #pragma comment( lib, "gpp_WMMX40_r.lib" )
#endif //DEBUG


template <int PBITS_>
class artkpFixedBase_gpp
{
public:
	enum {
		PBITS = PBITS_,
		CHECK = 0
	};

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
		return nV<<PBITS;
	}

	static int fixedFromFloat(float nV)
	{
		return (int)(nV *  (float)(1 << PBITS) + 0.5f);
	}

	static int fixedFromDouble(double nV)
	{
		return (int)(nV * (double)(1 << PBITS) + 0.5f);
	}

	static int inverse(int nFixed)
	{
		int ret=0;
		gppInvHP_n_32s(nFixed, &ret, PBITS);
		return ret;
	}

	static int multiply(int nLeftFixed, int nRightFixed)
	{
		int ret;
		gppMul_n_32s(nLeftFixed, nRightFixed, &ret, PBITS);
		return ret;
	}

	static int divide(int nLeftFixed, int nRightFixed)
	{
		int ret;
		gppDiv_n_32s(nLeftFixed, nRightFixed, &ret, PBITS);
		return ret;
	}

	static int cos(int nFixed)
	{
		int ret;
		gppCosHP_n_32s(nFixed, &ret, PBITS);
		return ret;
	}

	static int sin(int nFixed)
	{
		int ret;
		gppSinHP_n_32s(nFixed, &ret, PBITS);
		return ret;
	}

	static int fabs(int nFixed)
	{
		return nFixed<0 ? -nFixed : nFixed;
	}

	static int sqrt(int nFixed)
	{
		unsigned int ret;
		gppSqrtHP_n_32s(nFixed, &ret, PBITS);
		return (int)ret;
	}

	static int inverseSqrt(int nFixed)
	{
		unsigned int ret;
		gppInvSqrtHP_n_32s(nFixed, &ret, PBITS);
		return (int)ret;
	}

	static int ceil(int nFixed)
	{
		int ret = (nFixed>>PBITS)<<PBITS;

		if(nFixed>=0 && ret<nFixed)
			ret += fixedFromInt(1);

		return ret;
	}
};


#endif //__ARTKPFIXEDBASE_GPP_HEADERFILE__
