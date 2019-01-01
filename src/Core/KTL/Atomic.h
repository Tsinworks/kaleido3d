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

namespace k3d
{
    template <typename T>
    class Atomic
    {
        typedef Atomic<T> ThisType;
    public:

        static_assert(sizeof(T) <= sizeof(long), "");

        Atomic()
        {}

        Atomic(T const& Other) : m_Value(Other) {}

        void Store(T const& Val)
        {

        }

        void Load(T& Val)
        {

        }

        void Swap(ThisType const& Rhs)
        {
            __intrinsics__::AtomicCAS(&m_Value, Rhs.m_Value, 1);
        }

        T Get() const { return m_Value; }

    private:
        T m_Value;
    };

    class AtomicBool
    {
    public:
        AtomicBool(bool InVal = false) : m_Value(false) {}

        void Set(bool InVal)
        {
            m_Value = InVal;
        }
        bool Get() const
        {
            return m_Value;
        }

        operator bool() const
        {
            return m_Value;
        }

    private:
        bool m_Value;
    };
}