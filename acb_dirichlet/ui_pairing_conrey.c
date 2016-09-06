/*
    Copyright (C) 2015 Jonathan Bober
    Copyright (C) 2016 Fredrik Johansson
    Copyright (C) 2016 Pascal Molin

    This file is part of Arb.

    Arb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "acb_dirichlet.h"

/* todo: modular arithmetic */

ulong
acb_dirichlet_ui_pairing_conrey(const acb_dirichlet_group_t G, const acb_dirichlet_conrey_t a, const acb_dirichlet_conrey_t b)
{
    ulong x, k;
    x = 0;

    for (k = 0; k < G->num; k++)
        x = (x + G->PHI[k] * a->log[k] * b->log[k]) % G->expo;

    return x;
}
