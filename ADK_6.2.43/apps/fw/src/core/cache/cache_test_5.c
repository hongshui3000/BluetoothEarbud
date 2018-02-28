/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
#include "cache/cache.h"


static uint32 cache_test_fn_0(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 0;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 0;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 1;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_1(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 1;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 1;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 2;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_2(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 2;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 2;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 3;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_3(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 3;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 3;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 4;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_4(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 4;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 4;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 5;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_5(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 5;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 5;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 6;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_6(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 6;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 6;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 7;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_7(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 7;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 7;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 8;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_8(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 8;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 8;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 9;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_9(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 9;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 9;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 10;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_10(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 10;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 10;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 11;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_11(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 11;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 11;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 12;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_12(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 12;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 12;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 13;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_13(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 13;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 13;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 14;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_14(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 14;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 14;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 15;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_15(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 15;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 15;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 16;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_16(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 16;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 16;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 17;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_17(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 17;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 17;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 18;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_18(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 18;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 18;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 19;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_19(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 19;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 19;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 20;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_20(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 20;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 20;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 21;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_21(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 21;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 21;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 22;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_22(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 22;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 22;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 23;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_23(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 23;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 23;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 24;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_24(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 24;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 24;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 25;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_25(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 25;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 25;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 26;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_26(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 26;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 26;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 27;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_27(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 27;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 27;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 28;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_28(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 28;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 28;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 29;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_29(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 29;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 29;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 30;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_30(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 30;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 30;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 31;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_31(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 31;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 31;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 32;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_32(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 32;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 32;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 33;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_33(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 33;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 33;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 34;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_34(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 34;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 34;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 35;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_35(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 35;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 35;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 36;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_36(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 36;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 36;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 37;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_37(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 37;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 37;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 38;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_38(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 38;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 38;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 39;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_39(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 39;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 39;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 40;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_40(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 40;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 40;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 41;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_41(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 41;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 41;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 42;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_42(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 42;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 42;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 43;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_43(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 43;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 43;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 44;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_44(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 44;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 44;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 45;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_45(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 45;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 45;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 46;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_46(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 46;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 46;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 47;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_47(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 47;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 47;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 48;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_48(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 48;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 48;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 49;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_49(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 49;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 49;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 50;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_50(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 50;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 50;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 51;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_51(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 51;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 51;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 52;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_52(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 52;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 52;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 53;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_53(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 53;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 53;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 54;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_54(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 54;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 54;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 55;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_55(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 55;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 55;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 56;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_56(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 56;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 56;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 57;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_57(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 57;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 57;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 58;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_58(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 58;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 58;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 59;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_59(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 59;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 59;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 60;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_60(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 60;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 60;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 61;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_61(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 61;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 61;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 62;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_62(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 62;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 62;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 63;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_63(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 63;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 63;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 64;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_64(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 64;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 64;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 65;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_65(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 65;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 65;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 66;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_66(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 66;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 66;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 67;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_67(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 67;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 67;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 68;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_68(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 68;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 68;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 69;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_69(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 69;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 69;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 70;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_70(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 70;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 70;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 71;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_71(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 71;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 71;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 72;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_72(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 72;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 72;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 73;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_73(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 73;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 73;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 74;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_74(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 74;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 74;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 75;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_75(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 75;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 75;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 76;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_76(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 76;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 76;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 77;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_77(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 77;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 77;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 78;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_78(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 78;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 78;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 79;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_79(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 79;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 79;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 80;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_80(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 80;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 80;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 81;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_81(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 81;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 81;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 82;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_82(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 82;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 82;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 83;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_83(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 83;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 83;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 84;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_84(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 84;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 84;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 85;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_85(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 85;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 85;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 86;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_86(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 86;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 86;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 87;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_87(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 87;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 87;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 88;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_88(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 88;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 88;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 89;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_89(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 89;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 89;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 90;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_90(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 90;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 90;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 91;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_91(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 91;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 91;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 92;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_92(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 92;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 92;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 93;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_93(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 93;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 93;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 94;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_94(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 94;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 94;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 95;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_95(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 95;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 95;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 96;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_96(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 96;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 96;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 97;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_97(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 97;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 97;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 98;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_98(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 98;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 98;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 99;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_99(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 99;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 99;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 100;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_100(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 100;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 100;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 101;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_101(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 101;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 101;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 102;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_102(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 102;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 102;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 103;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_103(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 103;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 103;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 104;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_104(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 104;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 104;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 105;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_105(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 105;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 105;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 106;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_106(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 106;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 106;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 107;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_107(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 107;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 107;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 108;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_108(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 108;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 108;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 109;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_109(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 109;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 109;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 110;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_110(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 110;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 110;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 111;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_111(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 111;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 111;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 112;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_112(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 112;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 112;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 113;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_113(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 113;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 113;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 114;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_114(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 114;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 114;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 115;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_115(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 115;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 115;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 116;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_116(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 116;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 116;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 117;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_117(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 117;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 117;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 118;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_118(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 118;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 118;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 119;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_119(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 119;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 119;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 120;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_120(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 120;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 120;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 121;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_121(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 121;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 121;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 122;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_122(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 122;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 122;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 123;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_123(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 123;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 123;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 124;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_124(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 124;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 124;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 125;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_125(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 125;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 125;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 126;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_126(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 126;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 126;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 127;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_127(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 127;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 127;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 128;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_128(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 128;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 128;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 129;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_129(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 129;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 129;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 130;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_130(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 130;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 130;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 131;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_131(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 131;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 131;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 132;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_132(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 132;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 132;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 133;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_133(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 133;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 133;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 134;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_134(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 134;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 134;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 135;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_135(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 135;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 135;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 136;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_136(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 136;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 136;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 137;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_137(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 137;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 137;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 138;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_138(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 138;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 138;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 139;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_139(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 139;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 139;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 140;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_140(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 140;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 140;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 141;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_141(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 141;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 141;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 142;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_142(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 142;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 142;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 143;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_143(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 143;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 143;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 144;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_144(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 144;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 144;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 145;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_145(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 145;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 145;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 146;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_146(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 146;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 146;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 147;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_147(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 147;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 147;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 148;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_148(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 148;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 148;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 149;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_149(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 149;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 149;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 150;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_150(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 150;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 150;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 151;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_151(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 151;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 151;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 152;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_152(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 152;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 152;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 153;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_153(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 153;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 153;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 154;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_154(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 154;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 154;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 155;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_155(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 155;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 155;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 156;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_156(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 156;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 156;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 157;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_157(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 157;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 157;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 158;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_158(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 158;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 158;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 159;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_159(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 159;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 159;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 160;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_160(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 160;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 160;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 161;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_161(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 161;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 161;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 162;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_162(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 162;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 162;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 163;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_163(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 163;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 163;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 164;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_164(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 164;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 164;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 165;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_165(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 165;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 165;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 166;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_166(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 166;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 166;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 167;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_167(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 167;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 167;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 168;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_168(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 168;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 168;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 169;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_169(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 169;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 169;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 170;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_170(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 170;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 170;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 171;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_171(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 171;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 171;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 172;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_172(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 172;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 172;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 173;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_173(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 173;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 173;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 174;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_174(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 174;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 174;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 175;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_175(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 175;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 175;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 176;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_176(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 176;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 176;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 177;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_177(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 177;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 177;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 178;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_178(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 178;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 178;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 179;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_179(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 179;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 179;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 180;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_180(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 180;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 180;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 181;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_181(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 181;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 181;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 182;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_182(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 182;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 182;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 183;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_183(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 183;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 183;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 184;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_184(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 184;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 184;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 185;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_185(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 185;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 185;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 186;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_186(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 186;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 186;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 187;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_187(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 187;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 187;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 188;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_188(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 188;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 188;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 189;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_189(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 189;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 189;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 190;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_190(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 190;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 190;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 191;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_191(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 191;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 191;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 192;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_192(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 192;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 192;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 193;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_193(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 193;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 193;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 194;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_194(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 194;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 194;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 195;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_195(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 195;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 195;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 196;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_196(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 196;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 196;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 197;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_197(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 197;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 197;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 198;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_198(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 198;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 198;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 199;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_199(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 199;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 199;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 200;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_200(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 200;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 200;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 201;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_201(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 201;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 201;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 202;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_202(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 202;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 202;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 203;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_203(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 203;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 203;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 204;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_204(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 204;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 204;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 205;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_205(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 205;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 205;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 206;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_206(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 206;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 206;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 207;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_207(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 207;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 207;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 208;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_208(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 208;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 208;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 209;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_209(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 209;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 209;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 210;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_210(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 210;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 210;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 211;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_211(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 211;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 211;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 212;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_212(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 212;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 212;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 213;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_213(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 213;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 213;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 214;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_214(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 214;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 214;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 215;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_215(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 215;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 215;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 216;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_216(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 216;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 216;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 217;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_217(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 217;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 217;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 218;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_218(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 218;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 218;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 219;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_219(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 219;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 219;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 220;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_220(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 220;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 220;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 221;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_221(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 221;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 221;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 222;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_222(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 222;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 222;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 223;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_223(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 223;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 223;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 224;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_224(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 224;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 224;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 225;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_225(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 225;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 225;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 226;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_226(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 226;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 226;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 227;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_227(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 227;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 227;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 228;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_228(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 228;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 228;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 229;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_229(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 229;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 229;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 230;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_230(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 230;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 230;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 231;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_231(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 231;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 231;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 232;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_232(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 232;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 232;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 233;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_233(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 233;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 233;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 234;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_234(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 234;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 234;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 235;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_235(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 235;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 235;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 236;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_236(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 236;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 236;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 237;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_237(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 237;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 237;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 238;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_238(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 238;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 238;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 239;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_239(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 239;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 239;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 240;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_240(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 240;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 240;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 241;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_241(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 241;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 241;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 242;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_242(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 242;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 242;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 243;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_243(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 243;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 243;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 244;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_244(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 244;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 244;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 245;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_245(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 245;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 245;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 246;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_246(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 246;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 246;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 247;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_247(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 247;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 247;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 248;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_248(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 248;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 248;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 249;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_249(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 249;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 249;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 250;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_250(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 250;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 250;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 251;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_251(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 251;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 251;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 252;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_252(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 252;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 252;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 253;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_253(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 253;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 253;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 254;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_254(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 254;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 254;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 255;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_255(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 255;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 255;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 256;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_256(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 256;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 256;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 257;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_257(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 257;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 257;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 258;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_258(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 258;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 258;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 259;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_259(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 259;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 259;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 260;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_260(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 260;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 260;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 261;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_261(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 261;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 261;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 262;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_262(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 262;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 262;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 263;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_263(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 263;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 263;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 264;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_264(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 264;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 264;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 265;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_265(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 265;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 265;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 266;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_266(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 266;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 266;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 267;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_267(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 267;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 267;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 268;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_268(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 268;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 268;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 269;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_269(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 269;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 269;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 270;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_270(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 270;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 270;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 271;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_271(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 271;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 271;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 272;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_272(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 272;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 272;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 273;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_273(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 273;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 273;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 274;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_274(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 274;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 274;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 275;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_275(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 275;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 275;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 276;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_276(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 276;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 276;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 277;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_277(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 277;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 277;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 278;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_278(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 278;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 278;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 279;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_279(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 279;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 279;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 280;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_280(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 280;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 280;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 281;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_281(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 281;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 281;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 282;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_282(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 282;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 282;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 283;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_283(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 283;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 283;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 284;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_284(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 284;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 284;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 285;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_285(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 285;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 285;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 286;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_286(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 286;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 286;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 287;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_287(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 287;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 287;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 288;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_288(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 288;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 288;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 289;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_289(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 289;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 289;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 290;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_290(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 290;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 290;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 291;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_291(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 291;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 291;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 292;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_292(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 292;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 292;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 293;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_293(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 293;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 293;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 294;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_294(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 294;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 294;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 295;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_295(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 295;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 295;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 296;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_296(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 296;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 296;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 297;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_297(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 297;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 297;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 298;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_298(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 298;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 298;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 299;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_299(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 299;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 299;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 300;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_300(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 300;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 300;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 301;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_301(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 301;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 301;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 302;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_302(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 302;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 302;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 303;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_303(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 303;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 303;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 304;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_304(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 304;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 304;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 305;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_305(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 305;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 305;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 306;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_306(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 306;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 306;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 307;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_307(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 307;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 307;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 308;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_308(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 308;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 308;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 309;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_309(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 309;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 309;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 310;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_310(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 310;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 310;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 311;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_311(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 311;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 311;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 312;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_312(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 312;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 312;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 313;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_313(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 313;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 313;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 314;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_314(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 314;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 314;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 315;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_315(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 315;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 315;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 316;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_316(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 316;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 316;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 317;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_317(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 317;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 317;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 318;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_318(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 318;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 318;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 319;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_319(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 319;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 319;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 320;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_320(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 320;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 320;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 321;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_321(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 321;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 321;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 322;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_322(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 322;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 322;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 323;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_323(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 323;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 323;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 324;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_324(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 324;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 324;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 325;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_325(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 325;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 325;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 326;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_326(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 326;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 326;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 327;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_327(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 327;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 327;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 328;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_328(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 328;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 328;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 329;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_329(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 329;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 329;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 330;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_330(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 330;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 330;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 331;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_331(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 331;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 331;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 332;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_332(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 332;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 332;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 333;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_333(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 333;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 333;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 334;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_334(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 334;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 334;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 335;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_335(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 335;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 335;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 336;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_336(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 336;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 336;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 337;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_337(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 337;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 337;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 338;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_338(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 338;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 338;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 339;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_339(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 339;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 339;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 340;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_340(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 340;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 340;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 341;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_341(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 341;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 341;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 342;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_342(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 342;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 342;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 343;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_343(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 343;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 343;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 344;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_344(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 344;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 344;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 345;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_345(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 345;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 345;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 346;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_346(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 346;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 346;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 347;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_347(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 347;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 347;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 348;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_348(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 348;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 348;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 349;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_349(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 349;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 349;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 350;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_350(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 350;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 350;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 351;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_351(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 351;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 351;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 352;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_352(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 352;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 352;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 353;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_353(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 353;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 353;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 354;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_354(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 354;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 354;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 355;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_355(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 355;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 355;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 356;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_356(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 356;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 356;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 357;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_357(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 357;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 357;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 358;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_358(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 358;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 358;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 359;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_359(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 359;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 359;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 360;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_360(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 360;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 360;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 361;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_361(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 361;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 361;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 362;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_362(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 362;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 362;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 363;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_363(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 363;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 363;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 364;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_364(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 364;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 364;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 365;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_365(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 365;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 365;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 366;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_366(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 366;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 366;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 367;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_367(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 367;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 367;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 368;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_368(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 368;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 368;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 369;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_369(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 369;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 369;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 370;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_370(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 370;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 370;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 371;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_371(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 371;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 371;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 372;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_372(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 372;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 372;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 373;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_373(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 373;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 373;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 374;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_374(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 374;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 374;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 375;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_375(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 375;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 375;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 376;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_376(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 376;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 376;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 377;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_377(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 377;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 377;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 378;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_378(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 378;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 378;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 379;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_379(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 379;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 379;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 380;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_380(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 380;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 380;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 381;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_381(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 381;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 381;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 382;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_382(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 382;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 382;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 383;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_383(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 383;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 383;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 384;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_384(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 384;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 384;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 385;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_385(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 385;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 385;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 386;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_386(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 386;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 386;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 387;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_387(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 387;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 387;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 388;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_388(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 388;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 388;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 389;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_389(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 389;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 389;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 390;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_390(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 390;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 390;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 391;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_391(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 391;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 391;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 392;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_392(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 392;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 392;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 393;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_393(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 393;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 393;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 394;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_394(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 394;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 394;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 395;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_395(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 395;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 395;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 396;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_396(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 396;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 396;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 397;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_397(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 397;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 397;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 398;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_398(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 398;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 398;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 399;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_399(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 399;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 399;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 400;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_400(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 400;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 400;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 401;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_401(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 401;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 401;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 402;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_402(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 402;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 402;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 403;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_403(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 403;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 403;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 404;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_404(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 404;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 404;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 405;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_405(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 405;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 405;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 406;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_406(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 406;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 406;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 407;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_407(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 407;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 407;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 408;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_408(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 408;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 408;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 409;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_409(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 409;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 409;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 410;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_410(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 410;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 410;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 411;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_411(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 411;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 411;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 412;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_412(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 412;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 412;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 413;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_413(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 413;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 413;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 414;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_414(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 414;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 414;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 415;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_415(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 415;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 415;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 416;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_416(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 416;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 416;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 417;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_417(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 417;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 417;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 418;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_418(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 418;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 418;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 419;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_419(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 419;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 419;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 420;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_420(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 420;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 420;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 421;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_421(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 421;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 421;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 422;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_422(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 422;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 422;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 423;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_423(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 423;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 423;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 424;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_424(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 424;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 424;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 425;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_425(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 425;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 425;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 426;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_426(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 426;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 426;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 427;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_427(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 427;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 427;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 428;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_428(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 428;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 428;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 429;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_429(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 429;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 429;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 430;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_430(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 430;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 430;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 431;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_431(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 431;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 431;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 432;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_432(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 432;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 432;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 433;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_433(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 433;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 433;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 434;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_434(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 434;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 434;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 435;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_435(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 435;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 435;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 436;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_436(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 436;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 436;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 437;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_437(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 437;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 437;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 438;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_438(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 438;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 438;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 439;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_439(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 439;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 439;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 440;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_440(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 440;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 440;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 441;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_441(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 441;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 441;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 442;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_442(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 442;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 442;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 443;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_443(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 443;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 443;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 444;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_444(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 444;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 444;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 445;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_445(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 445;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 445;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 446;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_446(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 446;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 446;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 447;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_447(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 447;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 447;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 448;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_448(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 448;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 448;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 449;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_449(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 449;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 449;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 450;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_450(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 450;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 450;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 451;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_451(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 451;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 451;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 452;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_452(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 452;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 452;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 453;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_453(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 453;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 453;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 454;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_454(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 454;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 454;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 455;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_455(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 455;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 455;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 456;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_456(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 456;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 456;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 457;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_457(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 457;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 457;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 458;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_458(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 458;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 458;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 459;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_459(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 459;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 459;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 460;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_460(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 460;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 460;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 461;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_461(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 461;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 461;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 462;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_462(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 462;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 462;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 463;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_463(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 463;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 463;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 464;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_464(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 464;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 464;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 465;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_465(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 465;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 465;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 466;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_466(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 466;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 466;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 467;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_467(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 467;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 467;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 468;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_468(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 468;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 468;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 469;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_469(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 469;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 469;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 470;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_470(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 470;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 470;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 471;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_471(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 471;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 471;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 472;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_472(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 472;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 472;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 473;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_473(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 473;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 473;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 474;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_474(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 474;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 474;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 475;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_475(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 475;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 475;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 476;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_476(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 476;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 476;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 477;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_477(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 477;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 477;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 478;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_478(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 478;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 478;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 479;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_479(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 479;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 479;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 480;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_480(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 480;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 480;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 481;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_481(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 481;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 481;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 482;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_482(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 482;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 482;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 483;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_483(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 483;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 483;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 484;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_484(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 484;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 484;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 485;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_485(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 485;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 485;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 486;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_486(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 486;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 486;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 487;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_487(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 487;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 487;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 488;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_488(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 488;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 488;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 489;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_489(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 489;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 489;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 490;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_490(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 490;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 490;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 491;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_491(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 491;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 491;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 492;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_492(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 492;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 492;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 493;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_493(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 493;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 493;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 494;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_494(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 494;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 494;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 495;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_495(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 495;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 495;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 496;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_496(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 496;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 496;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 497;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_497(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 497;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 497;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 498;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_498(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 498;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 498;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 499;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_499(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 499;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 499;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 500;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_500(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 500;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 500;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 501;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_501(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 501;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 501;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 502;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_502(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 502;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 502;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 503;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_503(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 503;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 503;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 504;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_504(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 504;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 504;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 505;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_505(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 505;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 505;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 506;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_506(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 506;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 506;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 507;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_507(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 507;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 507;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 508;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_508(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 508;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 508;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 509;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_509(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 509;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 509;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 510;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_510(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 510;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 510;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 511;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_511(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 511;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 511;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 512;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_512(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 512;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 512;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 513;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_513(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 513;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 513;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 514;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_514(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 514;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 514;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 515;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_515(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 515;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 515;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 516;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_516(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 516;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 516;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 517;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_517(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 517;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 517;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 518;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_518(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 518;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 518;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 519;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_519(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 519;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 519;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 520;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_520(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 520;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 520;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 521;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_521(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 521;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 521;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 522;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_522(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 522;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 522;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 523;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_523(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 523;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 523;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 524;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_524(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 524;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 524;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 525;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_525(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 525;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 525;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 526;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_526(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 526;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 526;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 527;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_527(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 527;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 527;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 528;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_528(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 528;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 528;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 529;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_529(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 529;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 529;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 530;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_530(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 530;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 530;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 531;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_531(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 531;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 531;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 532;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_532(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 532;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 532;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 533;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_533(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 533;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 533;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 534;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_534(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 534;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 534;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 535;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_535(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 535;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 535;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 536;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_536(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 536;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 536;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 537;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_537(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 537;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 537;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 538;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_538(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 538;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 538;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 539;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_539(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 539;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 539;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 540;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_540(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 540;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 540;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 541;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_541(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 541;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 541;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 542;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_542(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 542;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 542;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 543;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_543(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 543;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 543;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 544;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_544(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 544;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 544;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 545;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_545(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 545;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 545;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 546;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_546(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 546;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 546;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 547;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_547(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 547;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 547;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 548;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_548(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 548;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 548;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 549;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_549(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 549;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 549;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 550;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_550(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 550;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 550;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 551;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_551(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 551;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 551;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 552;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_552(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 552;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 552;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 553;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_553(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 553;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 553;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 554;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_554(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 554;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 554;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 555;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_555(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 555;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 555;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 556;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_556(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 556;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 556;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 557;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_557(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 557;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 557;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 558;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_558(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 558;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 558;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 559;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_559(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 559;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 559;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 560;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_560(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 560;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 560;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 561;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_561(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 561;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 561;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 562;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_562(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 562;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 562;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 563;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_563(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 563;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 563;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 564;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_564(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 564;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 564;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 565;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_565(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 565;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 565;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 566;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_566(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 566;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 566;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 567;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_567(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 567;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 567;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 568;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_568(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 568;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 568;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 569;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_569(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 569;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 569;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 570;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_570(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 570;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 570;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 571;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_571(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 571;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 571;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 572;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_572(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 572;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 572;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 573;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_573(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 573;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 573;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 574;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_574(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 574;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 574;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 575;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_575(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 575;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 575;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 576;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_576(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 576;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 576;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 577;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_577(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 577;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 577;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 578;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_578(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 578;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 578;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 579;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_579(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 579;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 579;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 580;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_580(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 580;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 580;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 581;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_581(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 581;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 581;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 582;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_582(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 582;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 582;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 583;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_583(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 583;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 583;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 584;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_584(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 584;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 584;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 585;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_585(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 585;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 585;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 586;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_586(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 586;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 586;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 587;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_587(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 587;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 587;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 588;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_588(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 588;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 588;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 589;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_589(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 589;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 589;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 590;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_590(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 590;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 590;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 591;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_591(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 591;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 591;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 592;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_592(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 592;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 592;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 593;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_593(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 593;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 593;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 594;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_594(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 594;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 594;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 595;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_595(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 595;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 595;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 596;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_596(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 596;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 596;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 597;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_597(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 597;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 597;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 598;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_598(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 598;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 598;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 599;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_599(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 599;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 599;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 600;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_600(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 600;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 600;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 601;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_601(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 601;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 601;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 602;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_602(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 602;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 602;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 603;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_603(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 603;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 603;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 604;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_604(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 604;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 604;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 605;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_605(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 605;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 605;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 606;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_606(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 606;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 606;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 607;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_607(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 607;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 607;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 608;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_608(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 608;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 608;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 609;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_609(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 609;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 609;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 610;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_610(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 610;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 610;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 611;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_611(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 611;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 611;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 612;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_612(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 612;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 612;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 613;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_613(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 613;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 613;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 614;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_614(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 614;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 614;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 615;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_615(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 615;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 615;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 616;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_616(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 616;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 616;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 617;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_617(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 617;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 617;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 618;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_618(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 618;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 618;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 619;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_619(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 619;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 619;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 620;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_620(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 620;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 620;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 621;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_621(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 621;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 621;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 622;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_622(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 622;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 622;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 623;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_623(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 623;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 623;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 624;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_624(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 624;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 624;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 625;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_625(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 625;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 625;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 626;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_626(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 626;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 626;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 627;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_627(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 627;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 627;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 628;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_628(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 628;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 628;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 629;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_629(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 629;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 629;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 630;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_630(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 630;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 630;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 631;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_631(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 631;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 631;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 632;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_632(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 632;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 632;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 633;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_633(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 633;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 633;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 634;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_634(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 634;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 634;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 635;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_635(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 635;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 635;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 636;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_636(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 636;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 636;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 637;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_637(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 637;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 637;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 638;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_638(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 638;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 638;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 639;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_639(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 639;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 639;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 640;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_640(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 640;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 640;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 641;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_641(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 641;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 641;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 642;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_642(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 642;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 642;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 643;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_643(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 643;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 643;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 644;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_644(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 644;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 644;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 645;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_645(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 645;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 645;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 646;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_646(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 646;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 646;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 647;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_647(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 647;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 647;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 648;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_648(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 648;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 648;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 649;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_649(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 649;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 649;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 650;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_650(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 650;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 650;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 651;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_651(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 651;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 651;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 652;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_652(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 652;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 652;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 653;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_653(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 653;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 653;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 654;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_654(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 654;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 654;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 655;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_655(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 655;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 655;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 656;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_656(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 656;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 656;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 657;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_657(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 657;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 657;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 658;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_658(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 658;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 658;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 659;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_659(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 659;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 659;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 660;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_660(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 660;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 660;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 661;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_661(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 661;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 661;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 662;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_662(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 662;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 662;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 663;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_663(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 663;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 663;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 664;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_664(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 664;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 664;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 665;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_665(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 665;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 665;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 666;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_666(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 666;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 666;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 667;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_667(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 667;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 667;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 668;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_668(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 668;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 668;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 669;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_669(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 669;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 669;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 670;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_670(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 670;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 670;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 671;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_671(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 671;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 671;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 672;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_672(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 672;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 672;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 673;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_673(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 673;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 673;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 674;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_674(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 674;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 674;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 675;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_675(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 675;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 675;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 676;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_676(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 676;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 676;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 677;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_677(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 677;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 677;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 678;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_678(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 678;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 678;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 679;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_679(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 679;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 679;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 680;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_680(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 680;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 680;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 681;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_681(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 681;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 681;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 682;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_682(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 682;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 682;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 683;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_683(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 683;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 683;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 684;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_684(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 684;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 684;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 685;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_685(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 685;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 685;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 686;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_686(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 686;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 686;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 687;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_687(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 687;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 687;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 688;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_688(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 688;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 688;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 689;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_689(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 689;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 689;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 690;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_690(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 690;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 690;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 691;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_691(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 691;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 691;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 692;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_692(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 692;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 692;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 693;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_693(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 693;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 693;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 694;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_694(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 694;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 694;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 695;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_695(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 695;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 695;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 696;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_696(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 696;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 696;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 697;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_697(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 697;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 697;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 698;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_698(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 698;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 698;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 699;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_699(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 699;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 699;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 700;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_700(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 700;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 700;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 701;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_701(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 701;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 701;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 702;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_702(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 702;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 702;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 703;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_703(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 703;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 703;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 704;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_704(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 704;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 704;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 705;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_705(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 705;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 705;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 706;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_706(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 706;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 706;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 707;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_707(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 707;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 707;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 708;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_708(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 708;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 708;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 709;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_709(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 709;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 709;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 710;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_710(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 710;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 710;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 711;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_711(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 711;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 711;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 712;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_712(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 712;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 712;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 713;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_713(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 713;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 713;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 714;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_714(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 714;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 714;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 715;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_715(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 715;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 715;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 716;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_716(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 716;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 716;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 717;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_717(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 717;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 717;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 718;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_718(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 718;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 718;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 719;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_719(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 719;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 719;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 720;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_720(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 720;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 720;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 721;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_721(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 721;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 721;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 722;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_722(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 722;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 722;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 723;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_723(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 723;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 723;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 724;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_724(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 724;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 724;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 725;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_725(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 725;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 725;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 726;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_726(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 726;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 726;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 727;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_727(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 727;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 727;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 728;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_728(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 728;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 728;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 729;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_729(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 729;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 729;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 730;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_730(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 730;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 730;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 731;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_731(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 731;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 731;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 732;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_732(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 732;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 732;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 733;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_733(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 733;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 733;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 734;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_734(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 734;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 734;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 735;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_735(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 735;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 735;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 736;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_736(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 736;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 736;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 737;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_737(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 737;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 737;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 738;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_738(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 738;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 738;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 739;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_739(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 739;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 739;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 740;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_740(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 740;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 740;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 741;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_741(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 741;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 741;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 742;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_742(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 742;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 742;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 743;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_743(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 743;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 743;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 744;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_744(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 744;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 744;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 745;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_745(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 745;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 745;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 746;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_746(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 746;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 746;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 747;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_747(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 747;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 747;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 748;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_748(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 748;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 748;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 749;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_749(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 749;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 749;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 750;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_750(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 750;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 750;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 751;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_751(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 751;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 751;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 752;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_752(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 752;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 752;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 753;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_753(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 753;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 753;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 754;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_754(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 754;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 754;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 755;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_755(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 755;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 755;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 756;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_756(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 756;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 756;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 757;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_757(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 757;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 757;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 758;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_758(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 758;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 758;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 759;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_759(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 759;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 759;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 760;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_760(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 760;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 760;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 761;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_761(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 761;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 761;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 762;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_762(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 762;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 762;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 763;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_763(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 763;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 763;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 764;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_764(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 764;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 764;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 765;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_765(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 765;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 765;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 766;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_766(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 766;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 766;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 767;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_767(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 767;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 767;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 768;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_768(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 768;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 768;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 769;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_769(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 769;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 769;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 770;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_770(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 770;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 770;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 771;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_771(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 771;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 771;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 772;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_772(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 772;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 772;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 773;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_773(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 773;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 773;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 774;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_774(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 774;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 774;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 775;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_775(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 775;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 775;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 776;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_776(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 776;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 776;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 777;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_777(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 777;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 777;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 778;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_778(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 778;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 778;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 779;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_779(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 779;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 779;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 780;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_780(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 780;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 780;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 781;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_781(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 781;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 781;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 782;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_782(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 782;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 782;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 783;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_783(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 783;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 783;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 784;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_784(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 784;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 784;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 785;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_785(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 785;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 785;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 786;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_786(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 786;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 786;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 787;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_787(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 787;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 787;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 788;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_788(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 788;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 788;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 789;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_789(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 789;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 789;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 790;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_790(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 790;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 790;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 791;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_791(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 791;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 791;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 792;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_792(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 792;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 792;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 793;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_793(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 793;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 793;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 794;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_794(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 794;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 794;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 795;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_795(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 795;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 795;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 796;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_796(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 796;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 796;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 797;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_797(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 797;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 797;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 798;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_798(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 798;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 798;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 799;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_799(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 799;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 799;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 800;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_800(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 800;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 800;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 801;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_801(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 801;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 801;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 802;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_802(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 802;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 802;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 803;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_803(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 803;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 803;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 804;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_804(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 804;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 804;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 805;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_805(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 805;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 805;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 806;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_806(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 806;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 806;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 807;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_807(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 807;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 807;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 808;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_808(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 808;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 808;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 809;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_809(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 809;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 809;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 810;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_810(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 810;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 810;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 811;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_811(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 811;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 811;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 812;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_812(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 812;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 812;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 813;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_813(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 813;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 813;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 814;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_814(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 814;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 814;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 815;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_815(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 815;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 815;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 816;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_816(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 816;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 816;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 817;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_817(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 817;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 817;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 818;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_818(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 818;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 818;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 819;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_819(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 819;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 819;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 820;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_820(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 820;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 820;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 821;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_821(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 821;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 821;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 822;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_822(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 822;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 822;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 823;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_823(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 823;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 823;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 824;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_824(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 824;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 824;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 825;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_825(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 825;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 825;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 826;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_826(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 826;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 826;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 827;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_827(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 827;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 827;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 828;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_828(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 828;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 828;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 829;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_829(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 829;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 829;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 830;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_830(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 830;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 830;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 831;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_831(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 831;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 831;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 832;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_832(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 832;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 832;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 833;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_833(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 833;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 833;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 834;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_834(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 834;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 834;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 835;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_835(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 835;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 835;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 836;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_836(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 836;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 836;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 837;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_837(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 837;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 837;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 838;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_838(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 838;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 838;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 839;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_839(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 839;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 839;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 840;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_840(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 840;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 840;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 841;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_841(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 841;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 841;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 842;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_842(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 842;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 842;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 843;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_843(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 843;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 843;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 844;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_844(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 844;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 844;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 845;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_845(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 845;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 845;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 846;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_846(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 846;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 846;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 847;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_847(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 847;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 847;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 848;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_848(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 848;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 848;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 849;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_849(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 849;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 849;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 850;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_850(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 850;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 850;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 851;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_851(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 851;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 851;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 852;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_852(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 852;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 852;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 853;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_853(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 853;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 853;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 854;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_854(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 854;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 854;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 855;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_855(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 855;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 855;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 856;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_856(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 856;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 856;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 857;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_857(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 857;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 857;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 858;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_858(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 858;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 858;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 859;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_859(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 859;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 859;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 860;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_860(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 860;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 860;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 861;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_861(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 861;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 861;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 862;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_862(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 862;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 862;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 863;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_863(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 863;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 863;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 864;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_864(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 864;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 864;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 865;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_865(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 865;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 865;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 866;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_866(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 866;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 866;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 867;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_867(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 867;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 867;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 868;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_868(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 868;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 868;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 869;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_869(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 869;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 869;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 870;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_870(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 870;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 870;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 871;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_871(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 871;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 871;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 872;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_872(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 872;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 872;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 873;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_873(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 873;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 873;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 874;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_874(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 874;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 874;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 875;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_875(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 875;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 875;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 876;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_876(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 876;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 876;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 877;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_877(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 877;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 877;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 878;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_878(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 878;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 878;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 879;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_879(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 879;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 879;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 880;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_880(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 880;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 880;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 881;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_881(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 881;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 881;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 882;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_882(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 882;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 882;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 883;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_883(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 883;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 883;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 884;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_884(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 884;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 884;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 885;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_885(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 885;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 885;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 886;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_886(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 886;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 886;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 887;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_887(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 887;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 887;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 888;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_888(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 888;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 888;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 889;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_889(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 889;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 889;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 890;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_890(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 890;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 890;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 891;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_891(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 891;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 891;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 892;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_892(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 892;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 892;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 893;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_893(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 893;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 893;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 894;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_894(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 894;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 894;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 895;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_895(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 895;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 895;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 896;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_896(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 896;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 896;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 897;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_897(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 897;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 897;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 898;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_898(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 898;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 898;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 899;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_899(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 899;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 899;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 900;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_900(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 900;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 900;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 901;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_901(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 901;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 901;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 902;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_902(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 902;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 902;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 903;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_903(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 903;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 903;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 904;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_904(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 904;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 904;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 905;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_905(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 905;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 905;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 906;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_906(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 906;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 906;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 907;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_907(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 907;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 907;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 908;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_908(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 908;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 908;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 909;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_909(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 909;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 909;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 910;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_910(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 910;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 910;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 911;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_911(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 911;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 911;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 912;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_912(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 912;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 912;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 913;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_913(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 913;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 913;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 914;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_914(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 914;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 914;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 915;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_915(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 915;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 915;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 916;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_916(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 916;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 916;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 917;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_917(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 917;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 917;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 918;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_918(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 918;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 918;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 919;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_919(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 919;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 919;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 920;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_920(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 920;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 920;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 921;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_921(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 921;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 921;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 922;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_922(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 922;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 922;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 923;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_923(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 923;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 923;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 924;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_924(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 924;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 924;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 925;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_925(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 925;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 925;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 926;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_926(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 926;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 926;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 927;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_927(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 927;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 927;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 928;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_928(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 928;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 928;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 929;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_929(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 929;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 929;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 930;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_930(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 930;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 930;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 931;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_931(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 931;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 931;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 932;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_932(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 932;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 932;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 933;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_933(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 933;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 933;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 934;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_934(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 934;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 934;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 935;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_935(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 935;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 935;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 936;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_936(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 936;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 936;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 937;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_937(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 937;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 937;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 938;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_938(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 938;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 938;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 939;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_939(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 939;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 939;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 940;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_940(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 940;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 940;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 941;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_941(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 941;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 941;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 942;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_942(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 942;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 942;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 943;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_943(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 943;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 943;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 944;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_944(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 944;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 944;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 945;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_945(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 945;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 945;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 946;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_946(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 946;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 946;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 947;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_947(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 947;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 947;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 948;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_948(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 948;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 948;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 949;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_949(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 949;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 949;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 950;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_950(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 950;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 950;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 951;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_951(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 951;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 951;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 952;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_952(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 952;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 952;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 953;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_953(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 953;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 953;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 954;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_954(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 954;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 954;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 955;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_955(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 955;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 955;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 956;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_956(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 956;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 956;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 957;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_957(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 957;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 957;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 958;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_958(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 958;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 958;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 959;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_959(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 959;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 959;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 960;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_960(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 960;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 960;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 961;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_961(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 961;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 961;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 962;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_962(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 962;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 962;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 963;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_963(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 963;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 963;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 964;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_964(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 964;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 964;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 965;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_965(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 965;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 965;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 966;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_966(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 966;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 966;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 967;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_967(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 967;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 967;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 968;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_968(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 968;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 968;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 969;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_969(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 969;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 969;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 970;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_970(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 970;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 970;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 971;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_971(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 971;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 971;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 972;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_972(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 972;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 972;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 973;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_973(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 973;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 973;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 974;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_974(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 974;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 974;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 975;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_975(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 975;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 975;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 976;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_976(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 976;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 976;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 977;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_977(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 977;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 977;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 978;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_978(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 978;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 978;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 979;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_979(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 979;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 979;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 980;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_980(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 980;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 980;
            uint32 shift = (loop+2) & cache_const_mask[0];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 981;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_981(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 981;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 981;
            uint32 shift = (loop+2) & cache_const_mask[1];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 982;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_982(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 982;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 982;
            uint32 shift = (loop+2) & cache_const_mask[2];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 983;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_983(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 983;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 983;
            uint32 shift = (loop+2) & cache_const_mask[3];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 984;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_984(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 984;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 984;
            uint32 shift = (loop+2) & cache_const_mask[4];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 985;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_985(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 985;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 985;
            uint32 shift = (loop+2) & cache_const_mask[5];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 986;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_986(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 986;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 986;
            uint32 shift = (loop+2) & cache_const_mask[6];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 987;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_987(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 987;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 987;
            uint32 shift = (loop+2) & cache_const_mask[7];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 988;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_988(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 988;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 988;
            uint32 shift = (loop+2) & cache_const_mask[8];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 989;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_989(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 989;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 989;
            uint32 shift = (loop+2) & cache_const_mask[9];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 990;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_990(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 990;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 990;
            uint32 shift = (loop+2) & cache_const_mask[10];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 991;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_991(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 991;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 991;
            uint32 shift = (loop+2) & cache_const_mask[11];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 992;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_992(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 992;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 992;
            uint32 shift = (loop+2) & cache_const_mask[12];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 993;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_993(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 993;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 993;
            uint32 shift = (loop+2) & cache_const_mask[13];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 994;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_994(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 994;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 994;
            uint32 shift = (loop+2) & cache_const_mask[14];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 995;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_995(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 995;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 995;
            uint32 shift = (loop+2) & cache_const_mask[15];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 996;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_996(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 996;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 996;
            uint32 shift = (loop+2) & cache_const_mask[16];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 997;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_997(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 997;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 997;
            uint32 shift = (loop+2) & cache_const_mask[17];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 998;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_998(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 998;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 998;
            uint32 shift = (loop+2) & cache_const_mask[18];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 999;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}

static uint32 cache_test_fn_999(uint32 seed, uint32 loops)
{
    uint32 loop;
    uint32 value = seed * 999;
    if (value & 0x20)
    {
        for(loop=0; loop<loops; ++loop)
        {
            uint32 xor_value = value + loop + 999;
            uint32 shift = (loop+2) & cache_const_mask[19];
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    }
    else
        for(loop=loops; loop; --loop)
        {
            uint32 xor_value = value + loop + 1000;
            uint32 shift = (loop+7) & 31;
            xor_value = (xor_value << shift) | (xor_value >> (32-shift));
            value = value ^ xor_value;
        }
    return value;
}
uint32 cache_test_5(uint32 value, uint32 loops)
{
    value = cache_test_fn_0(value, loops);
    value = cache_test_fn_1(value, loops);
    value = cache_test_fn_2(value, loops);
    value = cache_test_fn_3(value, loops);
    value = cache_test_fn_4(value, loops);
    value = cache_test_fn_5(value, loops);
    value = cache_test_fn_6(value, loops);
    value = cache_test_fn_7(value, loops);
    value = cache_test_fn_8(value, loops);
    value = cache_test_fn_9(value, loops);
    value = cache_test_fn_10(value, loops);
    value = cache_test_fn_11(value, loops);
    value = cache_test_fn_12(value, loops);
    value = cache_test_fn_13(value, loops);
    value = cache_test_fn_14(value, loops);
    value = cache_test_fn_15(value, loops);
    value = cache_test_fn_16(value, loops);
    value = cache_test_fn_17(value, loops);
    value = cache_test_fn_18(value, loops);
    value = cache_test_fn_19(value, loops);
    value = cache_test_fn_20(value, loops);
    value = cache_test_fn_21(value, loops);
    value = cache_test_fn_22(value, loops);
    value = cache_test_fn_23(value, loops);
    value = cache_test_fn_24(value, loops);
    value = cache_test_fn_25(value, loops);
    value = cache_test_fn_26(value, loops);
    value = cache_test_fn_27(value, loops);
    value = cache_test_fn_28(value, loops);
    value = cache_test_fn_29(value, loops);
    value = cache_test_fn_30(value, loops);
    value = cache_test_fn_31(value, loops);
    value = cache_test_fn_32(value, loops);
    value = cache_test_fn_33(value, loops);
    value = cache_test_fn_34(value, loops);
    value = cache_test_fn_35(value, loops);
    value = cache_test_fn_36(value, loops);
    value = cache_test_fn_37(value, loops);
    value = cache_test_fn_38(value, loops);
    value = cache_test_fn_39(value, loops);
    value = cache_test_fn_40(value, loops);
    value = cache_test_fn_41(value, loops);
    value = cache_test_fn_42(value, loops);
    value = cache_test_fn_43(value, loops);
    value = cache_test_fn_44(value, loops);
    value = cache_test_fn_45(value, loops);
    value = cache_test_fn_46(value, loops);
    value = cache_test_fn_47(value, loops);
    value = cache_test_fn_48(value, loops);
    value = cache_test_fn_49(value, loops);
    value = cache_test_fn_50(value, loops);
    value = cache_test_fn_51(value, loops);
    value = cache_test_fn_52(value, loops);
    value = cache_test_fn_53(value, loops);
    value = cache_test_fn_54(value, loops);
    value = cache_test_fn_55(value, loops);
    value = cache_test_fn_56(value, loops);
    value = cache_test_fn_57(value, loops);
    value = cache_test_fn_58(value, loops);
    value = cache_test_fn_59(value, loops);
    value = cache_test_fn_60(value, loops);
    value = cache_test_fn_61(value, loops);
    value = cache_test_fn_62(value, loops);
    value = cache_test_fn_63(value, loops);
    value = cache_test_fn_64(value, loops);
    value = cache_test_fn_65(value, loops);
    value = cache_test_fn_66(value, loops);
    value = cache_test_fn_67(value, loops);
    value = cache_test_fn_68(value, loops);
    value = cache_test_fn_69(value, loops);
    value = cache_test_fn_70(value, loops);
    value = cache_test_fn_71(value, loops);
    value = cache_test_fn_72(value, loops);
    value = cache_test_fn_73(value, loops);
    value = cache_test_fn_74(value, loops);
    value = cache_test_fn_75(value, loops);
    value = cache_test_fn_76(value, loops);
    value = cache_test_fn_77(value, loops);
    value = cache_test_fn_78(value, loops);
    value = cache_test_fn_79(value, loops);
    value = cache_test_fn_80(value, loops);
    value = cache_test_fn_81(value, loops);
    value = cache_test_fn_82(value, loops);
    value = cache_test_fn_83(value, loops);
    value = cache_test_fn_84(value, loops);
    value = cache_test_fn_85(value, loops);
    value = cache_test_fn_86(value, loops);
    value = cache_test_fn_87(value, loops);
    value = cache_test_fn_88(value, loops);
    value = cache_test_fn_89(value, loops);
    value = cache_test_fn_90(value, loops);
    value = cache_test_fn_91(value, loops);
    value = cache_test_fn_92(value, loops);
    value = cache_test_fn_93(value, loops);
    value = cache_test_fn_94(value, loops);
    value = cache_test_fn_95(value, loops);
    value = cache_test_fn_96(value, loops);
    value = cache_test_fn_97(value, loops);
    value = cache_test_fn_98(value, loops);
    value = cache_test_fn_99(value, loops);
    value = cache_test_fn_100(value, loops);
    value = cache_test_fn_101(value, loops);
    value = cache_test_fn_102(value, loops);
    value = cache_test_fn_103(value, loops);
    value = cache_test_fn_104(value, loops);
    value = cache_test_fn_105(value, loops);
    value = cache_test_fn_106(value, loops);
    value = cache_test_fn_107(value, loops);
    value = cache_test_fn_108(value, loops);
    value = cache_test_fn_109(value, loops);
    value = cache_test_fn_110(value, loops);
    value = cache_test_fn_111(value, loops);
    value = cache_test_fn_112(value, loops);
    value = cache_test_fn_113(value, loops);
    value = cache_test_fn_114(value, loops);
    value = cache_test_fn_115(value, loops);
    value = cache_test_fn_116(value, loops);
    value = cache_test_fn_117(value, loops);
    value = cache_test_fn_118(value, loops);
    value = cache_test_fn_119(value, loops);
    value = cache_test_fn_120(value, loops);
    value = cache_test_fn_121(value, loops);
    value = cache_test_fn_122(value, loops);
    value = cache_test_fn_123(value, loops);
    value = cache_test_fn_124(value, loops);
    value = cache_test_fn_125(value, loops);
    value = cache_test_fn_126(value, loops);
    value = cache_test_fn_127(value, loops);
    value = cache_test_fn_128(value, loops);
    value = cache_test_fn_129(value, loops);
    value = cache_test_fn_130(value, loops);
    value = cache_test_fn_131(value, loops);
    value = cache_test_fn_132(value, loops);
    value = cache_test_fn_133(value, loops);
    value = cache_test_fn_134(value, loops);
    value = cache_test_fn_135(value, loops);
    value = cache_test_fn_136(value, loops);
    value = cache_test_fn_137(value, loops);
    value = cache_test_fn_138(value, loops);
    value = cache_test_fn_139(value, loops);
    value = cache_test_fn_140(value, loops);
    value = cache_test_fn_141(value, loops);
    value = cache_test_fn_142(value, loops);
    value = cache_test_fn_143(value, loops);
    value = cache_test_fn_144(value, loops);
    value = cache_test_fn_145(value, loops);
    value = cache_test_fn_146(value, loops);
    value = cache_test_fn_147(value, loops);
    value = cache_test_fn_148(value, loops);
    value = cache_test_fn_149(value, loops);
    value = cache_test_fn_150(value, loops);
    value = cache_test_fn_151(value, loops);
    value = cache_test_fn_152(value, loops);
    value = cache_test_fn_153(value, loops);
    value = cache_test_fn_154(value, loops);
    value = cache_test_fn_155(value, loops);
    value = cache_test_fn_156(value, loops);
    value = cache_test_fn_157(value, loops);
    value = cache_test_fn_158(value, loops);
    value = cache_test_fn_159(value, loops);
    value = cache_test_fn_160(value, loops);
    value = cache_test_fn_161(value, loops);
    value = cache_test_fn_162(value, loops);
    value = cache_test_fn_163(value, loops);
    value = cache_test_fn_164(value, loops);
    value = cache_test_fn_165(value, loops);
    value = cache_test_fn_166(value, loops);
    value = cache_test_fn_167(value, loops);
    value = cache_test_fn_168(value, loops);
    value = cache_test_fn_169(value, loops);
    value = cache_test_fn_170(value, loops);
    value = cache_test_fn_171(value, loops);
    value = cache_test_fn_172(value, loops);
    value = cache_test_fn_173(value, loops);
    value = cache_test_fn_174(value, loops);
    value = cache_test_fn_175(value, loops);
    value = cache_test_fn_176(value, loops);
    value = cache_test_fn_177(value, loops);
    value = cache_test_fn_178(value, loops);
    value = cache_test_fn_179(value, loops);
    value = cache_test_fn_180(value, loops);
    value = cache_test_fn_181(value, loops);
    value = cache_test_fn_182(value, loops);
    value = cache_test_fn_183(value, loops);
    value = cache_test_fn_184(value, loops);
    value = cache_test_fn_185(value, loops);
    value = cache_test_fn_186(value, loops);
    value = cache_test_fn_187(value, loops);
    value = cache_test_fn_188(value, loops);
    value = cache_test_fn_189(value, loops);
    value = cache_test_fn_190(value, loops);
    value = cache_test_fn_191(value, loops);
    value = cache_test_fn_192(value, loops);
    value = cache_test_fn_193(value, loops);
    value = cache_test_fn_194(value, loops);
    value = cache_test_fn_195(value, loops);
    value = cache_test_fn_196(value, loops);
    value = cache_test_fn_197(value, loops);
    value = cache_test_fn_198(value, loops);
    value = cache_test_fn_199(value, loops);
    value = cache_test_fn_200(value, loops);
    value = cache_test_fn_201(value, loops);
    value = cache_test_fn_202(value, loops);
    value = cache_test_fn_203(value, loops);
    value = cache_test_fn_204(value, loops);
    value = cache_test_fn_205(value, loops);
    value = cache_test_fn_206(value, loops);
    value = cache_test_fn_207(value, loops);
    value = cache_test_fn_208(value, loops);
    value = cache_test_fn_209(value, loops);
    value = cache_test_fn_210(value, loops);
    value = cache_test_fn_211(value, loops);
    value = cache_test_fn_212(value, loops);
    value = cache_test_fn_213(value, loops);
    value = cache_test_fn_214(value, loops);
    value = cache_test_fn_215(value, loops);
    value = cache_test_fn_216(value, loops);
    value = cache_test_fn_217(value, loops);
    value = cache_test_fn_218(value, loops);
    value = cache_test_fn_219(value, loops);
    value = cache_test_fn_220(value, loops);
    value = cache_test_fn_221(value, loops);
    value = cache_test_fn_222(value, loops);
    value = cache_test_fn_223(value, loops);
    value = cache_test_fn_224(value, loops);
    value = cache_test_fn_225(value, loops);
    value = cache_test_fn_226(value, loops);
    value = cache_test_fn_227(value, loops);
    value = cache_test_fn_228(value, loops);
    value = cache_test_fn_229(value, loops);
    value = cache_test_fn_230(value, loops);
    value = cache_test_fn_231(value, loops);
    value = cache_test_fn_232(value, loops);
    value = cache_test_fn_233(value, loops);
    value = cache_test_fn_234(value, loops);
    value = cache_test_fn_235(value, loops);
    value = cache_test_fn_236(value, loops);
    value = cache_test_fn_237(value, loops);
    value = cache_test_fn_238(value, loops);
    value = cache_test_fn_239(value, loops);
    value = cache_test_fn_240(value, loops);
    value = cache_test_fn_241(value, loops);
    value = cache_test_fn_242(value, loops);
    value = cache_test_fn_243(value, loops);
    value = cache_test_fn_244(value, loops);
    value = cache_test_fn_245(value, loops);
    value = cache_test_fn_246(value, loops);
    value = cache_test_fn_247(value, loops);
    value = cache_test_fn_248(value, loops);
    value = cache_test_fn_249(value, loops);
    value = cache_test_fn_250(value, loops);
    value = cache_test_fn_251(value, loops);
    value = cache_test_fn_252(value, loops);
    value = cache_test_fn_253(value, loops);
    value = cache_test_fn_254(value, loops);
    value = cache_test_fn_255(value, loops);
    value = cache_test_fn_256(value, loops);
    value = cache_test_fn_257(value, loops);
    value = cache_test_fn_258(value, loops);
    value = cache_test_fn_259(value, loops);
    value = cache_test_fn_260(value, loops);
    value = cache_test_fn_261(value, loops);
    value = cache_test_fn_262(value, loops);
    value = cache_test_fn_263(value, loops);
    value = cache_test_fn_264(value, loops);
    value = cache_test_fn_265(value, loops);
    value = cache_test_fn_266(value, loops);
    value = cache_test_fn_267(value, loops);
    value = cache_test_fn_268(value, loops);
    value = cache_test_fn_269(value, loops);
    value = cache_test_fn_270(value, loops);
    value = cache_test_fn_271(value, loops);
    value = cache_test_fn_272(value, loops);
    value = cache_test_fn_273(value, loops);
    value = cache_test_fn_274(value, loops);
    value = cache_test_fn_275(value, loops);
    value = cache_test_fn_276(value, loops);
    value = cache_test_fn_277(value, loops);
    value = cache_test_fn_278(value, loops);
    value = cache_test_fn_279(value, loops);
    value = cache_test_fn_280(value, loops);
    value = cache_test_fn_281(value, loops);
    value = cache_test_fn_282(value, loops);
    value = cache_test_fn_283(value, loops);
    value = cache_test_fn_284(value, loops);
    value = cache_test_fn_285(value, loops);
    value = cache_test_fn_286(value, loops);
    value = cache_test_fn_287(value, loops);
    value = cache_test_fn_288(value, loops);
    value = cache_test_fn_289(value, loops);
    value = cache_test_fn_290(value, loops);
    value = cache_test_fn_291(value, loops);
    value = cache_test_fn_292(value, loops);
    value = cache_test_fn_293(value, loops);
    value = cache_test_fn_294(value, loops);
    value = cache_test_fn_295(value, loops);
    value = cache_test_fn_296(value, loops);
    value = cache_test_fn_297(value, loops);
    value = cache_test_fn_298(value, loops);
    value = cache_test_fn_299(value, loops);
    value = cache_test_fn_300(value, loops);
    value = cache_test_fn_301(value, loops);
    value = cache_test_fn_302(value, loops);
    value = cache_test_fn_303(value, loops);
    value = cache_test_fn_304(value, loops);
    value = cache_test_fn_305(value, loops);
    value = cache_test_fn_306(value, loops);
    value = cache_test_fn_307(value, loops);
    value = cache_test_fn_308(value, loops);
    value = cache_test_fn_309(value, loops);
    value = cache_test_fn_310(value, loops);
    value = cache_test_fn_311(value, loops);
    value = cache_test_fn_312(value, loops);
    value = cache_test_fn_313(value, loops);
    value = cache_test_fn_314(value, loops);
    value = cache_test_fn_315(value, loops);
    value = cache_test_fn_316(value, loops);
    value = cache_test_fn_317(value, loops);
    value = cache_test_fn_318(value, loops);
    value = cache_test_fn_319(value, loops);
    value = cache_test_fn_320(value, loops);
    value = cache_test_fn_321(value, loops);
    value = cache_test_fn_322(value, loops);
    value = cache_test_fn_323(value, loops);
    value = cache_test_fn_324(value, loops);
    value = cache_test_fn_325(value, loops);
    value = cache_test_fn_326(value, loops);
    value = cache_test_fn_327(value, loops);
    value = cache_test_fn_328(value, loops);
    value = cache_test_fn_329(value, loops);
    value = cache_test_fn_330(value, loops);
    value = cache_test_fn_331(value, loops);
    value = cache_test_fn_332(value, loops);
    value = cache_test_fn_333(value, loops);
    value = cache_test_fn_334(value, loops);
    value = cache_test_fn_335(value, loops);
    value = cache_test_fn_336(value, loops);
    value = cache_test_fn_337(value, loops);
    value = cache_test_fn_338(value, loops);
    value = cache_test_fn_339(value, loops);
    value = cache_test_fn_340(value, loops);
    value = cache_test_fn_341(value, loops);
    value = cache_test_fn_342(value, loops);
    value = cache_test_fn_343(value, loops);
    value = cache_test_fn_344(value, loops);
    value = cache_test_fn_345(value, loops);
    value = cache_test_fn_346(value, loops);
    value = cache_test_fn_347(value, loops);
    value = cache_test_fn_348(value, loops);
    value = cache_test_fn_349(value, loops);
    value = cache_test_fn_350(value, loops);
    value = cache_test_fn_351(value, loops);
    value = cache_test_fn_352(value, loops);
    value = cache_test_fn_353(value, loops);
    value = cache_test_fn_354(value, loops);
    value = cache_test_fn_355(value, loops);
    value = cache_test_fn_356(value, loops);
    value = cache_test_fn_357(value, loops);
    value = cache_test_fn_358(value, loops);
    value = cache_test_fn_359(value, loops);
    value = cache_test_fn_360(value, loops);
    value = cache_test_fn_361(value, loops);
    value = cache_test_fn_362(value, loops);
    value = cache_test_fn_363(value, loops);
    value = cache_test_fn_364(value, loops);
    value = cache_test_fn_365(value, loops);
    value = cache_test_fn_366(value, loops);
    value = cache_test_fn_367(value, loops);
    value = cache_test_fn_368(value, loops);
    value = cache_test_fn_369(value, loops);
    value = cache_test_fn_370(value, loops);
    value = cache_test_fn_371(value, loops);
    value = cache_test_fn_372(value, loops);
    value = cache_test_fn_373(value, loops);
    value = cache_test_fn_374(value, loops);
    value = cache_test_fn_375(value, loops);
    value = cache_test_fn_376(value, loops);
    value = cache_test_fn_377(value, loops);
    value = cache_test_fn_378(value, loops);
    value = cache_test_fn_379(value, loops);
    value = cache_test_fn_380(value, loops);
    value = cache_test_fn_381(value, loops);
    value = cache_test_fn_382(value, loops);
    value = cache_test_fn_383(value, loops);
    value = cache_test_fn_384(value, loops);
    value = cache_test_fn_385(value, loops);
    value = cache_test_fn_386(value, loops);
    value = cache_test_fn_387(value, loops);
    value = cache_test_fn_388(value, loops);
    value = cache_test_fn_389(value, loops);
    value = cache_test_fn_390(value, loops);
    value = cache_test_fn_391(value, loops);
    value = cache_test_fn_392(value, loops);
    value = cache_test_fn_393(value, loops);
    value = cache_test_fn_394(value, loops);
    value = cache_test_fn_395(value, loops);
    value = cache_test_fn_396(value, loops);
    value = cache_test_fn_397(value, loops);
    value = cache_test_fn_398(value, loops);
    value = cache_test_fn_399(value, loops);
    value = cache_test_fn_400(value, loops);
    value = cache_test_fn_401(value, loops);
    value = cache_test_fn_402(value, loops);
    value = cache_test_fn_403(value, loops);
    value = cache_test_fn_404(value, loops);
    value = cache_test_fn_405(value, loops);
    value = cache_test_fn_406(value, loops);
    value = cache_test_fn_407(value, loops);
    value = cache_test_fn_408(value, loops);
    value = cache_test_fn_409(value, loops);
    value = cache_test_fn_410(value, loops);
    value = cache_test_fn_411(value, loops);
    value = cache_test_fn_412(value, loops);
    value = cache_test_fn_413(value, loops);
    value = cache_test_fn_414(value, loops);
    value = cache_test_fn_415(value, loops);
    value = cache_test_fn_416(value, loops);
    value = cache_test_fn_417(value, loops);
    value = cache_test_fn_418(value, loops);
    value = cache_test_fn_419(value, loops);
    value = cache_test_fn_420(value, loops);
    value = cache_test_fn_421(value, loops);
    value = cache_test_fn_422(value, loops);
    value = cache_test_fn_423(value, loops);
    value = cache_test_fn_424(value, loops);
    value = cache_test_fn_425(value, loops);
    value = cache_test_fn_426(value, loops);
    value = cache_test_fn_427(value, loops);
    value = cache_test_fn_428(value, loops);
    value = cache_test_fn_429(value, loops);
    value = cache_test_fn_430(value, loops);
    value = cache_test_fn_431(value, loops);
    value = cache_test_fn_432(value, loops);
    value = cache_test_fn_433(value, loops);
    value = cache_test_fn_434(value, loops);
    value = cache_test_fn_435(value, loops);
    value = cache_test_fn_436(value, loops);
    value = cache_test_fn_437(value, loops);
    value = cache_test_fn_438(value, loops);
    value = cache_test_fn_439(value, loops);
    value = cache_test_fn_440(value, loops);
    value = cache_test_fn_441(value, loops);
    value = cache_test_fn_442(value, loops);
    value = cache_test_fn_443(value, loops);
    value = cache_test_fn_444(value, loops);
    value = cache_test_fn_445(value, loops);
    value = cache_test_fn_446(value, loops);
    value = cache_test_fn_447(value, loops);
    value = cache_test_fn_448(value, loops);
    value = cache_test_fn_449(value, loops);
    value = cache_test_fn_450(value, loops);
    value = cache_test_fn_451(value, loops);
    value = cache_test_fn_452(value, loops);
    value = cache_test_fn_453(value, loops);
    value = cache_test_fn_454(value, loops);
    value = cache_test_fn_455(value, loops);
    value = cache_test_fn_456(value, loops);
    value = cache_test_fn_457(value, loops);
    value = cache_test_fn_458(value, loops);
    value = cache_test_fn_459(value, loops);
    value = cache_test_fn_460(value, loops);
    value = cache_test_fn_461(value, loops);
    value = cache_test_fn_462(value, loops);
    value = cache_test_fn_463(value, loops);
    value = cache_test_fn_464(value, loops);
    value = cache_test_fn_465(value, loops);
    value = cache_test_fn_466(value, loops);
    value = cache_test_fn_467(value, loops);
    value = cache_test_fn_468(value, loops);
    value = cache_test_fn_469(value, loops);
    value = cache_test_fn_470(value, loops);
    value = cache_test_fn_471(value, loops);
    value = cache_test_fn_472(value, loops);
    value = cache_test_fn_473(value, loops);
    value = cache_test_fn_474(value, loops);
    value = cache_test_fn_475(value, loops);
    value = cache_test_fn_476(value, loops);
    value = cache_test_fn_477(value, loops);
    value = cache_test_fn_478(value, loops);
    value = cache_test_fn_479(value, loops);
    value = cache_test_fn_480(value, loops);
    value = cache_test_fn_481(value, loops);
    value = cache_test_fn_482(value, loops);
    value = cache_test_fn_483(value, loops);
    value = cache_test_fn_484(value, loops);
    value = cache_test_fn_485(value, loops);
    value = cache_test_fn_486(value, loops);
    value = cache_test_fn_487(value, loops);
    value = cache_test_fn_488(value, loops);
    value = cache_test_fn_489(value, loops);
    value = cache_test_fn_490(value, loops);
    value = cache_test_fn_491(value, loops);
    value = cache_test_fn_492(value, loops);
    value = cache_test_fn_493(value, loops);
    value = cache_test_fn_494(value, loops);
    value = cache_test_fn_495(value, loops);
    value = cache_test_fn_496(value, loops);
    value = cache_test_fn_497(value, loops);
    value = cache_test_fn_498(value, loops);
    value = cache_test_fn_499(value, loops);
    value = cache_test_fn_500(value, loops);
    value = cache_test_fn_501(value, loops);
    value = cache_test_fn_502(value, loops);
    value = cache_test_fn_503(value, loops);
    value = cache_test_fn_504(value, loops);
    value = cache_test_fn_505(value, loops);
    value = cache_test_fn_506(value, loops);
    value = cache_test_fn_507(value, loops);
    value = cache_test_fn_508(value, loops);
    value = cache_test_fn_509(value, loops);
    value = cache_test_fn_510(value, loops);
    value = cache_test_fn_511(value, loops);
    value = cache_test_fn_512(value, loops);
    value = cache_test_fn_513(value, loops);
    value = cache_test_fn_514(value, loops);
    value = cache_test_fn_515(value, loops);
    value = cache_test_fn_516(value, loops);
    value = cache_test_fn_517(value, loops);
    value = cache_test_fn_518(value, loops);
    value = cache_test_fn_519(value, loops);
    value = cache_test_fn_520(value, loops);
    value = cache_test_fn_521(value, loops);
    value = cache_test_fn_522(value, loops);
    value = cache_test_fn_523(value, loops);
    value = cache_test_fn_524(value, loops);
    value = cache_test_fn_525(value, loops);
    value = cache_test_fn_526(value, loops);
    value = cache_test_fn_527(value, loops);
    value = cache_test_fn_528(value, loops);
    value = cache_test_fn_529(value, loops);
    value = cache_test_fn_530(value, loops);
    value = cache_test_fn_531(value, loops);
    value = cache_test_fn_532(value, loops);
    value = cache_test_fn_533(value, loops);
    value = cache_test_fn_534(value, loops);
    value = cache_test_fn_535(value, loops);
    value = cache_test_fn_536(value, loops);
    value = cache_test_fn_537(value, loops);
    value = cache_test_fn_538(value, loops);
    value = cache_test_fn_539(value, loops);
    value = cache_test_fn_540(value, loops);
    value = cache_test_fn_541(value, loops);
    value = cache_test_fn_542(value, loops);
    value = cache_test_fn_543(value, loops);
    value = cache_test_fn_544(value, loops);
    value = cache_test_fn_545(value, loops);
    value = cache_test_fn_546(value, loops);
    value = cache_test_fn_547(value, loops);
    value = cache_test_fn_548(value, loops);
    value = cache_test_fn_549(value, loops);
    value = cache_test_fn_550(value, loops);
    value = cache_test_fn_551(value, loops);
    value = cache_test_fn_552(value, loops);
    value = cache_test_fn_553(value, loops);
    value = cache_test_fn_554(value, loops);
    value = cache_test_fn_555(value, loops);
    value = cache_test_fn_556(value, loops);
    value = cache_test_fn_557(value, loops);
    value = cache_test_fn_558(value, loops);
    value = cache_test_fn_559(value, loops);
    value = cache_test_fn_560(value, loops);
    value = cache_test_fn_561(value, loops);
    value = cache_test_fn_562(value, loops);
    value = cache_test_fn_563(value, loops);
    value = cache_test_fn_564(value, loops);
    value = cache_test_fn_565(value, loops);
    value = cache_test_fn_566(value, loops);
    value = cache_test_fn_567(value, loops);
    value = cache_test_fn_568(value, loops);
    value = cache_test_fn_569(value, loops);
    value = cache_test_fn_570(value, loops);
    value = cache_test_fn_571(value, loops);
    value = cache_test_fn_572(value, loops);
    value = cache_test_fn_573(value, loops);
    value = cache_test_fn_574(value, loops);
    value = cache_test_fn_575(value, loops);
    value = cache_test_fn_576(value, loops);
    value = cache_test_fn_577(value, loops);
    value = cache_test_fn_578(value, loops);
    value = cache_test_fn_579(value, loops);
    value = cache_test_fn_580(value, loops);
    value = cache_test_fn_581(value, loops);
    value = cache_test_fn_582(value, loops);
    value = cache_test_fn_583(value, loops);
    value = cache_test_fn_584(value, loops);
    value = cache_test_fn_585(value, loops);
    value = cache_test_fn_586(value, loops);
    value = cache_test_fn_587(value, loops);
    value = cache_test_fn_588(value, loops);
    value = cache_test_fn_589(value, loops);
    value = cache_test_fn_590(value, loops);
    value = cache_test_fn_591(value, loops);
    value = cache_test_fn_592(value, loops);
    value = cache_test_fn_593(value, loops);
    value = cache_test_fn_594(value, loops);
    value = cache_test_fn_595(value, loops);
    value = cache_test_fn_596(value, loops);
    value = cache_test_fn_597(value, loops);
    value = cache_test_fn_598(value, loops);
    value = cache_test_fn_599(value, loops);
    value = cache_test_fn_600(value, loops);
    value = cache_test_fn_601(value, loops);
    value = cache_test_fn_602(value, loops);
    value = cache_test_fn_603(value, loops);
    value = cache_test_fn_604(value, loops);
    value = cache_test_fn_605(value, loops);
    value = cache_test_fn_606(value, loops);
    value = cache_test_fn_607(value, loops);
    value = cache_test_fn_608(value, loops);
    value = cache_test_fn_609(value, loops);
    value = cache_test_fn_610(value, loops);
    value = cache_test_fn_611(value, loops);
    value = cache_test_fn_612(value, loops);
    value = cache_test_fn_613(value, loops);
    value = cache_test_fn_614(value, loops);
    value = cache_test_fn_615(value, loops);
    value = cache_test_fn_616(value, loops);
    value = cache_test_fn_617(value, loops);
    value = cache_test_fn_618(value, loops);
    value = cache_test_fn_619(value, loops);
    value = cache_test_fn_620(value, loops);
    value = cache_test_fn_621(value, loops);
    value = cache_test_fn_622(value, loops);
    value = cache_test_fn_623(value, loops);
    value = cache_test_fn_624(value, loops);
    value = cache_test_fn_625(value, loops);
    value = cache_test_fn_626(value, loops);
    value = cache_test_fn_627(value, loops);
    value = cache_test_fn_628(value, loops);
    value = cache_test_fn_629(value, loops);
    value = cache_test_fn_630(value, loops);
    value = cache_test_fn_631(value, loops);
    value = cache_test_fn_632(value, loops);
    value = cache_test_fn_633(value, loops);
    value = cache_test_fn_634(value, loops);
    value = cache_test_fn_635(value, loops);
    value = cache_test_fn_636(value, loops);
    value = cache_test_fn_637(value, loops);
    value = cache_test_fn_638(value, loops);
    value = cache_test_fn_639(value, loops);
    value = cache_test_fn_640(value, loops);
    value = cache_test_fn_641(value, loops);
    value = cache_test_fn_642(value, loops);
    value = cache_test_fn_643(value, loops);
    value = cache_test_fn_644(value, loops);
    value = cache_test_fn_645(value, loops);
    value = cache_test_fn_646(value, loops);
    value = cache_test_fn_647(value, loops);
    value = cache_test_fn_648(value, loops);
    value = cache_test_fn_649(value, loops);
    value = cache_test_fn_650(value, loops);
    value = cache_test_fn_651(value, loops);
    value = cache_test_fn_652(value, loops);
    value = cache_test_fn_653(value, loops);
    value = cache_test_fn_654(value, loops);
    value = cache_test_fn_655(value, loops);
    value = cache_test_fn_656(value, loops);
    value = cache_test_fn_657(value, loops);
    value = cache_test_fn_658(value, loops);
    value = cache_test_fn_659(value, loops);
    value = cache_test_fn_660(value, loops);
    value = cache_test_fn_661(value, loops);
    value = cache_test_fn_662(value, loops);
    value = cache_test_fn_663(value, loops);
    value = cache_test_fn_664(value, loops);
    value = cache_test_fn_665(value, loops);
    value = cache_test_fn_666(value, loops);
    value = cache_test_fn_667(value, loops);
    value = cache_test_fn_668(value, loops);
    value = cache_test_fn_669(value, loops);
    value = cache_test_fn_670(value, loops);
    value = cache_test_fn_671(value, loops);
    value = cache_test_fn_672(value, loops);
    value = cache_test_fn_673(value, loops);
    value = cache_test_fn_674(value, loops);
    value = cache_test_fn_675(value, loops);
    value = cache_test_fn_676(value, loops);
    value = cache_test_fn_677(value, loops);
    value = cache_test_fn_678(value, loops);
    value = cache_test_fn_679(value, loops);
    value = cache_test_fn_680(value, loops);
    value = cache_test_fn_681(value, loops);
    value = cache_test_fn_682(value, loops);
    value = cache_test_fn_683(value, loops);
    value = cache_test_fn_684(value, loops);
    value = cache_test_fn_685(value, loops);
    value = cache_test_fn_686(value, loops);
    value = cache_test_fn_687(value, loops);
    value = cache_test_fn_688(value, loops);
    value = cache_test_fn_689(value, loops);
    value = cache_test_fn_690(value, loops);
    value = cache_test_fn_691(value, loops);
    value = cache_test_fn_692(value, loops);
    value = cache_test_fn_693(value, loops);
    value = cache_test_fn_694(value, loops);
    value = cache_test_fn_695(value, loops);
    value = cache_test_fn_696(value, loops);
    value = cache_test_fn_697(value, loops);
    value = cache_test_fn_698(value, loops);
    value = cache_test_fn_699(value, loops);
    value = cache_test_fn_700(value, loops);
    value = cache_test_fn_701(value, loops);
    value = cache_test_fn_702(value, loops);
    value = cache_test_fn_703(value, loops);
    value = cache_test_fn_704(value, loops);
    value = cache_test_fn_705(value, loops);
    value = cache_test_fn_706(value, loops);
    value = cache_test_fn_707(value, loops);
    value = cache_test_fn_708(value, loops);
    value = cache_test_fn_709(value, loops);
    value = cache_test_fn_710(value, loops);
    value = cache_test_fn_711(value, loops);
    value = cache_test_fn_712(value, loops);
    value = cache_test_fn_713(value, loops);
    value = cache_test_fn_714(value, loops);
    value = cache_test_fn_715(value, loops);
    value = cache_test_fn_716(value, loops);
    value = cache_test_fn_717(value, loops);
    value = cache_test_fn_718(value, loops);
    value = cache_test_fn_719(value, loops);
    value = cache_test_fn_720(value, loops);
    value = cache_test_fn_721(value, loops);
    value = cache_test_fn_722(value, loops);
    value = cache_test_fn_723(value, loops);
    value = cache_test_fn_724(value, loops);
    value = cache_test_fn_725(value, loops);
    value = cache_test_fn_726(value, loops);
    value = cache_test_fn_727(value, loops);
    value = cache_test_fn_728(value, loops);
    value = cache_test_fn_729(value, loops);
    value = cache_test_fn_730(value, loops);
    value = cache_test_fn_731(value, loops);
    value = cache_test_fn_732(value, loops);
    value = cache_test_fn_733(value, loops);
    value = cache_test_fn_734(value, loops);
    value = cache_test_fn_735(value, loops);
    value = cache_test_fn_736(value, loops);
    value = cache_test_fn_737(value, loops);
    value = cache_test_fn_738(value, loops);
    value = cache_test_fn_739(value, loops);
    value = cache_test_fn_740(value, loops);
    value = cache_test_fn_741(value, loops);
    value = cache_test_fn_742(value, loops);
    value = cache_test_fn_743(value, loops);
    value = cache_test_fn_744(value, loops);
    value = cache_test_fn_745(value, loops);
    value = cache_test_fn_746(value, loops);
    value = cache_test_fn_747(value, loops);
    value = cache_test_fn_748(value, loops);
    value = cache_test_fn_749(value, loops);
    value = cache_test_fn_750(value, loops);
    value = cache_test_fn_751(value, loops);
    value = cache_test_fn_752(value, loops);
    value = cache_test_fn_753(value, loops);
    value = cache_test_fn_754(value, loops);
    value = cache_test_fn_755(value, loops);
    value = cache_test_fn_756(value, loops);
    value = cache_test_fn_757(value, loops);
    value = cache_test_fn_758(value, loops);
    value = cache_test_fn_759(value, loops);
    value = cache_test_fn_760(value, loops);
    value = cache_test_fn_761(value, loops);
    value = cache_test_fn_762(value, loops);
    value = cache_test_fn_763(value, loops);
    value = cache_test_fn_764(value, loops);
    value = cache_test_fn_765(value, loops);
    value = cache_test_fn_766(value, loops);
    value = cache_test_fn_767(value, loops);
    value = cache_test_fn_768(value, loops);
    value = cache_test_fn_769(value, loops);
    value = cache_test_fn_770(value, loops);
    value = cache_test_fn_771(value, loops);
    value = cache_test_fn_772(value, loops);
    value = cache_test_fn_773(value, loops);
    value = cache_test_fn_774(value, loops);
    value = cache_test_fn_775(value, loops);
    value = cache_test_fn_776(value, loops);
    value = cache_test_fn_777(value, loops);
    value = cache_test_fn_778(value, loops);
    value = cache_test_fn_779(value, loops);
    value = cache_test_fn_780(value, loops);
    value = cache_test_fn_781(value, loops);
    value = cache_test_fn_782(value, loops);
    value = cache_test_fn_783(value, loops);
    value = cache_test_fn_784(value, loops);
    value = cache_test_fn_785(value, loops);
    value = cache_test_fn_786(value, loops);
    value = cache_test_fn_787(value, loops);
    value = cache_test_fn_788(value, loops);
    value = cache_test_fn_789(value, loops);
    value = cache_test_fn_790(value, loops);
    value = cache_test_fn_791(value, loops);
    value = cache_test_fn_792(value, loops);
    value = cache_test_fn_793(value, loops);
    value = cache_test_fn_794(value, loops);
    value = cache_test_fn_795(value, loops);
    value = cache_test_fn_796(value, loops);
    value = cache_test_fn_797(value, loops);
    value = cache_test_fn_798(value, loops);
    value = cache_test_fn_799(value, loops);
    value = cache_test_fn_800(value, loops);
    value = cache_test_fn_801(value, loops);
    value = cache_test_fn_802(value, loops);
    value = cache_test_fn_803(value, loops);
    value = cache_test_fn_804(value, loops);
    value = cache_test_fn_805(value, loops);
    value = cache_test_fn_806(value, loops);
    value = cache_test_fn_807(value, loops);
    value = cache_test_fn_808(value, loops);
    value = cache_test_fn_809(value, loops);
    value = cache_test_fn_810(value, loops);
    value = cache_test_fn_811(value, loops);
    value = cache_test_fn_812(value, loops);
    value = cache_test_fn_813(value, loops);
    value = cache_test_fn_814(value, loops);
    value = cache_test_fn_815(value, loops);
    value = cache_test_fn_816(value, loops);
    value = cache_test_fn_817(value, loops);
    value = cache_test_fn_818(value, loops);
    value = cache_test_fn_819(value, loops);
    value = cache_test_fn_820(value, loops);
    value = cache_test_fn_821(value, loops);
    value = cache_test_fn_822(value, loops);
    value = cache_test_fn_823(value, loops);
    value = cache_test_fn_824(value, loops);
    value = cache_test_fn_825(value, loops);
    value = cache_test_fn_826(value, loops);
    value = cache_test_fn_827(value, loops);
    value = cache_test_fn_828(value, loops);
    value = cache_test_fn_829(value, loops);
    value = cache_test_fn_830(value, loops);
    value = cache_test_fn_831(value, loops);
    value = cache_test_fn_832(value, loops);
    value = cache_test_fn_833(value, loops);
    value = cache_test_fn_834(value, loops);
    value = cache_test_fn_835(value, loops);
    value = cache_test_fn_836(value, loops);
    value = cache_test_fn_837(value, loops);
    value = cache_test_fn_838(value, loops);
    value = cache_test_fn_839(value, loops);
    value = cache_test_fn_840(value, loops);
    value = cache_test_fn_841(value, loops);
    value = cache_test_fn_842(value, loops);
    value = cache_test_fn_843(value, loops);
    value = cache_test_fn_844(value, loops);
    value = cache_test_fn_845(value, loops);
    value = cache_test_fn_846(value, loops);
    value = cache_test_fn_847(value, loops);
    value = cache_test_fn_848(value, loops);
    value = cache_test_fn_849(value, loops);
    value = cache_test_fn_850(value, loops);
    value = cache_test_fn_851(value, loops);
    value = cache_test_fn_852(value, loops);
    value = cache_test_fn_853(value, loops);
    value = cache_test_fn_854(value, loops);
    value = cache_test_fn_855(value, loops);
    value = cache_test_fn_856(value, loops);
    value = cache_test_fn_857(value, loops);
    value = cache_test_fn_858(value, loops);
    value = cache_test_fn_859(value, loops);
    value = cache_test_fn_860(value, loops);
    value = cache_test_fn_861(value, loops);
    value = cache_test_fn_862(value, loops);
    value = cache_test_fn_863(value, loops);
    value = cache_test_fn_864(value, loops);
    value = cache_test_fn_865(value, loops);
    value = cache_test_fn_866(value, loops);
    value = cache_test_fn_867(value, loops);
    value = cache_test_fn_868(value, loops);
    value = cache_test_fn_869(value, loops);
    value = cache_test_fn_870(value, loops);
    value = cache_test_fn_871(value, loops);
    value = cache_test_fn_872(value, loops);
    value = cache_test_fn_873(value, loops);
    value = cache_test_fn_874(value, loops);
    value = cache_test_fn_875(value, loops);
    value = cache_test_fn_876(value, loops);
    value = cache_test_fn_877(value, loops);
    value = cache_test_fn_878(value, loops);
    value = cache_test_fn_879(value, loops);
    value = cache_test_fn_880(value, loops);
    value = cache_test_fn_881(value, loops);
    value = cache_test_fn_882(value, loops);
    value = cache_test_fn_883(value, loops);
    value = cache_test_fn_884(value, loops);
    value = cache_test_fn_885(value, loops);
    value = cache_test_fn_886(value, loops);
    value = cache_test_fn_887(value, loops);
    value = cache_test_fn_888(value, loops);
    value = cache_test_fn_889(value, loops);
    value = cache_test_fn_890(value, loops);
    value = cache_test_fn_891(value, loops);
    value = cache_test_fn_892(value, loops);
    value = cache_test_fn_893(value, loops);
    value = cache_test_fn_894(value, loops);
    value = cache_test_fn_895(value, loops);
    value = cache_test_fn_896(value, loops);
    value = cache_test_fn_897(value, loops);
    value = cache_test_fn_898(value, loops);
    value = cache_test_fn_899(value, loops);
    value = cache_test_fn_900(value, loops);
    value = cache_test_fn_901(value, loops);
    value = cache_test_fn_902(value, loops);
    value = cache_test_fn_903(value, loops);
    value = cache_test_fn_904(value, loops);
    value = cache_test_fn_905(value, loops);
    value = cache_test_fn_906(value, loops);
    value = cache_test_fn_907(value, loops);
    value = cache_test_fn_908(value, loops);
    value = cache_test_fn_909(value, loops);
    value = cache_test_fn_910(value, loops);
    value = cache_test_fn_911(value, loops);
    value = cache_test_fn_912(value, loops);
    value = cache_test_fn_913(value, loops);
    value = cache_test_fn_914(value, loops);
    value = cache_test_fn_915(value, loops);
    value = cache_test_fn_916(value, loops);
    value = cache_test_fn_917(value, loops);
    value = cache_test_fn_918(value, loops);
    value = cache_test_fn_919(value, loops);
    value = cache_test_fn_920(value, loops);
    value = cache_test_fn_921(value, loops);
    value = cache_test_fn_922(value, loops);
    value = cache_test_fn_923(value, loops);
    value = cache_test_fn_924(value, loops);
    value = cache_test_fn_925(value, loops);
    value = cache_test_fn_926(value, loops);
    value = cache_test_fn_927(value, loops);
    value = cache_test_fn_928(value, loops);
    value = cache_test_fn_929(value, loops);
    value = cache_test_fn_930(value, loops);
    value = cache_test_fn_931(value, loops);
    value = cache_test_fn_932(value, loops);
    value = cache_test_fn_933(value, loops);
    value = cache_test_fn_934(value, loops);
    value = cache_test_fn_935(value, loops);
    value = cache_test_fn_936(value, loops);
    value = cache_test_fn_937(value, loops);
    value = cache_test_fn_938(value, loops);
    value = cache_test_fn_939(value, loops);
    value = cache_test_fn_940(value, loops);
    value = cache_test_fn_941(value, loops);
    value = cache_test_fn_942(value, loops);
    value = cache_test_fn_943(value, loops);
    value = cache_test_fn_944(value, loops);
    value = cache_test_fn_945(value, loops);
    value = cache_test_fn_946(value, loops);
    value = cache_test_fn_947(value, loops);
    value = cache_test_fn_948(value, loops);
    value = cache_test_fn_949(value, loops);
    value = cache_test_fn_950(value, loops);
    value = cache_test_fn_951(value, loops);
    value = cache_test_fn_952(value, loops);
    value = cache_test_fn_953(value, loops);
    value = cache_test_fn_954(value, loops);
    value = cache_test_fn_955(value, loops);
    value = cache_test_fn_956(value, loops);
    value = cache_test_fn_957(value, loops);
    value = cache_test_fn_958(value, loops);
    value = cache_test_fn_959(value, loops);
    value = cache_test_fn_960(value, loops);
    value = cache_test_fn_961(value, loops);
    value = cache_test_fn_962(value, loops);
    value = cache_test_fn_963(value, loops);
    value = cache_test_fn_964(value, loops);
    value = cache_test_fn_965(value, loops);
    value = cache_test_fn_966(value, loops);
    value = cache_test_fn_967(value, loops);
    value = cache_test_fn_968(value, loops);
    value = cache_test_fn_969(value, loops);
    value = cache_test_fn_970(value, loops);
    value = cache_test_fn_971(value, loops);
    value = cache_test_fn_972(value, loops);
    value = cache_test_fn_973(value, loops);
    value = cache_test_fn_974(value, loops);
    value = cache_test_fn_975(value, loops);
    value = cache_test_fn_976(value, loops);
    value = cache_test_fn_977(value, loops);
    value = cache_test_fn_978(value, loops);
    value = cache_test_fn_979(value, loops);
    value = cache_test_fn_980(value, loops);
    value = cache_test_fn_981(value, loops);
    value = cache_test_fn_982(value, loops);
    value = cache_test_fn_983(value, loops);
    value = cache_test_fn_984(value, loops);
    value = cache_test_fn_985(value, loops);
    value = cache_test_fn_986(value, loops);
    value = cache_test_fn_987(value, loops);
    value = cache_test_fn_988(value, loops);
    value = cache_test_fn_989(value, loops);
    value = cache_test_fn_990(value, loops);
    value = cache_test_fn_991(value, loops);
    value = cache_test_fn_992(value, loops);
    value = cache_test_fn_993(value, loops);
    value = cache_test_fn_994(value, loops);
    value = cache_test_fn_995(value, loops);
    value = cache_test_fn_996(value, loops);
    value = cache_test_fn_997(value, loops);
    value = cache_test_fn_998(value, loops);
    value = cache_test_fn_999(value, loops);
    return value;
}
