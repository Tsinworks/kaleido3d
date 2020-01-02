#pragma once

namespace vulkan
{
	template<class T, int inlineNum = 1>
	class LinearArray
	{
	private:
		T	inline_[inlineNum];
		T*	data_;
		int num_;
	public:
		explicit LinearArray();
		~LinearArray();

		void			setNum(int num);
		int				num() const { return num_; }
		LinearArray&	add(T const& elem);
		LinearArray&	remove(int index);

		T&				operator[](int index) { return *(data_ + index); }		
		const T&		operator[](int index) const { return *(data_ + index); }
	};

	template<class T, int inlineNum>
	inline LinearArray<T, inlineNum>::LinearArray() 
	{
		//if constexpr (inlineNum > 0)
		//{
		//	data_ = inline_;
		//}
		//else
		//{
		//	data_ = 0;
		//}
	}

	template<class T, int inlineNum>
	inline LinearArray<T, inlineNum>::~LinearArray()
	{
	}

	template<class T, int inlineNum>
	inline void LinearArray<T, inlineNum>::setNum(int num)
	{
		if (num > inlineNum)
		{

		}
	}

	template<class T, int inlineNum>
	inline LinearArray<T, inlineNum>& LinearArray<T, inlineNum>::add(T const & elem)
	{
		return *this;
	}

	template<class T, int inlineNum>
	inline LinearArray<T, inlineNum>& LinearArray<T, inlineNum>::remove(int index)
	{
		return *this;
	}
}