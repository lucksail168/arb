/*=============================================================================

    This file is part of ARB.

    ARB is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ARB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ARB; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2013, 2014 Fredrik Johansson

******************************************************************************/

#include <math.h>
#include "elefun.h"

#define TABSIZE 256

/* bounds for log2(1/n!) */
static const int rec_fac_bound_2exp_si_tab[TABSIZE] = 
{
    0, 0, -1, -2, -4, -6, -9, -12, -15, -18, -21, -25, -28, -32, -36,
    -40, -44, -48, -52, -56, -61, -65, -69, -74, -79, -83, -88, -93, -97,
    -102, -107, -112, -117, -122, -127, -132, -138, -143, -148, -153,
    -159, -164, -169, -175, -180, -186, -191, -197, -202, -208, -214,
    -219, -225, -231, -237, -242, -248, -254, -260, -266, -272, -278,
    -284, -289, -295, -302, -308, -314, -320, -326, -332, -338, -344,
    -350, -357, -363, -369, -375, -382, -388, -394, -401, -407, -413,
    -420, -426, -433, -439, -446, -452, -458, -465, -472, -478, -485,
    -491, -498, -504, -511, -518, -524, -531, -538, -544, -551, -558,
    -564, -571, -578, -585, -591, -598, -605, -612, -619, -626, -632,
    -639, -646, -653, -660, -667, -674, -681, -688, -695, -702, -709,
    -716, -723, -730, -737, -744, -751, -758, -765, -772, -779, -786,
    -793, -801, -808, -815, -822, -829, -836, -844, -851, -858, -865,
    -872, -880, -887, -894, -901, -909, -916, -923, -931, -938, -945,
    -952, -960, -967, -975, -982, -989, -997, -1004, -1011, -1019, -1026,
    -1034, -1041, -1049, -1056, -1064, -1071, -1078, -1086, -1093, -1101,
    -1108, -1116, -1123, -1131, -1139, -1146, -1154, -1161, -1169, -1176,
    -1184, -1192, -1199, -1207, -1214, -1222, -1230, -1237, -1245, -1253,
    -1260, -1268, -1276, -1283, -1291, -1299, -1306, -1314, -1322, -1329,
    -1337, -1345, -1353, -1360, -1368, -1376, -1384, -1391, -1399, -1407,
    -1415, -1423, -1430, -1438, -1446, -1454, -1462, -1470, -1477, -1485,
    -1493, -1501, -1509, -1517, -1525, -1532, -1540, -1548, -1556, -1564,
    -1572, -1580, -1588, -1596, -1604, -1612, -1620, -1628, -1636, -1644,
    -1652, -1660, -1668, -1675
};

#define ONE_OVER_LOG2 1.4426950408889634074
#define LOG2 0.69314718055994530942

/* For all we know, the log() function shipped with some libm implementation
   for MS-DOS from 1987 might have given less than six digits of accuracy, so
   we reimplement the function poorly but reproducibly here. */

#include "double_extras.h"

static const double logcoeffs[] = {
    0.0,
    1.0,
    0.5,
    0.33333333333333333333,
    0.25,
    0.2,
    0.16666666666666666667,
    0.14285714285714285714,
    0.125,
    0.11111111111111111111,
    0.1,
    0.090909090909090909091,
    0.083333333333333333333,
    0.076923076923076923077,
    0.071428571428571428571,
    0.066666666666666666667,
    0.0625,
};

double idiot_log(double x)
{
    double t;
    int n;

    t = frexp(x, &n);
    t = sqrt(t);

    return n * LOG2 - 2.0 * d_polyval(logcoeffs, 14, 1.0 - t);
}

static __inline__ long
rec_fac_bound_2exp_si(long n)
{
    if (n < TABSIZE)
    {
        return rec_fac_bound_2exp_si_tab[n];
    }
    else
    {
        double x;

        /* log(1/n!) < log((e/n)^n) = n - n*log(n) */

        x = (n - n * idiot_log(n)) * ONE_OVER_LOG2;
        x = x * 0.9999;   /* counter rounding error */
        x = ceil(x);      /* round to 0 */

        /* TODO: check for overflow here */

        return (long) x;
    }
}

long
elefun_exp_taylor_bound(long mag, long prec)
{
    long i;

    /* assume x <= 1/4 */
    if (mag > -2)
    {
        printf("elefun_exp_taylor_bound: too large input %ld\n", mag);
        abort();
    }

    for (i = 1; ; i++)
    {
        if (mag * i + rec_fac_bound_2exp_si(i) < -prec - 1)
            return i;
    }
}

