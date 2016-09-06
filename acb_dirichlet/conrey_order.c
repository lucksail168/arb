/*
    Copyright (C) 2016 Pascal Molin

    This file is part of Arb.

    Arb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "acb_dirichlet.h"

ulong
acb_dirichlet_conrey_order(const acb_dirichlet_group_t G, const acb_dirichlet_conrey_t x)
{
    ulong k, g;
    g = G->expo;

    for (k = 0; k < G->num; k++)
        g = n_gcd(g, G->PHI[k] * x->log[k]);

    return G->expo / g;
}
