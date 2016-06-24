/* This is a statistical significance test to tell whether the Brexit 51.9%
 * vote to leave EU was statistically significant.
 *
 * Project page: https://github.com/al-caveman/brexit
 *
 * Licensed under the MIT license
 *
 * Copyright (c) 2016 caveman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <stdlib.h> /* for rand(3), exit(3), malloc(3) */

/* constants */
#define N 33568184
#define N_LEAVE 17421887
#define N_REMAIN 16146297
#define R 1000

/* fair coin toss function */
int C(void) {
    double r;
    do {
        r = rand()/((double)RAND_MAX);
    } while (r == 0.5);
    if (r < 0.5) return 0;
    else return 1;
}

/* array summation function */
int sum(int *x, int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum += x[i];
    }
    return sum;
}

int main() {
    /* init prng */
    srand(0);

    /* define o_leave, o_remain */
    int *o_leave = malloc(sizeof(int) * N);
    int *o_remain = malloc(sizeof(int) * N);
    int i;
    for (i = 0; i < N; i++) {
        if (i < N_LEAVE) {
            o_leave[i] = 1;
            o_remain[i] = 0;
        } else {
            o_leave[i] = 0;
            o_remain[i] = 1;
        }
    }

    fprintf(stderr, "total leave votes: %d\n", sum(o_leave, N));
    fprintf(stderr, "total remain votes: %d\n", sum(o_remain, N));

    /* simulate p_leave, p_remain */
    fprintf(stderr, "simulating p values..");
    double p_leave = 0, p_remain = 0;
    int *o_unsure_m = malloc(sizeof(int) * N);
    int m;
    for (m = 0; m < R; m++) {
        /* simulate o_unsure */
        for (i = 0; i < N; i++) {
            if (C()) o_unsure_m[i] = 1;
            else o_unsure_m[i] = 0;
        }

        /* calculate p values */
        int sum_o_unsure_m = sum(o_unsure_m, N);
        if (sum(o_leave, N) <= sum_o_unsure_m) p_leave++;
        if (sum(o_remain, N) >= sum_o_unsure_m) p_remain++;

        /* ui */
        if ((m % (R/10)) == 0) {
            fprintf(stderr, ".");
        }
    }
    fprintf(stderr, " ok\n");

    /* print results */
    printf("p value for leave: %f\n", p_leave / N);
    printf("p value for remain: %f\n", p_remain / N);

    return 0;
}
