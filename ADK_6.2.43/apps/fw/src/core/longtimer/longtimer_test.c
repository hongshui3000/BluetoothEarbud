/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/*
 * A unit test for the longtimer library.
 *
 * Run this and it will simulate 25 years of usage of the longtimer library.
 * The test result will be output on stdout and reflected in the exit status
 * of the program.
*/

#include <stdlib.h>
#include <stdio.h>

#include "longtimer/longtimer.h"
#include "sched/sched.h"

TIME timer_time = 0;
long long long_timer_time = 0;

void time_passes(TIME ticks)
{
    timer_time += ticks;
    long_timer_time += ticks;
}

void some_time_passes(TIME maxtime)
{
    time_passes(lrand48() % maxtime);
}

TIME get_time(void)
{
    TIME t;

    t = timer_time;

    return t;
}

TIME event_time;
void (*event_fn)(uint16, void*) = NULL;

tid timed_event_in( TIME delay, void (*fn)(uint16, void*),
                    uint16 fniarg, void *fnvarg )
{
    some_time_passes(1341);
    event_time = get_time() + delay;
    some_time_passes(283);
    event_fn = fn;

    return (tid) 1;
}

#define HOUR (60LL * MINUTE)
#define DAY (24 * HOUR)
#define YEAR (365 * DAY)

int main(void)
{
    /* start using the long timer */
    get_milli_time();

    while(long_timer_time < 25*YEAR)
    {
    TIME new_time = timer_time + lrand48() % (25 * MINUTE);
    uint32 t, mt, dt, st, et, emt, edt, est, cmt, cdt, cst;

    while(time_le(timer_time, new_time))
    {
        TIME delta = time_sub(new_time, timer_time);
        TIME delta2 = time_sub(event_time, timer_time);

        if (time_lt(delta2, delta))
        delta = delta2;

        time_passes(delta);
        some_time_passes(42);

        if (event_fn != NULL && time_ge(timer_time, event_time))
        event_fn(0,NULL);
    }

    t = timer_time;
    mt = get_milli_time();
    dt = get_deci_time();
    st = get_second_time();

    et = (uint32) long_timer_time;
    emt = (uint32) (long_timer_time / MILLISECOND);
    edt = (uint32) (long_timer_time / (100 * MILLISECOND));
    est = (uint32) (long_timer_time / SECOND);

    cmt = millitime_to_time(mt);
    cdt = decitime_to_time(dt);
    cst = secondtime_to_time(st);

#ifndef DEBUG
    if (t != et || mt != emt || dt != edt || st != est ||
        t - cmt >= 1000UL || t - cdt >= 100000UL || t - cst >= 1000000UL)
#endif
    {
        printf("64 bit time: %20llu\n", long_timer_time);
        printf("microsecond: expected %10lu got %10lu\n", et, t);
        printf("millisecond: expected %10lu got %10lu "
           "back to %10lu (%+ld)\n", emt, mt, cmt, t-cmt);
        printf(" decisecond: expected %10lu got %10lu "
           "back to %10lu (%+ld)\n", edt, dt, cdt, t-cdt);
        printf("     second: expected %10lu got %10lu "
           "back to %10lu (%+ld)\n", est, st, cst, t-cst);
#ifndef DEBUG
        printf("TEST FAILED.\n");
        return 1;
#endif
    }
    }

#ifndef DEBUG
    printf("No discrepancies up to long time %llu\n", long_timer_time);
    printf("TEST PASSED.\n");
#endif

    return 0;
}

