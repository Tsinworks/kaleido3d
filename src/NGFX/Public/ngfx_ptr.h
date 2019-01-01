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
#ifndef __ngfx_ptr_h__
#define __ngfx_ptr_h__
#include "ngfx_atomic.h"
namespace ngfx
{
	struct Rc 
	{
		Rc() {}
		virtual ~Rc() {}

		int32_t retain_internal()
		{
			return atomic_increment(&ref_int_);
		}
		int32_t release_internal()
		{
			auto c = atomic_decrement(&ref_int_);
			if (ref_int_ == 0)
			{
				delete this;
			}
			return c;
		}
		int32_t release()
		{
			auto c = atomic_decrement(&ref_ext_);
			if (ref_ext_ == 0)
			{
				release_internal();
			}
			return c;
		}
		int32_t retain()
		{
			return atomic_increment(&ref_ext_);
		}
	private:
		int32_t ref_int_ = 1;
		int32_t ref_ext_ = 1;
	};


	template <class T>
	class Ptr
	{
	public:
		explicit Ptr(T * pObj) : ptr_(pObj) {}
		Ptr(Ptr<T> const& Other)
			: ptr_(Other.ptr_) {
			if (ptr_)
				ptr_->retain();
		}
		Ptr() : ptr_(nullptr) {}
		~Ptr()
		{
			if (ptr_)
			{
				ptr_->release();
				ptr_ = nullptr;
			}
		}
		T& operator*() const { { return *ptr_; } }
		T* operator->() const { { return ptr_; } }
		explicit operator bool() const
		{
			return ptr_ != nullptr;
		}
		void Swap(Ptr& Other)
		{
			T * const pValue = Other.ptr_;
			Other.ptr_ = ptr_;
			ptr_ = pValue;
		}
		Ptr& operator=(const Ptr& Other) {
			typedef Ptr<T> ThisType;
			if (&Other != this) {
				ThisType(Other).Swap(*this);
			}
			return *this;
		}
		T* Get() const { return ptr_; }
		T** GetAddressOf() { return &ptr_;}
	private:
		T* ptr_;
	};
}
#endif
