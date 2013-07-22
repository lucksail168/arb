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

    Copyright (C) 2012 Fredrik Johansson

******************************************************************************/

#include "fmpcb_poly.h"

int main()
{
    long iter;
    flint_rand_t state;

    printf("evaluate_vec_fast....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 10000; iter++)
    {
        long i, n, qbits1, qbits2, rbits1, rbits2, rbits3;
        fmpq_poly_t F;
        fmpq * X, * Y;
        fmpcb_poly_t f;
        fmpcb_ptr x, y;

        qbits1 = 2 + n_randint(state, 100);
        qbits2 = 2 + n_randint(state, 100);
        rbits1 = 2 + n_randint(state, 200);
        rbits2 = 2 + n_randint(state, 200);
        rbits3 = 2 + n_randint(state, 200);

        n = n_randint(state, 10);

        fmpq_poly_init(F);
        X = _fmpq_vec_init(n);
        Y = _fmpq_vec_init(n);

        fmpcb_poly_init(f);
        x = _fmpcb_vec_init(n);
        y = _fmpcb_vec_init(n);

        fmpq_poly_randtest(F, state, 1 + n_randint(state, 20), qbits1);
        for (i = 0; i < n; i++)
            fmpq_randtest(X + i, state, qbits2);
        for (i = 0; i < n; i++)
            fmpq_poly_evaluate_fmpq(Y + i, F, X + i);

        fmpcb_poly_set_fmpq_poly(f, F, rbits1);
        for (i = 0; i < n; i++)
            fmpcb_set_fmpq(x + i, X + i, rbits2);
        fmpcb_poly_evaluate_vec_fast(y, f, x, n, rbits3);

        for (i = 0; i < n; i++)
        {
            if (!fmpcb_contains_fmpq(y + i, Y + i))
            {
                printf("FAIL (%ld of %ld)\n\n", i, n);

                printf("F = "); fmpq_poly_print(F); printf("\n\n");
                printf("X = "); fmpq_print(X + i); printf("\n\n");
                printf("Y = "); fmpq_print(Y + i); printf("\n\n");

                printf("f = "); fmpcb_poly_printd(f, 15); printf("\n\n");
                printf("x = "); fmpcb_printd(x + i, 15); printf("\n\n");
                printf("y = "); fmpcb_printd(y + i, 15); printf("\n\n");

                abort();
            }
        }

        fmpq_poly_clear(F);
        _fmpq_vec_clear(X, n);
        _fmpq_vec_clear(Y, n);

        fmpcb_poly_clear(f);
        _fmpcb_vec_clear(x, n);
        _fmpcb_vec_clear(y, n);
    }

    flint_randclear(state);
    _fmpz_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}