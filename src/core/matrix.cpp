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

#include <cmath>
#include <cstdlib>
#include <ARToolKitPlus/matrix.h>

namespace ARToolKitPlus {

namespace Matrix {

#define MATRIX(name,x,y,width)  ( *(name + (width) * (x) + (y)) )

// declaration of internal helper functions (see end of file)
// static ARFloat mdet(ARFloat *ap, int dimen, int rowa);
static ARFloat *minv(ARFloat *ap, int dimen, int rowa);

// from mAlloc.c
ARMat*
alloc(int row, int clm) {
	ARMat *m;

	m = (ARMat *) malloc(sizeof(ARMat));
	if (m == NULL)
		return NULL;

	m->m = (ARFloat *) malloc(sizeof(ARFloat) * row * clm);
	if (m->m == NULL) {
		free(m);
		return NULL;
	} else {
		m->row = row;
		m->clm = clm;
	}

	return m;
}

// from mFree.c
int free(ARMat *m) {
	::free(m->m);
	::free(m);

	return 0;
}

// from mAllocDup.c
ARMat*
allocDup(ARMat *source) {
	ARMat *dest;

	dest = alloc(source->row, source->clm);
	if (dest == NULL)
		return NULL;

	if (dup(dest, source) < 0) {
		free(dest);
		return NULL;
	}

	return dest;
}

// from mDup.c
int dup(ARMat *dest, ARMat *source) {
	int r, c;

	if (dest->row != source->row || dest->clm != source->clm) {
		return -1;
	}
	for (r = 0; r < source->row; r++) {
		for (c = 0; c < source->clm; c++) {
			ARELEM0(dest, r, c) = ARELEM0(source, r, c);
		}
	}
	return 0;
}

// from mMul.c
int mul(ARMat *dest, ARMat *a, ARMat *b) {
	int r, c, i;

	if (a->clm != b->row || dest->row != a->row || dest->clm != b->clm)
		return -1;

	for (r = 0; r < dest->row; r++) {
		for (c = 0; c < dest->clm; c++) {
			ARELEM0(dest, r, c) = 0.0;
			for (i = 0; i < a->clm; i++) {
				ARELEM0(dest, r, c) += ARELEM0(a, r, i) * ARELEM0(b, i, c);
			}
		}
	}

	return 0;
}

// from mSelfInv.c
int selfInv(ARMat *m) {
	if (minv(m->m, m->row, m->row) == NULL)
		return -1;

	return 0;
}

// from mSelfInv.c -- MATRIX inverse function
static ARFloat*
minv(ARFloat *ap, int dimen, int rowa) {
	ARFloat *wap, *wcp, *wbp;/* work pointer                 */
	int i, j, n, ip = 0, nwork;
	int nos[50];
	ARFloat epsl;
	ARFloat p, pbuf, work;
	//ARFloat  fabs();

	epsl = (ARFloat) 1.0e-10; /* Threshold value      */

	switch (dimen) {
	case (0):
		return (NULL); /* check size */
	case (1):
		*ap = (ARFloat) 1.0 / (*ap);
		return (ap); /* 1 dimension */
	}

	for (n = 0; n < dimen; n++)
		nos[n] = n;

	for (n = 0; n < dimen; n++) {
		wcp = ap + n * rowa;

		for (i = n, wap = wcp, p = 0.0; i < dimen; i++, wap += rowa)
			if (p < (pbuf = (ARFloat) fabs(*wap))) {
				p = pbuf;
				ip = i;
			}
		if (p <= epsl)
			return (NULL);

		nwork = nos[ip];
		nos[ip] = nos[n];
		nos[n] = nwork;

		for (j = 0, wap = ap + ip * rowa, wbp = wcp; j < dimen; j++) {
			work = *wap;
			*wap++ = *wbp;
			*wbp++ = work;
		}

		for (j = 1, wap = wcp, work = *wcp; j < dimen; j++, wap++)
			*wap = *(wap + 1) / work;
		*wap = (ARFloat) 1.0 / work;

		for (i = 0; i < dimen; i++) {
			if (i != n) {
				wap = ap + i * rowa;
				for (j = 1, wbp = wcp, work = *wap; j < dimen; j++, wap++, wbp++)
					*wap = *(wap + 1) - work * (*wbp);
				*wap = -work * (*wbp);
			}
		}
	}

	for (n = 0; n < dimen; n++) {
		for (j = n; j < dimen; j++)
			if (nos[j] == n)
				break;
		nos[j] = nos[n];
		for (i = 0, wap = ap + j, wbp = ap + n; i < dimen; i++, wap += rowa, wbp += rowa) {
			work = *wap;
			*wap = *wbp;
			*wbp = work;
		}
	}
	return (ap);
}

} // namespace Matrix


} // namespace ARToolKitPlus
