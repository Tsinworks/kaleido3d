/**
 * MIT License
 *
 * Copyright (c) 2019 Zhou Qin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once

#ifndef __k3d_Types_h__
#define __k3d_Types_h__

#if K3D_USE_SSE
#include <emmintrin.h>
#elif K3D_USE_NEON
#include <arm_neon.h>
#endif

namespace k3d
{
    typedef int8_t    I8;
    typedef int16_t   I16;
    typedef int32_t   I32;

    typedef uint8_t   U8;
    typedef uint16_t  U16;
    typedef int64_t   I64;
    typedef uint32_t  U32;
    typedef uint64_t  U64;

#if K3D_USE_SSE
    typedef __m128  V4F;
    typedef __m128i V4I;
#elif K3D_USE_NEON
    typedef float32x4_t __attribute((aligned(16))) V4F;
    typedef int32x4_t __attribute((aligned(16))) V4I;
#else
    typedef struct
    {
        float f32[4];
    } V4F;
    typedef struct
    {
        union
        {
            U32 u32[4];
            I32 i32[4];
        };
    } V4I;
#endif
    
}

#endif