/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file
 *
 * Extra Integer methods/macros.
 */

#ifndef INTX_H
#define INTX_H

/** Is this integer an exact multiple of integer n? */
#define int_is_multiple(i, n)       (0 == ((i) % (n)))

/** Is this integer even? */
#define int_is_even(i)              int_is_multiple((i), 2)

/** Is this integer odd? */
#define int_is_odd(i)               (!int_is_even(i))

#endif /* INTX_H */
