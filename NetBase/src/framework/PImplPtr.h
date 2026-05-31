#ifndef __NETBASE_FRAMEWORK_PIMPLPTR_H__
#define __NETBASE_FRAMEWORK_PIMPLPTR_H__

#include <utility>

#include "NetBaseAPI.h"

template<typename T>
class NETBASE_API PImplPtr
{
public:
	using DeleterFn = void(*)(T *);

	PImplPtr(T *ptr, DeleterFn deleter);

	PImplPtr(const PImplPtr &) = delete;
	PImplPtr<T> &operator=(const PImplPtr &) = delete;

	PImplPtr(PImplPtr &&other) noexcept;
	PImplPtr<T> &operator=(PImplPtr &&other) noexcept;

	~PImplPtr();

	T *operator->() const;
	T &operator*() const;

	template<typename ...Args>
	static PImplPtr<T> MakePImpl(Args && ...args);

private:
	T *ptr;
	DeleterFn deleter;
};

template<typename T>
inline PImplPtr<T>::PImplPtr(T *ptr, DeleterFn deleter)
	: ptr(ptr),
	deleter(deleter)
{
}

template<typename T>
inline PImplPtr<T>::PImplPtr(PImplPtr &&other) noexcept
	: ptr(other.ptr),
	deleter(other.deleter)
{
	other.ptr = nullptr;
	other.deleter = nullptr;
}

template<typename T>
inline PImplPtr<T> &PImplPtr<T>::operator=(PImplPtr &&other) noexcept
{
	if (this != &other)
	{
		if (ptr && deleter)
			deleter(ptr);

		ptr = other.ptr;
		deleter = other.deleter;

		other.ptr = nullptr;
		other.deleter = nullptr;
	}

	return *this
}

template<typename T>
inline PImplPtr<T>::~PImplPtr()
{
	if (ptr && deleter)
		deleter(ptr);
}

template<typename T>
inline T *PImplPtr<T>::operator->() const
{
	return ptr;
}

template<typename T>
inline T &PImplPtr<T>::operator*() const
{
	return *ptr;
}

template<typename T>
template<typename ...Args>
inline PImplPtr<T> PImplPtr<T>::MakePImpl(Args && ...args)
{
	auto deleter = [](T *p)
	{
		delete p;
	};

	return PImplPtr<T>(new T(std::forward<Args>(args)...), deleter);
}

#endif
