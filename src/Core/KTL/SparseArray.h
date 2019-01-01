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

#ifndef __k3d_SparseArray_h__
#define __k3d_SparseArray_h__

#if 0
namespace k3d
{
    template <typename T>
    class SparseArray
    {
    public:

        SparseArray(int InitialCapacity = 10)
            : m_Keys(InitialCapacity)
            , m_Values(InitialCapacity)
        {
        }

        ~SparseArray()
        {
        }

        SparseArray & Put(int Key, T Value)
        {
            int Index = BinarySearch(m_Keys, Key);
            if (Index >= 0)
            {
                m_Values[Index] = Value;
            }
            else
            {
                Index = ~Index;
                if (Index < m_Keys.Count() /*&& m_Values[Index] == Deleted*/)
                {
                    m_Keys[Index] = Key;
                    m_Values[Index] = Value;
                    return;
                }

            }
            return *this;
        }

        //const T* Get(int Key) const
        //{
        //    int Index = BinarySearch(m_Keys, Key);
        //    if (Index >= 0))
        //    {
        //        return m_Values[Index];
        //    }
        //    else
        //    {
        //        return nullptr;
        //    }
        //}

        T Get(int Key, T Expected = T())
        {
            int Index = BinarySearch(m_Keys, Key);
            //if (Iter != m_Keys.end())
            {
                //return m_Values[*Iter];
                return Expected;
            }
            //else
            //{
            //    return Expected;
            //}
        }

        bool Remove(int Key)
        {
            return true;
        }

        //class KeyIterator
        //{
        //public:

        //private:
        //    KeyIterator();
        //};

        //class ValueIterator
        //{
        //public:

        //private:
        //    ValueIterator();
        //};

        //KeyIterator Keys() const;
        //ValueIterator Values() const;
        static int BinarySearch(DynArray<int> const& Array, int Value) 
        {
            int lo = 0;
            int hi = Array.Count() - 1;
            while (lo <= hi) 
            {
                int mid = (lo + hi) >> > 1;
                int midVal = Array[mid];
                if (midVal < Value) 
                {
                    lo = mid + 1;
                }
                else if (midVal > Value) 
                {
                    hi = mid - 1;
                }
                else 
                {
                    return mid;
                }
            }
            return ~lo;
        }
    private:
        DynArray<int> m_Keys;
        DynArray<T>   m_Values;
    };
}
#endif
#endif
